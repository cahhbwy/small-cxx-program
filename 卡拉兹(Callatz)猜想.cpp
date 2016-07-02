#include<stdio.h>
#include<stdlib.h>
void swap(int *a,int *b)
{
	int t;
	t=*a;
	*a=*b;
	*b=t;
}
int main()
{
	int n,temp;
	scanf("%d",&n);
	int *p=(int*)calloc(n,sizeof(int));
	int i;
	for(i=0;i<n;i++)
		scanf("%d",p+i);
	for(i=0;i<n;i++){
		temp=*(p+i);
		while(temp!=1){
			if(temp%2==0)
				temp/=2;
			else
				temp=(3*temp+1)/2;
			for (int j=0;j<n;j++){
				if((*(p+j)==temp))
					*(p+j)=1;
			}
		}
	}
	for(i=0;i<n-1;i++){
		temp=i;
		for (int j=i+1;j<n;j++)
			if(*(p+temp)<*(p+j))
				temp=j;
		swap(p+temp,p+i);
	}
	for(i=0;*(p+i)!=1;i++)
		printf("%4d",*(p+i));
		 
		
	return 0;
}

