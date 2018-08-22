
typedef struct {
	char* wavName;		//考生号
	float totalScore_H;			//人工分总分 
	float totalScore_M;			//机器总分
	float fluScore_H;			//人工分流畅度分 
	float fluScore_M;			//机器分流畅度分 
	float accScore_H;			//人工分测评准确度分
	float accScore_M;			//机器分测评准确度分
}SCORE,*LPSCORE;

//参数可根据需要调整，注意代码可读性和简易性.
namespace correl{
	void fileParse(const char* path, LPSCORE data, int wovNum);
	void calcCorrel(LPSCORE data, const int wovNum, double &correlTotalScore, double &correlFluScore, double &correlAccScore);
}
