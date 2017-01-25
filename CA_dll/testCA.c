/*

There is an array NUMS[] with a total of N+2 numbers. The first and the last element are equal to 1 ( NUMS[0]=1, NUMS[N+1]=1 ).
You are asked to remove one element each time until you remove every element except NUMS[0] and NUMS[N+1] in this array. Please note that you should not remove NUMS[0] and NUMS[N+1]. Each time you remove a element, you will get a score. For example, if you remove NUMS[i], the score is  NUMS[i-1]*NUMS[i]*NUMS[i+1]. The scores add up. At the end, you will remove a total of N elements and the array becomes [1,1]. 

Please find out Find out a sequence to remove the elements that results in the maximum score. Return the score.



Example:

1 <= N <= 500
1 <= Nums[i] <= 500

Given NUMS = [1, 3, 1, 5, 7, 1]

Return 148

NUMS  = [1,3,1,5,7,1] --> [1,3,5,7,1] --> [1,3,7,1] --> [1,7,1] --> [1,1]
scores =   3*1*5    +    3*5*7  +  1*3*7  + 1*7*1  = 148

*/



/*Finish the problem using the following framework.*/
//algorithm
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
