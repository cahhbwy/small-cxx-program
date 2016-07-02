
#include "stdafx.h"
#include "buffer_manager.h"

using namespace std;

//CBufferManager m_ptheblocks[MAX_BLOCKS];
CBufferManager* CBufferManager::m_ptheblocks = NULL;


/*��ʼ��buffer�������������ڴ�*/
void CBufferManager::initiate_blocks()
{
	unsigned int i;
	m_ptheblocks = new CBufferManager [MAX_BLOCKS];
	for(i=0;i<MAX_BLOCKS;i++){//��ÿһ��Ĳ�����ʼ��
		m_ptheblocks[i].m_address = NULL;
		m_ptheblocks[i].m_index_table=0;
		m_ptheblocks[i].m_is_written=0;
		m_ptheblocks[i].m_being_used=0;
		m_ptheblocks[i].m_count=0;
	}
}

/*�˳�ʱ���ã���������Ҫд�صĿ�����д����Ӧ�ļ������ͷ�buffer��*/
void CBufferManager::flush_all_blocks(){   
	for(int i=0;i<MAX_BLOCKS;i++){
		m_ptheblocks[i].flush_block();
		if(m_ptheblocks[i].m_address != NULL)
			delete [] m_ptheblocks[i].m_address;
	}
}

/*ʹ�ÿ麯����buffer�ڲ�����*/
void CBufferManager::using_block(unsigned int number)
{
	unsigned int i;
	m_ptheblocks[number].m_being_used=1;
	for(i=0;i<MAX_BLOCKS;i++){
		if(i!=number && !m_ptheblocks[number].m_being_used)
			m_ptheblocks[i].m_count++;  //����û����ʹ�õĿ�count��1
		else if(i==number)
			m_ptheblocks[i].m_count=0;  //����ʹ�õĿ�count��0��ʹ���ڱ�ʹ�õĿ鲻������
	}
}

/*�ҵ�countֵ���Ŀ�ţ�������LRU�㷨��Ҫ���滻���Ŀ飩��buffer�ڲ�����*/	
unsigned int CBufferManager::max_count_number()
{
	unsigned int i;
	unsigned int max_count=m_ptheblocks[0].m_count;
	unsigned int block_number=0;
	for(i=0;i<MAX_BLOCKS;i++)
		if(m_ptheblocks[i].m_count>max_count){
			max_count=m_ptheblocks[i].m_count;
			block_number=i;
		}
	return block_number;
}

/*���ÿ���Ҫд�أ����ÿ������д����Ӧ�ļ�������ʱ���˳�ʱ����,buffer�ڲ�����*/
void CBufferManager::flush_block()
{
	FILE *fp;
	if(m_is_written && m_index_table) {
		if(fp=fopen((LPCTSTR)m_name,"rb+")){  
			fseek(fp,BLOCK_SIZE*m_offset_number,0);
			fwrite(m_address,BLOCK_SIZE,1,fp);
			fclose(fp);
		}
	}
}

/*Ϊrecord manager��index manager�ṩ�飨д���������ʼ����ʱ����*/
unsigned int CBufferManager::get_blank_block(short int index_table)
{
	unsigned int number;
	unsigned int i;
	char *p;
	for(i=0;i<MAX_BLOCKS;i++)
		if(!m_ptheblocks[i].m_index_table && !m_ptheblocks[i].m_being_used)
			break;
	if(i<MAX_BLOCKS) { //��ʾ�пտ�
		number=i;
		m_ptheblocks[number].m_address = new char [BLOCK_SIZE];	

        for(p=m_ptheblocks[number].m_address;p<m_ptheblocks[number].m_address+BLOCK_SIZE;p++)
		   *p='$';
	}
	else{ //��ʾ�޿տ飬���жϻ����ĸ�����Ƿ��б�Ҫд���ļ�
		number=max_count_number();
		m_ptheblocks[number].flush_block();
	}
	using_block(number);
	m_ptheblocks[number].m_index_table=index_table;
	return number;
}

/*Ϊrecord manager��index manager�ṩ�飨����ģ�,����buffer���Ҳ��������get_blank_blockΪ�����ռ䲢�����*/
unsigned int CBufferManager::get_block(short int index_table, CString filename, unsigned int offset_number)
{
	FILE *fp;
	unsigned int number;
	unsigned int i,mark;
	
	for(i=0;i<MAX_BLOCKS;i++) {
		if(m_ptheblocks[i].m_index_table==index_table && m_ptheblocks[i].m_name == filename && 
			m_ptheblocks[i].m_offset_number==offset_number) { //��ʾ��buffer���ҵ��˸ÿ�
			number=i;
			using_block(number);
			return number;
		}
	}

	mark=get_blank_block(index_table);//δ�ҵ��ÿ������get_blank_blockΪ�����ռ䲢�����
	m_ptheblocks[mark].mark_block(filename,offset_number);
	
	fp = fopen((LPCTSTR)filename, "rb");
	fseek(fp,BLOCK_SIZE*offset_number, 0);
	fread(m_ptheblocks[mark].m_address,BLOCK_SIZE,1,fp);
	fclose(fp);

	using_block(mark);
	return mark;
}

