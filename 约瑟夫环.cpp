#include<stdio.h>
#include<stdlib.h>
struct people{
	int num;
	struct people *next;
};
int main()
{
	printf("约瑟夫环是一个数学的应用问题：已知n个人（以编号1，2，3...n分别表示）围坐在一张圆桌周围。从编号为k的人开始报数，数到m的那个人出列；他的下一个人又从1开始报数，数到m的那个人又出列；依此规律重复下去，直到圆桌周围的人全部出列。\n"); 
	int n,k,m;
	printf("Enter n,k,m:"); 
	scanf("%d%d%d",&n,&k,&m);
	int x,i;
	struct people *p,*p1,*p2;
	struct people *peo=(struct people*)calloc(n,sizeof(struct people));
	for(i=0;i<n-1;i++){
		(peo+i)->num=i+1;
		(peo+i)->next=peo+i+1;
	}
	p=peo+n-1;
	p->num=n;
	p->next=peo;
	if (k==1)
		p1=peo+n-1;
	else
		p1=peo+k-2;
	p2=p1->next;
	printf("出局顺序：\n"); 
	while(n>=1){ 
		x=m;
		while(--x>0){
			p1=p2;
			p2=p2->next;
		}	
		printf("%4d",p2->num);
		p2=p2->next;
		p1->next =p2;
		n--;
	}
	printf("\n最后一位：%d\n",p1->num);
	free(peo);
	return 0;
}

