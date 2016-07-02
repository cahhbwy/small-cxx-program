#ifndef _LinkList
#define _LinkList
#include<stdlib.h>
typedef struct Node {
	int elem;
	struct Node *next;
} LNode,*Lptr;
void initialLinkList(Lptr &L);
void insertLinkNode(Lptr &L,int e);
Lptr deleteLinkNode(Lptr &L,int n,int &e);

#endif

