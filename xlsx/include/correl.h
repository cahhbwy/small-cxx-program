
typedef struct {
	char* wavName;		//������
	float totalScore_H;			//�˹����ܷ� 
	float totalScore_M;			//�����ܷ�
	float fluScore_H;			//�˹��������ȷ� 
	float fluScore_M;			//�����������ȷ� 
	float accScore_H;			//�˹��ֲ���׼ȷ�ȷ�
	float accScore_M;			//�����ֲ���׼ȷ�ȷ�
}SCORE,*LPSCORE;

//�����ɸ�����Ҫ������ע�����ɶ��Ժͼ�����.
namespace correl{
	void fileParse(const char* path, LPSCORE data, int wovNum);
	void calcCorrel(LPSCORE data, const int wovNum, double &correlTotalScore, double &correlFluScore, double &correlAccScore);
}
