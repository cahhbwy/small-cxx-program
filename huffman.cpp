/*
	*******************************************************************************
	**********接受两个参数，第一个是要处理的文件名，第二个是处理后的文件名*************
	********************正确的参数会进入压缩和解压的选项*****************************
	*******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	unsigned char elem;
	unsigned int weight;
	unsigned short parent;
	unsigned short lchild;
	unsigned short rchild;
	char* code;
}huffmanTree;
typedef struct{
	huffmanTree *HT;
	unsigned short length;
}Huffman;
unsigned short calcWeight(int *weight, FILE *fileIn);
void creatHT(Huffman &huffman, int *weight);
void creatHC(Huffman huffman);
void creatTF(Huffman huffman, FILE *fileIn, FILE *fileTmp);
void creatHF(Huffman huffman, FILE *fileTmp, FILE *fileOut);
void decompress(Huffman &huffman, FILE *fileIn,char *nameTmp, FILE *fileOut);
void printHT(Huffman huffman);


int main(int argc,char *argv[]){
	if (argc != 3){
		printf("Wrong argv.\n");
		return -1;
	}
	char *nameIn = argv[1];
	char *nameTmp = "huffman.temp";
	char *nameOut = argv[2];
	int weight[256];
	FILE *fileIn, *fileOut;
	Huffman huffman;
	huffman.HT = NULL;
	int select;
	printf("select:1.compress,2.decompress\n");
	scanf("%d", &select);
	if (select == 1){
		FILE *fileTmp;
		fileIn = fopen(nameIn, "rb");
		if (!fileIn){
			printf("Can't open file %s.\n", nameIn);
			return 1;
		}
		huffman.length = calcWeight(weight, fileIn);
		creatHT(huffman, weight);
		creatHC(huffman);
		fileTmp = fopen(nameTmp, "w");
		if (!fileTmp){
			printf("Can't creat temp file.\n");
			return 2;
		}
		creatTF(huffman, fileIn, fileTmp);
		fclose(fileIn);
		fclose(fileTmp);
		fileTmp = fopen(nameTmp, "r");
		if (!fileTmp){
			printf("Can't open temp file.\n");
			return 3;
		}
		fileOut = fopen(nameOut, "wb");
		if (!fileOut){
			printf("Can't creat output file %s.\n",nameOut);
			return 4;
		}
		creatHF(huffman, fileTmp, fileOut);
		fclose(fileTmp);
		fclose(fileOut);
		printHT(huffman);
		fclose(fileIn);
		printf("Compressed!\n");
	}
	else if (select == 2){
		fileIn = fopen(nameIn, "rb");
		if (!fileIn){
			printf("Can't open file %s.\n", nameIn);
			return 1;
		}
		fileOut = fopen(nameOut, "wb");
		if (!fileOut){
			printf("Can't creat output file %s.\n", nameOut);
			return 4;
		}
		decompress(huffman, fileIn, nameTmp, fileOut);
		printf("Decompressed!\n");
	}
	else
	{
		printf("Error select!\n");
	}
	remove(nameTmp);
	return 0;
}
unsigned short calcWeight(int *weight, FILE *fileIn){
	memset(weight, 0, 256 * sizeof(int));
	unsigned char bufferIn;
	while (fread(&bufferIn, sizeof(char), 1, fileIn)){
		weight[bufferIn]++;
	}
	unsigned short i,count=0;
	for (i = 0; i < 256; i++){
		if (weight[i] > 0){
			count++;
		}
	}
	return count;
}
void find2min(int &min1, int &min2, huffmanTree* HT,short curlen){
	min1 = min2 = 0;
	short i = 1;
	while (i < curlen){
		if (HT[i].parent == 0){
			if (HT[i].weight < HT[min2].weight){
				if (HT[i].weight < HT[min1].weight){
					min2 = min1;
					min1 = i;
				}
				else{
					min2 = i;
				}
			}
		}
		i++;
	}
}
void creatHT(Huffman &huffman, int *weight){
	huffman.HT = (huffmanTree *)calloc(huffman.length * 2, sizeof(huffmanTree));
	memset(huffman.HT, 0, sizeof(huffmanTree));
	huffman.HT[0].weight = 4294967295;
	int i, j = 1;
	for (i = 0; i < 256; i++){
		if (weight[i] > 0){
			huffman.HT[j].elem = i;
			huffman.HT[j].weight = weight[i];
			j++;
		}
	}
	int min1, min2;
	int curlen = huffman.length + 1;;
	while (curlen < huffman.length * 2){
		find2min(min1, min2, huffman.HT, curlen);
		huffman.HT[min1].parent = curlen;
		huffman.HT[min2].parent = curlen;
		huffman.HT[curlen].lchild = min1;
		huffman.HT[curlen].rchild = min2;
		huffman.HT[curlen].weight = huffman.HT[min1].weight + huffman.HT[min2].weight;
		curlen++;
	}
}
void creatHC(Huffman huffman){
	char *buffer = (char*)calloc(huffman.length * 2, sizeof(char));
	int i, j, codeLen, pos;
	for (i = 1; i <= huffman.length; i++){
		codeLen = 0;
		pos = i;
		memset(buffer, 0, huffman.length * 2);
		while (pos != huffman.length * 2 - 1){
			if (huffman.HT[huffman.HT[pos].parent].lchild == pos){
				buffer[codeLen] = '0';
			}
			else
			{
				buffer[codeLen] = '1';
			}
			pos = huffman.HT[pos].parent;
			codeLen++;
		}
		huffman.HT[i].code = (char*)calloc(codeLen + 1, sizeof(char));
		for (j = 0; j < codeLen; j++){
			huffman.HT[i].code[j] = buffer[codeLen - j - 1];
		}
	}
}
void creatTF(Huffman huffman, FILE *fileIn, FILE *fileTmp){
	fseek(fileIn, 0, SEEK_SET);
	unsigned char bufferIn;
	unsigned short i;
	while (fread(&bufferIn, sizeof(char), 1, fileIn)){
		for (i = 1; i <= huffman.length; i++){
			if (bufferIn == huffman.HT[i].elem){
				fprintf(fileTmp, "%s", huffman.HT[i].code);
				break;
			}
		}
	}
}
void creatHF(Huffman huffman, FILE *fileTmp, FILE *fileOut){
	fseek(fileTmp, 0, SEEK_END);
	unsigned char zero = (8 - ftell(fileTmp) % 8) % 8;
	fwrite(&zero, sizeof(unsigned char), 1, fileOut);
	fseek(fileTmp, 0, SEEK_SET);
	fwrite(&huffman.length, sizeof(unsigned short), 1, fileOut);
	int i;
	for (i = 1; i <= huffman.length; i++){
		fwrite(&huffman.HT[i].elem, sizeof(unsigned char), 1, fileOut);
		fwrite(&huffman.HT[i].weight, sizeof(unsigned int), 1, fileOut);
	}
	unsigned char bufferIn[8], bufferOut;
	for (i = 0; i < zero; i++){
		bufferIn[i] = 0;
	}
	for (i = zero; i < 8; i++){
		bufferIn[i] = fgetc(fileTmp);
		if (bufferIn[i] == '1' || bufferIn[i] == '0'){
			bufferIn[i] &= 0x0F;
		}
		else
		{
			return;
		}
	}
	bufferOut = (bufferIn[0] << 7) + (bufferIn[1] << 6) + (bufferIn[2] << 5) + (bufferIn[3] << 4) + (bufferIn[4] << 3) + (bufferIn[5] << 2) + (bufferIn[6] << 1) + (bufferIn[7]);
	fwrite(&bufferOut, sizeof(unsigned char), 1, fileOut);
	while (!feof(fileTmp)){
		for (i = 0; i < 8; i++){
			bufferIn[i] = (fgetc(fileTmp) & 0x0F);
		}
		if (bufferIn[7] == 0x0F)	return;
		bufferOut = (bufferIn[0] << 7) + (bufferIn[1] << 6) + (bufferIn[2] << 5) + (bufferIn[3] << 4) + (bufferIn[4] << 3) + (bufferIn[5] << 2) + (bufferIn[6] << 1) + (bufferIn[7]);
		fwrite(&bufferOut, sizeof(unsigned char), 1, fileOut);
	}
}
void decompress(Huffman &huffman, FILE *fileIn,char *nameTmp,FILE *fileOut){
	unsigned char zero;
	unsigned short i;
	FILE *fileTmp;
	fread(&zero, sizeof(unsigned char), 1, fileIn);
	fread(&huffman.length, sizeof(unsigned short), 1, fileIn);
	huffman.HT = (huffmanTree *)calloc(huffman.length * 2, sizeof(huffmanTree));
	memset(huffman.HT, 0, sizeof(huffmanTree));
	huffman.HT[0].weight = 4294967295;
	for (i = 1; i <= huffman.length; i++){
		fread(&huffman.HT[i].elem, sizeof(unsigned char), 1, fileIn);
		fread(&huffman.HT[i].weight, sizeof(unsigned int), 1, fileIn);
	}
	int min1, min2;
	int curlen = huffman.length + 1;;
	while (curlen < huffman.length * 2){
		find2min(min1, min2, huffman.HT, curlen);
		huffman.HT[min1].parent = curlen;
		huffman.HT[min2].parent = curlen;
		huffman.HT[curlen].lchild = min1;
		huffman.HT[curlen].rchild = min2;
		huffman.HT[curlen].weight = huffman.HT[min1].weight + huffman.HT[min2].weight;
		curlen++;
	}
	unsigned char bufferIn,bufferOut;
	if (!feof(fileIn)){
		if (fread(&bufferIn, sizeof(unsigned char), 1, fileIn)){
			fileTmp = fopen(nameTmp, "w");
			if (!fileTmp){
				printf("Can't creat temp file.\n");
				exit(2);
			}
			for (i = zero; i < 8; i++){
				bufferOut = ((bufferIn >> (7 - i)) % 2) | 0x30;
				fputc(bufferOut, fileTmp);
			}
			while (fread(&bufferIn, sizeof(unsigned char), 1, fileIn) && !feof(fileIn)){
				for (i = 0; i < 8; i++){
					bufferOut = ((bufferIn >> (7 - i)) % 2) | 0x30;
					fputc(bufferOut, fileTmp);
				}
			}
		}
	}
	fclose(fileTmp);
	fileTmp = fopen(nameTmp, "r");
	if (!fileTmp){
		printf("Can't read temp file.\n");
		exit(3);
	}
	unsigned short pos = huffman.length * 2 - 1;
	while (!feof(fileTmp)){
		bufferIn = fgetc(fileTmp);
		if (bufferIn == '0'){
			pos = huffman.HT[pos].lchild;
		}
		else if(bufferIn=='1'){
			pos = huffman.HT[pos].rchild;
		}
		else
		{
			fclose(fileTmp);
			break;
		}
		if (pos <= huffman.length){
			fwrite(&huffman.HT[pos].elem, sizeof(unsigned char), 1, fileOut);
			pos = huffman.length * 2 - 1;
		}
	}
}
void printHT(Huffman huffman){
	FILE *fileHT = fopen("HT.txt", "w");
	int i;
	for (i = 0; i <= huffman.length; i++){
		if (huffman.HT[i].code){
			fprintf(fileHT, "%4d%16u%8d%8d%8d\t%s\n", huffman.HT[i].elem, huffman.HT[i].weight, huffman.HT[i].parent, huffman.HT[i].lchild, huffman.HT[i].rchild, huffman.HT[i].code);
		}
		else{
			fprintf(fileHT, "%4d%16u%8d%8d%8d\n", huffman.HT[i].elem, huffman.HT[i].weight, huffman.HT[i].parent, huffman.HT[i].lchild, huffman.HT[i].rchild);
		}
	}
	for (i = huffman.length + 1; i < huffman.length * 2; i++){
		fprintf(fileHT, "%4d%16u%8d%8d%8d\n", huffman.HT[i].elem, huffman.HT[i].weight, huffman.HT[i].parent, huffman.HT[i].lchild, huffman.HT[i].rchild);
	}
}