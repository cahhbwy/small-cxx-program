    
#include "stdafx.h"
#include "record_manager.h"

using namespace std;

short int CRecordManager::ValueCompare(char *value, unsigned int type, unsigned int cond, char *compvalue)
{
	long intvalue;
	double floatvalue;
	
	switch(type)
	{
	case INT:
		{
			intvalue = atoi(value);
			switch(cond)
			{
			case LT:
				if(intvalue < atoi(compvalue))
					return 1;
				break;
			case LE:
				if(intvalue <= atoi(compvalue))
					return 1;
				break;
			case GT:
				if(intvalue > atoi(compvalue))
					return 1;
				break;
			case GE:
				if(intvalue >= atoi(compvalue))
					return 1;
				break;
			case EQ:
				if(intvalue == atoi(compvalue))
					return 1;
				break;
			case NE:
				if(intvalue != atoi(compvalue))
					return 1;
				break;
			}
		}
		break;
	case FLOAT:
		{
			floatvalue = atof(value);
			switch(cond)
			{
			case LT:
				if(floatvalue < atof(compvalue))
					return 1;
				break;
			case LE:
				if(floatvalue <= atof(compvalue))
					return 1;
				break;
			case GT:
				if(floatvalue > atof(compvalue))
					return 1;
				break;
			case GE:
				if(floatvalue >= atof(compvalue))
					return 1;
				break;
			case EQ:
				if(floatvalue == atof(compvalue))
					return 1;
				break;
			case NE:
				if(floatvalue != atof(compvalue))
					return 1;
				break;
			}
		}
		break;
	case CHAR:
		{
			switch(cond)
			{
			case LT:
				if(strcmp(value, compvalue) < 0)
					return 1;
				break;
			case LE:
				if(strcmp(value, compvalue) <= 0)
					return 1;
				break;
			case GT:
				if(strcmp(value, compvalue) > 0)
					return 1;
				break;
			case GE:
				if(strcmp(value, compvalue) >= 0)
					return 1;
				break;
			case EQ:
				if(strcmp(value, compvalue) == 0)
					return 1;
				break;
			case NE:
				if(strcmp(value, compvalue) != 0)
					return 1;
				break;
			}
		}
		break;
	}
	return 0;

}

short int CRecordManager::SelectRecord(condition *conds, column *cols, unsigned int recordlen, unsigned int recordnum)
{
	int *intp;
	int mapnum, tabnum;				//map�ļ���tab�ļ�����
	int mapblocknum, tabblocknum;	//map�ļ���tab�ļ����ڴ��еĿ��
	int mapoffset = 0, taboffset = 2;
	int record_perblock;
	CString tabfilename, mapfilename;
	char value[VALUE_LENGTH];
	int i;
	column *tempcol = cols;
	condition *tempcond;
	int conform = 1;
	unsigned int printnum = 0;

	CString name = (LPCTSTR)m_tablename;
	tabfilename = name + ".tab"; 
	mapfilename = name + ".map";


	mapblocknum = get_block(MAP, mapfilename, 0);
	m_ptheblocks[mapblocknum].m_being_used = 1;
	intp = (int *)m_ptheblocks[mapblocknum].m_address;
	mapnum = *intp;
	m_ptheblocks[mapblocknum].m_being_used = 0;

	tabblocknum = get_block(TABLE, tabfilename, 0);
	m_ptheblocks[tabblocknum].m_being_used = 1;
	intp = (int *)m_ptheblocks[tabblocknum].m_address;
	tabnum = *intp;
	record_perblock = BLOCK_SIZE / recordlen;
	m_ptheblocks[tabblocknum].m_being_used = 0;

	//map�ļ�������û�У��ȶ���һ��
	mapblocknum = get_block(MAP, mapfilename, 1);

	while(tempcol)
	{
		int remain = tempcol -> collength - strlen(tempcol -> colname);
		printf("%s", tempcol -> colname);
		for(; remain ; remain --)
			printf(" ");
		tempcol = tempcol -> next;
	}
	printf("\n");
	for(i = 1; i < tabnum; i++)                     //tab�ļ���ѭ��
	{
		tabblocknum = get_block(TABLE, tabfilename, i);
		for(;taboffset < record_perblock ; taboffset ++)	//tabһ������offsetѭ��
		{
			//�����Ӧ��.map
			if(*(m_ptheblocks[mapblocknum].m_address + mapoffset) =='1')
			{	
				tempcond = conds;
				conform = 1;
				if(!tempcond)
					goto print;
				while(tempcond)
				{
					strcpy(value, m_ptheblocks[tabblocknum].m_address + taboffset * recordlen + tempcond -> attroffset);
					if(!ValueCompare(value, tempcond -> type, tempcond -> cond, tempcond -> value))
						conform = 0;
					tempcond = tempcond -> next;
				}
				//������������ʼ��ӡ
				print:
				if(conform)
				{
					tempcol = cols;
					printnum ++;
					while(tempcol)
					{
						int remain;
						strcpy(value, m_ptheblocks[tabblocknum].m_address + taboffset * recordlen + tempcol -> coloffset);
						remain = tempcol -> collength - strlen(value);
						printf("%s", value);
						for(; remain ; remain --)
							printf(" ");
						tempcol = tempcol -> next;
					}
					printf("\n");
				}
			}
			//map offset��1������BLOCK_SIZE���һ�¿顣
			mapoffset ++;
			if(mapoffset >= BLOCK_SIZE)
			{
				mapoffset = 0;
				mapblocknum ++;
				if(mapblocknum > mapnum)
					return 1;
				mapblocknum = get_block(MAP, mapfilename, mapblocknum);
			}				
		}	
	}
	printf("%d record(s) are selected from table '%s'!\n", printnum, m_tablename);
	return 1;	
}


short int CRecordManager::DeleteRecord(condition *conds, unsigned int recordlen,unsigned int recordnum)
{
	int *intp;
	int mapnum, tabnum;				//map�ļ���tab�ļ�����
	int mapblocknum, tabblocknum;	//map�ļ���tab�ļ����ڴ��еĿ��
	int mapoffset = 0, taboffset = 2;
	int record_perblock;
	CString tabfilename, mapfilename;
	char value[VALUE_LENGTH];
	int i;
	unsigned int j;
	condition *tempcond;
	int conform = 1;
	short int deleted = 0;
	char *ptr;
	
	CString name = (LPCTSTR)m_tablename;
	tabfilename = name + ".tab"; 
	mapfilename = name + ".map";

	//��map�ļ�
	mapblocknum = get_block(MAP, mapfilename, 0);
	m_ptheblocks[mapblocknum].m_being_used = 1;
	intp = (int *)m_ptheblocks[mapblocknum].m_address;
	mapnum = *intp;
	m_ptheblocks[mapblocknum].m_being_used = 0;

	//��tab�ļ�
	tabblocknum = get_block(TABLE, tabfilename, 0);
	m_ptheblocks[tabblocknum].m_being_used = 1;
	intp = (int *)m_ptheblocks[tabblocknum].m_address;
	tabnum = *intp;
	record_perblock = BLOCK_SIZE / recordlen;
	m_ptheblocks[tabblocknum].m_being_used = 0;

	//map�ļ�������û�У��ȶ���һ��
	mapblocknum = get_block(MAP, mapfilename, 1);

	for(i = 1; i < tabnum; i++)                     //tab�ļ���ѭ��
	{
		tabblocknum = get_block(TABLE, tabfilename, i);
		for(;taboffset < record_perblock ; taboffset ++)	//tabһ������offsetѭ��
		{
			//�����Ӧ��.map
			if(*(m_ptheblocks[mapblocknum].m_address + mapoffset) =='1')
			{	
				tempcond = conds;
				conform = 1;
				if(!tempcond)
					goto todelete;
				while(tempcond)
				{
					strcpy(value, m_ptheblocks[tabblocknum].m_address + taboffset * recordlen + tempcond -> attroffset);
					if(!ValueCompare(value, tempcond -> type, tempcond -> cond, tempcond -> value))
						conform = 0;
					tempcond = tempcond -> next;
				}
				//������������ʼɾ
				todelete:
				if(conform)
				{
					*(m_ptheblocks[mapblocknum].m_address + mapoffset) ='$';
					deleted++;
					ptr = m_ptheblocks[tabblocknum].m_address + taboffset * recordlen;
					for(j = 0 ; j < recordlen ; j++)
						*(ptr + j) = '$';					
				}
			}
			//map offset��1������BLOCK_SIZE���һ�¿顣
			mapoffset ++;
			if(mapoffset >= BLOCK_SIZE)
			{
				mapoffset = 0;
				mapblocknum ++;
				if(mapblocknum > mapnum)
					return deleted;
				mapblocknum = get_block(MAP, mapfilename, mapblocknum);
			}				
		}	
	}
	return deleted;	
	
}

short int CRecordManager::IsValueExists(column *cols, char *value,unsigned int recordlen)		//�Ƿ�Ӧ�Ĵ���recordlen����
{
	int number,iter,totalblock;
	int i,find= 0;
	char *p,*temp; 
	int *intp;
	CString tablefilename;
	int  recordpblk=BLOCK_SIZE/recordlen;

	CString name = (LPCTSTR)m_tablename;
	tablefilename = name + ".tab"; 
	
	number=get_block(TABLE,tablefilename,0);
    m_ptheblocks[number].m_being_used=1;
    p=m_ptheblocks[number].m_address;
	intp=(int*)p;
    totalblock=*intp;
	m_ptheblocks[number].used_block();
	for(iter = 1;iter<totalblock;iter++)
	{
	   number = get_block(TABLE,tablefilename,iter);
	   m_ptheblocks[number].m_being_used = 1;
	   p=m_ptheblocks[number].m_address;
	   for(i = 2; i < recordpblk; i++)
		{	
			temp=p+recordlen*i+cols->coloffset;
			if(strcmp(temp,value)==0)
			{	
	    	m_ptheblocks[number].used_block();
			return 1;
			}
		}
	    m_ptheblocks[number].used_block();
	}
   return 0;
}

//����һ����¼�����ڡ�Map�ļ����ҵ�һ��Ϊ0��λ��Ȼ��Ͱ�values�����value����һ��һ��д��
//��Ӧ��¼�ļ���ȥ����û�У���ֱ��ڡ�Map�ļ�β׷��һ��1���ڼ�¼�ļ�β׷��һ����¼��Ҫ��������
//��¼����Ŀ�š�
//record��map�ļ��ڳ�ʼ��ʱ������BLOCK0�д��ϵ�ǰ�ж��ٸ�BLOCK
unsigned int CRecordManager::InsertValues(insertvalue *values, unsigned int recordlen)
{
	//���ؼ�¼����Ŀ��
	int map_block_num,tab_block_num;
	int map_total_block,tab_total_block;
	insertvalue *current_value;
	char *p,*temp,*point;
	int *intp;
	int i;
	CString tablefilename,mapfilename;
	int record_per_block=BLOCK_SIZE/recordlen;
	int block_no,offset_in_block,record_number;


	CString name = (LPCTSTR)m_tablename;
	tablefilename = name + ".tab"; 
	mapfilename = name + ".map";
		
  	tab_block_num=get_block(TABLE,tablefilename,0);
	m_ptheblocks[tab_block_num].m_being_used=1;
	p=m_ptheblocks[tab_block_num].m_address;
	intp=(int*)p;
	tab_total_block=*intp;
	m_ptheblocks[tab_block_num].used_block();
	//
	map_block_num=get_block(MAP,mapfilename,0);
	if(map_block_num==-1)
	{
		printf("memory allocation error\n");
		return 0;
	}
	m_ptheblocks[map_block_num].m_being_used=1;
	p=m_ptheblocks[map_block_num].m_address;
	intp=(int*)p;
    map_total_block=*intp;//������ǰMAP���м����飬
	if(map_total_block==1)
	{
		*intp=2;
		
		m_ptheblocks[map_block_num].written_block();
		m_ptheblocks[map_block_num].used_block();
		map_block_num=get_blank_block(MAP);
		m_ptheblocks[map_block_num].m_being_used=1;
		m_ptheblocks[map_block_num].m_index_table=MAP;
		m_ptheblocks[map_block_num].m_name = mapfilename;
	    m_ptheblocks[map_block_num].m_offset_number=1;
		p=m_ptheblocks[map_block_num].m_address;

		*p='1';//��BLOCKͷ����
		m_ptheblocks[map_block_num].written_block();
		m_ptheblocks[map_block_num].used_block();
  
		tab_block_num=get_blank_block(TABLE);
		m_ptheblocks[tab_block_num].m_being_used=1;
		m_ptheblocks[tab_block_num].m_index_table=TABLE;
		m_ptheblocks[tab_block_num].m_name = tablefilename;
		m_ptheblocks[tab_block_num].m_offset_number=1;
		p=m_ptheblocks[tab_block_num].m_address;
	    intp=(int*)p;
		*intp=1;//�ÿ�ĺ���
		p=m_ptheblocks[tab_block_num].m_address+recordlen*2;
		for(current_value=values;current_value!=NULL;current_value=current_value->next)
		{
		   strcpy(p,current_value->value);
		   p=p+current_value->length;
		}
	
		m_ptheblocks[tab_block_num].written_block();
		m_ptheblocks[tab_block_num].used_block();
		//
		tab_block_num=get_block(TABLE,tablefilename,0);
		m_ptheblocks[tab_block_num].m_being_used=1;
		p=m_ptheblocks[tab_block_num].m_address;
		intp=(int*)p;
		*intp=2;
	
		m_ptheblocks[tab_block_num].written_block();
		m_ptheblocks[tab_block_num].used_block();
		return 1;
	}
	else
	{
        for(i=1;i<map_total_block;i++)
		{
           map_block_num=get_block(MAP,mapfilename,i);
           if(map_block_num==-1)
		   {
		       printf("memory allocation error\n");
		       return 0;
		   }
		   m_ptheblocks[map_block_num].m_being_used=1;
		   p=m_ptheblocks[map_block_num].m_address;
		   for(temp=p;temp<p+BLOCK_SIZE;temp++)
			   if(*temp=='$')
			   {    
				   *temp='1';
				   m_ptheblocks[map_block_num].written_block();
				   m_ptheblocks[map_block_num].used_block();
				   record_number=(i-1)*BLOCK_SIZE+temp-p+1;//��1��ʼ����
                   block_no=record_number/record_per_block+1;
                   offset_in_block=record_number-record_per_block*(block_no-1);//��1����
				   ////
				  if(block_no>=tab_total_block)
				  {
					  char *t1; int *it1;
					  tab_block_num=get_block(TABLE,tablefilename,0);
					  m_ptheblocks[tab_block_num].m_being_used=1;
					  t1=m_ptheblocks[tab_block_num].m_address;
                      it1=(int*)t1;
					  (*it1)++;
					  
					  m_ptheblocks[tab_block_num].written_block();
					  m_ptheblocks[tab_block_num].used_block();
					  tab_block_num=get_blank_block(TABLE);
					  m_ptheblocks[tab_block_num].m_being_used=1;
					  m_ptheblocks[tab_block_num].m_name = tablefilename;
					  m_ptheblocks[tab_block_num].m_offset_number=tab_total_block;
					  tab_total_block++;
					  t1=m_ptheblocks[tab_block_num].m_address;
					  it1=(int*)t1;
					  *it1=m_ptheblocks[tab_block_num].m_offset_number;
				  }
                  else 
				  {
					  tab_block_num=get_block(TABLE,tablefilename,block_no);
					  m_ptheblocks[tab_block_num].m_being_used=1;
				  }
                   point=m_ptheblocks[tab_block_num].m_address+recordlen*(1+offset_in_block);
                   for(current_value=values;current_value!=NULL;current_value=current_value->next)
				   {
		                  strcpy(point,current_value->value);
		                  point=point+current_value->length;
				   }
	             	
					m_ptheblocks[tab_block_num].written_block();
	            	m_ptheblocks[tab_block_num].used_block();
					return record_number;
			   }
          m_ptheblocks[map_block_num].used_block();
		}
		//���������ʾ���е�MAP�Ѿ�������
        map_block_num=get_blank_block(MAP);
		m_ptheblocks[map_block_num].m_being_used=1;
		m_ptheblocks[map_block_num].m_index_table=MAP;
		m_ptheblocks[map_block_num].m_name = tablefilename;
	    m_ptheblocks[map_block_num].m_offset_number=map_total_block;
		p=m_ptheblocks[map_block_num].m_address;

		temp=p;
		*temp='1';////��BLOCKͷ����
		m_ptheblocks[map_block_num].written_block();
		m_ptheblocks[map_block_num].used_block();
       
		map_total_block++;
		map_block_num=get_block(MAP,mapfilename,0);
         m_ptheblocks[map_block_num].m_being_used=1;
	    p=m_ptheblocks[map_block_num].m_address;
	    intp=(int*)p;
		*intp=map_total_block;
		m_ptheblocks[map_block_num].written_block();
		m_ptheblocks[map_block_num].used_block();
	    
		record_number=(map_total_block-2)*BLOCK_SIZE+1;
		block_no=record_number/record_per_block+1;
        offset_in_block=record_number-record_per_block*(block_no-1);
        tab_block_num=get_block(TABLE,tablefilename,block_no);
		m_ptheblocks[tab_block_num].m_being_used=1;
        point=m_ptheblocks[tab_block_num].m_address+recordlen*(offset_in_block+1);//OFFSET��1��ʼ�㣬ͷ������¼�����涫��
        for(current_value=values;current_value!=NULL;current_value=current_value->next)
				   {
		                  strcpy(point,current_value->value);
		                  point=point+current_value->length;
				   }
	   	
		m_ptheblocks[tab_block_num].written_block();
      	m_ptheblocks[tab_block_num].used_block();			
		return record_number;         
	}
	return 0;
}






