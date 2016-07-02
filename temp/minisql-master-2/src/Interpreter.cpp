#include "Interpreter.h"
#include "API.h"
#include <stdlib.h>
#include <string.h>
#include <fstream>

string getUserCommand()
{
	char* command;  //�û����������
	command = (char*)malloc(1024);
	char ch;
	int i = 0;

	while (1){
		cin.get(ch);
		command[i++] = ch;
		if (ch == ';') break;
		else if (ch == '\n'){
			cout << "     ->";
		}
	}
	command[i] = '\0';  //������
	string s(command);
	delete[] command;
	return s;
}


void handleWithCommand(string command)
{
	int i, j, opos, npos;

	i = 0; j = 0;
	opos = command.find_first_not_of(" \n", 0);
	npos = command.find_first_of(" ", opos);
	string comtype = command.substr(opos, npos - opos);
	//�ж���������
	if (comtype == "execfile"){
		//�򿪴洢�������¼���ļ�
		opos = command.find_first_not_of(" ", npos);
		npos = command.find_first_of(";", opos);
		string filename = command.substr(opos, npos - opos);
		
		ifstream infile;
		char buf[1024];
		infile.open("./student/" + filename, ios::in);
		while (!infile.eof()){
			memset(buf, 0, 1024);
			infile.getline(buf, 1024);
			if (infile.eof()) break;
			string cmd(buf);
			handleWithCommand(cmd);
		}
		cout << "Query OK." << endl;
	}
	else if (comtype == "create"){
		opos = npos + 1;
		npos = command.find_first_of(" ", opos); 
		string object = command.substr(opos, npos - opos);
		if (object == "table"){
			//��������
			opos = npos + 1;
			npos = command.find_first_of(" (", opos);
			string tbname = command.substr(opos, npos - opos);
			//����������﷨��
			FieldTree* T;
			T = createCreateTableTree(tbname, command.substr(npos, command.length() - npos));
			
			//�����﷨�������θ�API
			API_Create_Table(tbname, T);
			/*Table Table(tbname);
			Table.initializeTable(T);
			Table.createTable(); //�˴�Ϊ����API��create table����*/

			cout << "Query OK, 0 rows affected." << endl;
			return;
		}
		else if (object == "index"){
			//����������
			opos = npos + 1;
			opos = command.find_first_not_of(" ", opos);  //��������ʼ��λ��
			npos = command.find_first_of(" ", opos);  //������������ĵ�һ���ո�λ��
			string indexname = command.substr(opos, npos - opos);
			//���������ṹ
			IndexStruct* I;
			I = createCreateIndexStruct(indexname, command.substr(npos, command.length() - npos));
			
			//���θ�API
			API_Create_Index(*I);
			/*Index Idx(indexname);
			Idx.initializeIndex(I);
			Idx.createIndex();  //���ý�������API*/

		}
		else{
			//�Ƿ�����
			throw Grammer_error(object);
		}
	}
	else if (comtype == "drop"){
		opos = command.find_first_not_of(" ", npos);
		npos = command.find_first_of(" ", opos);
		string dropOrient = command.substr(opos, npos - opos);
		if (dropOrient == "table"){
			//ɾ��������
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" ;", opos);
			string tbname = command.substr(opos, npos - opos);
			//��һ���ַǿո��Ӧֻ�зֺ�
			opos = command.find_first_not_of(" ", npos);
			if (command.at(opos) != ';'){
				//�﷨����
				throw Grammer_error(tbname);
			}

			//���θ�API
			Table T(tbname);
			T.getTableInfo();
			API_Drop_Table(T);
			/*
			T.dropTable();  //����API��ɾ��������*/
		}
		else if (dropOrient == "index"){
			//ɾ����������
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" ;", opos);
			string idxname = command.substr(opos, npos - opos);
			//��һ���ַǿո��Ӧֻ�зֺ�
			opos = command.find_first_not_of(" ", npos);
			if (command.at(opos) != ';'){
				//�﷨����
				throw Grammer_error(idxname);
			}

			//���θ�API
			API_Drop_Index(idxname);
			/*Index Idx(idxname);
			Idx.dropIndex(); //����API��ɾ������������*/
		}
		else{
			//�Ƿ�����
			throw Grammer_error(dropOrient);
		}
	}
	else if (comtype == "insert"){
		//�����¼ָ��
		opos = command.find_first_not_of(" ", npos);
		npos = command.find_first_of(" ", opos);
		string into = command.substr(opos, npos - opos);
		if (into == "into"){
			//�����﷨Ҫ��
			//����
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" ", opos);
			string tbname = command.substr(opos, npos - opos);

			//"values"
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" (", opos);
			string values = command.substr(opos, npos - opos);
			if (values == "values"){
				//�����﷨Ҫ��
				TupleStruct* tup;
				tup = CreateTupleStruct(command.substr(npos, command.length() - npos));

				//��tuple��API
				Table tbl(tbname);
				tbl.getTableInfo();
				Tuple tuple(tbname);
				tuple.initializeTuple(tup);

				API_Insert(tbl, tuple);
				/*
				tuple.Insert();   //����API�е�insert����*/
			}
			else{
				//�������﷨Ҫ��
				throw Grammer_error(into);
			}
		}
		else{
			//�������﷨Ҫ��
			throw Grammer_error(into);
		}
		cout << "Query OK, 1 rows affected." << endl;
	}
	else if (comtype == "select"){
		//select * from student;
		//select * from student where...
		opos = command.find_first_not_of(" ", npos);
		npos = command.find_first_of(" ", opos);
		string selectall = command.substr(opos, npos - opos);
		if (selectall == "*"){
			//�����﷨Ҫ��
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" ", opos);
			string from = command.substr(opos, npos - opos);
			if (from == "from"){
				//�����﷨Ҫ��
				opos = command.find_first_not_of(" ", npos);
				npos = command.find_first_of(" ;", opos);
				string tbname = command.substr(opos, npos - opos);
				list<Condition> conditionlist;   //�����ѯ����
				conditionlist = CreateConditionlist(command.substr(npos, command.length() - npos));

				//���θ�API
				Table tbl(tbname);
				tbl.getTableInfo();
				API_Select(tbl,conditionlist);
				/*vector<int> offsetlist;
				//API����������ң�����offserlist
				Tuple tuple(tbname);
				tuple.Select(offsetlist, conditionlist); //API����select����*/
			}
			else{
				//�������﷨Ҫ��
				throw Grammer_error(from);
			}
		}
		else{
			//�������﷨Ҫ��
			throw Grammer_error(selectall);
		}
	}
	else if (comtype == "delete"){
		//delete from student;
		//delete from student where sno = ��88888888��;
		opos = command.find_first_not_of(" ", npos);
		npos = command.find_first_of(" ", opos);
		string from = command.substr(opos, npos - opos);
		if (from == "from"){
			//�����﷨Ҫ��
			opos = command.find_first_not_of(" ", npos);
			npos = command.find_first_of(" ;", opos);
			string tbname = command.substr(opos, npos - opos);
			list<Condition> conditionlist;   //�����ѯ����
			conditionlist = CreateConditionlist(command.substr(npos, command.length() - npos));

			//���θ�API
			Table tbl(tbname);
			tbl.getTableInfo();
			API_Delete(tbl, conditionlist);
			/*vector<int> offsetlist;
			//API����������ң�����offserlist
			Tuple tuple(tbname);
			tuple.Delete(offsetlist, conditionlist); //API����delete����*/
		}
		else{
			//�������﷨Ҫ��
			throw Grammer_error(from);
		}
	}
	else{
		//�Ƿ�����
		throw Grammer_error(comtype);
	}
}



/*����������﷨��
create table student(
	sno char(8),
	sname char(16) unique,
	sage int,
	sgender char(1),
	primary key(sno)
	);
*/
//����Ϊtbname, �ֶ���Ϊfieldmodule���� (�����ڣ��������ţ��Ĳ���)
FieldTree* createCreateTableTree(string tbname, string fieldmodule)
{
	int i, opos, npos, endpos;
	int primary_num;
	string primary_attr = "";
	FieldTree* T;
	FieldNode* Tnode, *Lnode;
	string field;   //һ���ֶ���Ϣ

	T = new FieldTree;
	T->tablename = tbname;
	T->field = new CreateTableNode;//�����ڵ�
	T->field->next = NULL;
	Lnode = T->field;

	i = 0;
	primary_num = 0;
	opos = fieldmodule.find_first_not_of(" (\n");

	while (1){
		fieldmodule = fieldmodule.substr(opos, fieldmodule.length() - opos);
		npos = fieldmodule.find_first_of(",", 0);
		if (npos == -1){
			//���һ���ֶ�
			//�����źͷֺŽ���
			npos = fieldmodule.find_first_of(";", 0);
		}
		else npos++;

		opos = fieldmodule.find_first_not_of(" ", 0);
		if (opos == -1 || fieldmodule.at(opos) == ';') break;   //������

		string field = fieldmodule.substr(0, npos);
		//sno char(8) unique,
		//primary key(sno))

		//ȷ���ֶ���
		opos = field.find_first_not_of(" \n(", 0);
		npos = field.find_first_of(" ", opos);
		if (npos == -1 || opos > npos){
			//�ֶ�Ϊ�գ�������ʾ����Ҫ��һ������
			cerr << "At least one attribute is aquired." << endl;
			throw Grammer_error(field);
		}
		string def = field.substr(opos, npos - opos);
		if (def == "primary"){
			//����������䣬�����ֶζ������
			opos = field.find_first_not_of(" ", npos);
			npos = field.find_first_of(" (", opos);
			string key = field.substr(opos, npos - opos);
			if (key == "key"){
				opos = field.find_first_not_of(" (", npos);
				npos = field.find_first_of(" )", opos);
				if (opos == -1 || npos == -1){
					//���Ų�ƥ��
					throw Grammer_error(def + " " + key);
					return NULL;
				}
				primary_num++;
				if (primary_num == 2){
					//����������ֹһ��
					cerr << "Only one primary key is allowed!" << endl;
					return NULL;
				}
				primary_attr = field.substr(opos, npos - opos);
			}
			else{
				//�﷨����
				throw Grammer_error(def);
				return NULL;
			}
		}
		else{
			Tnode = new CreateTableNode;  //�����½ڵ�
			Lnode->next = Tnode;
			Tnode->next = NULL;

			Tnode->attr_name = field.substr(opos, npos - opos);  //�ֶ���

			//ȷ���ֶ�����
			opos = field.find_first_not_of(" \n,", npos);
			npos = field.find_first_of(" \n(),", opos);
			string type = field.substr(opos, npos - opos);

			if (type == "char"){
				Tnode->attr_type = 0;
				opos = field.find_first_not_of(" (\n", npos);
				npos = field.find_first_of(')', opos);
				if (opos == -1 || (opos != -1 && npos == -1)){
					//û������ or �������Ų�ƥ��
					throw Grammer_error(type);
					return NULL;
				}
				int len = atoi(field.substr(opos, npos - opos).c_str());  //char�������Եĳ���
				Tnode->attr_len = len;
			}
			else if (type == "int"){
				Tnode->attr_type = 1;
				Tnode->attr_len = 4;
			}
			else if (type == "float"){
				Tnode->attr_type = 2;
				Tnode->attr_len = 4;
			}
			else{
				//�ֶ����ͷǷ�
				cerr << type << " is not a legal type." << endl;
				throw Grammer_error(type);
				return NULL;
			}

			//ȷ���ֶζ����Լ������
			opos = field.find_first_not_of(" )\n", npos);
			if (opos == -1 || opos == field.length() - 1){
				//δ��ʽ����Լ������
				Tnode->attr_def_type = -1;
			}
			else{
				//��ʽ������Լ������
				npos = field.find_first_of(" \n),", opos);
				string deftype = field.substr(opos, npos - opos);

				if (deftype == "primary"){
					//��������
					opos = field.find_first_not_of(" \n", npos);
					npos = field.find_first_of(" ,)\n", opos);
					string key = field.substr(opos, npos - opos);
					if (key == "key"){
						//���key����û�зǷ�����
						opos = field.find_first_not_of(" \n", npos);
						if (opos == -1 || (field.at(opos) != ',' && field.at(opos) != ')')){
							//�﷨����
							throw Grammer_error(key);
						}
						primary_num++;
						Tnode->attr_def_type = 0; //primary
					}
					else{
						//�﷨����
						throw Grammer_error(deftype);
					}
				}
				else if (deftype == "unique"){
					//���unique����û�зǷ�����
					opos = field.find_first_not_of(" \n", npos);
					if (opos == -1 || (field.at(opos) != ',' && field.at(opos) != ')')){
						//�﷨����
						throw Grammer_error(deftype);
					}
						Tnode->attr_def_type = 1; //unique
				}
				else{
					//�Ƿ�����
					throw Grammer_error(deftype);
				}
			}//�ж�Լ������
			Lnode = Tnode;
			//��һ���ֶ���Ϣ
		}//�ֶζ������
		opos = 0;
		while (opos < field.length()) opos++;
	}//while
	int flag = 0;
	if (primary_attr != ""){
		//�и��ӵ���������
		primary_num = 0;
		for (Tnode = T->field->next; Tnode != NULL; Tnode = Tnode->next){
			if (Tnode->attr_name == primary_attr){
				flag = 1;
				Tnode->attr_def_type = 0;
			}
			if (Tnode->attr_def_type == 0)
				primary_num++;
		}
		if (!flag){
			//û����������ĸ��ֶ�
			cout<< "There is no attribute \"" << primary_attr << "\"" << endl;
			return NULL;
		}
		if (primary_num >= 2){
			//����һ����������
			cout << "Only one primary key is allowed!" << endl;
			return NULL;
		}
		if (primary_num == 0){
			//û����������
			cout << "At least one attribute is acquired! 1" << endl;
		}
	}
	else if (primary_num == 0){
		cout << "At least one attribute is acquired! 2" << endl;
	}
	return T;
}

/*��������������Ϣ�Ľṹ
create index stunameidx on student ( sname );
*/
//idxnameΪ��������infoΪ������֮�����Ϣ
IndexStruct* createCreateIndexStruct(string idxname, string info)
{
	IndexStruct* Indexs;
	int i, opos, npos;

	opos = 0;
	opos = info.find_first_not_of(" ", opos);
	npos = info.find_first_of(" ", opos);
	string on = info.substr(opos, npos - opos);
	if (on == "on"){
		//�����﷨Ҫ��
		Indexs = new IndexStruct;
		Indexs->index_name = idxname;
		Indexs->attr_name = "";
		Indexs->table_name = "";  //��ʼ��

		//��һ����Ϊ�������ڵı������
		opos = info.find_first_not_of(" ", npos);
		npos = info.find_first_of(" (", opos);
		string tbname = info.substr(opos, npos - opos);
		Indexs->table_name = tbname; //����

		//��һ����Ϊ�����������ֶ���
		opos = info.find_first_not_of(" (", npos);
		npos = info.find_first_of(" )", opos);
		string attrname = info.substr(opos, npos - opos);
		Indexs->attr_name = attrname; //�ֶ���

		//��һ���ַǿո��Ӧֻ�зֺ�
		opos = info.find_first_not_of(" )", npos);
		if (info.at(opos) != ';'){
			//�﷨����
			cout << "SQL syntax error near \"" << attrname << "\"" << endl;
			return NULL;
		}
		return Indexs;
	}
	else{
		//�������﷨Ҫ�󣬱���
		cout << "SQL syntax error near \"" << idxname <<" "<< on << "\"" << endl;
		return NULL;
	}
	return NULL;
}

//insert into student values(��12345678��, ��wy��, 22, ��M��);
//tupinfoΪvalues��[(...);]����
TupleStruct* CreateTupleStruct(string tupinfo)
{
	int opos, npos;
	int num; //���Ը���
	TupleStruct* tup;
	tup = new TupleStruct;

	num = 0;
	opos = tupinfo.find_first_not_of(" \n(", 0);
	while (1){
		tupinfo = tupinfo.substr(opos, tupinfo.length() - opos);
		npos = tupinfo.find_first_of(",)", 0);
		if (npos == -1){
			//��������ֵ���Ѵ�����
			npos = tupinfo.find_first_not_of(" \n");
			if (tupinfo.at(npos) != ';'){
				//�﷨����
				cout << "SQL syntax error near \"" << tupinfo.at(npos) << "\"" << endl;
				return NULL;
			}
			else break; //�������
		}
		string value = tupinfo.substr(0, npos + 1); //���һ������ֵ(����','��')')
		opos = value.find_first_not_of(" '", 0);
		npos = value.find_first_of("' ,)", opos);
		if (npos == -1) npos = value.length();
		tup->tuplevalues[num++] = value.substr(opos, npos - opos); //��ô���������ֵ��ȥ����β�ո��'��

		opos = 0;
		while (opos < value.length()) opos++;
		//������һ������ֵ
	}//ѭ������������ÿ������ֵ
	tup->value_num = num;
	return tup;
}

//���������ѯ�����Ľṹ
//where sage > 20 and sgender = ��F��;
list<Condition> CreateConditionlist(string condinfo)
{
	list<Condition> condlist;
	int opos, npos;

	condlist.clear();
	//�ж��Ƿ���where���
	opos = condinfo.find_first_not_of(" ", 0);
	if (condinfo.at(opos) == ';'){ 
		//û��where���,��ѯ����Ϊ��
		return condlist;
	}
	else{
		//��where���
		npos = condinfo.find_first_of(" ", opos);
		string where = condinfo.substr(opos, npos - opos);
		if (where == "where"){
			//�����﷨Ҫ��
			opos = npos;  //��where�������俪ʼ����
			while (1){
				condinfo = condinfo.substr(opos, condinfo.length() - opos);
				opos = condinfo.find_first_not_of(" \n", 0);
				npos = condinfo.find("and", opos);
				if (opos == -1 || condinfo.at(opos) == ';') break;   //������
				if (npos == string::npos){
					//���һ����ѯ����
					//�ֺŽ������(�ֺ�ǰ�����пո�)
					npos = condinfo.find_first_of(";", opos);
					npos++;
				}
				else npos = condinfo.find_first_of(" \n", npos);
				string condstr = condinfo.substr(opos, npos - opos);  
				//id >= '10010' (and/;)

				//��Ҫ��ѯ��������
				opos = condstr.find_first_not_of(" \n", 0);
				npos = condstr.find_first_of(" \n", opos);
				if (opos == -1 || npos == -1){
					//�﷨����
					throw Grammer_error(condstr);
				}
				string attr = condstr.substr(opos, npos - opos);

				//��ѯ�ıȽ�������== / >= / <...��
				opos = condstr.find_first_not_of(" \n", npos);
				npos = condstr.find_first_of(" \n", opos);
				if (opos == -1 || npos == -1){
					//�﷨����
					throw Grammer_error(condstr);
				}
				string type = condstr.substr(opos, npos - opos);
				if (type != "=" && type != ">" && type != ">="
					&& type != "<" && type != "<=" && type != "<>"){
					//�ȽϷ��Ŵ���
					throw Grammer_error(type);
				}

				//�Ƚ�ֵ
				opos = condstr.find_first_not_of(" ';\n", npos);
				npos = condstr.find_first_of(" ';\n", opos);  //�����ԷֺŽ�β
				if (opos == -1 || npos == -1){
					//�﷨����
					throw Grammer_error(condstr);
				}
				string value = condstr.substr(opos, npos - opos);

				struct Condition oneCondition;
				oneCondition.compare_attr = attr;
				oneCondition.compare_type = type;
				oneCondition.compare_value = value;
				
				condlist.push_back(oneCondition);  //�������в���һ����¼
				
				while (opos++ < condstr.length());

			} //while
		}
		else throw Grammer_error(where); //�׳��쳣
	}
	return condlist;
}