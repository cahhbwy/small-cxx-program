#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int symbol[1000];
	int p;
}SymbolStack;

typedef struct{
	int elem[1000];
	int type[1000];
	int p;
}RPN;

typedef struct{
	int num[1000];
	int p;
}Result;

SymbolStack syms;
RPN expr;
Result result;
char buffer[1000];
int length;

void init(){
	syms.p=-1;
	expr.p=-1;
	result.p=-1;
}
void pushSym(int sym){
	syms.symbol[++syms.p]=sym;
}
void pushRPN(int elem,int type){
	++expr.p;
	expr.elem[expr.p]=elem;
	expr.type[expr.p]=type;
}
void pushNum(int num){
	result.num[++result.p]=num;
}
void popSym(int &sym){
	sym=syms.symbol[syms.p--];
}
void popRPN(int &elem,int &type){
	elem=expr.elem[expr.p];
	type=expr.type[expr.p];
	expr.p--;
}
void popNum(int &num){
	num=result.num[result.p--];
}
bool checkExpr(){
	int i=0,j;
	int brackets=0;
	while(i<length){
		if(buffer[i]=='+'||buffer[i]=='-'){
			if(i>0&&(buffer[i-1]=='+'||buffer[i-1]=='-'||buffer[i-1]=='*'||buffer[i-1]=='/')){
				return false;
			}
			if(i==0||buffer[i-1]=='('){
				for(j=length;j>i;j--){
					buffer[j]=buffer[j-1];
				}
				buffer[i]='0';
				length++;
				i++;
			}
		}else if(buffer[i]=='*'||buffer[i]=='/'){
			if(i>0&&(buffer[i-1]=='+'||buffer[i-1]=='-'||buffer[i-1]=='*'||buffer[i-1]=='/'||buffer[i-1]=='(')){
				return false;
			}
		}else if(buffer[i]=='('){
			if(i>0&&(buffer[i-1]==')'||(buffer[i-1]>='0'&&buffer[i-1]<='9'))){
				for(j=length;j>i;j--){
					buffer[j]=buffer[j-1];
				}
				buffer[i]='*';
				length++;
				i++;
			}
			brackets++;
		}else if(buffer[i]==')'){
			brackets--;
			if(brackets<0){
				return false;
			}
		}else if(buffer[i]>='0'&&buffer[i]<='9'){
			if(i>0&&buffer[i-1]==')'){
				for(j=length;j>i;j--){
					buffer[j]=buffer[j-1];
				}
				buffer[i]='*';
				length++;
				i++;
			}
		}else{
			return false;
		}
		i++;
	}
}
int cmpSym(int a,int b){
	if(a=='#'){
		a=0;
	}else if(a=='+'||a=='-'){
		a=2;
	}else if(a=='*'||a=='/'){
		a=3;
	}else if(a=='('){
		a=1;
	}
	if(b=='#'){
		b=0;
	}else if(b=='+'||b=='-'){
		b=2;
	}else if(b=='*'||b=='/'){
		b=3;
	}else if(b=='('){
		b=1;
	}
	return a-b;
}
void printRPN(){
	int i=0;
	int elem,type;
	while(i<=expr.p){
		if(expr.type[i]==0){
			printf(" %d",expr.elem[i]);
		}else{
			printf(" %c",expr.elem[i]);
		}
		i++;
	}
}

int main(){
	char c;
	length=0;
	while((c=getchar())!='\n'){
		buffer[length++]=c;
	}
	//检查表达式
	checkExpr();
	for(int i=0;i<length;i++){
		printf("%c",buffer[i]);
	}
	printf("\n");
	int i=0;
	int a;
	//建立逆波兰表达式 
	init();
	pushSym('#');
	while(i<length){
		if(buffer[i]>='0'&&buffer[i]<='9'){
			a=0;
			do{
				a*=10;
				a+=buffer[i]-'0';
				i++;
			}while(buffer[i]>='0'&&buffer[i]<='9');
			pushRPN(a,0);
			continue;
		}else if(buffer[i]=='+'||buffer[i]=='-'||buffer[i]=='*'||buffer[i]=='/'){
			popSym(a);
			if(cmpSym(buffer[i],a)>0){
				pushSym(a);
				pushSym(buffer[i]);
			}else{
				do{
					pushRPN(a,1);
					popSym(a);
				}while(cmpSym(buffer[i],a)<=0);
				pushSym(a);
				pushSym(buffer[i]);
			}
		}else if(buffer[i]=='('){
			pushSym(buffer[i]);
		}else if(buffer[i]==')'){
			while(true){
				popSym(a);
				if(a!='('){
					pushRPN(a,1);
				}else{
					break;
				}
			}
		}else{
			printf("error expr\n");
			break;
		}
		i++;
	}
	while(syms.p>0){
		popSym(a);
		pushRPN(a,1);
	}
	//打印逆波兰表达式 
	printRPN();
	//计算逆波兰表达式
	i=0;
	int elem,type;
	while(i<=expr.p){
		elem=expr.elem[i];
		type=expr.type[i];
		if(type==0){
			pushNum(elem);
		}else{
			int a,b;
			popNum(b);
			popNum(a);
			if(elem=='+'){
				pushNum(a+b);
			}else if(elem=='-'){
				pushNum(a-b);
			}else if(elem=='*'){
				pushNum(a*b);
			}else if(elem=='/'){
				pushNum(a/b);
			}
		}
		i++;
	}
	printf("\n%d\n",result.num[0]);
	
	return 0;
}		
