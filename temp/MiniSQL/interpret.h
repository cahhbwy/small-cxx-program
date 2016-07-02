#ifndef __interpret_h__
#define __interpret_h__

//#include "catalog_manager.h"

class CInterpret {
public:
	//������,���е���������Է�ӳ������һ������
	unsigned int m_operation;		//Ҫִ�еĲ�����������,�ú��ʾ
	CString m_tabname;		//Ҫ�����ı����
	CString m_indname;		//Ҫ������������
	CString m_filename;		//Ҫ�������ļ���,execfile�õ�
	column *m_cols;				//Ҫ��������������
	condition *m_conds;			//Ҫ�Ƚϵ�where�־������
	insertvalue *m_values;		//Ҫ�����ֵ����

	//static CCatalogManager Catalog;

	CInterpret(){
		m_operation = UNKNOWN;
		m_tabname = "";
		m_indname = "";
		m_filename = "";
		m_cols = NULL;
		m_conds = NULL;
		m_values = NULL;		
	}
	~CInterpret(){}

	void Parse(char* command);
	void ExecSelect();
	void ExecDelete();
	void ExecDropTable();
	void ExecDropIndex();
	void ExecCreateTable();
	void ExecCreateIndex();
	void ExecInsert();
	void ExecFile();
	void Execute();
	void initcol(column *p);
	void DumpTree();

protected:
	void initcond(condition *p);
	void initvalue(insertvalue *p);
	void MakeInitilate(){
		m_operation = UNKNOWN;
		m_tabname = "";
		m_indname = "";
		m_filename = "";
		m_cols = NULL;
		m_conds = NULL;
		m_values = NULL;	
	}

	bool GetWord(CString& src, CString& des);
	bool GetStr(CString& src, CString& des);

	short int IsInt(const char *);
	short int IsFloat(char *input);
};

#endif
