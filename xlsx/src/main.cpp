#include "include.h"
#include "correl.h"

using namespace libxl;
using namespace correl;

int main(int argc, char** argv) {
	int wovNum = 29;
	LPSCORE data = (LPSCORE)calloc(wovNum, sizeof(SCORE));
	fileParse("data.xlsx", data, wovNum);
	
	for(int i = 0; i < wovNum; ++i) {
		printf("%-20s%6g%6g%6g%6g%6g%6g\n", data[i].wavName, data[i].totalScore_H, data[i].totalScore_M, data[i].fluScore_H, data[i].fluScore_M, data[i].accScore_H, data[i].accScore_M);
	}
	
	double correlTotalScore, correlFluScore, correlAccScore;
	calcCorrel(data, wovNum, correlTotalScore, correlFluScore, correlAccScore);
	
	printf("\n%g\t%g\t%g\n", correlTotalScore, correlFluScore, correlAccScore);
	return 0;
}
