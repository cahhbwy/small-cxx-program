#include "CA.h"
int factorial(int i,int end){
	if(i<=end){
		return 1;
	}else{
		return i*factorial(i-1,end);
	}
}
int arrangementNumber(int n,int m){
	return factorial(n,n-m);
}
int combinationNumber(int n,int m){
	return factorial(n,n-m)/factorial(m,1);
}
int** arrangementArray(int* array,int length,int select,int *total){
	*total=arrangementNumber(length,select);
	int **result=(int**)calloc(*total,sizeof(int*));
	char *status=(char*)calloc(length,sizeof(char));
	int **point=(int**)calloc(select,sizeof(int*));
	int i,j,k;
	for(i=0;i<*total;++i){
		result[i]=(int*)calloc(select,sizeof(int));
	}
	memset(status,false,length);
	memset(point,0x00,select*sizeof(int*));
	//deepth search
	char back=false;
	i=0;
	j=0;
	point[0]=array;
	status[0]=true;
	while(i<*total){
		if(back){
			for(k=point[j]-array+1;k<length;++k){
				if(!status[k]){
					status[point[j]-array]=false;
					point[j]=array+k;
					status[k]=true;
					back=false;
					break;
				}
			}
			if(k>=length){
				status[point[j]-array]=false;
				point[j]=NULL;
				--j;
			}
		}else{
			if(j>=select-1){
				back=true;
				for(k=0;k<select;++k){
					result[i][k]=*(point[k]);
				}
				++i;
			}else{
				for(k=0;k<length;++k){
					if(!status[k]){
						break;
					}
				}
				++j;
				point[j]=array+k;
				status[k]=true;
			}
		}
	}
	return result;
}
int** combinationArray(int* array,int length,int select,int *total){
	*total=combinationNumber(length,select);
	int **result=(int**)calloc(*total,sizeof(int*));
	char *status=(char*)calloc(length,sizeof(char));
	int **point=(int**)calloc(select,sizeof(int*));
	int i,j,k;
	for(i=0;i<*total;++i){
		result[i]=(int*)calloc(select,sizeof(int));
	}
	memset(status,false,length);
	memset(point,0x00,select*sizeof(int*));
	//deepth search
	char back=false;
	i=0;
	j=0;
	point[0]=array;
	status[0]=true;
	while(i<*total){
		if(back){
			if(length-(point[j]-array)<=select-j){
				status[point[j]-array]=false;
				point[j]=NULL;
				--j;
			}else{
				status[point[j]-array]=false;
				++point[j];
				status[point[j]-array]=true;
				back=false;
			}
		}else{
			if(j>=select-1){
				back=true;
				for(k=0;k<select;++k){
					result[i][k]=*(point[k]);
				}
				++i;
			}else{
				++j;
				point[j]=point[j-1]+1;
				status[point[j]-array]=true;
			}
		}
	}
	return result;
}
void print(int** array,int total,int size){
	int i,j;
	for(i=0;i<total;++i){
		for(j=0;j<size;++j){
			printf("%4d",array[i][j]);
		}
		printf("\n");
	}
}
