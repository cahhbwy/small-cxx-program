#include "correl.h"
#include "include.h"

using namespace libxl;

namespace correl{
	bool isHundred(double x) {
		if (x - floor(x) < 0.000001 && x <= 100.0 && x >= 0.0) {
			return true;
		} else {
			return false;
		}
	}
	bool isFive(double x) {
		if (x <= 5.0 && x >= 0.0) {
			return true;
		} else {
			return false;
		}
	}
	void fileParse(const char* path, LPSCORE data, int wovNum){
		Book *bookxml = xlCreateXMLBook();
		bool xmload = bookxml->load(path);
		Sheet *sheetxml = bookxml->getSheet(0);
		Format* format_str = sheetxml->cellFormat(1, 0);
		Format* format_num = sheetxml->cellFormat(1, 1);
		for (int i = 1; i <= wovNum; ++i) {
			data[i - 1].wavName = (char*)calloc(15, sizeof(char));
			strcpy(data[i - 1].wavName, sheetxml->readStr(i, 0, &format_str));
			data[i - 1].totalScore_H = sheetxml->readNum(i, 1, &format_num);
			data[i - 1].totalScore_M = sheetxml->readNum(i, 2, &format_num);
			data[i - 1].fluScore_H = sheetxml->readNum(i, 3, &format_num);
			data[i - 1].fluScore_M = sheetxml->readNum(i, 4, &format_num);
			data[i - 1].accScore_H = sheetxml->readNum(i, 5, &format_num);
			data[i - 1].accScore_M = sheetxml->readNum(i, 6, &format_num);
		}
		return;
	}
	void calcCorrel(LPSCORE data, const int wovNum, double &correlTotalScore, double &correlFluScore, double &correlAccScore) {
		double mean_x = 0.0, mean_y = 0.0, mean_x2 = 0.0, mean_y2 = 0.0, mean_xy = 0.0;
		for (int i = 0; i < wovNum; ++i) {
			mean_x += data[i].totalScore_H;
			mean_y += data[i].totalScore_M;
			mean_x2 += data[i].totalScore_H * data[i].totalScore_H;
			mean_y2 += data[i].totalScore_M * data[i].totalScore_M;
			mean_xy += data[i].totalScore_H * data[i].totalScore_M;
		}
		mean_x /= wovNum;
		mean_y  /= wovNum;
		mean_x2 /= wovNum;
		mean_y2 /= wovNum;
		mean_xy /= wovNum;
		correlTotalScore = (mean_xy - mean_x * mean_y) / sqrt((mean_x2 - mean_x * mean_x) * (mean_y2 - mean_y * mean_y));
		
		mean_x = 0.0, mean_y = 0.0, mean_x2 = 0.0, mean_y2 = 0.0, mean_xy = 0.0;
		for (int i = 0; i < wovNum; ++i) {
			mean_x += data[i].fluScore_H;
			mean_y += data[i].fluScore_M;
			mean_x2 += data[i].fluScore_H * data[i].fluScore_H;
			mean_y2 += data[i].fluScore_M * data[i].fluScore_M;
			mean_xy += data[i].fluScore_H * data[i].fluScore_M;
		}
		mean_x /= wovNum;
		mean_y  /= wovNum;
		mean_x2 /= wovNum;
		mean_y2 /= wovNum;
		mean_xy /= wovNum;
		correlFluScore = (mean_xy - mean_x * mean_y) / sqrt((mean_x2 - mean_x * mean_x) * (mean_y2 - mean_y * mean_y));
		
		mean_x = 0.0, mean_y = 0.0, mean_x2 = 0.0, mean_y2 = 0.0, mean_xy = 0.0;
		for (int i = 0; i < wovNum; ++i) {
			mean_x += data[i].accScore_H;
			mean_y += data[i].accScore_M;
			mean_x2 += data[i].accScore_H * data[i].accScore_H;
			mean_y2 += data[i].accScore_M * data[i].accScore_M;
			mean_xy += data[i].accScore_H * data[i].accScore_M;
		}
		mean_x /= wovNum;
		mean_y  /= wovNum;
		mean_x2 /= wovNum;
		mean_y2 /= wovNum;
		mean_xy /= wovNum;
		correlAccScore = (mean_xy - mean_x * mean_y) / sqrt((mean_x2 - mean_x * mean_x) * (mean_y2 - mean_y * mean_y));
		return;
	}
}
