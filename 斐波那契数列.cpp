//ì³²¨ÄÇÆõÊýÁÐ 
#include<stdio.h>
void print(void)
{
	long long a=1,b=1,c;
	printf("%16Id%16Id",a,b);
	for (int i=0;i<68;i++){
		c=a+b;
		printf("%16Id",c);
		a=b;
		b=c;
	}
	return;
}
long long search(int n)
{
	long long a=1,b=1,c;
	if (n==1||n==2)
		c=1;
	else
		for (int i=3;i<=n;i++){
			c=a+b;
			a=b;
			b=c;
		}
	return c;
}
void judge(long long n)
{
	int i=2;
	long long a=1,b=1,c=1;
	if(n==1)
		printf("1 or 2\n");
	else{
		while(c<n){
			c=a+b;
			a=b;
			b=c;
			i++;
		}
		if(c==n)
			printf("%d\n",i);
		else
			printf("wrong number!\n");
	}
}
int main()
{
	while(1){
		int x;
		printf("1.print		2.search	3.judge\n");
		scanf("%d",&x);
		switch(x){
			case 1:print();break;
			case 2:{
				int n;
				printf("Enter n:\n");
				scanf("%d",&n);
				printf("%Id\n",search(n));
			}
			break;
			case 3:{
				long long n;
				printf("Enter n:\n");
				scanf("%I64d",&n);
				judge(n);
			}
			break;
			default:break;
		}
		int y;
		printf("continue? 1.contunue ANYKEY.exit;\n");
		scanf("%d",&y);
		if(y!=1)
			break;
	}
	return 0;
}

