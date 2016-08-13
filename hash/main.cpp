#include "hash.hpp"

int main(int argc, char** argv) {
	char **hash;
	ifstream keyfile("keyword.txt");
	buildHash(&hash,keyfile);
	keyfile.close();
	ifstream file1("main.cpp");
	ifstream file2("hash.hpp");
	int *count1,*count2;
	countTime(hash,file1,file2,&count1,&count2);

	for(int i=0;i<43;i++){
		printf("%2d%10s\t%d\t%d\n",i,hash[i],count1[i],count2[i]);
	}

	double S,D;
	similarity(count1,count2,S,D);
	printf("S=%g,D=%g\n",S,D);
	return 0;
}
