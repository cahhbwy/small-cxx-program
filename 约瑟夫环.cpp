#include<stdio.h>
#include<stdlib.h>
struct people{
	int num;
	struct people *next;
};
int main()
{
	printf("Լɪ����һ����ѧ��Ӧ�����⣺��֪n���ˣ��Ա��1��2��3...n�ֱ��ʾ��Χ����һ��Բ����Χ���ӱ��Ϊk���˿�ʼ����������m���Ǹ��˳��У�������һ�����ִ�1��ʼ����������m���Ǹ����ֳ��У����˹����ظ���ȥ��ֱ��Բ����Χ����ȫ�����С�\n"); 
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
	printf("����˳��\n"); 
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
	printf("\n���һλ��%d\n",p1->num);
	free(peo);
	return 0;
}

