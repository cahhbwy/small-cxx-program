#include <cstdlib>
#include <cstdio>
#include <string>
#include <memory>

typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTNode, *HuffmanTree;
typedef char* *HuffmanCode;

void Create(FILE *fp, char* &ch, int* &w, int &n){
	ch = (char*)malloc(257 * sizeof(char));
	char* c = ch;
	w = (int*)malloc(257 * sizeof(int));
	memset(c, 0, 257 * sizeof(char));
	memset(w, 0, 257 * sizeof(int));
	int i;
	n = 1;
	char ct;
	while (true){
		int k=fread(&ct, 1, 1, fp);
		if (k!=1)	break;
		for (i = 1; i < n; i++){
			if (*(c + i) == ct){
				(*(w + i))++;
				break;
			}
		}
		if (i == n){
			*(c + n) = ct;
			*(w + n) = 1;
			n++;
		}
	}
	n--;
	return;
}

void Select(HuffmanTree HT, int k, int &s1, int &s2){
	int i = 1;
	while (HT[i].parent != 0)	i++;
	s1 = s2 = i;
	for (; i <= k; i++){
		if (HT[i].weight < HT[s1].weight&&HT[i].parent==0)
			s1 = i;
	}
	if (s2 == s1){
		s2++;
		while (HT[s2].parent != 0)	s2++;
	}
	for (i = s2; i <= k; i++){
		if (HT[i].weight < HT[s2].weight&&i != s1&&HT[i].parent == 0)
			s2 = i;
	}
	return;
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n){
	//w存放n个字符的权值(均>0)，构造赫夫曼树HT，并求出n个字符的赫夫曼编码HC。
	if (n <= 1)    return;
	int m = 2 * n - 1;
	int i, s1, s2;
	HuffmanTree p;
	HT = (HuffmanTree)malloc((m + 1)*sizeof(HTNode));		//0号单元未用
	for (p = HT, i = 0; i <= n; ++i, ++p, ++w)  
		*p = { *w, 0, 0, 0 };
	for (; i <= m; ++i, ++p)  *p = { 0, 0, 0, 0 };
	for (i = n + 1; i <= m; i++){	//建赫夫曼树
		//在HT[1..i-1]选择parent为0且weight最小的两个结点，其序号分别为s1和s2
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i;    HT[s2].parent = i;
		HT[i].lchild = s1;    HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//从叶子到根逆向求每个字符的赫夫曼编码
	HC = (HuffmanCode)malloc((n + 1)*sizeof(char *));		//分配n个字符编码的头指针向量
	char* cd = (char*)malloc(n*sizeof(char));				//分配求编码的工作空间
	cd[n - 1] = '\0';										//编码结束符
	for (i = 1; i <= n; ++i){								//逐个字符求赫夫曼编码
		int start = n - 1;                                  //编码结束符位置
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) //从叶子到根逆向求编码
			if (HT[f].lchild == c) cd[--start] = '0';
			else cd[--start] = '1';
		HC[i] = (char*)malloc((n - start)*sizeof(char));    //为第i个字符编码分配空间
		strcpy_s(HC[i],n-start, &cd[start]);							//从cd复制编码串到HC
	}
	free(cd);                                       
}

void TempFile(FILE* &temp,FILE* fp,char *ch, HuffmanCode HC){
	//创建临时文件01串
	char* chp;
	char ct;
	while (true){
		chp = ch + 1;
		int k=fread(&ct, 1, 1, fp);
		if (!k)	break;
		while (ct != *chp)	chp++;
		int xxx = chp - ch;
		fputs(HC[xxx], temp);
	}
	fclose(temp);
}

void NewFile(FILE* &fnp, FILE* temp, int n, char *ch, int *w){
	int i; int byte[8]; char c;
	//存储基本信息
	fprintf(fnp, "%d ", n);
	int numzero = 0, zosum = 0;
	while (!feof(temp)){
		fread(&c, sizeof(char), 1, temp);
		zosum = (zosum + 1) % 8;
	}
	zosum=(zosum+7)%8;
	numzero = 8 - zosum;
	fprintf(fnp,"%d ", numzero);
	for (i = 1; i <= n; i++){
		fprintf(fnp,"%c%d ", *(ch + i), *(w + i));
	}
	//存储编码
	for (i = 0; i < numzero; i++){
		byte[i] = 0;
	}
	rewind(temp);
	for (; i < 8; i++){
		c = fgetc(temp);
		if (c == '1')
			byte[i] = 1;
		else
			byte[i] = 0;
	}
	char inttemp = byte[0] + byte[1] * 2 + byte[2] * 4 + byte[3] * 8 + byte[4] * 16 + byte[5] * 32 + byte[6] * 64 + byte[7] * 128;
	fwrite(&inttemp, sizeof(char), 1, fnp);
	while (!feof(temp)){
		for (i = 0; i < 8; i++){
			c = fgetc(temp);
			if (c == '1')
				byte[i] = 1;
			else
				byte[i] = 0;
		}
		if (c == EOF)	break;
		char inttemp = byte[0] + byte[1] * 2 + byte[2] * 4 + byte[3] * 8 + byte[4] * 16 + byte[5] * 32 + byte[6] * 64 + byte[7] * 128;
		fwrite(&inttemp, sizeof(char), 1, fnp);
	}
	fclose(fnp);
}

void compress(char* s){
	char so[100];
	strcpy_s(so, 100, s);
	char st[104];strcpy_s(st, 100, s);
	char sn[105];strcpy_s(sn, 100, s);
	strcat_s(st, 100, ".temp");
	strcat_s(sn, 100, ".mycpr");
	FILE *fp, *fnp, *temp;//源文件，压缩文件，临时文件
	char *ch;//字符
	int n, *w;//总数，权重
	HuffmanCode HC;
	HuffmanTree HT;
	int err = fopen_s(&fp, s, "rb");
	if (err){
		printf("open failed!");
		system("pause");
		exit(1);
	}
	err = fopen_s(&temp, st, "wb");
	if (err){
		printf("Creat tempfile failed!");
		system("pause");
		exit(1);
	}
	err = fopen_s(&fnp, sn, "wb");
	if (err){
		printf("Can't creat a new file!");
		system("pause");
		exit(1);
	}
	Create(fp, ch, w, n);
	HuffmanCoding(HT, HC, w, n);
	rewind(fp);
	TempFile(temp, fp, ch, HC);
	fclose(fp);
	fclose(temp);
	err = fopen_s(&temp, st, "rb");
	if (err){
		printf("Creat tempfile failed!");
		system("pause");
		exit(1);
	}
	NewFile(fnp, temp, n, ch, w);
	fclose(fnp);
	fclose(temp);
	remove(st);
}

void DeTempFile(FILE* fp, FILE* &temp, char* &ch, int &n, int* &w){
	int zeronum = 0, i; char tempc;
	//获取基本信息
	fscanf_s(fp,"%d%d", &n, &zeronum);
	ch = (char*)malloc(n*sizeof(char));
	w = (int*)malloc(n*sizeof(int));
	memset(ch, 0, n*sizeof(char));
	for (i = 1; i <= n; i++){
		fread(&tempc, 1, 1, fp);
		fread(ch + i, 1, 1, fp);
		fscanf_s(fp, "%d", w+i);
	}
	int byte[8];
	unsigned int inttemp;
	char c;
	fread(&c, sizeof(char), 1, fp);
	fread(&c, sizeof(char), 1, fp);
	inttemp = c;
	for (i = 0; i < 8; i++){
		byte[i] = inttemp % 2;
		inttemp = inttemp / 2;
	}
	for (i = zeronum; i < 8; i++){
		fprintf(temp,"%d", byte[i]);
	}
	while (!feof(fp)){
		int k=fread(&c, sizeof(char), 1, fp);
		if (!k)	break;
		inttemp = c;
		for (i = 0; i < 8; i++){
			byte[i] = inttemp % 2;
			inttemp = inttemp / 2;
			fprintf(temp, "%d", byte[i]);
		}
	}
	fclose(fp);
	fclose(temp);
}

void DeNewFile(FILE* temp, FILE* &fnp, char* ch, HuffmanTree HT,int n){
	char c;
	int m;
	while (!feof(temp)){
		m = 2 * n - 1;
		while (m > n){
			c = fgetc(temp);
			if (c == EOF)
				return;
			if (c - '0')
				m = HT[m].rchild;
			else
				m = HT[m].lchild;
		}
		fwrite(ch + m, sizeof(char), 1, fnp);
	}
}

void decompress(char* s){
	FILE *fp, *temp, *fnp;
	char *ch;//字符
	int n, *w;//总数，权重
	int l = strlen(s);
	char so[100]; strcpy_s(so, 100, s);
	char st[105]; strcpy_s(st, 100, s);
	char sn[100]; strcpy_s(sn, 100, s);
	strcat_s(st, 100, ".dtemp");
	strncpy_s(sn, 90, s, l - 6);
	HuffmanCode HC;
	HuffmanTree HT;
	int err = fopen_s(&fp, so, "rb");
	if (err){
		printf("open failed!");
		system("pause");
		exit(1);
	}
	err = fopen_s(&temp, st, "wb");
	if (err){
		printf("Creat tempfile failed!");
		system("pause");
		exit(1);
	}
	DeTempFile(fp, temp, ch, n, w);
	fclose(fp);
	fclose(temp);
	err = fopen_s(&temp, st, "rb");
	if (err){
		printf("Creat tempfile failed!");
		system("pause");
		exit(1);
	}
	HuffmanCoding(HT, HC, w, n);
	err = fopen_s(&fnp, sn, "wb");
	if (err){
		printf("Can't creat a new file!");
		system("pause");
		exit(1);
	}
	DeNewFile(temp, fnp, ch, HT, n);
	fclose(temp);
	remove(st);
	fclose(fnp);
}

int main(){
	int sel = 2;
	printf("1.compress\t2.decompress\n");
	scanf_s("%d", &sel);
	printf("input file:\n");
	char s[100];
	scanf_s("%s", s, 100);
	if (sel==1)
		compress(s);
	else
		decompress(s);
	printf("Finished!\n");
	system("pause");
	return 0;
}
