#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SWAP(a,b)	if(&a!=&b){a^=b;b^=a;a^=b;}

int* normal(int low,int high,int count);
int* non_repeating(int low,int high,int count);
int* inorder(int start,int steplow,int stephigh,int count);
void change_range(int *array,int count,int low,int high);
void quick_sort(int* array,int p,int r);
void printArray(int *array,int count);

int main(){
	int low=0;
	int high=100;
	int count=100;
	int steplow=2;
	int stephigh=5;
	int *array;
	srand((unsigned)time(NULL));

//	array=normal(low,high,count);
	array=non_repeating(low,high,count);
//	array=inorder(low,steplow,stephigh,count);
	printArray(array,count);
//	change_range(array,count,0,100);
	quick_sort(array,0,count-1);
	printArray(array,count);


	return 0;
}
int* normal(int low,int high,int count){
	int range=high-low;
	if(range<=0){
		return NULL;
	}
	int *array=(int*)calloc(count,sizeof(int));
	while(count-->0){
		array[count]=rand()%range+low;
	}
	return array;
}
int* non_repeating(int low,int high,int count){
	int range=high-low;
	int i,t;
	if(range<=0||range<count){
		return NULL;
	}
	int *array=(int*)calloc(count,sizeof(int));
	int *temp=(int*)calloc(range,sizeof(int));
	for(i=0;i<range;i++){
		temp[i]=i+low;
	}
	for(i=range-1;i>0;i--){
		t=rand()%i; 
		SWAP(temp[i],temp[t]);
	}
	for(i=0;i<count;i++){
		array[i]=temp[i];
	}
	free(temp);
	return array;
}
int* inorder(int start,int steplow,int stephigh,int count){
	int i,step=stephigh-steplow+1;
	if(step<=0||stephigh<=0){
		return NULL;
	}
	int *array=(int*)calloc(count,sizeof(int));
	array[0]=start+rand()%steplow;
	for(i=1;i<count;i++){
		array[i]=array[i-1]+rand()%step+steplow;
	}
	return array;
}
void change_range(int *array,int count,int low,int high){
	double a=(double)(high-low)/(array[count-1]-array[0]);
	double b=(double)(low*array[count-1]-high*array[0])/(array[count-1]-array[0]);
	for(int i=0;i<count;i++){
		array[i]=(int)(a*array[i]+b);
	}
}
int partition(int *array,int p,int r){
	int x=array[r];
	int i=p-1;
	for(int j=p;j<r;j++){
		if(array[j]<=x){
			i++;
			SWAP(array[i],array[j]);
		}
	}
	SWAP(array[i+1],array[r]);
	return i+1;
}
void quick_sort(int* array,int p,int r){
	if(p<r){
		int q=partition(array,p,r);
		quick_sort(array,p,q-1);
		quick_sort(array,q+1,r);
	}
}	
void printArray(int *array,int count){
	if(!array){
		printf("(NULL)\n");
		return;
	}
	for(int i=0;i<count;i++){
		printf("%4d",array[i]);
	}
	printf("\n");
	return;
}

