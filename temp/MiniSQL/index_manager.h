#ifndef __index_h__
#define __index_h__

#include "MiniSQL.h"
#include "buffer_manager.h"

typedef struct struct_unit_node
{
	int block_number;//这条纪录是第几块的
	int offset_in_block;//从1开始算
	char key[VALUE_LENGTH];
	struct struct_unit_node *next;
}unit_node;

class CIndexManager : public CBufferManager
{
public:
	CString m_tablename;
	CString m_indexname;

	CIndexManager(){}
	CIndexManager(CString indexname, CString tablename){
		m_tablename = tablename;
		m_indexname = indexname;
	}
	~CIndexManager(){}

	short int SelectIndex(condition *conds, column *cols, unsigned int recordlen);
	short int DeleteIndex(condition *conds, unsigned int recordlen);
	short int CreateIndex(column *cols, unsigned int recordlen, unsigned int recordnum);
	short int InsertIndex(insertvalue *value, unsigned int recordnum);
	int keycompare(char *a,char *b, int tag);

protected:
	int m_blocknumber;
	int m_count;
	CIndexManager *m_next;
	unit_node *m_first;
	char m_minkey[VALUE_LENGTH];
	int m_next_block_number;

	void freespace();
	int save_index();
	int insert_entry(char *value,int tag,int blocknumber,int offsetinblock);
	int quick_insert(char *value,int tag,int blocknumber,int offsetinblock);
};

#endif