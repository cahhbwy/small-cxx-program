//����n����������n�������������֮��������
#include<stdio.h>
#include<stdlib.h>
void swap(int *a,int *b)
{
	int t;
	t=*a;
	*a=*b;
	*b=t;
}
void sort(int *p,int n)
{
	int i,j;
	for (i=0;i<n-1;i++)
		for (j=i+1;j<n;j++)
			if (*(p+i)>*(p+j))
				swap(p+i,p+j);
	return;
}
int main()
{
	int n;
	printf("����n����������n�������������֮��������\n");
	printf("Enter n:");
	scanf("%d",&n);
	int *p;
	if((p=(int*)calloc(n,sizeof(int)))==NULL){
		printf("�ռ����ʧ��\n");
		exit(1);
	}
	int i;
	printf("ԭʼ����\n"); 
	for (i=0;i<n;i++){
		*(p+i)=rand()%100;
		printf("%8d",*(p+i));
	}
	printf("\n�����\n"); 
	sort(p,n);
	for (i=0;i<n;i++)
		printf("%8d",*(p+i));
	free(p);
	return 0;
}
