#include<stdio.h>
#include<stdlib.h>
int** create(int rows,int cols){
	int **m=(int**)calloc(rows,sizeof(int*));
	for(int i=0;i<rows;i++){
		m[i]=(int*)calloc(cols,sizeof(int));
	}
	return m;
}
void fill(int rows,int cols,int** m,int start,int deepth){
	if(deepth*2+1>rows)	return;
	for(int i=deepth;i<cols-deepth;i++){
		m[deepth][i]=start++;
	}
	start--;
	if(deepth*2+1>cols)	return;
	for(int i=deepth;i<rows-deepth;i++){
		m[i][cols-deepth-1]=start++;
	}
	start--;
	if(deepth*2+2>rows)	return;
	for(int i=cols-deepth-1;i>deepth;i--){
		m[rows-deepth-1][i]=start++;
	}
	if(deepth*2+2>cols)	return;
	for(int i=rows-deepth-1;i>deepth;i--){
		m[i][deepth]=start++;
	}
	fill(rows,cols,m,start,deepth+1);
}
void show(int rows,int cols,int **m){
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			printf("%4d",m[i][j]);
		}
		printf("\n");
	}
}
int main(){
	int rows,cols;
	scanf("%d%d",&rows,&cols);
	int **m=create(rows,cols);
	fill(rows,cols,m,0,0);
	show(rows,cols,m);

	return 0;
}
