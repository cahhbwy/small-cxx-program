#include<stdio.h>
#include<stdlib.h>
void print(double** a,int m,int n) {			//打印
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            printf("\t%g", *((double*)a + i*n + j));
        }
        printf("\n");
    }
}
void sonMatrix(double** a, double** son,int size,int m,int n) {		//子矩阵
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            *((double*)son+i*(size-1)+j)=*((double*)a + i*size + j);
        }
        for(int j=n+1; j<size; j++) {
            *((double*)son+i*(size-1)+j-1)=*((double*)a + i*size + j);
        }
    }
    for(int i=m+1; i<size; i++) {
        for(int j=0; j<n; j++) {
            *((double*)son+(i-1)*(size-1)+j)=*((double*)a + i*size + j);
        }
        for(int j=n+1; j<size; j++) {
            *((double*)son+(i-1)*(size-1)+j-1)=*((double*)a + i*size + j);
        }
    }
}
void sonMatrix2(double** a,double** son,int size,int m,int n) {		//子矩阵
    int pos=0;
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            *((double*)son+(pos++))=*((double*)a + i*size + j);
        }
        for(int j=n+1; j<size; j++) {
            *((double*)son+(pos++))=*((double*)a + i*size + j);
        }
    }
    for(int i=m+1; i<size; i++) {
        for(int j=0; j<n; j++) {
            *((double*)son+(pos++))=*((double*)a + i*size + j);
        }
        for(int j=n+1; j<size; j++) {
            *((double*)son+(pos++))=*((double*)a + i*size + j);
        }
    }
}
double determinant(double** a,int size) {							//行列式的值
    if(size==1)
        return *((double*)a);
    double son[size-1][size-1];
    double d=0.0;
    for(int i=0; i<size; i++) {
        sonMatrix((double**)a,(double**)son,size,0,i);
        d+=(i%2?-1:1)*(*((double*)a+i))*determinant((double**)son,size-1);
    }
    return d;
}
void companion(double** a,double** b,int size) {						//伴随矩阵
    int pos=0;
    double son[size-1][size-1];
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            sonMatrix((double**)a,(double**)son,size,j,i);
            *((double*)b+(pos++))=determinant((double**)son,size-1);
        }
    }
}
double inverse(double** a,double** ra,int size) {					//求逆
    double d=determinant((double**)a,size);
    int k=size*size;
    if(abs(d-0.0)<0.00001) {
        printf("奇异矩阵\n");
        for(int i=0; i<k; i++) {
            *((double*)ra+i)=0.0;
        }
    } else {
        companion((double**)a,(double**)ra,size);
        for(int i=0; i<k; i++) {
            *((double*)ra+i)/=d;
        }
    }
}
int main() {
    int size=3;
    double a[3][3]= {
        {1,2,3},
        {0,5,5},
        {5,0,2}
    };
    print((double**)a,size,size);
    double b[3][3];
    companion((double**)a,(double**)b,size);
    print((double**)b,size,size);
    double ra[3][3];
    inverse((double**)a,(double**)ra,size);
    print((double**)ra,size,size);
    return 0;
}
