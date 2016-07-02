//这个程序是用来输出两数相除得出的小数的循环部分，输入方法如“1/3”，之后回车 
#include<stdio.h>
#include<stdlib.h>
int main()
{
    int a=0,b=0,n,m,i,j,k,state,find=0;
    int x[1000];
    char c;
    printf("这个程序是用来输出两数相除得出的小数的循环部分，输入方法如“1/3”，之后回车\n"); 
    while ((c=getchar())!='/')
          a=a*10+(c-'0');
    while ((c=getchar())!='\n')
          b=b*10+(c-'0');
//    printf("%d  %d\n",a,b);
    a=a%b;
    
    for (n=0;n<1000;n++){
        x[n]=10*a/b;
        a=(10*a)%b;
//        printf("%d",x[n]);
        }
//    printf("\n");

    for (i=1;i<1000;i++){
         for (j=i-1;j>=0;j--){
             state=1;
             if (x[j]==x[i]){
                for (k=0;k<(i-j);k++)
                    if (x[j+k]!=x[i+k]){
                       state=0;
                       break;
                       }
                if (state==1){
                   find=1;
                   break;
                   }
                }
             }
         n=i;
         m=j;
         while (n<1000){
             if (x[m]!=x[n]){
                find=0;
                break;
                }
             n++;
             m++;
             }
         if (find==1)
            break;
         }
    
    for (k=j;k<i;k++)
        printf("%d",x[k]);
    printf("\n");
    system("PAUSE");
    return 0;
}
