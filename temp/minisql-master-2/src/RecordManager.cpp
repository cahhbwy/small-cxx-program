#include "Interpreter.h"
#include "IndexManager.h"
#include <fstream>
#include <stdlib.h>
#include <string.h>


vector <char*> select_list;

RBlock::~RBlock()
{
	if (buffer != NULL){
		delete[] buffer;  //�ͷ�buffer���ڴ�
	}
}

//is_want_empty=1����ʾ��Ҫ���ļ����ܲ����¼�Ŀ�
//is_want_empty=0, ��ʾ�����ļ��е�offsetB��


int RBlock::getBlock(string filename, int offsetB, int is_want_empty)
{
	ifstream infile;

	infile.open(filename.c_str(), ios::binary | ios::in);  //�����ƶ�дģʽ���ļ�
	if (!infile) throw File_openfail(filename.c_str());     //����ʧ�����׳��쳣

	int offsetNow = 0;    //��ʼƫ�ƿ���
	char tmpc;  //��ʱ������ļ��ж�ȡ���ַ�

	if (is_want_empty == 2){
		//���ļ��е�һ�����Դ���¼�¼�Ŀ�
		char* record;
		T.getTableInfo();  //��ȡ����Ϊtablename�ı����Ϣ
		record = (char*)malloc(T.record_len);   //��¼�ĳ���(��һ���ֽ�������¼������¼�Ƿ����or��ɾ��)

		int i = 0;
		while (!infile.eof()){
			while (i < BLOCKSIZE){
				//��tmp�ڶ���һ���������
				tmpc = infile.get();
				if (infile.eof()) break;
				buffer[i++] = tmpc;
			}
			if (isBlockFull(buffer, T.record_len) >= 0){
				//������Ŀ黹�д�ż�¼�Ŀռ�
				ptr = &buffer[0];   //ָ��һ��ʼָ�򻺴���ͷ��
				break;
			}
			memset(buffer, 0, BLOCKSIZE);
			offsetNow++;   //��ǰ�������λ�ü�һ;
			i = 0;
		}
		infile.close();
		return offsetNow;
	}

	else if (is_want_empty == 0){
		//���ļ��е�offsetB��
		int i = 0;
		while (!infile.eof()){
			while (i < BLOCKSIZE && !infile.eof()){
				//��tmp�ڶ���һ���������
				infile.read(&tmpc, 1);
				buffer[i++] = tmpc;
			}
			if (offsetNow == offsetB){
				//�ҵ���offsetB��
				break;
			}
			else if (infile.eof()){
				infile.close();
				return -1;  //�ļ�����offsetB���С���򷵻�-1��ʾ�Ҳ���ƫ����ΪoffsetB���λ��
			}
			memset(buffer, 0, BLOCKSIZE);
			offsetNow++;
			i = 0;
		}
		infile.close();
		return offsetB;
	}
		
	else if(is_want_empty == 1){
		//Ҫһ����տ�
		//�����ļ�
		int i = 0;
		while (!infile.eof()){
			while (i < BLOCKSIZE){
				//��tmp�ڶ���һ���������
				tmpc = infile.get();
				if (infile.eof()) break;
				buffer[i++] = tmpc;
			}
			int isEmpty;
			memcpy(&isEmpty, buffer + 4, 4);
			if (isEmpty == 0){
				//�տ�
				ptr = &buffer[0];   //ָ��һ��ʼָ�򻺴���ͷ��
				break;
			}
			memset(buffer, 0, BLOCKSIZE);
			offsetNow++;
			i = 0;
		}
		infile.close();
		return offsetNow;
	}
    return 0;
}


void RBlock::writeBackBlock(const char* filename, int offsetB)
{
	fstream iofile;
	iofile.open(filename, ios::binary | ios::in | ios::out);
	if (!iofile) throw File_openfail(filename);     //����ʧ�����׳��쳣
	
	//�ҵ���offsetB��
	iofile.seekp(offsetB * BLOCKSIZE, ios::beg);
	iofile.write(buffer, BLOCKSIZE); //���������������д�ص�offsetB���λ��
}

//�����ļ��п���
int totalBlockNum(string filename)
{
	ifstream infile(filename.c_str(), ios::in);
	infile.seekg(0, ios::end); //��λ���ļ�β
	streampos pos = infile.tellg();  //�ļ�����
	return (pos / BLOCKSIZE);
}

int isBlockFull(char* buf, int reclen)
{
	if (buf == NULL) return 0;
	int i = 0;
	int lenCur = 0;
	while (lenCur < BLOCKSIZE){
		if (lenCur + reclen >= BLOCKSIZE) break;
		if (buf[lenCur] == '0' || buf[lenCur] == '\0'){
			//������¼�ı�־λΪ0��˵���˴�û�м�¼���������м�¼���Ѿ���ɾ�������¼�¼���Բ��뵽����
			return lenCur;
		}
		lenCur += reclen;
	}
	//buf��û�п�λ�ô���¼�¼������-1
	return -1;
}



//���ݴ�����tup���������Ԫ�����Ϣ
void Tuple::initializeTuple(TupleStruct* tup)
{
	T.getTableInfo();
	T.CalcRecordLen();
	if (T.attr_num == tup->value_num){
		//���Բ���
		for (int i = 0; i < T.attr_num; i++){
			tuple_values[i] = tup->tuplevalues[i];
		}
	}
	else{
		//��������Ԫ����ֵ��������������
		cerr << "values number in tuple is less or more than attributes number. Insert failed" << endl;
		throw Grammer_error("insert");
	}
}

char* Tuple::convertTuple()
{
	char* value = new char[T.record_len];
	memset(value, 0, T.record_len);
	char *p = value;
	*p = '1';  p++;   //����Ѳ����¼
	for (int i = 0; i < T.attr_num; i++){
		if (T.attrs[i].attr_type == CHAR){
			//����Ϊchar����
			strcpy(p, tuple_values[i].data());
			p += T.attrs[i].attr_len;
		}
		else if (T.attrs[i].attr_type == INT){
			//����Ϊint����
			int ivalue;
			ivalue = atoi(tuple_values[i].c_str());

			memcpy(p, &ivalue , 4);
			p += 4;
		}
		else if (T.attrs[i].attr_type == FLOAT){
			//����Ϊfloat����
			float fvalue;
			fvalue = atof(tuple_values[i].c_str());

			memcpy(p, &fvalue, 4);
			p += 4;
		}
	}
	*p = '\0'; //��ǽ���

	return value;
}

bool Tuple::Insert(INT32 &position)
{
	int offsetblock;
	int lenCur = 0;   //��ǰƫ����
	RBlock* bufblock = new RBlock(T.table_name);
	string filename;
	filename = T.table_name + "_tablereco";

	if (!IsUniqueKeyExisted()){
		//���Ѵ��ڵļ�¼�����ظ��ĵ�ֵ����
		return false;
	}
	offsetblock =  bufblock->getBlock(T.table_name + "_tablereco", 0, 2); //�ҵ��ļ��пɴ����¼�¼�Ŀ�ƫ��
	bufblock->ptr = &bufblock->buffer[0];
	while (lenCur < BLOCKSIZE){
		if (*(bufblock->ptr) == '0' || *(bufblock->ptr) == '\0'){
			//�ɲ����¼�¼
			bufblock->is_Changed = 1; //���������ı�
			char* value = convertTuple();  //��Ԫ���ֵת��Ϊ�ַ���
			memcpy(bufblock->ptr, value, T.record_len); //��ֵ�浽������
			position=PushPosition(offsetblock, lenCur, T.record_len);
			break;
		}
		lenCur += T.record_len;
		bufblock->ptr += T.record_len;
	}

	if (bufblock->is_Changed){
		bufblock->writeBackBlock(filename.c_str(), offsetblock);  //�����bufferд���ļ�
		return true;
	}
	return false;
}

bool Tuple::IsUniqueKeyExisted()
{
	int offsetblock, offsetNow, maxoffset;
	ifstream infile;
	string filename = T.table_name + "_tablereco";

	RBlock* bufblock = new RBlock(T.getTableName());
	maxoffset = totalBlockNum(filename);  //�ļ������п���
	if (maxoffset == 0){
		//�ļ�Ϊ�գ���δд���κμ�¼
		return true;
	}

	int i, len;
	int is_Duplic = 0; //�ж��Ƿ��ظ�
	char *tmpAttr; //��ʱ��Ŵ��Ƚϵ�����ֵ
	for (offsetNow = 0; offsetNow < maxoffset; offsetNow++){
		offsetblock = bufblock->getBlock(T.table_name + "_tablereco", offsetNow, 0);  //�ҵ���offsetNow�鲢���������
		bufblock->ptr = &(bufblock->buffer[0]);
		for (len = 0; len < BLOCKSIZE; len += T.record_len){
			//�ӻ������ж���ÿ����¼������ÿ�����Ե�ֵ��������¼�Ƚϡ�
			bufblock->ptr = &bufblock->buffer[len];
			if (*(bufblock->ptr) == '1'){
				//������¼����
				bufblock->ptr++; //ָ�����һλ
				for (i = 0; i < T.attr_num; i++){
					//�Ա�ÿ������
					if (T.attrs[i].attr_def_type == PRIMARY || T.attrs[i].attr_def_type == UNIQUE){
						//������ΪUNIQUE��PRIMARY���жϸ�����ֵ�Ƿ����Ѵ��ڵļ�¼�ظ�
						if (T.attrs[i].attr_type == CHAR){
							//����Ϊchar����
							tmpAttr = new char[T.attrs[i].attr_len + 1];
							memset(tmpAttr, 0, T.attrs[i].attr_len + 1);
							memcpy(tmpAttr, bufblock->ptr, T.attrs[i].attr_len);  //���������Ե�ֵ����tmpAttr��
							if (tuple_values[i] == tmpAttr){
								//������ͬ
								is_Duplic = 1;
								delete[] tmpAttr;
								break;
							}
						}
						else if (T.attrs[i].attr_type == INT){
							//����Ϊint����
							int tmpivalue, ivalue;
							memcpy(&tmpivalue, bufblock->ptr, 4);
							ivalue = atoi(tuple_values[i].c_str());
							if (ivalue == tmpivalue){
								//������ͬ
								is_Duplic = 1;
								break;
							}
						}
						else if (T.attrs[i].attr_type == FLOAT){
							//����Ϊfloat����
							float tmpfvalue, fvalue;
							memcpy(&tmpfvalue, bufblock->ptr, 4);
							fvalue = atof(tuple_values[i].c_str());
							if (fvalue == tmpfvalue){
								//������ͬ
								is_Duplic = 1;
								break;
							}
						}
					}//if:UNIQUE or PRIMARY
					bufblock->ptr += T.attrs[i].attr_len;  //ָ�����һ����¼�ĳ���
				}//for (i = 0; i < T.attr_num; i++)
				if (is_Duplic){
					//�����ظ��ĵ�ֵ����
					return false;
				}
			}//if:��λ�ô��м�¼����
		}//for (len = 0; len < BLOCKSIZE; len += T.record_len)
	}//for (offsetNow = 0; offsetNow < maxoffset; offsetNow++)
	return true;
}


//����
void Tuple::Select(vector<int> offsetlist, list<Condition> conditionlist)
{

	int offsetNow = 0; //��ǰƫ�ƿ�
	int maxoffset = 0;     //���ƫ�ƿ�
	int is_index_exist = 1;      //����������ؽ���Ƿ�Ϊ��
	string filename = T.table_name + "_tablereco";
	select_list.clear();  //��մ�ż�¼��vector

	list<Condition>::iterator it;
	RBlock* bufblock = new RBlock(T.getTableName());
	int vi = 0;  //vector�ĵ�i��Ԫ��
	int i, len, ptroffset;

	char* tmpAttr;
	int ivalue, tmpivalue;
	float fvalue, tmpfvalue;

	T.getTableInfo(); //��ñ����Ϣ

	int sign = *(offsetlist.end() - 1);
	offsetlist.pop_back(); //ɾ��βԪ��
	if (sign == 0 || sign == 1) {
		//��ѯ����ԭ����Ϊ�� or ��ѯ������Ϊ�գ���û���κ�������Ϣ
		//���ļ���˳������������������ļ�¼
		maxoffset = totalBlockNum(filename);  //�ļ���������
		//����ÿ�����е�ÿ����¼���ж�����
		for (offsetNow = 0; offsetNow < maxoffset; offsetNow++){
			offsetNow = bufblock->getBlock(T.table_name + "_tablereco", offsetNow, 0);
			bufblock->ptr = &(bufblock->buffer[0]);
			for (len = 0; len < BLOCKSIZE; len += T.record_len){
				//�ӻ������ж���ÿ����¼
				bufblock->ptr = &bufblock->buffer[len];
				ptroffset = 0;
				if (*(bufblock->ptr) == '1'){
					//������¼����
					if (!conditionlist.empty()){
						//���в�ѯ���������ж��Ƿ�������������
						for (it = conditionlist.begin(); it != conditionlist.end(); it++){
							ptroffset = 1;  //�Ӹ�����¼����λ��ʼ��������
							for (i = 0; i < T.attr_num; i++){
								if (T.attrs[i].attr_name == it->compare_attr) break;
								ptroffset += T.attrs[i].attr_len;  //�ҵ���i�����Ե�ƫ��λ��
							}//�ҵ���Ӧ����
							if (T.attrs[i].attr_type == CHAR){
								//����Ϊchar����
								tmpAttr = new char[T.attrs[i].attr_len + 1];
								memset(tmpAttr, 0, T.attrs[i].attr_len + 1);
								memcpy(tmpAttr, bufblock->ptr + ptroffset, T.attrs[i].attr_len);  //���������Ե�ֵ����tmpAttr��
								if (it->compare_type == "="){
									if (tmpAttr != it->compare_value) break;
								}
								else if (it->compare_type == "<>"){
									if (tmpAttr == it->compare_value) break;
								}
								else if (it->compare_type == ">"){
									if (tmpAttr <= it->compare_value) break;
								}
								else if (it->compare_type == ">="){
									if (tmpAttr < it->compare_value) break;
								}
								else if (it->compare_type == "<"){
									if (tmpAttr >= it->compare_value) break;
								}
								else if (it->compare_type == "<="){
									if (tmpAttr > it->compare_value) break;
								}
							}
							else if (T.attrs[i].attr_type == INT){
								memcpy(&tmpivalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								ivalue = atoi(it->compare_value.c_str());
								if (it->compare_type == "="){
									if (tmpivalue != ivalue) break;
								}
								if (it->compare_type == "<>"){
									if (tmpivalue == ivalue) break;
								}
								if (it->compare_type == ">"){
									if (tmpivalue <= ivalue) break;
								}
								if (it->compare_type == ">="){
									if (tmpivalue > ivalue) break;
								}
								if (it->compare_type == "<"){
									if (tmpivalue >= ivalue) break;
								}
								if (it->compare_type == "<="){
									if (tmpivalue > ivalue) break;
								}
							}
							else if (T.attrs[i].attr_type == FLOAT){
								memcpy(&tmpfvalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								fvalue = atof(it->compare_value.c_str());
								if (it->compare_type == "="){
									if (tmpfvalue != fvalue) break;
								}
								if (it->compare_type == "<>"){
									if (tmpfvalue == fvalue) break;
								}
								if (it->compare_type == ">"){
									if (tmpfvalue <= fvalue) break;
								}
								if (it->compare_type == ">="){
									if (tmpfvalue > fvalue) break;
								}
								if (it->compare_type == "<"){
									if (tmpfvalue >= fvalue) break;
								}
								if (it->compare_type == "<="){
									if (tmpfvalue > fvalue) break;
								}
							}
						}
					}//���в�ѯ����
					if (conditionlist.empty() || it == conditionlist.end()){
						//��û�в�ѯ����/�����������еĲ�ѯ����
						//�򽫴�����¼���뵽select_list��
						bufblock->ptr++;
						char* value = new char[T.record_len];
						memset(value, 0, T.record_len);
						memcpy(value, bufblock->ptr, T.record_len - 1);
						select_list.push_back(value);  //�Ѹ�����¼ֵ����select_list
					}
				}//������¼����
			}
		}
	}
	
	else if (sign == 2){
		//��ѯ������Ϊ�գ���������Ϣ��û�з����������κμ�¼
		//select_list�ÿգ�ֱ��return
		return;
	}

	else if (sign == 3){
		//�������
		INT32 blockAddr = 0, offset = 0;		
		vector<int>::iterator offsetIt;
		for (offsetIt = offsetlist.begin(); offsetIt != offsetlist.end(); offsetIt++){
			//����offsetlist
			PopPosition((*offsetIt), T.record_len, blockAddr, offset);
			bufblock->getBlock(T.table_name + "_tablereco", blockAddr, 0);
			bufblock->ptr = &bufblock->buffer[offset];
			ptroffset = 0;
			if (*(bufblock->ptr) == '1'){
				//������¼����
				if (!conditionlist.empty()){
					//���в�ѯ���������ж��Ƿ�������������
					for (it = conditionlist.begin(); it != conditionlist.end(); it++){
						ptroffset = 1;  //�Ӹ�����¼����λ��ʼ��������
						for (i = 0; i < T.attr_num; i++){
							if (T.attrs[i].attr_name == it->compare_attr) break;
							ptroffset += T.attrs[i].attr_len;  //�ҵ���i�����Ե�ƫ��λ��
						}//�ҵ���Ӧ����
						if (T.attrs[i].attr_type == CHAR){
							//����Ϊchar����
							tmpAttr = new char[T.attrs[i].attr_len + 1];
							memset(tmpAttr, 0, T.attrs[i].attr_len + 1);
							memcpy(tmpAttr, bufblock->ptr + ptroffset, T.attrs[i].attr_len);  //���������Ե�ֵ����tmpAttr��
							if (it->compare_type == "="){
								if (tmpAttr != it->compare_value) break;
							}
							else if (it->compare_type == "<>"){
								if (tmpAttr == it->compare_value) break;
							}
							else if (it->compare_type == ">"){
								if (tmpAttr <= it->compare_value) break;
							}
							else if (it->compare_type == ">="){
								if (tmpAttr < it->compare_value) break;
							}
							else if (it->compare_type == "<"){
								if (tmpAttr >= it->compare_value) break;
							}
							else if (it->compare_type == "<="){
								if (tmpAttr > it->compare_value) break;
							}
						}
						else if (T.attrs[i].attr_type == INT){
							memcpy(&tmpivalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
							ivalue = atoi(it->compare_value.c_str());
							if (it->compare_type == "="){
								if (tmpivalue != ivalue) break;
							}
							if (it->compare_type == "<>"){
								if (tmpivalue == ivalue) break;
							}
							if (it->compare_type == ">"){
								if (tmpivalue <= ivalue) break;
							}
							if (it->compare_type == ">="){
								if (tmpivalue > ivalue) break;
							}
							if (it->compare_type == "<"){
								if (tmpivalue >= ivalue) break;
							}
							if (it->compare_type == "<="){
								if (tmpivalue > ivalue) break;
							}
						}
						else if (T.attrs[i].attr_type == FLOAT){
							memcpy(&tmpfvalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
							fvalue = atof(it->compare_value.c_str());
							if (it->compare_type == "="){
								if (tmpfvalue != fvalue) break;
							}
							if (it->compare_type == "<>"){
								if (tmpfvalue == fvalue) break;
							}
							if (it->compare_type == ">"){
								if (tmpfvalue <= fvalue) break;
							}
							if (it->compare_type == ">="){
								if (tmpfvalue > fvalue) break;
							}
							if (it->compare_type == "<"){
								if (tmpfvalue >= fvalue) break;
							}
							if (it->compare_type == "<="){
								if (tmpfvalue > fvalue) break;
							}
						}
					}
				}//���в�ѯ����
				if (conditionlist.empty() || it == conditionlist.end()){
					//��û�в�ѯ����/�����������еĲ�ѯ����
					//�򽫴�����¼���뵽select_list��
					bufblock->ptr++;
					char* value = new char[T.record_len];
					memset(value, 0, T.record_len);
					memcpy(value, bufblock->ptr, T.record_len - 1);
					select_list.push_back(value);  //�Ѹ�����¼ֵ����select_list
				}
			}//������¼����
		}
	}
	return;
}

char value[32][256];
void Tuple::printSelectResult()
{
	char* tuplevalue;
	for (int vi = 0; vi < select_list.size(); vi++){
		tuplevalue = select_list.at(vi);
		for (int i = 0; i < T.attr_num; i++){
			memset(value[i], 0, T.attrs[i].attr_len+1);
			memcpy(value[i], tuplevalue, T.attrs[i].attr_len);
			tuplevalue += T.attrs[i].attr_len;
		}
		int length[32],totalLength;
		totalLength=getPrintLength(length);
		if (select_list.size()==1)
			PrintTuple(totalLength, 3, length);//�������һ��Ҳ�ǵ�һ��
		else if (vi == select_list.size()-1)
			PrintTuple(totalLength, 2, length);//���һ����¼
		else if (vi == 0)
			PrintTuple(totalLength, 0, length);//��һ����¼
		else 
			PrintTuple(totalLength, 1, length);
	}

	if (select_list.size() == 0){
		cout << "Empty set."<< endl;
	}
	else if (select_list.size() == 1){
		cout << "1 row in set." << endl;
	}
	else{
		cout << select_list.size() << " row in set." << endl;
	}

/*	char* tuplevalue = new char[T.record_len];
	char* charvalue;
	int intvalue;
	float floatvalue;
	int i;

	for (int vi = 0; vi < select_list.size(); vi++){
		tuplevalue = select_list.at(vi);
		for (i = 0; i < T.attr_num; i++){
			if (T.attrs[i].attr_type == CHAR){
				//char����
				charvalue = new char[T.attrs[i].attr_len];
				memset(charvalue, 0, T.attrs[i].attr_len);
				memcpy(charvalue, tuplevalue, T.attrs[i].attr_len); //�õ�������ֵ���ַ���
				cout << T.attrs[i].attr_name << ":" << charvalue << endl;
				delete[] charvalue;
			}
			else if (T.attrs[i].attr_type == INT){
				//INT����
				memcpy(&intvalue, tuplevalue, 4);  //�õ������Ե�ֵ
				cout << T.attrs[i].attr_name << ":" << intvalue << endl;
			}
			else if (T.attrs[i].attr_type == FLOAT){
				//FLOAT����
				memcpy(&floatvalue, tuplevalue, 4);  //�õ������Ե�ֵ
				cout << T.attrs[i].attr_name << ":" << floatvalue << endl;
			}
			tuplevalue += T.attrs[i].attr_len;
		}
	}*/
}

int Tuple::getPrintLength(int *length)
{
	int sum = 1;
	for (int i = 0; i<T.attr_num; i++){
		int al = T.attrs[i].attr_type == CHAR ? T.attrs[i].attr_len : 12;
		int len = al>T.attrs[i].attr_name.size() ? al:T.attrs[i].attr_name.size();
		length[i] = len;
		sum += length[i]+3;
	}
	return sum;
}

void Tuple::PrintTuple(int totalLength, int FirstOrEnd,int *length)
{
	if (FirstOrEnd == 0 || FirstOrEnd == 3){
		for (int i = 0; i<totalLength; i++){
			cout << '-';
		}
		cout << endl;
		for (int i = 0; i<T.attr_num; i++){
			//int length = T.attrs[i].attr_len>T.attrs[i].attr_name.size() ? T.attrs[i].attr_len : T.attrs[i].attr_name.size();
			cout << "| ";
			cout << setw(length[i]) << T.attrs[i].attr_name;
			cout << " ";
		}
		cout << "|" << endl;
		for (int i = 0; i<totalLength; i++){
			cout << '-';
		}
		cout << endl;
	}
	for (int i = 0; i<T.attr_num; i++){
		int type = T.attrs[i].attr_type;
		cout << "| ";
		printValue(i,type,length[i]);
		cout << " ";
	}
	cout << "|" << endl;
	if (FirstOrEnd == 2 || FirstOrEnd == 3){
		for (int i = 0; i<totalLength; i++){
			cout << '-';
		}
		cout << endl;
	}
}

void Tuple::printValue(int i, int type,int width)
{
	if (type == CHAR){
		cout << setw(width) << value[i];
	}
	else if (type == FLOAT){
		float floatvalue;
		memcpy(&floatvalue,value[i],4);
		cout << setw(width) << floatvalue;
	}
	else if (type == INT){
		int intvalue;
		memcpy(&intvalue, value[i], 4);
		cout << setw(width) << intvalue;
	}
}

int Tuple::Delete(vector<int> offsetlist, list<Condition> conditionlist)
{
	int offsetNow = 0; //��ǰƫ�ƿ�
	int maxoffset = 0;     //���ƫ�ƿ�
	string filename = T.table_name + "_tablereco";
	select_list.clear();  //��մ�ż�¼��vector

	list<Condition>::iterator it;
	RBlock* bufblock = new RBlock(T.getTableName());
	int vi = 0;  //vector�ĵ�i��Ԫ��
	int i, len, ptroffset;
	int deleteNum = 0;  //ɾ���ļ�¼�� 

	char* tmpAttr;
	int ivalue, tmpivalue;
	float fvalue, tmpfvalue;

	T.getTableInfo(); //��ñ����Ϣ

	//�ҵ������������� ����b+����ɾ��
	list<IndexInfo> indexlist;
	IndexInfo idxinfo;
	for (int num = 0; num < T.attr_num; num++){
		idxinfo.indexname = T.getAttrIndex(T.getAttr(num).attr_name);
		idxinfo.attr_size = T.getAttr(num).attr_len;
		idxinfo.maxKeyNum = (4096 - 4 - INDEX_BLOCK_INFO) / (4 + idxinfo.attr_size);

		indexlist.push_back(idxinfo);
	}

	int sign = *(offsetlist.end() - 1);
	offsetlist.pop_back(); //ɾ��βԪ��
	if (sign == 0 || sign == 1) {
		//��ѯ����ԭ����Ϊ�� or ��ѯ������Ϊ�գ���û���κ�������Ϣ
		//���ļ���˳������������������ļ�¼
		maxoffset = totalBlockNum(filename);  //�ļ���������
		//����ÿ�����е�ÿ����¼���ж�����
		for (offsetNow = 0; offsetNow < maxoffset; offsetNow++){
			offsetNow = bufblock->getBlock(T.table_name + "_tablereco", offsetNow, 0);
			bufblock->ptr = &(bufblock->buffer[0]);
			bufblock->is_Changed = 0;
			for (len = 0; len < BLOCKSIZE; len += T.record_len){
				//�ӻ������ж���ÿ����¼
				bufblock->ptr = &bufblock->buffer[len];
				ptroffset = 0;
				if (*(bufblock->ptr) == '1'){
					//������¼����
					if (!conditionlist.empty()){
						//���в�ѯ���������ж��Ƿ�������������
						for (it = conditionlist.begin(); it != conditionlist.end(); it++){
							ptroffset = 1;  //�Ӹ�����¼����λ��ʼ��������
							for (i = 0; i < T.attr_num; i++){
								if (T.attrs[i].attr_name == it->compare_attr) break;
								ptroffset += T.attrs[i].attr_len;  //�ҵ���i�����Ե�ƫ��λ��
							}//�ҵ���Ӧ����
							if (T.attrs[i].attr_type == CHAR){
								//����Ϊchar����
								tmpAttr = new char[T.attrs[i].attr_len + 1];
								memset(tmpAttr, 0, T.attrs[i].attr_len + 1);
								memcpy(tmpAttr, bufblock->ptr + ptroffset, T.attrs[i].attr_len);  //���������Ե�ֵ����tmpAttr��
								if (it->compare_type == "="){
									if (tmpAttr != it->compare_value) break;
								}
								else if (it->compare_type == "<>"){
									if (tmpAttr == it->compare_value) break;
								}
								else if (it->compare_type == ">"){
									if (tmpAttr <= it->compare_value) break;
								}
								else if (it->compare_type == ">="){
									if (tmpAttr < it->compare_value) break;
								}
								else if (it->compare_type == "<"){
									if (tmpAttr >= it->compare_value) break;
								}
								else if (it->compare_type == "<="){
									if (tmpAttr > it->compare_value) break;
								}
							}
							else if (T.attrs[i].attr_type == INT){
								memcpy(&tmpivalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								ivalue = atoi(it->compare_value.c_str());
								if (it->compare_type == "="){
									if (tmpivalue != ivalue) break;
								}
								if (it->compare_type == "<>"){
									if (tmpivalue == ivalue) break;
								}
								if (it->compare_type == ">"){
									if (tmpivalue <= ivalue) break;
								}
								if (it->compare_type == ">="){
									if (tmpivalue > ivalue) break;
								}
								if (it->compare_type == "<"){
									if (tmpivalue >= ivalue) break;
								}
								if (it->compare_type == "<="){
									if (tmpivalue > ivalue) break;
								}
							}
							else if (T.attrs[i].attr_type == FLOAT){
								memcpy(&tmpfvalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								fvalue = atof(it->compare_value.c_str());
								if (it->compare_type == "="){
									if (tmpfvalue != fvalue) break;
								}
								if (it->compare_type == "<>"){
									if (tmpfvalue == fvalue) break;
								}
								if (it->compare_type == ">"){
									if (tmpfvalue <= fvalue) break;
								}
								if (it->compare_type == ">="){
									if (tmpfvalue > fvalue) break;
								}
								if (it->compare_type == "<"){
									if (tmpfvalue >= fvalue) break;
								}
								if (it->compare_type == "<="){
									if (tmpfvalue > fvalue) break;
								}
							}
						}
					}//���в�ѯ����
					if (conditionlist.empty() || it == conditionlist.end()){
						//��û�в�ѯ����/�����������еĲ�ѯ����
						//�򽫴�����¼ɾ�����������д�뻺��飬������һ��ʱ��д
						*bufblock->ptr = '0';  //��һλ���ɾ����־
						deleteNum++;
						bufblock->is_Changed = 1; //����鱻�ı䣬��Ҫ��д
					}
				}//������¼����
				if (bufblock->is_Changed){
					//����Ҫ��д
					bufblock->writeBackBlock(filename.c_str(), offsetNow);  //�����bufferд���ļ�
				}
			}
		}
	}

	else if (sign == 2){
		//��ѯ������Ϊ�գ���������Ϣ��û�з����������κμ�¼
		return 0;
	}

	else if (sign == 3){
		//�������
		INT32 blockAddr = 0, offset = 0;
		vector<int>::iterator offsetIt;
		for (offsetIt = offsetlist.begin(); offsetIt != offsetlist.end(); offsetIt++){
			//����offsetlist
			PopPosition((*offsetIt), T.record_len, blockAddr, offset);
			bufblock->getBlock(T.table_name + "_tablereco", blockAddr, 0);
			bufblock->ptr = &bufblock->buffer[offset];
			ptroffset = 0;
			if (*(bufblock->ptr) == '1'){
				//������¼����
				if (!conditionlist.empty()){
					//���в�ѯ���������ж��Ƿ�������������
					for (it = conditionlist.begin(); it != conditionlist.end(); it++){
						ptroffset = 1;  //�Ӹ�����¼����λ��ʼ��������
						for (i = 0; i < T.attr_num; i++){
							if (T.attrs[i].attr_name == it->compare_attr) break;
							ptroffset += T.attrs[i].attr_len;  //�ҵ���i�����Ե�ƫ��λ��
						}//�ҵ���Ӧ����
						if (T.attrs[i].attr_type == CHAR){
							//����Ϊchar����
							tmpAttr = new char[T.attrs[i].attr_len + 1];
							memset(tmpAttr, 0, T.attrs[i].attr_len + 1);
							memcpy(tmpAttr, bufblock->ptr + ptroffset, T.attrs[i].attr_len);  //���������Ե�ֵ����tmpAttr��
							if (it->compare_type == "="){
								if (tmpAttr != it->compare_value) break;
							}
							else if (it->compare_type == "<>"){
								if (tmpAttr == it->compare_value) break;
							}
							else if (it->compare_type == ">"){
								if (tmpAttr <= it->compare_value) break;
							}
							else if (it->compare_type == ">="){
								if (tmpAttr < it->compare_value) break;
							}
							else if (it->compare_type == "<"){
								if (tmpAttr >= it->compare_value) break;
							}
							else if (it->compare_type == "<="){
								if (tmpAttr > it->compare_value) break;
							}
						}
						else if (T.attrs[i].attr_type == INT){
							memcpy(&tmpivalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
							ivalue = atoi(it->compare_value.c_str());
							if (it->compare_type == "="){
								if (tmpivalue != ivalue) break;
							}
							if (it->compare_type == "<>"){
								if (tmpivalue == ivalue) break;
							}
							if (it->compare_type == ">"){
								if (tmpivalue <= ivalue) break;
							}
							if (it->compare_type == ">="){
								if (tmpivalue > ivalue) break;
							}
							if (it->compare_type == "<"){
								if (tmpivalue >= ivalue) break;
							}
							if (it->compare_type == "<="){
								if (tmpivalue > ivalue) break;
							}
						}
						else if (T.attrs[i].attr_type == FLOAT){
							memcpy(&tmpfvalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
							fvalue = atof(it->compare_value.c_str());
							if (it->compare_type == "="){
								if (tmpfvalue != fvalue) break;
							}
							if (it->compare_type == "<>"){
								if (tmpfvalue == fvalue) break;
							}
							if (it->compare_type == ">"){
								if (tmpfvalue <= fvalue) break;
							}
							if (it->compare_type == ">="){
								if (tmpfvalue > fvalue) break;
							}
							if (it->compare_type == "<"){
								if (tmpfvalue >= fvalue) break;
							}
							if (it->compare_type == "<="){
								if (tmpfvalue > fvalue) break;
							}
						}
					}
				}//���в�ѯ����
				if (conditionlist.empty() || it == conditionlist.end()){
					//��û�в�ѯ����/�����������еĲ�ѯ����
					//�򽫴�����¼ɾ�����������д�뻺��飬������һ��ʱ��д
					*bufblock->ptr = '0';  //��һλ���ɾ����־
					deleteNum++;
					bufblock->is_Changed = 1; //����鱻�ı䣬��Ҫ��д
					//��������¼��B+����ɾ��
					int j;
					ptroffset = 1;
					list<IndexInfo>::iterator li;
					for (j = 0, li = indexlist.begin(); j < T.attr_num, li != indexlist.end(); j++, li++){
						if (li->indexname != ""){
							//������
							if (T.attrs[j].attr_type == CHAR){
								//����Ϊchar����
								tmpAttr = new char[T.attrs[j].attr_len + 1];
								memset(tmpAttr, 0, T.attrs[j].attr_len + 1);
								memcpy(tmpAttr, bufblock->ptr + ptroffset, T.attrs[j].attr_len);
								string tmp = tmpAttr;
								Delete_Key_From_Index(*li, tmp);
								delete[] tmpAttr;
							}
							else if (T.attrs[j].attr_type == INT){
								memcpy(&tmpivalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								Delete_Key_From_Index(*li, tmpivalue);
							}
							else if (T.attrs[j].attr_type == FLOAT){
								memcpy(&tmpfvalue, bufblock->ptr + ptroffset, 4);  //���������Ե�ֵ����tmpivalue��
								Delete_Key_From_Index(*li, tmpfvalue);
							}
						}
						ptroffset += T.attrs[j].attr_len;
					}
				}
			}//������¼����
			if (bufblock->is_Changed){
				//����Ҫ��д
				bufblock->writeBackBlock(filename.c_str(), blockAddr);  //�����bufferд���ļ�
			}
		}
	}
	return deleteNum;
}
vector<TupleIndex_number> TupleList_IDX;  //ȫ��

void Tuple::FetchAll(int AttrId)
{
	T.getTableInfo();

	int offsetblock, offsetNow, maxoffset;
	ifstream infile;
	string filename = T.table_name + "_tablereco";

	RBlock* bufblock = new RBlock(T.getTableName());
	maxoffset = totalBlockNum(filename);  //�ļ������п���
	if (maxoffset == 0){
		//�ļ�Ϊ�գ���δд���κμ�¼
		return;
	}

	int i, len;
	char *tmpAttr; //��ʱ��Ŵ��Ƚϵ�����ֵ
	int delta = 0;
	for (int j = 0; j < AttrId; j++){
		delta += T.attrs[j].attr_len;
	}
	for (offsetNow = 0; offsetNow < maxoffset; offsetNow++){
		offsetblock = bufblock->getBlock(T.table_name + "_tablereco", offsetNow, 0);  //�ҵ���offsetNow�鲢���������
		bufblock->ptr = &(bufblock->buffer[0]);
		for (len = 0; len < BLOCKSIZE; len += T.record_len){
			//�ӻ������ж���ÿ����¼������ÿ�����Ե�ֵ��������¼�Ƚϡ�
			bufblock->ptr = &bufblock->buffer[len];
			if (*(bufblock->ptr) == '1'){
				//������¼����
				bufblock->ptr++; //ָ�����һλ
				int sum = 0;
				for (i = 0; i < AttrId; i++){
					sum += T.attrs[i].attr_len;
				}//for (i = 0; i < T.attr_num; i++)
				char tmpAttr[256];
				memcpy(tmpAttr, bufblock->ptr + delta, T.attrs[AttrId].attr_len);
				TupleIndex tpl;
				PushPosition(offsetNow, len, tmpAttr, T.attrs[AttrId].attr_type, T.record_len);
			}//if:��λ�ô��м�¼����
		}//for (len = 0; len < BLOCKSIZE; len += T.record_len)
	}//for (offsetNow = 0; offsetNow < maxoffset; offsetNow++)
	//cout << "Fetch All" << TupleList_IDX.size() << endl;
}

void PushPosition(INT32 BlockAddr, INT32 Offset, char* key, int type, INT32 recordLength)
{
	TupleIndex_number tuple;
	INT32 position;
	int maxKeySize = (4096 - 4) / recordLength;
	position = maxKeySize*BlockAddr + Offset / recordLength;
	tuple.position = position;
	tuple.type = type;
	char buffer[256];
	if (type == INT){
		int intvalue;
		memcpy(&intvalue,key,4);
		tuple.key = "";
		tuple.intkey = intvalue;
		tuple.floatkey = 0;
	}
	else if (type == FLOAT){
		float floatvalue;
		memcpy(&floatvalue, key, 4);
		tuple.key = "";
		tuple.intkey = 0;
		tuple.floatkey = floatvalue;
	}
	else{
		tuple.key = key;
		tuple.intkey = 0;
		tuple.floatkey = 0;
	}

	TupleList_IDX.push_back(tuple);
}

void PopPosition(INT32 position, INT32 recordLength, INT32 &BlockAddr, INT32 &Offset)
{
	int maxKeySize = (4096 - 4) / recordLength;
	BlockAddr = position / maxKeySize;
	Offset = (position%maxKeySize)*recordLength;
}

INT32 PushPosition(INT32 BlockAddr, INT32 Offset, INT32 recordLength)
{
	INT32 position;
	int maxKeySize = (4096 - 4) / recordLength;
	position = maxKeySize*BlockAddr + Offset/recordLength;
	return position;
}