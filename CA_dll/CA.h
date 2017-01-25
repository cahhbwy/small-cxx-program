#ifndef _CA_H_
#define _CA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true  0x01
#define false 0x00

long long factorial(int i,int end);
long long arrangementNumber(int n,int m);
long long combinationNumber(int n,int m);
int** arrangementArray(int* array,int length,int select,long long *total);
int** combinationArray(int* array,int length,int select,long long *total);
void print(int** array,int total,int size);

#endif
