
#include "stdafx.h"
#include "index_manager.h"

using namespace std;

int CIndexManager::keycompare(char *a,char *b, int tag)//a>=b 返回1
{   
    char *cha; char *chb; char tempa,tempb;
	int numa,numb;
    double floata,floatb;

	switch(tag)
	{
	case CHAR:
		   cha=a;
		   chb=b;
		   for(;*cha!='\0'&&(*chb!='\0');cha++,chb++)
		   {    
			   tempa=*cha; tempb=*chb;
			   if(tempa>='A'&&tempa<='Z')
				   tempa=tempa+32;
			   if(tempb>='B'&&tempb<='Z')
				   tempb=tempb+32;
			   if(tempa>tempb)
				   return 1;
			   if(tempa<tempb)
				   return 0;
		   }
		   if(*cha=='\0'&&*chb=='\0')
			   return 1;//相等
		   else if( *cha!='\0'&&*chb=='\0')
			   return 1;//a>b;
		   else return 0;

           break;
	case INT:
		   numa=atoi(a);
		   numb=atoi(b);
		   if(numa>=numb)  return 1;
		   else return 0;
		   break;
	case FLOAT:
		   floata=atof(a);
		   floatb=atof(b);
		   if(floata>=floatb) return 1;
		   else return 0;
		   break;
	}
	return -1;
}

int CIndexManager::quick_insert(char *value,int tag,int blocknumber,int offsetinblock)//tag为比较方式
{
	unit_node *currentnode,*save_pointer;
	unit_node *newnode = new unit_node;
	newnode->block_number=blocknumber;
	newnode->offset_in_block=offsetinblock;
	strcpy(newnode->key,value);
	if(this->m_count>=31)
	{
		printf("mistake in using this function\n");
		return 0;
	}
	else
	{
		currentnode=this->m_first;
		while(currentnode!=NULL&&keycompare(newnode->key,currentnode->key,tag)==1)
			save_pointer=currentnode;
			currentnode=currentnode->next;
	}
	save_pointer->next=newnode;
	newnode->next=currentnode;
	this->m_count++;
	return 1;
}

int CIndexManager::insert_entry(char *value,int tag,int blocknumber,int offsetinblock)
{
	CIndexManager *currenthead=this,*newhead,*head_save;
	unit_node *newnode;

    for(;currenthead!=NULL && keycompare(value,currenthead->m_minkey,tag)==1;currenthead=currenthead->m_next)
		head_save=currenthead;
	if(head_save->m_count<31)
	{
		if(head_save->quick_insert(value,tag,blocknumber,offsetinblock)==1)
			return 1;
		else return 0;
	}
	else
	{
        newhead = new CIndexManager;
	    newnode = new unit_node;
	    newhead->m_count=1;
		
		strcpy(newhead->m_minkey,value);
		newhead->m_first=newnode;
		newhead->m_next=currenthead;
		head_save->m_next=newhead;

		newnode->next=NULL;
		strcpy(newnode->key,value);
		newnode->block_number=blocknumber;
		newnode->offset_in_block=offsetinblock;
		return 1;
	}

	return 0;
}

short int CIndexManager::CreateIndex(column *cols, unsigned int recordlen, unsigned int recordnum)
{
	int table_blocknum=0;//记录当前是TABLE 第几块BLOCK
	int index_blocknum=0;//记录当前是INDEX 第几块BLOCK
	int map_blocknum=0;
	unsigned int current_record=1;//当前第几条记录
	CString this_table,this_index;
	int currentblock;
	int table_total_block;
	int offset_in_block;
	int records_per_block=BLOCK_SIZE/recordlen;
	insertvalue value;
	int record_number;	
	char *p,*temp;
	int *intp;

	//this_index = m_indexname + ".idx";
	//this_table = m_tablename + ".tab";

	CString name = (LPCTSTR)m_tablename;
	this_table = name + ".tab"; 
	name = (LPCTSTR)m_indexname;
	this_index = name + ".idx";
   
    currentblock=get_blank_block(INDEX);
	m_ptheblocks[currentblock].m_being_used=1;
	m_ptheblocks[currentblock].m_name=this_index;
	m_ptheblocks[currentblock].m_offset_number=0;
	p=m_ptheblocks[currentblock].m_address;
	intp=(int*)p;
	*intp=1;
	intp++;
	*intp=-1;//下一块号
	p=p+8;
	strcpy(p,cols->colname);

	m_ptheblocks[currentblock].written_block();
	m_ptheblocks[currentblock].used_block();
	
	currentblock=get_block(TABLE,this_table,0);
	if(currentblock==-1)
	{
		printf("can not find the proper table for index\n");
		return 0;
	}
	m_ptheblocks[currentblock].m_being_used=1;
	p=m_ptheblocks[currentblock].m_address;
	intp=(int*)p;
    table_total_block=*intp;
	intp++;
	m_ptheblocks[currentblock].used_block();
	////
	for(table_blocknum=1;table_blocknum<=table_total_block;table_blocknum++)
	{ 
		 currentblock=get_block(TABLE,this_table,table_blocknum);
		 m_ptheblocks[currentblock].m_being_used=1;
		 p=m_ptheblocks[currentblock].m_address+recordlen*2;
		 for(;p<m_ptheblocks[currentblock].m_address+BLOCK_SIZE;p=p+recordlen)
		 {
			if(*p!='$')
			{

				offset_in_block=(p-m_ptheblocks[currentblock].m_address)/recordlen-1;
				record_number=records_per_block*(m_ptheblocks[currentblock].m_offset_number-1)+offset_in_block;
				temp=p+cols->coloffset;
				value.length=cols->collength;
				value.next=NULL;
				value.type=cols->type;
				strcpy(value.value,temp);
                
				if(InsertIndex(&value, record_number)==0)
				{
					printf("Error when insert value %s into index",value.value);
					return 0;
				}
				
			}
		 }
		 m_ptheblocks[currentblock].used_block();;

	}
     printf("The index on attribute '%s' of table '%s' has been created successfully\n", cols->colname,m_tablename);
	 return 1;
}

int CIndexManager::save_index()
{
	CIndexManager *currenthead=this;
	unit_node *currentnode;
	int current_block;
	int block_count=0;
	char *p; int *intp;
	for(;currenthead!=NULL;currenthead=currenthead->m_next) {
        block_count++; 
		current_block=get_blank_block(INDEX);
		if(current_block==-1) {
			printf("can't allocate memory while saving\n");
			return 0;
		}
		m_ptheblocks[current_block].m_being_used=1;
		m_ptheblocks[current_block].m_name = m_indexname;
		m_ptheblocks[current_block].m_index_table=INDEX;
		m_ptheblocks[current_block].m_offset_number=block_count;
		p=m_ptheblocks[current_block].m_address;
		for(;p<m_ptheblocks[current_block].m_address+BLOCK_SIZE;p++)
			*p='$';
		intp=(int*)p;
		*intp=currenthead->m_blocknumber;
		intp++;
		*intp=currenthead->m_count;
		intp++;
		*intp=currenthead->m_next_block_number;
		p=p+12;
		strcpy(p,currenthead->m_minkey);
		p=m_ptheblocks[current_block].m_address;
		
		for(currentnode=currenthead->m_first;currentnode!=NULL;currentnode=currentnode->next) {
			p=p+INDEX_RECORD_LEN;
			intp=(int*)p;
			*intp=currentnode->block_number;
			intp++;
			*intp=currentnode->offset_in_block;
			p=p+8;
			strcpy(p,currentnode->key);
		}

        m_ptheblocks[current_block].written_block();
		m_ptheblocks[current_block].used_block();

	}
	
	return 1;
}

void CIndexManager::freespace()
{
	CIndexManager *current_head1,*current_head2;
	unit_node *current_node1,*current_node2;
	for(current_head1=this;current_head1!=NULL;)
	{
		for(current_node1=current_head1->m_first;current_node1!=NULL;)
		{
            current_node2=current_node1->next;
			delete current_node1;
			current_node1=current_node2;
		}
		current_head2=current_head1->m_next;
		delete current_head1;
		current_head1=current_head2;
	}

}

short int CIndexManager::SelectIndex(condition *conds, column *cols, unsigned int recordlen)
{
	CString this_table,this_index;
    int blocknumber,block_save,nextblock_number,recordblock;
	char *p,*min_key,*key,*attr_p;
	char *recordpoint;
	int  *intp;
	int destine_blocknum,destine_offset;
	column *col=cols;

	//this_index = m_indexname + ".idx";
	//this_table = m_tablename + ".tab";

	CString name = (LPCTSTR)m_tablename;
	this_table = name + ".tab"; 
	name = (LPCTSTR)m_indexname;
	this_index = name + ".idx";
	
    if(conds->cond!=EQ)
	{
		printf("compare condition error\n");
		return 0;
	}
	blocknumber=get_block(
		INDEX,this_index,0);
	if(blocknumber==-1)
	{
		printf("memory access error");
		return 0;
	}
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address;
	intp=(int*)p;
	intp++;
    nextblock_number=*intp;
	m_ptheblocks[blocknumber].used_block();
	 
	blocknumber=get_block(INDEX,this_index,nextblock_number);
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address;
	intp=(int*)(p+8);
	nextblock_number=*intp;
	min_key=p+12;
	while(keycompare(conds->value,min_key,conds->type)==1)
	 {
		 block_save=m_ptheblocks[blocknumber].m_offset_number;
		 m_ptheblocks[blocknumber].used_block();
		 if(nextblock_number==-1)//到了最后一块
			 break;
		 blocknumber=get_block(INDEX,this_index,nextblock_number);
         m_ptheblocks[blocknumber].m_being_used=1;
		 p=m_ptheblocks[blocknumber].m_address;
	     intp=(int*)(p+8);
         nextblock_number=*intp;
	     min_key=p+12;
	}
	m_ptheblocks[blocknumber].used_block();
	blocknumber=get_block(INDEX,this_index,block_save);//找到目标块
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address+INDEX_RECORD_LEN;
	while(col) 
	{
		int remain = col->collength - strlen(col->colname);
		printf("%s", col->colname);
		for(; remain ; remain --)
			printf(" ");
		col = col->next;
	 }
	printf("\n");
    while(p<m_ptheblocks[blocknumber].m_address+BLOCK_SIZE)
	 {
		key=p+8;
		if(strcmp(key,conds->value)==0)
		{
			intp=(int*)p;
			destine_blocknum=*intp;
			intp++;
			destine_offset=*intp;
            recordblock=get_block(TABLE,this_table,destine_blocknum);
			m_ptheblocks[recordblock].m_being_used=1;
			recordpoint=m_ptheblocks[recordblock].m_address+recordlen*(destine_offset+1);
			 
			col = cols;
			while(col)
			{
				int remain;
				char value[255];
				strcpy(value, attr_p=recordpoint+col->coloffset);
				remain = col->collength - strlen(attr_p);
				printf("%s", value);
				for(; remain ; remain --)
					printf(" ");
				col = col->next;
			}
			printf("\n");
			printf("1 record(s) are selected from table '%s'!\n", m_tablename);

			m_ptheblocks[recordblock].used_block();
			 return 1;
		}
		p=p+INDEX_RECORD_LEN;
	}
	printf("can not find this record by index\n");
	return 0; 
}

short int CIndexManager::DeleteIndex(condition *conds,unsigned int recordlen)
{
	CString  this_table,this_index,this_map;
    int blocknumber,block_save,nextblock_number,recordblock,mapblock;
	char *p,*min_key,*key;
	char *record_point;
	int  *intp;
	int destine_blocknum,destine_offset;
	char *temp;
	int records_per_block=BLOCK_SIZE/recordlen;

	//this_index = m_indexname + ".idx";
	//this_table = m_tablename + ".tab";
	//this_map = m_tablename + ".map";

	CString name = (LPCTSTR)m_tablename;
	this_table = name + ".tab"; 
	this_map = name + ".map";
	name = (LPCTSTR)m_indexname;
	this_index = name + ".idx";

    if(conds->cond!=EQ)
	{
		printf("compare condition error\n");
		return 0;
	}
	 blocknumber=get_block( INDEX,this_index,0);
	 if(blocknumber==-1)
	 {
		 printf("memory access error");
		 return 0;
	 }
	 m_ptheblocks[blocknumber].m_being_used=1;
	 p=m_ptheblocks[blocknumber].m_address;
	 intp=(int*)(p+4);
     nextblock_number=*intp;
	 m_ptheblocks[blocknumber].used_block();
     blocknumber=get_block(INDEX,this_index,nextblock_number);
     m_ptheblocks[blocknumber].m_being_used=1;
	 p=m_ptheblocks[blocknumber].m_address;
	 intp=(int*)(p+8);
	 nextblock_number=*intp;
	 min_key=p+12;
	 while(keycompare(conds->value,min_key,conds->type)==1)
	 {
		 block_save=m_ptheblocks[blocknumber].m_offset_number;
		 m_ptheblocks[blocknumber].used_block();
         if(nextblock_number==-1)//到了最后一块
			 break;
		 blocknumber=get_block(INDEX,this_index,nextblock_number);
         m_ptheblocks[blocknumber].m_being_used=1;
		 p=m_ptheblocks[blocknumber].m_address;
	     intp=(int*)(p+8);
         nextblock_number=*intp;
	     min_key=p+12;
	 }
	 m_ptheblocks[blocknumber].used_block();
	 blocknumber=get_block(INDEX,this_index,block_save);//找到目标块
	 m_ptheblocks[blocknumber].m_being_used=1;
	 p=m_ptheblocks[blocknumber].m_address+INDEX_RECORD_LEN;
     while(p<m_ptheblocks[blocknumber].m_address+BLOCK_SIZE)
	 {
		 key=p+8;
		 if(strcmp(key,conds->value)==0)//找到
		 {
			 int map_block_number,map_in_offset,record_num;
			 intp=(int*)p;
			 destine_blocknum=*intp;
			 intp++;
			 destine_offset=*intp;
			 record_num=records_per_block *(destine_blocknum-1)+destine_offset;
             recordblock=get_block( TABLE,this_table,destine_blocknum);
             m_ptheblocks[recordblock].m_being_used=1;
			 record_point=m_ptheblocks[recordblock].m_address+recordlen*(destine_offset+1);
			 //删除RECORD
             for(temp=record_point;temp<record_point+recordlen;temp++)
				 *temp='$';
			
			 m_ptheblocks[recordblock].written_block();
			 m_ptheblocks[recordblock].used_block();
			 //删除INDEX
             for(temp=p;temp<p+INDEX_RECORD_LEN;temp++)
			    *temp='$';
			 written_block();
			 m_ptheblocks[blocknumber].written_block();
			 m_ptheblocks[blocknumber].used_block();
			 //删除MAP
             map_block_number=record_num/BLOCK_SIZE+1;
			 map_in_offset=record_num-(map_block_number-1)*BLOCK_SIZE;
			 mapblock=get_block(MAP,this_index,map_block_number);
			 if(mapblock==-1)
			 {
		      printf("memory access error");
		      return 0;
			 }
			 m_ptheblocks[mapblock].m_being_used=1;
			 p=m_ptheblocks[mapblock].m_address;
			 temp=p+map_in_offset-1;//因为OFFSET从1算起
			 *p='$';
			 written_block();
			 m_ptheblocks[mapblock].written_block();
			 m_ptheblocks[mapblock].used_block();
			 return 1;
		 }
		 p=p+INDEX_RECORD_LEN;
	 }
	 printf("Can not find this record by index\n");
	 return 0;
}

short int CIndexManager::InsertIndex(insertvalue *value, unsigned int recordnum)
{
	CString this_table,this_index,this_map;
	int destine_blocknum,destine_offset;
	int blocknumber,nextblock,save_number=0,newblock,firstblock,first_block;
	char *p,*temp,*p1,*p2;
	int *intp,*intp1,*intp2;
	int newblocknumber;
	int total_block_number;
	int start_block=1;

	//this_index = m_indexname + ".idx";
	//this_table = m_tablename + ".tab";
	//this_map = m_tablename + ".map";

	CString name = (LPCTSTR)m_tablename;
	this_table = name + ".tab"; 
	this_map = name + ".map";
	name = (LPCTSTR)m_indexname;
	this_index = name + ".idx";
		
	destine_blocknum=recordnum/RECORDS_PER_BLOCK+1;
	destine_offset=recordnum%RECORDS_PER_BLOCK;		//要塞入RECORD如何在文件中定位？？？？？？？？？？？？？？？？

	blocknumber=get_block(INDEX,this_index,0);
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address;
	intp=(int*)p;
	total_block_number=*intp;
	intp++;
	start_block=*intp;
	if(total_block_number==1)
	{
		intp--;
		(*intp)++;
		intp++;
		*intp=1;
		start_block=*intp;

		m_ptheblocks[blocknumber].written_block();
		m_ptheblocks[blocknumber].used_block();
		blocknumber=get_blank_block(INDEX);
		m_ptheblocks[blocknumber].m_being_used=1;
		m_ptheblocks[blocknumber].m_name = this_index;
		m_ptheblocks[blocknumber].m_offset_number=1;
		p=m_ptheblocks[blocknumber].m_address;
		for(temp=p;temp<p+BLOCK_SIZE;temp++)
			*temp='$';
		intp=(int*)p;
		*intp=1;//HEAD中的块号
		intp++;
		*intp=1;//HEAD中的COUNT
		intp++;
		*intp=-1;//下一块的块号
		p=m_ptheblocks[blocknumber].m_address+12;
		strcpy(p,value->value);
		//写NODE
		p=m_ptheblocks[blocknumber].m_address+INDEX_RECORD_LEN;
		intp=(int*)p;
		*intp=destine_blocknum;
		intp++;
        *intp=destine_offset;
		p=p+8;
		strcpy(p,value->value);
		
		m_ptheblocks[blocknumber].written_block();
		m_ptheblocks[blocknumber].used_block();
		return 1;
	}
	else
	{
	save_number=0;
	blocknumber=get_block( INDEX,this_index,start_block);
	if(blocknumber==-1)
	{
	 printf("memory access error");
	 return 0;
	}
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address+12;
	while(keycompare(value->value,p,value->type)==1)
	{
		intp=(int*)m_ptheblocks[blocknumber].m_address;
		save_number=*intp;
		intp=intp+2;
		nextblock=*intp;
		m_ptheblocks[blocknumber].used_block();
		if(nextblock==-1)
		 break;
		blocknumber=get_block(INDEX,this_index,nextblock);
		if(blocknumber==-1)
		{
		printf("memory access error");
		return 0;
		}
		m_ptheblocks[blocknumber].m_being_used=1;
		p=m_ptheblocks[blocknumber].m_address+12;
	}
	m_ptheblocks[blocknumber].used_block();
	if(save_number==0)
	{
		newblock=get_blank_block(INDEX);
		first_block=get_block(INDEX,this_index,0);
		m_ptheblocks[first_block].m_being_used=1;
		m_ptheblocks[newblock].m_being_used=1;
		m_ptheblocks[newblock].m_name=this_index;
		m_ptheblocks[newblock].m_offset_number=total_block_number;
		total_block_number++;
		p2=m_ptheblocks[first_block].m_address;
		intp2=(int*)p2;
		(*intp2)++;
		intp2++;
		nextblock=*intp2;
		*intp2=m_ptheblocks[newblock].m_offset_number;

		m_ptheblocks[first_block].written_block();
		m_ptheblocks[first_block].used_block();
		p1=m_ptheblocks[newblock].m_address;
		for(temp=p1;temp<p1+BLOCK_SIZE;temp++)
			*temp='$';
		//写新块的HEAD
		intp1=(int*)p1;
		*intp1=m_ptheblocks[newblock].m_offset_number;//HEAD中的块号
		intp1++;
		*intp1=1;//HEAD中的COUNT
		intp1++;
		*intp1=nextblock;//下一块的块号????????????
		p1=m_ptheblocks[newblock].m_address+12;
		strcpy(p1,value->value);
		//写NODE
		p1=m_ptheblocks[newblock].m_address+INDEX_RECORD_LEN;
		intp1=(int*)p1;
		*intp1=destine_blocknum;
		intp1++;
        *intp1=destine_offset;
		p1=p1+8;
		strcpy(p1,value->value);
		
		m_ptheblocks[newblock].written_block();
		m_ptheblocks[newblock].used_block();
		return 1;
	}
	//save_number是目标的块,nextblock是其下一块
	blocknumber=get_block(INDEX,this_index,save_number);
	m_ptheblocks[blocknumber].m_being_used=1;
	p=m_ptheblocks[blocknumber].m_address;
	intp=(int*)p;
	intp++;
	if(*intp<31)
	{
		for(p1=p+INDEX_RECORD_LEN;*p1!='$';p1=p1+INDEX_RECORD_LEN)
		;
		if(p1>=m_ptheblocks[blocknumber].m_address+BLOCK_SIZE)
		{
			printf("error:this m_ptheblocks is already full\n");
			return 0;
		}
		intp1=(int*)p1;
		*intp1=destine_blocknum;
		intp1++;
		*intp1=destine_offset;
		temp=p1+8;
		strcpy(temp,value->value);
		(*intp)++;

		m_ptheblocks[blocknumber].written_block();
		m_ptheblocks[blocknumber].used_block();

		return 1;
	}
	else
	{
        firstblock=get_block(INDEX,this_index,0);
		m_ptheblocks[firstblock].m_being_used=1;
		p1=m_ptheblocks[firstblock].m_address;
		intp1=(int*)p1;
        newblocknumber=*intp1;//因为从0算起，差个1
		*intp1++;
	
		m_ptheblocks[firstblock].written_block();
		m_ptheblocks[firstblock].used_block();
		newblock=get_blank_block(INDEX);
		m_ptheblocks[newblock].m_being_used=1;
		m_ptheblocks[newblock].m_name=this_index;
		m_ptheblocks[newblock].m_offset_number=newblocknumber;
		p1=m_ptheblocks[newblock].m_address;
		for(temp=p1;temp<p1+BLOCK_SIZE;temp++)
			*temp='$';
		//写新块的HEAD
		intp1=(int*)p1;
		*intp1=newblocknumber;//HEAD中的块号
		intp1++;
		*intp1=1;//HEAD中的COUNT
		intp1++;
		*intp1=nextblock;//下一块的块号
		p1=m_ptheblocks[newblock].m_address+12;
		strcpy(p1,value->value);
		//写NODE
		p1=m_ptheblocks[newblock].m_address+INDEX_RECORD_LEN;
		intp1=(int*)p1;
		*intp1=destine_blocknum;
		intp1++;
        *intp1=destine_offset;
		p1=p1+8;
		strcpy(p1,value->value);
	
		m_ptheblocks[newblock].written_block();
		m_ptheblocks[newblock].used_block();
		
		intp++;
		*intp=newblocknumber;//刚才满的那块的下一块应该是这个新块
	
		m_ptheblocks[blocknumber].written_block();
		m_ptheblocks[blocknumber].used_block();

  		return 1;

	}
	return 0;
	}
}
