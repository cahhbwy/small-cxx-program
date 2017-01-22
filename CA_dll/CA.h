#ifndef _CA_H_
#define _CA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true  0x01
#define false 0x00

int factorial(int i,int end);
int arrangementNumber(int n,int m);
int combinationNumber(int n,int m);
int** arrangementArray(int* array,int length,int select,int *total);
int** combinationArray(int* array,int length,int select,int *total);
void print(int** array,int total,int size);

#endif
