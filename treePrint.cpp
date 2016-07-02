#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
typedef struct TreeNode {
	int elem;
	int position;
	struct TreeNode *parent,*lchild,*rchild;
} TreeNode,*TreePtr;

int pos=0;

void generate(int num){
	srand((unsigned)time(NULL));
	FILE* fp=fopen("num.txt","w");
	while(num>0){
		fprintf(fp,"%d\n",rand()%1000);
		num--;
	}
	fclose(fp);
}

void childADD(TreePtr p){
	if(p){
		p->position++;
		childADD(p->lchild);
		childADD(p->rchild);
	}
}

void insertTree(TreePtr &tree,int e) {
	if(!tree) {
		tree=(TreePtr)malloc(sizeof(TreeNode));
		tree->elem=e;
		tree->position=0;
		tree->parent=NULL;
		tree->lchild=NULL;
		tree->rchild=NULL;
	} else {
		TreePtr p=tree;
		TreePtr pp;
		while(p) {
			pp=p;
			if(e<p->elem) {
				p->position++;
				childADD(p->rchild);
				p=p->lchild;
			} else {
				p=p->rchild;
			}
		}
		if(e<pp->elem) {
			pp->lchild=(TreePtr)malloc(sizeof(TreeNode));
			p=pp->lchild;
			p->position=pp->position-1;
		} else {
			pp->rchild=(TreePtr)malloc(sizeof(TreeNode));
			p=pp->rchild;
			p->position=pp->position+1;
		}
		p->elem=e;
		p->parent=pp;
		p->lchild=NULL;
		p->rchild=NULL;
	}
}

void insertTree2(TreePtr &tree,int e){
	if(!tree) {
		tree=(TreePtr)malloc(sizeof(TreeNode));
		tree->elem=e;
		tree->position=0;
		tree->parent=NULL;
		tree->lchild=NULL;
		tree->rchild=NULL;
	} else {
		TreePtr p=tree;
		TreePtr pp;
		while(p) {
			pp=p;
			if(e<p->elem) {
				p=p->lchild;
			} else {
				p=p->rchild;
			}
		}
		if(e<pp->elem) {
			pp->lchild=(TreePtr)malloc(sizeof(TreeNode));
			p=pp->lchild;
		} else {
			pp->rchild=(TreePtr)malloc(sizeof(TreeNode));
			p=pp->rchild;
		}
		p->elem=e;
		p->parent=pp;
		p->lchild=NULL;
		p->rchild=NULL;
	}
}

void input(int num,TreePtr &tree){
	FILE* fp=fopen("num.txt","r");
	while(num>0){
		int temp;
		fscanf(fp,"%d",&temp);
		insertTree2(tree,temp);
		num--;
	}
	fclose(fp);
}

void printIN(TreePtr p){
	if(p){
		printIN(p->lchild);
//		printf("(%2d,%2d)",p->elem,p->position);
		p->position=pos;
		pos++;
		printIN(p->rchild);
	}
}

void gotoxy(int x,int y){
	COORD coord={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	return;
}

void drawTree(TreePtr p,int line,int &deepth){
	if(p){
		deepth=deepth>line?deepth:line;
		drawTree(p->lchild,line+1,deepth);
		gotoxy(p->position*2,2*line);
		printf("%d",p->elem);
		if(p->lchild){
			gotoxy(p->lchild->position*2,2*line+1);
			printf("©°");
			for(int i=p->lchild->position+1;i<p->position;i++){
				printf("¡ª");
			}
			if(p->rchild){
				printf("©Ø");
			}else{
				printf("©¼");
			}
		}
		if(p->rchild){
			gotoxy(p->position*2,2*line+1);
			if(p->lchild){
				printf("©Ø");
			}else{
				printf("©¸");
			}
			for(int i=p->position+1;i<p->rchild->position;i++){
				printf("¡ª");
			}
			printf("©´");
		}
		drawTree(p->rchild,line+1,deepth);
	}
}

void drawTree(TreePtr tree){
	system("cls");
	int deepth=0;
	drawTree(tree,0,deepth);
	COORD coord={0,2*deepth+3};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void drawTreeDot(TreePtr tree,FILE *file){
	if(tree->lchild){
		fprintf(file,"%d -> %d\n",tree->elem,tree->lchild->elem);
		drawTreeDot(tree->lchild,file);
	}
	if(tree->rchild){
		fprintf(file,"%d -> %d\n",tree->elem,tree->rchild->elem);
		drawTreeDot(tree->rchild,file);
	}
}

void drawTreeDot(TreePtr tree){
	FILE *file=fopen("tree.gv","w");
	fprintf(file,"digraph tree{\n");
	drawTreeDot(tree,file);
	fprintf(file,"}\n");
	fclose(file);
}

int main() {
	TreePtr tree=NULL;
	generate(30);
	input(30,tree);
	printIN(tree);
	drawTree(tree);
	drawTreeDot(tree);
	return 0;
}
