/*
*****************************LSFR加密解密*****************************
********************接受两个参数，依次是文件名，密码********************
************加密密匙与文件名相关，更改文件名之后会导致解密失败************
********************运行完后会覆盖原文件，请做好备份********************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
unsigned long key;
char selectBit[8];
void initialKey(char *password, char *nameIn);
void generateSelectBit(char *password, char *nameIn);
void generateNextKey();
unsigned char getByte();
void encrypte(char *nameIn);
void printKey();
int main(int argc, char* argv[]){
	if (argc != 3){
		printf("Wrong argv.\n");
		exit(1);
	}
	char *nameIn = argv[1];
	char *password = argv[2];
	initialKey(password, nameIn);
	generateSelectBit(password, nameIn);
	encrypte(nameIn);
	printf("finished!\n");
	return 0;
}
void initialKey(char *password,char *nameIn){
	int lenPassword = strlen(password);
	int lenNameIn = strlen(nameIn);
	long temp = 0L;
	int i;
	for (i = 0; i < lenPassword; i++){
		temp += (i*lenNameIn)*password[i];
	}
	for (i = 0; i < lenNameIn; i++){
		temp += (i*lenPassword)*nameIn[i];
	}
	key = 1;
	while (key*temp){
		key *= temp;
	}
}
void generateSelectBit(char *password, char *nameIn){
	int lenPassword = strlen(password);
	int lenNameIn = strlen(nameIn);
	selectBit[0] = abs(lenPassword - lenNameIn + 24) % 64;
	selectBit[1] = abs(lenPassword + lenNameIn + 24) % 64;
	selectBit[2] = abs(selectBit[0] * selectBit[1] + 24) % 64;
	selectBit[3] = abs(selectBit[0] % selectBit[1] + 24) % 64;
	selectBit[4] = (lenPassword + 24) % 64;
	selectBit[5] = (lenNameIn + 24) % 64;
	selectBit[6] = abs(selectBit[2] + lenNameIn + 24) % 64;
	selectBit[7] = abs(selectBit[3] + lenPassword + 24) % 64;
}
void generateNextKey(){
	char seed[64];
	int i;
	for (i = 0; i < 60; i++){
		seed[i + 4] = ((key >> i) % 2);
	}
	seed[0] = ((key >> selectBit[0]) ^ (key >> selectBit[4])) % 2;
	seed[1] = ((key >> selectBit[1]) ^ (key >> selectBit[5])) % 2;
	seed[2] = ((key >> selectBit[2]) ^ (key >> selectBit[6])) % 2;
	seed[3] = ((key >> selectBit[3]) ^ (key >> selectBit[7])) % 2;
	key = 0L;
	for (i = 0; i < 64; i++){
		key += (seed[i] << i);
	}
}
unsigned char getByte(){
	return (unsigned char)key;
}
void encrypte(char *nameIn){
	FILE *fileIn = fopen(nameIn, "rb");
	if (!fileIn){
		perror("fopen");
		exit(2);
	}
	char *nameTmp = "lsfr.temp";
	FILE *fileTmp = fopen(nameTmp, "wb");
	if (!fileTmp){
		perror("fopen");
		exit(2);
	}
	unsigned char buffer;
	while (fread(&buffer, sizeof(char), 1, fileIn)){
		generateNextKey();
		buffer ^= getByte();
		fwrite(&buffer, sizeof(char), 1, fileTmp);
	}
	fclose(fileIn);
	fclose(fileTmp);
	remove(nameIn);
	rename(nameTmp, nameIn);
}
void printKey(){
	for (int i = 63; i >= 0; i--){
		printf("%c", ((key >> i) % 2) | 0x30);
	}
	printf("\n");
}
