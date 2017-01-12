#include<stdio.h>
#include<stdlib.h>
typedef struct{
	int *array;
	int *head,*tail;
	int length;
	int mid0,mid1;
}Info;
int max(int a,int b){
	return a>b?a:b;
}
int min(int a,int b){
	return a<b?a:b;
}
int argmin(Info *info,int n){
	int arg=0,i=0;
	while(i<n){
		if(info[i].length>0){
			arg=i;
			break;
		}
		++i;
	}
	while(i<n){
		if(info[i].length>0&&info[i].mid0<info[arg].mid0){
			arg=i;
		}
		++i;
	}
	return arg;
}
int argmax(Info *info,int n){
	int arg=0,i=0;
	while(i<n){
		if(info[i].length>0){
			arg=i;
			break;
		}
		++i;
	}
	while(i<n){
		if(info[i].length>0&&info[i].mid1>info[arg].mid1){
			arg=i;
		}
		++i;
	}
	return arg;
}
void show(Info *info,int n){
	int i,*p;
	printf("+------------------------------+\n");
	for(i=0;i<n;++i){
		for(p=info[i].head;p<=info[i].tail;++p){
			printf("%4d",*p);
		}
		printf("\n");
	}
	printf("+------------------------------+\n");
	return;
}
int reduce(Info *info,int minIndex,int maxIndex){
	int deltaSize=max(min(info[minIndex].length,info[maxIndex].length)/2,1);
	info[minIndex].head+=deltaSize;
	info[minIndex].length-=deltaSize;
	info[minIndex].mid0=*(info[minIndex].head+(info[minIndex].tail-info[minIndex].head)/2);
	if(info[minIndex].length%2){
		info[minIndex].mid1=info[minIndex].mid0;
	}else{
		info[minIndex].mid1=*(info[minIndex].head+(info[minIndex].tail-info[minIndex].head)/2+1);
	}
	info[maxIndex].tail-=deltaSize;
	info[maxIndex].length-=deltaSize;
	info[maxIndex].mid0=*(info[maxIndex].head+(info[maxIndex].tail-info[maxIndex].head)/2);
	if(info[maxIndex].length%2){
		info[maxIndex].mid1=info[maxIndex].mid0;
	}else{
		info[maxIndex].mid1=*(info[maxIndex].head+(info[maxIndex].tail-info[maxIndex].head)/2+1);
	}
	if(minIndex==maxIndex){
		return (info[minIndex].length<=0);
	}else{
		return (info[minIndex].length<=0)+(info[maxIndex].length<=0);
	}
}
int main(int argc,char** argv){
	int n,i,j;
	printf("Enter the number of array:\n");
	scanf("%d",&n);
	int *len=(int*)calloc(n,sizeof(int));
	Info *info=(Info*)calloc(n,sizeof(Info));
	for(i=0;i<n;++i){
		printf("Enter the length of array:\n");
		scanf("%d",&len[i]);
		info[i].length=len[i];
		printf("Enter the array:\n");
		info[i].array=(int*)calloc(info[i].length,sizeof(int));
		info[i].head=info[i].array;
		info[i].tail=info[i].array+(info[i].length-1);
		for(j=0;j<info[i].length;++j){
			scanf("%d",&info[i].array[j]);
		}
		info[i].mid0=*(info[i].head+(info[i].tail-info[i].head)/2);
		if(len[i]%2){
			info[i].mid1=info[i].mid0;
		}else{
			info[i].mid1=*(info[i].head+(info[i].tail-info[i].head)/2+1);
		}
	}
	//show(info,n);
	int alive=n;
	int minIndex,maxIndex,deltaSize;
	while(alive>2){
		minIndex=argmin(info,n);
		maxIndex=argmax(info,n);
		alive-=reduce(info,minIndex,maxIndex);
		//show(info,n);
	}
	int exist0=-1,exist1=-1,exist;
	//FILE* fp=fopen("result.txt","w");
	if(alive==2){
		for(i=0;i<n;++i){
			if(info[i].length>0){
				if(exist0==-1){
					exist0=i;
				}else{
					exist1=i;
					break;
				}
			}
		}
		while(alive>1){
			if(info[exist0].length==1&&info[exist1].length==1){
				printf("%d\n",min(*(info[exist0].head),*(info[exist1].head)));
				//fprintf(fp,"%d",min(*(info[exist0].head),*(info[exist1].head)));
				//fclose(fp);
				return 0;
			}else if(info[exist0].length==1&&info[exist1].length%2&&*(info[exist0].head)<*(info[exist1].head+info[exist1].length/2)&&*(info[exist0].head)>*(info[exist1].head+info[exist1].length/2-1)){
				printf("%d\n",*(info[exist0].head));
				//fprintf(fp,"%d",*(info[exist0].head));
				//fclose(fp);
				return 0;
			}else if(info[exist1].length==1&&info[exist0].length%2&&*(info[exist1].head)<*(info[exist0].head+info[exist0].length/2)&&*(info[exist1].head)>*(info[exist0].head+info[exist0].length/2-1)){
				printf("%d\n",*(info[exist1].head));
				//fprintf(fp,"%d",*(info[exist1].head));
				//fclose(fp);
				return 0;
			}
			minIndex=info[exist0].mid0<info[exist1].mid0?exist0:exist1;
			maxIndex=info[exist0].mid1>info[exist1].mid1?exist0:exist1;
			alive-=reduce(info,minIndex,maxIndex);
			//show(info,n);
		}
		if(info[exist0].length>0){
			exist=exist0;
		}else{
			exist=exist1;
		}
	}else{
		for(i=0;i<n;++i){
			if(info[i].length>0){
				exist=i;
				break;
			}
		}
	}
	printf("%d\n",*(info[exist].head+(info[exist].tail-info[exist].head)/2));
	//fprintf(fp,"%d",*(info[exist].head+(info[exist].tail-info[exist].head)/2));
	//fclose(fp);
	return 0;
}
