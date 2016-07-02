//输入n，程序生成n个随机数，并对之排序后输出
#include<stdio.h>
#include<stdlib.h>
void swap(int *a,int *b) {
    int t;
    t=*a;
    *a=*b;
    *b=t;
}
void sort(int *p,int n) {
    int i,j,t;
    for (i=0; i<n-1; i++) {
        t=i;
        for (j=i+1; j<n; j++)
            if (*(p+j)<*(p+t))
                t=j;
        swap(p+i,p+t);
    }
    return;
}
int main() {
    int n;
    printf("输入n，程序生成n个随机数，并对之排序后输出\n");
    printf("Enter n:");
    scanf("%d",&n);
    int i;
    int *p;
    if((p=(int*)calloc(n,sizeof(int)))==NULL) {
        printf("空间分配失败\n");
        exit(1);
    }
    printf("原始数据\n");
    for (i=0; i<n; i++) {
        *(p+i)=rand()%100;
        printf("%8d",*(p+i));
    }
    printf("\n排序后\n");
    sort(p,n);
    for (i=0; i<n; i++)
        printf("%8d",*(p+i));
    free(p);
    return 0;
}
