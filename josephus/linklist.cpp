#include "linklist.h"
void initialLinkList(Lptr &L) {
	L=(Lptr)malloc(sizeof(LNode));
	L->elem=-1;
	L->next=L;
}
void insertLinkNode(Lptr &L,int e) {
	if(L->elem==-1){
		L->elem=e;
		return;
	}
	Lptr p=L;
	while(p->next!=L) {
		p=p->next;
	}
	p->next=NULL;
	p->next=(Lptr)malloc(sizeof(LNode));
	p->next->elem=e;
	p->next->next=L;
}
Lptr deleteLinkNode(Lptr &L,int m,int &e) {
	if(L==NULL) {
		return NULL;
	}
	Lptr p=L;
	while(p->next!=L) {
		p=p->next;
	}
	Lptr q=L;
	while(--m>0){
		p=q;
		q=q->next;
	}
	e=q->elem;
	p->next=q->next;
	q->next=NULL;
	free(q);
	return p->next;
}
