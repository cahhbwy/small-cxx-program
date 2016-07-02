#include <stdio.h>
#include "linklist.h"

int main(){
	Lptr L;
	initialLinkList(L);
	int n,m,password;
	printf("please input the data:\nhow many people in total ?\n");
	scanf("%d",&n);
	printf("please input all passwords:\n");
	for(int i=0; i<n; i++) {
		scanf("%d",&password);
		insertLinkNode(L,password);
	}
	printf("please input the initial number:\n");
	scanf("%d",&m);
	//Josephus
	while(L->next!=L){
		L=deleteLinkNode(L,m,password);
		m=password;
		printf("password is %d\n",password);
		printf("these are remained\n");
		{
			Lptr p=L;
			do{
				printf("%d ",p->elem);
				p=p->next;
			}while(p!=L);
			printf("\n");
		}
	}
	printf("The winner's password is %d\n",L->elem);
	return 0;
}
