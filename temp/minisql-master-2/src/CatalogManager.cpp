#include "Interpreter.h"
#include <fstream>

//�˴�ֻ���ڵ���
//������Ҫͨ��interpreter��ʵ��
/*void Table::initializeTable(const string createTableCommand)
{
	attr_num = 4;
	attrs[0].attr_name = "studend_id";
	attrs[0].attr_id = 0;
	attrs[0].attr_type = CHAR;
	attrs[0].attr_len = 15;
	attrs[0].attr_def_type = PRIMARY;
	
	attrs[1].attr_name = "studend_name";
	attrs[1].attr_id = 1;
	attrs[1].attr_type = CHAR;
	attrs[1].attr_len = 15;
	attrs[1].attr_def_type = 2;

	attrs[2].attr_name = "studend_age";
	attrs[2].attr_id = 2;
	attrs[2].attr_type = INT;
	attrs[2].attr_len = 4;
	attrs[2].attr_def_type = 2;

	attrs[3].attr_name = "department";
	attrs[3].attr_id = 3;
	attrs[3].attr_type = CHAR;
	attrs[3].attr_len = 20;
	attrs[3].attr_def_type = 2;

	CalcRecordLen();
}*/

/////////////////////////////////////////////////////////////////
/***************************Table��*****************************/
////////////////////////////////////////////////////////////////

//�����﷨����ʼ����
void Table::initializeTable(FieldTree* T)
{
	FieldNode* Tnode, *node;
	//�ж��Ƿ����ظ����Ƶ�����
	for (Tnode = T->field->next; Tnode != NULL; Tnode = Tnode->next){
		for (node = Tnode->next; node != NULL; node = node->next){
			if (Tnode->attr_name == node->attr_name){
				//���������ظ������ԣ�����
				cerr << "duplicated attribute is not allow." << endl;
				throw Grammer_error("create table");
			}
		}
	}
	//�����﷨����ʼ����
	int i = 0;
	for (Tnode = T->field->next; Tnode != NULL; Tnode = Tnode->next){
		attrs[i].attr_name = Tnode->attr_name;
		attrs[i].attr_type = Tnode->attr_type;
		attrs[i].attr_len = Tnode->attr_len;
		attrs[i].attr_def_type = Tnode->attr_def_type;
		i++;
	}
	attr_num = i;
	CalcRecordLen();  //����record_len���Ե�ֵ
}

//�������»򴴽�����ļ�
bool Table::createTable()
{
	string filename;
	ofstream outfile;
	if (isTableExist()){  //�ñ������ݿ����Ѵ���
		cerr << "The table has already been existed!" << endl;
		return false;     //����ʧ�ܣ�����false
	}
	//�����ڣ������±�

	//���´���������ļ�
	outfile.open("tablelist.txt", ios::app); //��׷��ģʽ��
	if (!outfile) throw File_openfail("tablelist");

	outfile << table_name << endl;  //���ļ����������
	outfile.close();

	//�½�����ñ���ֶ���Ϣ���ļ�
	filename = table_name + "_tableinfo.txt";
	outfile.open(filename.c_str(), ios::out);
	if (!outfile) throw File_openfail(filename.c_str());

	for (int i = 0; i < attr_num; i++){  //������ֶ���Ϣ�����ļ���
		outfile << attrs[i].attr_name << " ";
		outfile << attrs[i].attr_def_type << " ";
		outfile << attrs[i].attr_type << " ";
		outfile << i << " ";
		outfile << attrs[i].attr_len << endl;
	}
	outfile.close();

	//�½�����ñ�ļ�¼��Ϣ���ļ�
	filename = table_name + "_tablereco";
	outfile.open(filename.c_str(), ios::out);
	if (!outfile) throw File_openfail(filename.c_str());

	outfile.close();  //�������ɣ���ʱ����Ҫ�����κμ�¼

	return true;
}

bool Table::dropTable()
{
	string filename;
	string newTableList = "";
	ifstream infile;
	ofstream outfile;
	if (!isTableExist()){  //�ñ�����
		cerr << "The table doesn't exist!" << endl;
		return false;      //ɾ��ʧ�ܣ�����false
	}
	//����ڣ���ɾ�����������ļ�

	//ɾ��tablelist.txt�б���
	infile.open("tablelist.txt", ios::in); //��ֻ��ģʽ���ļ�����ȡ������Ϣ
	if (!infile) throw File_openfail("tablelist.txt");

	string tablename;
	while (!infile.eof()){
		getline(infile, tablename);  //ÿ��ֻ����һ��������Ϣ
		if (tablename != "" && tablename != table_name){
			//���˵�Ҫɾ���ı�������������ı�����Ϣ������newTableList��
			newTableList = newTableList + tablename + '\n';
		}
	}
	outfile.open("tablelist.txt", ios::out);  //��tablelist.txt�����
	if (!outfile) throw File_openfail("tablelist.txt");
	outfile << newTableList;  //�������ı�����Ϣ�����ļ���
	outfile.close();

	//ɾ����¼����ֶε��ļ�
	filename = table_name + "_tableinfo.txt";
	remove(filename.c_str());  //�Ƴ��ļ�

	//ɾ����¼��������������Ϣ���ļ�
	filename = table_name + "_tableindexinfo.txt";
	remove(filename.c_str());

	//ɾ����¼���м�¼���ļ�
	filename = table_name + "_tablereco";
	remove(filename.c_str());  //�Ƴ��ļ�

	return true;
}

//�жϱ��Ƿ��Ѿ����ڣ����Ѿ������򷵻�1�����򷵻�0
bool Table::isTableExist()
{
	ifstream infile;
	infile.open("tablelist.txt", ios::in);
	if (!infile){
		return false; //�ļ���û�б�������˵����û�н�����
	}

	string tablename;
	while (!infile.eof()){
		getline(infile, tablename); //��ȡһ������
		if (tablename == table_name){
			infile.close();
			return true; //�ñ��Ѵ���
		}
	}
	//�ñ�����
	infile.close();
	return false;
}

//�жϱ����Ƿ���attname�������
//����ͨ��isTableExist�����жϣ�ȷ���Ѵ��ڴ˱�
bool Table::isAttriExist(const string attname)
{
	ifstream infile;
	string filename;
	
	filename = table_name + "_tableinfo.txt";
	infile.open(filename.c_str(), ios::in);
	if (!infile) throw File_openfail(filename.c_str());

	//Ѱ�Ҽ�¼��������Ϣ��һ��
	string attrinfo;
	string attrname;
	int i;
	while (!infile.eof()){
		i = 0;
		getline(infile, attrinfo);
		while (attrinfo.at(i) != ' ') i++;
		attrname = attrinfo.substr(0, i);  //���������
		//����������attname�Ƚ�
		if (attrname == attname){
			//����������
			infile.close();
			return true;
		}
	}

	//�����ļ���û���ҵ����������������˵������û���������
	infile.close();
	return false;
}

//�ж�attname��������Ƿ�Ψһ
//attname��ͨ��isAttriExist�����жϣ�ȷ�����д����������
bool Table::isAttriUnique(const string attname)
{
	ifstream infile;
	string filename;

	filename = table_name + "_tableinfo.txt";
	infile.open(filename.c_str(), ios::in);
	if (!infile) throw File_openfail(filename.c_str());

	//Ѱ�Ҽ�¼��������Ϣ��һ��
	string attrinfo;
	string attrname;
	int attrdef = -1;
	int i;
	while (!infile.eof()){
		i = 0;
		getline(infile, attrinfo);
		while (attrinfo.at(i) != ' ') i++;
		attrname = attrinfo.substr(0, i);
		if (attrname == attname){
			attrdef = attrinfo.at(i + 1) - '0';
			break;
		}
	}

	if (attrdef == PRIMARY || attrdef == UNIQUE){
		//������Ϊ��ֵ����
		infile.close();
		return true;
	}
	else{
		infile.close();
		return false;
	}
}

void Table::CalcRecordLen()
{
	record_len = 0;
	for (int i = 0; i < attr_num; i++){
		record_len += attrs[i].attr_len;
	}
	record_len += 2;  //������һ����¼��������'\0'��β��ͬʱ��¼�ĵ�һ���ֽ�������¼�Ƿ���ɾ�������
					  //���Ա���������ֽڵĴ�С
}


//��֪������Ҫ��øñ���Ϣ
void Table::getTableInfo()
{
	ifstream infile;
	string filename;
	filename = table_name + "_tableinfo.txt";

	if (!isTableExist()){
		//������
		throw Table_Index_Error("table", table_name);
	}

	infile.open(filename.c_str(), ios::in);
	if (!infile) throw File_openfail(filename.c_str());
	
	int i = 0;
	while (!infile.eof()){
		infile >> attrs[i].attr_name;
		infile >> attrs[i].attr_def_type;
		infile >> attrs[i].attr_type;
		infile >> attrs[i].attr_id;
		infile >> attrs[i].attr_len;
		i++;
	}
	attr_num = i - 1;
	CalcRecordLen();
}


string Table::getPrimaryKey()
{
	for (int i = 0; i < attr_num; i++){
		if (attrs[i].attr_def_type == 0) 
			return attrs[i].attr_name;
	}
    return "";
}

int Table::getPrimaryKeySize()
{
	for (int i = 0; i < attr_num; i++){
		if (attrs[i].attr_def_type == 0)
			return attrs[i].attr_len;
	}
    return 0;
}


string Table::getAttrIndex(string attrname)
{
	string filename;
	filename = table_name + "_tableindexinfo.txt";

	ifstream infile;
	infile.open(filename.c_str(), ios::in);
	if (!infile){
		//�ļ������ڣ�������Ҳ�Ͳ�����
		//���ؿ��ַ���
		return "";
	}

	string idxname, atrname;
	while (!infile.eof()){
		infile >> idxname;
		infile >> atrname;
		if (atrname == attrname){
			//��������
			return idxname;
		}
	}
	//����������
	return "";
}

/////////////////////////////////////////////////////////////////
/***************************Index��*****************************/
////////////////////////////////////////////////////////////////

//��ʼ��������Ϣ
void Index::initializeIndex(IndexStruct* Idx)
{
	table_name = Idx->table_name;
	attr_name = Idx->attr_name;
}

bool Index::createIndex()
{
	if (isIndexExist()){
		//�������Ѵ���
		throw Table_Index_Error("index", index_name);
	}

	Table T(table_name);
	if (!T.isTableExist()){
		//������
		throw Table_Index_Error("table", table_name);
	}

	T.getTableInfo();
	if (!T.isAttriExist(attr_name)){
		//���Բ�����
		throw Table_Index_Error("attribute", attr_name);
	}
	if (!T.isAttriUnique(attr_name)){
		//���Բ���unique����
		throw Multip_Error(attr_name);
	}
	
	//���㴴������������
	//��indexlist.txt��׷��������
	ofstream outfile;
	outfile.open("indexlist.txt", ios::app);
	if (!outfile) throw File_openfail("indexlist.txt");
	outfile << index_name << endl;
	outfile.close();

	//�½���¼��������Ϣ���ļ�
	string filename;
	filename = index_name + "_indexinfo.txt";
	outfile.open(filename.c_str(), ios::app);
	if (!outfile) throw File_openfail(filename.c_str());
	outfile << index_name << " ";
	outfile << table_name << " ";
	outfile << attr_name << endl;
	outfile.close();

	//�½���¼�ñ�����������Ϣ���ļ�
	filename = table_name + "_tableindexinfo.txt";
	outfile.open(filename.c_str(), ios::app);
	if (!outfile) throw File_openfail(filename.c_str());
	outfile << index_name << " ";
	outfile << attr_name << endl;
	outfile.close();

	//�½������������B+�����ļ�
	filename = index_name;
	outfile.open(filename.c_str(), ios::app);
	if (!outfile) throw File_openfail(filename.c_str());
	outfile.close();

	return true;
}

//ɾ������
bool Index::dropIndex()
{
	if (!isIndexExist()){
		//���������ڣ��޷�ɾ��
		cerr << "The index doesn't exist!" << endl;
		return false;
	}
	//�������ڣ���ɾ�����������ļ�

	//ɾ��indexlist.txt��������
	ifstream infile;
	infile.open("indexlist.txt", ios::in); //��ֻ��ģʽ���ļ�����ȡ������Ϣ
	if (!infile) throw File_openfail("indexlist.txt");

	string indexname;
	string newIndexList = "";
	while (!infile.eof()){
		getline(infile, indexname);  //ÿ��ֻ����һ��������Ϣ
		if (indexname != "" && indexname != index_name){
			//���˵�Ҫɾ�������������������������Ϣ������newIndexList��
			newIndexList = newIndexList + indexname + '\n';
		}
	}
	infile.close();
	ofstream outfile;
	outfile.open("indexlist.txt", ios::out);  //��indexlist.txt�����
	if (!outfile) throw File_openfail("indexlist.txt");
	outfile << newIndexList;  //��������������Ϣ�����ļ���
	outfile.close();

	string filename;

	string tbname;
	string idname;
	//ɾ����¼������Ϣ���ļ�
	filename = index_name + "_indexinfo.txt";
	infile.open(filename.c_str(), ios::in);
	//��ȡ�������ڵı������ֶ���Ϣ
	{
		infile >> indexname >>tbname >>idname;
	}
	string thisIndexinfo = indexname + " " + idname;
	infile.close();
	remove(filename.c_str());  //�Ƴ��ļ�

	//ɾ��_tableindexinfo.txt��������Ϣ
	filename = tbname + "_tableindexinfo.txt";
	infile.open(filename.c_str(), ios::in); //��ֻ��ģʽ���ļ�����ȡ������Ϣ
	if (!infile) throw File_openfail(filename.c_str());

	string newIndexinfo = "";
	string indexinfo;
	while (!infile.eof()){
		getline(infile, indexinfo);  //ÿ��ֻ����һ��������Ϣ
		if (indexinfo != "" && indexinfo != thisIndexinfo){
			//���˵�Ҫɾ����������Ϣ�����������������Ϣ������newIndexList��
			newIndexinfo = newIndexinfo + indexinfo + '\n';
		}
	}
	infile.close();
	outfile.open(filename.c_str(), ios::out);  //��_tableindexlist.txt�����
	if (!outfile) throw File_openfail(filename.c_str());
	outfile << newIndexinfo;  //�������ĸñ��������Ϣ�����ļ���
	outfile.close();

	//ɾ�����������B+�����ļ�
	filename = index_name;
	remove(filename.c_str());  //�Ƴ��ļ�

	return true;
}


//����index�����ж�index�Ƿ����
bool Index::isIndexExist()
{
	//��indexlist.txt��Ѱ��������
	ifstream infile;
	string name;
	infile.open("indexlist.txt", ios::in);
	if (!infile) return false;
	while (!infile.eof()){
		infile >> name;
		if (name == index_name) 
			return true; //�����Ѵ���
	}
	infile.close();
	return false;  //����������
}
