#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;
struct node{
	int i;
};
void func(struct node *&a){
	a=(struct node *)malloc(sizeof(struct node));
	a->i=10;
}
int main(){
	struct node *a=NULL;
	func(a);
	cout<<a->i<<endl;
	return 0;
}
