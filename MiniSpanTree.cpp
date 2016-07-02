#include<stdio.h>
#include<stdlib.h>

typedef struct{
	char *v;
	int **e;
	int size;
}Graph;

void init(Graph &G,int size){
	G.size=size;
	G.v=(char*)malloc(size*sizeof(char));
	G.e=(int**)malloc(size*sizeof(int*));
	for(int i=0;i<size;i++){
		*(G.e+i)=(int*)malloc(size*sizeof(int));
		for(int j=0;j<size;j++)
			*((*(G.e+i))+j)=0;
	}
	for(int i=0;i<size;i++){
		printf("input %d v name:",i+1);
		scanf("%c",G.v+i);getchar();
	}
	for(int i=0;i<size;i++)
		for(int j=i+1;j<size;j++){
			printf("%c,%c c(e)=",G.v[i],G.v[j]);
			scanf("%d",(*(G.e+i))+j);
			*(*(G.e+j)+i)=*(*(G.e+i)+j);
		}
}

void print(Graph G){
	printf("\t");
	for(int i=0;i<G.size;i++)
		printf("%8c",G.v[i]);
	printf("\n");
	for(int i=0;i<G.size;i++){
		printf("%8c",G.v[i]);
		for(int j=0;j<G.size;j++)
			printf("%8d",*((*(G.e+i))+j));
		printf("\n");
	}
}

void Copy(Graph G,Graph &NG){
	NG.size=G.size;
	NG.v=(char*)malloc(NG.size*sizeof(char));
	NG.e=(int**)malloc(NG.size*sizeof(int*));
	for(int i=0;i<NG.size;i++){
		*(NG.v+i)=*(G.v+i);
		*(NG.e+i)=(int*)malloc(NG.size*sizeof(int));
		for(int j=0;j<NG.size;j++)
			*((*(NG.e+i))+j)=0;
	}
}
	
	
void MiniSpanTree(Graph G,int n,Graph &NG){
	if(n<1||n>G.size){	printf("Error\n");	return;	}
	n--;
	Copy(G,NG);
	int* tree=(int*)malloc(NG.size*sizeof(int));		//已经生成的树上的顶 
	int* other=(int*)malloc(NG.size*sizeof(int));		//剩余的顶 
	for(int i=0;i<NG.size;i++){
		*(tree+i)=0;			//0表示不存在 
		*(other+i)=1;			//1表示存在 
	}
	tree[n]=1;	//n号点读入
	other[n]=0;
	int treesize=1;
	int othersize=NG.size-1;
	int min,p,q;	//最小权和其关联的顶点
	while(othersize>0){
		min=-1;
		for(int i=0;i<NG.size;i++){		//找到与tree中关联的最小权的边 
			if(tree[i]==1){
				for(int j=0;j<NG.size;j++){
					if(tree[j]==0&&G.e[i][j]>0&&(min>G.e[i][j]||min<0)){
						min=G.e[i][j];
						p=i;
						q=j;
					}
				}
			}
		}
		NG.e[p][q]=min;
		NG.e[q][p]=min;
		tree[q]=1;
		other[q]=0;
		treesize++;
		othersize--;
	}
}

int main(){
	Graph G,NG;
	int size;
	printf("input G size:");
	scanf("%d",&size);getchar();
	init(G,size);
	print(G);
	MiniSpanTree(G,1,NG);
	print(NG);
	return 0;
}
	
