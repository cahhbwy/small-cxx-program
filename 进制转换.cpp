//2到36进制的任意转换，字母为小写，输入方式：“16 回车 ff 回车 10 回车” 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int power(int n,int i)//int类型的ｎ的i次幂 
{
    int sum=1;
    while (--i>=0)
          sum*=n;
    return sum;
} 
int check(char c,int m)//检查输入的合法性 
{
    if (m>1&&m<=10)
       if (c-'0'>=0&&c-'0'<=m)
          return 0;
       else
           return 1;
    else
        if ((c-'0'>=0&&c-'0'<=9)||(c-'a'>=0&&c-'a'<(m-10)))
           return 0;
        else
            return 1;
}
int main()
{
	while(1){
		int m,n;//m是输入的数据的进制，n是输出的数据的进制 
		char c;//临时存储读入的字符 
	//构建进制数库
		char a[36];
		int i=0,j=0;
		for (i=0;i<10;i++)
			a[i]='0'+i;
		for (i=0;i<26;i++)
			a[i+10]='a'+i;
	    
	    char b[100]="";//输入的数据以字符串的形式存储 
	//构建输出的字符串并初始化 	
		char re[100]="";
		char *con;
		con=re;
		
		int max;//输入的字符串的位数 
	    int sum=0;//将输入的数据以十进制存储
	//录入数据     
		printf("输入进制\n");
		scanf("%d",&m);
		if (m<=1||m>36){
		   printf("进制错误\n");
	       goto end; 
	    }
	    printf("输入要转换的数字：\n"); 
		while (getchar()!='\n');
		i=0;
		
		while ((c=getchar())!='\n')
			if (check(c,m)) {
			   printf("数据格式错误\n");
			   goto end;
	        }
	        else 
			     b[i++]=c;//存入b中 
		max=i-1;		//获取b的位数
	//转十进制
		for (i=0;i<=max;i++)
			if (b[i]-'0'>=0&&b[i]-'0'<=9) {
				sum+=(b[i]-'0')*power(m,max-i);
	        }
			else {
				sum+=(b[i]-'a'+10)*power(m,max-i);
	        }
	//获取输出数据的进制 
	    printf("输出进制：\n"); 
		scanf("%d",&n);
		while (n<=1) {
	          printf("进制输入错误\n");
	          goto end;
	          }
	//得到sum对n的最大次幂i 
	    i=0;
	    while (power(n,i)<=sum)
	          i++;
	    i--;
	//将sum转成目的进制 
	    while (i>0) {
	          strncat(con,&a[sum/power(n,i)],1);
	          sum=sum%power(n,i);
	          i--; 
	    }
	    strncat(con,&a[sum],1);
	    printf("原数据的%d进制数为\n",n); 
		printf("%s\n",re);
	
		end:
		int x;
		printf("continue?  1.continue   ANYKEY.exit;\n");
		scanf("%d",&x);
		if (x!=1)
			break;
	}
	return 0;
}
