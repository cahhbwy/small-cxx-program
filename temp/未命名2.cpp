#include<stdio.h>
int main(){
	double b,g,d;
	b=g=d=0.0;
	double t=3.0;
	b+=4.0*t;
	g-=2.0*t;
	bool toBoy=true;
	while(b<7200.0&&g>-3600.0){
		if(toBoy){
			t=(b-d)/(6.0-4.0)<(7200.0-b)/4.0?(b-d)/(6.0-4.0):(7200.0-b)/4.0;
			b+=4.0*t;
			g-=2.0*t;
			d+=6.0*t;
			toBoy=false;
		}else{
			t=(d-g)/(6.0-2.0)<(g+3600.0)/2.0?(d-g)/(6.0-2.0):(g+3600.0)/2.0;
			b+=4.0*t;
			g-=2.0*t;
			d-=6.0*t;
			toBoy=true;
		}
	}
	printf("%lf\n",d/3600.0);
	return 0;
}
