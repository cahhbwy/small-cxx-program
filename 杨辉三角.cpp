#include<stdio.h>
#include<stdlib.h>
void combine(void);
void add(void);
void addp(void);
int comb(int n,int m);
int main() {
    printf("杨辉三角\n");
    int x;
    printf("select:	1.combine 2.add\n");
    scanf("%d",&x);
    switch(x) {
    case 1:
        combine();
        break;
    case 2:
        add();
        break;
    default:
        break;
    }
    return 0;
}
void combine(void) {
    int n;
    printf("输出n行\n");
    scanf("%d",&n);
    int i,j;
    for(i=0; i<n; i++) {
        for(j=n; j>i; j--)
            printf("  ");
        for(j=0; j<=i; j++)
            printf("%4d",comb(i,j));
        printf("\n");
    }
    return;
}
int comb(int n,int m) {
    int result=1;
    int i,j;
    for(i=n,j=1; j<=m; i--,j++)
        result*=i;
    for(i=m; i>=1; i--)
        result/=i;
    return result;
}
void add(void) {
    int n;
    printf("输出n行\n");
    scanf("%d",&n);
    int a[n][n];
    int i,j;
    for(i=0; i<n; i++)
        a[i][0]=a[i][i]=1;
    for(i=2; i<n; i++)
        for(j=1; j<i; j++)
            a[i][j]=a[i-1][j-1]+a[i-1][j];
    for(i=0; i<n; i++) {
        for(j=n; j>i; j--)
            printf("  ");
        for(j=0; j<=i; j++)
            printf("%4d",a[i][j]);
        printf("\n");
    }
}
