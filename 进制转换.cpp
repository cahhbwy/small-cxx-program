//2��36���Ƶ�����ת������ĸΪСд�����뷽ʽ����16 �س� ff �س� 10 �س��� 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int power(int n,int i)//int���͵ģ��i���� 
{
    int sum=1;
    while (--i>=0)
          sum*=n;
    return sum;
} 
int check(char c,int m)//�������ĺϷ��� 
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
		int m,n;//m����������ݵĽ��ƣ�n����������ݵĽ��� 
		char c;//��ʱ�洢������ַ� 
	//������������
		char a[36];
		int i=0,j=0;
		for (i=0;i<10;i++)
			a[i]='0'+i;
		for (i=0;i<26;i++)
			a[i+10]='a'+i;
	    
	    char b[100]="";//������������ַ�������ʽ�洢 
	//����������ַ�������ʼ�� 	
		char re[100]="";
		char *con;
		con=re;
		
		int max;//������ַ�����λ�� 
	    int sum=0;//�������������ʮ���ƴ洢
	//¼������     
		printf("�������\n");
		scanf("%d",&m);
		if (m<=1||m>36){
		   printf("���ƴ���\n");
	       goto end; 
	    }
	    printf("����Ҫת�������֣�\n"); 
		while (getchar()!='\n');
		i=0;
		
		while ((c=getchar())!='\n')
			if (check(c,m)) {
			   printf("���ݸ�ʽ����\n");
			   goto end;
	        }
	        else 
			     b[i++]=c;//����b�� 
		max=i-1;		//��ȡb��λ��
	//תʮ����
		for (i=0;i<=max;i++)
			if (b[i]-'0'>=0&&b[i]-'0'<=9) {
				sum+=(b[i]-'0')*power(m,max-i);
	        }
			else {
				sum+=(b[i]-'a'+10)*power(m,max-i);
	        }
	//��ȡ������ݵĽ��� 
	    printf("������ƣ�\n"); 
		scanf("%d",&n);
		while (n<=1) {
	          printf("�����������\n");
	          goto end;
	          }
	//�õ�sum��n��������i 
	    i=0;
	    while (power(n,i)<=sum)
	          i++;
	    i--;
	//��sumת��Ŀ�Ľ��� 
	    while (i>0) {
	          strncat(con,&a[sum/power(n,i)],1);
	          sum=sum%power(n,i);
	          i--; 
	    }
	    strncat(con,&a[sum],1);
	    printf("ԭ���ݵ�%d������Ϊ\n",n); 
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
