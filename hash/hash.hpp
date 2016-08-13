#ifndef _HASH
#define _HASH

#include <iostream>
#include <cstdlib> 
#include <fstream>
#include <math.h>
#include <cstring>
using namespace std;

void buildHash(char ***hash,ifstream &keyfile){
	char keyword[10];
	*hash=(char**)calloc(43,sizeof(char*));
	while(!keyfile.eof()){
		keyfile>>keyword;
		int length=strlen(keyword);
		int pos=(keyword[0]*100+keyword[length-1])%41;
		while(*((*hash)+pos))	pos=(pos+1)%43;
		(*hash)[pos]=(char*)calloc(length+1,1);
		strncpy((*hash)[pos],keyword,length);
	}
}

void countTime_s(char **hash,ifstream &file,int **count){
	const char separator[]=" \n\t~!@#$%^&*()+-={}[];:\"'<>,.?/|\\";
	char buffer[100];
	char *word;
	int length,pos;
	while((file>>buffer)&&!file.eof()){
		word=strtok(buffer,separator);
		while(word){
			length=strlen(word);
			pos=(word[0]*100+word[length-1])%41;
			while(hash[pos]!=NULL&&strcmp(word,hash[pos])!=0)
				pos=(pos+1)%43;
			if(hash[pos]!=NULL)
				(*count)[pos]++;
			word=strtok(NULL,separator);
		}
	}
}

void countTime(char **hash,ifstream &file1,ifstream &file2,int **count1,int **count2){
	*count1=(int*)calloc(43,sizeof(int));
	*count2=(int*)calloc(43,sizeof(int));
	countTime_s(hash,file1,count1);
	countTime_s(hash,file2,count2);
}
	
void similarity(int *count1,int *count2,double &S,double &D){
	double numerator=0.0L,length1=0.0L,length2=0.0L;
	D=0.0L;
	for(int i=0;i<43;i++){
		numerator+=count1[i]*count2[i];
		length1+=count1[i]*count1[i];
		length2+=count2[i]*count2[i];
		D+=(count1[i]-count2[i])*(count1[i]-count2[i]);
	}
	length1=sqrt(length1);
	length2=sqrt(length2);
	S=numerator/(length1*length2);
	D=sqrt(D);
}
	

#endif
