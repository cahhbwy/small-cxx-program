#ifndef __buffer_h__
#define __buffer_h__

#include "MiniSQL.h"

class CBufferManager
{
public:
	char* m_address;				//����׵�ַ
	CString m_name;					//���Ӧ�ı��������
	unsigned int m_offset_number;	//���Ӧ�ı�������е�ҳ����
	short int m_index_table;		//���Ӧ���Ǳ���ֵΪTABLE;������������ֵΪINDEX;����Ϊ0
	short int m_is_written;			//�ÿ��Ƿ񱻸Ĺ�����ֵΪ0���Ĺ���1
	short int m_being_used;			//�ÿ鵱ǰ�Ƿ񱻶���д��������ֵΪ1
	unsigned int m_count;			//����ʵ��LRU�㷨

	CBufferManager(){}
	~CBufferManager(){}

	static void flush_all_blocks();
	static void initiate_blocks();

	unsigned int get_block(short int index_table, CString filename, unsigned int offset_number);
	unsigned int get_blank_block(short int index_table);

	/*��־�ÿ��ѱ��Ĺ�*/
	void written_block() {
		m_is_written=1;
	}
	/*��־�ÿ��Ѿ���������*/
	void used_block() {
		m_being_used=0;
	}

protected:
	static CBufferManager *m_ptheblocks;

	static void using_block(unsigned int number);
	static unsigned int max_count_number();

	void flush_block();
	/*Ϊ�¿������*/
	void mark_block(CString filename,unsigned int offset) {
		m_name = filename;
		m_offset_number = offset;
	}
	  
};

#endif