#ifndef _RECORDMANAGER_H
#define	_RECORDMANAGER_H
#define _CRT_SECURE_NO_WARNINGS
#include "minisql.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

//�����﷨���еĽڵ㡣ÿ���ڵ㴢��һ���ֶ���Ϣ
typedef struct CreateTableNode FieldNode;
struct CreateTableNode{
	string attr_name;  //�ֶ���
	INT32 attr_len;    //�ֶδ�С
	int attr_type;     //�ֶ����ͣ�int/char/float��
	int attr_def_type; //�ֶζ������ͣ�primary key/unique/not null��
	FieldNode* next;   //��һ�ֶ�
};


//���������﷨��
typedef struct CreateTableTree FieldTree;
struct CreateTableTree{
	string tablename;     //����
	FieldNode* field;  //�����ֶ�
};

//����������Ϣ�Ľṹ
typedef struct CreateIndexStruct IndexStruct;
struct CreateIndexStruct{
	string index_name;  //������
	string table_name;  //����������
	string attr_name;   //�����������ֶ�
};

//��������Ԫ��Ľṹ
typedef struct InsertTupleStruct TupleStruct;
struct InsertTupleStruct{
	string tuplevalues[32];
	int value_num; //Ԫ����������
};

struct Condition{
	string compare_attr;     //Ҫ��ѯ��������
	string compare_type;     //��ѯ���ͣ��磺==, >=, <=�ȣ�
	string compare_value;    //��ѯ��ֵ   
};



//��¼���������ļ�¼
extern vector <char*> select_list;

int isBlockFull(char* buf, int reclen);  //�ж�buf���Ƿ��п�λ���볤��Ϊreclen�ļ�¼
int totalBlockNum(string filename);  //�ļ����Ѿ��еĿ���
string getUserCommand(); //����û�����
void handleWithCommand(string command); //��������
FieldTree* createCreateTableTree(string tbname, string fieldmodule);//����Ϊtbname, �ֶ���Ϊfieldmodule���� (�����ڣ��������ţ��Ĳ���)
IndexStruct* createCreateIndexStruct(string idxname, string info); //��������������Ϣ�Ľṹ
TupleStruct* CreateTupleStruct(string tupinfo); //��������Ԫ��������ֵ�Ľṹ
list<Condition> CreateConditionlist(string condinfo); //���������ѯ�����Ľṹ
void PushPosition(INT32 BlockAddr, INT32 Offset, char* key, int type, INT32 recordLength);
void PopPosition(INT32 position, INT32 recordLength, INT32 &BlockAddr, INT32 &Offset);
INT32 PushPosition(INT32 BlockAddr, INT32 Offset, INT32 recordLength);


//Attribute����¼����ÿ���ֶεĶ�����Ϣ
class Attribute{
public:
	int attr_id;        //���еĵڼ����ֶ�
	string attr_name;   //�ֶ���
	int attr_type;      //�ֶε���������(int, char, float)
	int attr_def_type;  //�ֶεĶ�������(PRIMARY, UNIQUE, NULL)
	INT32 attr_len;     //�ֶδ�С���ֽ�Ϊ��λ��
	Attribute(){}
	~Attribute(){}
};


//Table����¼��Ķ�����Ϣ
class Table{
	friend class Tuple;
	friend class RBlock;
private:
	string table_name;    //����
	INT32 attr_num;       //�����ֶ���
	INT32 record_len;     //����ÿ����¼�ĳ���
	Attribute attrs[32];  //�����ֶ�
public:
	Table(const string tbname) : table_name(tbname){}		//��ʼ������
	void initializeTable(FieldTree* T);  //��ʼ������һ������interpreter���
	//catalog managerֻ�������Ѿ���ʼ������table����
	void CalcRecordLen();       //��ñ���ÿ����¼�ĳ���
	bool isTableExist();	//���ݷ���ֵ�жϱ��Ƿ��Ѿ�����
	bool isAttriExist(const string attname);  //�жϱ����Ƿ�����Ϊattname���ֶ�
	bool isAttriUnique(const string attname); //�жϱ�����Ϊattname���ֶ��Ƿ�Ψһ
	bool createTable();              //����
	bool dropTable();                //ɾ����

	string getTableName(){ return table_name; }
	void getTableInfo();             //��֪������Ҫ��ñ����Ϣ
	string getPrimaryKey();          //���ر��primary key
	int getPrimaryKeySize();         //���ر��primary key�Ĵ�С
	int getAttrNum() { return attr_num; }         //���ر��������
	Attribute getAttr(int i){ return attrs[i]; }  //���������±귵��������
	Attribute getAttr(string AttrName){      
		for (int i = 0; i < attr_num; i++){
			if (attrs[i].attr_name == AttrName){
				return attrs[i];
			}
		}
        return attrs[0];
	}
	string getAttrIndex(string attrname);         //�ж�attrname�Ƿ���������������򷵻�������
	~Table(){}
};


//Index����¼����������
class Index{
private:
	string index_name;  //������
	string table_name;  //����������
	string attr_name;   //�����������ֶ�
public:
	Index(const string idxname) : index_name(idxname){}
    void initializeIndex(IndexStruct* Idx); //��ʼ��������Ϣ����һ������interpreter���

	bool isIndexExist();  //�������������ж������Ƿ��Ѵ���
	bool createIndex();   //��������
	bool dropIndex();     //ɾ������
	string getIndexName(){
		return index_name;
	}
	string FindIndexTable(){ return table_name; }   //����������Ӧ�ı���

	~Index(){}
};

//Tuple��Ԫ��
//�磺('zhangjin', '3140105196', 18, 'Computer Science')��һ��Ԫ��
class Tuple{
private:
	string tuple_values[32];  //Ԫ�������Ե�ֵ
	Table T;                  //Ԫ�������ı�
public:
	Tuple(string tbname) :T(tbname){}
	void initializeTuple(TupleStruct* tup); //��ʼ��Ԫ�顣��һ������interpreter���
	//��ʼ����ɺ󣬵õ�Ԫ������Ե�ֵ�Լ���Ԫ�������ı�
	char* convertTuple();  //��Ԫ��ת��Ϊchar*���ͣ�������뻺����
	bool IsUniqueKeyExisted(); //�жϸ�Ԫ���UNIQUR��PRIMARY�����Ƿ��Ѿ����ڡ����Ѿ������򷵻�1�������ظ�����

	bool Insert(INT32 &position);  //��Ԫ���������reco�ļ��С�����ʱ��Ҫ�ڼ�¼ǰ��һ���ֽڵ��ж��Ƿ�ɾ��λ���ڼ�¼ĩ��һ��'\0'
	void Select(vector<int> offsetlist, list<Condition> conditionlist);  //��reco�ļ���ѡ�����������ļ�¼
	int Delete(vector<int> offsetlist, list<Condition> conditionlist);  //��reco�ļ���ɾ�����������ļ�¼������ɾ���ļ�¼��

	string getTupleValue(int i){
		return tuple_values[i];
	}
	void printSelectResult();  //�����Դ�ӡѡ����Ľ��
	void FetchAll(int AttrId);           //�ҵ����м�¼����Ϣ������TupleIndex��
	int recordLen(){   
		return T.record_len;
	}
	int getPrintLength(int *length);
	void PrintTuple(int totalLength, int FirstOrEnd, int *length);
	void printValue(int i, int type, int width);

	~Tuple(){}
};

//Block����¼һ����������Ϣ
class RBlock{
	friend class Tuple;
public:
	Table T;
	char* buffer;   //ָ�򻺳���ָ��
	char* ptr;      //�������ָ���λ��
	int cnt;        //�������ʣ��ɶ��ֽ���
	int is_Changed; //��¼������Ƿ��޸Ĺ�
	INT32 BlockAddr;

	RBlock(string tablename) : T(tablename){
		buffer = new char[BLOCKSIZE];  //��ʼ�������
		memset(buffer, 0, BLOCKSIZE);
		ptr = &buffer[0];   //ָ��һ��ʼָ�򻺴���ͷ��
		is_Changed = 0;
		BlockAddr = 0;
	}
	int getBlock(string tablename, int offsetB, int is_want_empty);  //���ļ��е�offsetB���λ�ô���ʼ��ȡһ�黺����С�����ݻ��ҵ���һ���տ顣
	void writeBackBlock(const char* filename, int offsetB); //������������ݴ�offsetB���λ�ô���ʼд���ļ���
	~RBlock();
};

#endif