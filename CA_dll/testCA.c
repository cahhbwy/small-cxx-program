#include"CA.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main(int argc,char** argv){
	int array[]={1,2,3,4,5};
	int length=5;
	int select=5;
	long long total;
	int **result;
	//ÅÅÁÐ
	printf("Arrangement:\n");
	result=arrangementArray(array,length,select,&total);
	print(result,total,select);
	//×éºÏ 
	printf("Combination:\n");
	result=combinationArray(array,length,select,&total);
	print(result,total,select);
	return;
}
