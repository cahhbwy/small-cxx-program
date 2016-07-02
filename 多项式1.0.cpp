#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TRUE	1
#define FALSE	0
#define OK 		1
#define ERROR 	0

typedef int Status;
typedef struct Mon{
	double coe;
	int exp;
	struct Mon *next;
}Mon,*Pol;

Status Format(Pol &pol);
Status InitPol(Pol &pol);
Status Print(Pol pol);
Status Insert(Pol &pol,double c,int e);
Status Add(Pol pol1,Pol pol2,Pol &pol);
Status Red(Pol pol1,Pol pol2,Pol &pol);
Status Mul(Pol pol1,Pol pol2,Pol &pol);
Status Mul2(Pol pol1,Pol pol2,Pol &pol);
Status Mul3(Pol pol1,Pol pol2,Pol &pol);
Status Copy(Pol oldpol,Pol &newpol);
double Cal(Pol pol,double x);
Status Destroy(Pol &p);
Status Control();

int main(){
	Control();
	return 0;
}

Status Format(Pol &pol){
	Pol p=pol;
	while(p->next!=NULL){
		if(p->next->coe==0.0){
			Pol q=p->next;
			p->next=q->next;
			free(q);
		}
		else
			p=p->next;
	}
	return OK;
}

Status Destroy(Pol &p){
	if(p==NULL)	return OK;
	if(p->next==NULL){
		free(p);
		return OK;
	}
	else{
		Destroy(p->next);
		p->next=NULL;
		return OK;
	}
}

Status Insert(Pol &pol,double c,int e){
	Pol p=pol;
	while(p->next!=NULL&&p->next->exp<e)
		p=p->next;
	if(p->next==NULL){
		p->next=(Pol)malloc(sizeof(Mon));
		p=p->next;
		p->coe=c;
		p->exp=e;
		p->next=NULL;
	}
	else{
		if(p->next->exp==e)	p->next->coe+=c;
		if(p->next->exp>e){
			Pol q=p->next;
			p->next=NULL;
			p->next=(Pol)malloc(sizeof(Mon));
			p->next->coe=c;
			p->next->exp=e;
			p->next->next=q;
		}
	}
	Format(pol);
	return OK;
}

Status InitPol(Pol &pol){
	pol=(Pol)malloc(sizeof(Mon));
	pol->coe=0.0;
	pol->exp=-1;
	pol->next=NULL;
	printf("请输入项数：");
	int n=0;
	scanf("%d",&n);
	if(n <=0)	return FALSE;
	Pol tempM,p;
	for(int i=0;i<n;i++){
		tempM=(Pol)malloc(sizeof(Mon));
		printf("请输入系数和次数："); 
		scanf("%lf%d",&(tempM->coe),&(tempM->exp));
		tempM->next=NULL;
		p=pol;
		while(p->next!=NULL&&p ->next->exp <tempM->exp)
			p=p->next;
		if(p ->next==NULL){	
			p ->next=tempM;
			continue;
		}
		if(p->next!=NULL){
			if(p ->next->exp==tempM->exp)
				p->next->coe+=tempM->coe;
			if(p ->next->exp>tempM->exp){
				tempM->next=p->next;
				p->next=tempM;
			}
		}
	}
	Format(pol);
	return OK;
}

Status Print(Pol pol){
	if(pol==NULL||pol->next==NULL){printf("0\n");	return OK;}
	Pol p=pol->next;
	while(p!=NULL){
		printf("%s",(p!=pol->next&&p->coe>0.0)?"+":"");
		printf("%g",p->coe);
		if(p->exp!=0){
			printf("%s",(p->coe==1.0||p->coe==-1.0)?"\b":"");
			(p->exp==1)?printf("x"):printf("x^%d",p->exp);
		}
		p=p->next;
	}		
	printf("\n");
	return OK;
}
			
Status Add(Pol pol1,Pol pol2,Pol &pol){
	Pol p1,p2,p,temp;
	p1=pol1->next;
	p2=pol2->next;
	pol=(Pol)malloc(sizeof(Mon));
	pol->coe=0.0;
	pol->exp=-1;
	pol->next=NULL;
	p=pol;
	while(p1!=NULL&&p2!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		if(p1->exp<p2->exp){
			temp->coe=p1->coe;
			temp->exp=p1->exp;
			p1=p1->next;
		}
		else if(p1->exp>p2->exp){
			temp->coe=p2->coe;
			temp->exp=p2->exp;
			p2=p2->next;
		}
		else{
			temp->coe=p1->coe+p2->coe;
			temp->exp=p1->exp;
			p1=p1->next;
			p2=p2->next;
		}
		p->next=temp;
		p=p->next;
	}
	while(p1!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		temp->coe=p1->coe;
		temp->exp=p1->exp;
		p1=p1->next;
		p->next=temp;
		p=p->next;
	}
	while(p2!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		temp->coe=p2->coe;
		temp->exp=p2->exp;
		p2=p2->next;
		p->next=temp;
		p=p->next;
	}
	Format(pol);
	return OK;
}

Status Red(Pol pol1,Pol pol2,Pol &pol){
	Pol p1,p2,p,temp;
	p1=pol1->next;
	p2=pol2->next;
	pol=(Pol)malloc(sizeof(Mon));
	pol->coe=0.0;
	pol->exp=-1;
	pol->next=NULL;
	p=pol;
	while(p1!=NULL&&p2!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		if(p1->exp<p2->exp){
			temp->coe=p1->coe;
			temp->exp=p1->exp;
			p1=p1->next;
		}
		else if(p1->exp>p2->exp){
			temp->coe=-p2->coe;
			temp->exp=p2->exp;
			p2=p2->next;
		}
		else{
			temp->coe=p1->coe-p2->coe;
			temp->exp=p1->exp;
			p1=p1->next;
			p2=p2->next;
		}
		p->next=temp;
		p=p->next;
	}
	while(p1!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		temp->coe=p1->coe;
		temp->exp=p1->exp;
		p1=p1->next;
		p->next=temp;
		p=p->next;
	}
	while(p2!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->next=NULL;
		temp->coe=-p2->coe;
		temp->exp=p2->exp;
		p2=p2->next;
		p->next=temp;
		p=p->next;
	}
	Format(pol);
	return OK;
}

Status Mul(Pol pol1,Pol pol2,Pol &pol){
	Pol t,temp,p1,p2,temppol;
	p1=pol1->next;
	p2=pol2->next;
	t=(Pol)malloc(sizeof(Mon));
	t->coe=0.0;
	t->exp=-1;
	t->next=NULL;
	pol=(Pol)malloc(sizeof(Mon));
	pol->coe=0.0;
	pol->exp=-1;
	pol->next=NULL;
	while(p2!=NULL){
		while(p1!=NULL){
			temp=(Pol)malloc(sizeof(Mon));
			temp->coe=p1->coe*p2->coe;
			temp->exp=p1->exp+p2->exp;
			temp->next=NULL;
			t->next=temp;
			Copy(pol,temppol);
			Destroy(pol);
			Add(t,temppol,pol);
			Destroy(temppol);
			Destroy(t->next);
			p1=p1->next;
		}
		p2=p2->next;
		p1=pol1->next;
	}
	Format(pol);
	return OK;
}

Status Mul2(Pol pol1,Pol pol2,Pol &pol){
	Pol t,temp,p,p1,p2,temppol;
	p1=pol1->next;
	p2=pol2->next;
	t=(Pol)malloc(sizeof(Mon));
	t->coe=0.0;
	t->exp=-1;
	t->next=NULL;
	pol=(Pol)malloc(sizeof(Mon));
	pol->coe=0.0;
	pol->exp=-1;
	pol->next=NULL;
	while(p2!=NULL){
		p=t;
		while(p1!=NULL){
			temp=(Pol)malloc(sizeof(Mon));
			temp->coe=p1->coe*p2->coe;
			temp->exp=p1->exp+p2->exp;
			temp->next=NULL;
			p->next=temp;
			p=p->next;
			p1=p1->next;
			temp=NULL;
		}
		Copy(pol,temppol);
		Destroy(pol);
		Add(t,temppol,pol);
		Destroy(temppol);
		Destroy(t->next);
		p2=p2->next;
		p1=pol1->next;
	}
	Format(pol);
	return OK;
}

Status Copy(Pol oldpol,Pol &newpol){
	newpol=(Pol)malloc(sizeof(Mon));
	newpol->coe=0.0;
	newpol->exp=-1;
	newpol->next=NULL;
	Pol p=newpol;
	Pol q=oldpol->next;
	Pol temp;
	while(q!=NULL){
		temp=(Pol)malloc(sizeof(Mon));
		temp->coe=q->coe;
		temp->exp=q->exp;
		temp->next=NULL;
		p->next=temp;
		p=p->next;
		q=q->next;
		temp=NULL;
	}
	return OK;
}

double Cal(Pol pol,double x){
	Pol p=pol;
	double result=0.0;
	while(p!=NULL){
		result+=p->coe*pow(x,p->exp);
		p=p->next;
	}
	return result;
}

Status Control(){
	Pol pol1=NULL,pol2=NULL,pol3=NULL;
	printf("请选择创建函数y1,y2:\n");
	printf("y1:");InitPol(pol1);
	printf("y2:");InitPol(pol2);
	printf("y1=");Print(pol1);
	printf("y2=");Print(pol2);
	char c=' ',t;
	getchar();
	while(c!='0'){
		switch(c){
			case '1':printf("y1=");Print(pol1);printf("y2=");Print(pol2);printf("y3=");Print(pol3);break;
			case '2':Add(pol1,pol2,pol3);printf("y3=");Print(pol3);break;
			case '3':Red(pol1,pol2,pol3);printf("y3=");Print(pol3);break;
			case '4':Mul(pol1,pol2,pol3);printf("y3=");Print(pol3);break;
			case '5':{
					printf("y1=");Print(pol1);
					double c;int e;printf("请输入加入的项的系数和指数:");
					scanf("%lf%d",&c,&e);Insert(pol1,c,e);
					printf("y1=");Print(pol1);
				}break;
			case '6':{
					printf("y2=");Print(pol2);
					double c;int e;printf("请输入加入的项的系数和指数:");
					scanf("%lf%d",&c,&e);Insert(pol2,c,e);
					printf("y2=");Print(pol2);
				}break;
			case '7':Destroy(pol1);printf("y1:");InitPol(pol1);printf("y1=");Print(pol1);break;
			case '8':Destroy(pol2);printf("y2:");InitPol(pol2);printf("y2=");Print(pol2);break;
			case 'c':{
					int i=0;
					double x=0.0;
					printf("选择要计算的多项式：1.y1\t2.y2\t3.y3\n");
					scanf("%d",&i);
					printf("x=");
					scanf("%d",&x);
					switch(i){
						case 1:printf("%g\n",Cal(pol1,x));break;
						case 2:printf("%g\n",Cal(pol2,x));break;
						case 3:printf("%g\n",Cal(pol3,x));break;
						default:printf("Error\n");break;
					}
				}break;
			case '9':Destroy(pol1);Destroy(pol2);Destroy(pol3);printf("y1=0\ny2=0\ny3=0\n");break;
			case '0':break;
			default:printf("选择操作：\n0.exit\t1.查看\t2.y3=y1+y2\t3.y3=y1-y2\t4.y3=y1*y2\tAnyKey.查看操作\n"
					"5.修改y1\t6.修改y2\t7.重置y1\t8.重置y2\t9.销毁全部\nc.计算\n");break;
		}
		scanf("%c",&c);
		while((t=getchar())!='\n')	c=t;
	}
}
