#include<cstdio>
#include<cstdlib>
#include<string>
#define LONGEST 1000000
typedef struct{
	int **e;	//边长
	char *v;	//顶名
	int size;	//图大小
}Graph;//图
typedef struct{
	int start, end;		//起点，终点
	int *pathvex;		//路径
	int length;			//路程
	int vexnum;			//顶点数
	bool state;			//顶点归属
}Distance;
void init(Graph &G, int size, FILE *fp){
	G.size = size;
	G.v = (char*)malloc(size*sizeof(char));
	G.e = (int**)malloc(size*sizeof(int*));
	for (int i = 0; i<size; i++){
		*(G.e + i) = (int*)malloc(size*sizeof(int));
		for (int j = 0; j<size; j++)
			*((*(G.e + i)) + j) = 0;
	}
	for (int i = 0; i<size; i++){
		fscanf_s(fp,"%c", G.v + i);
		fgetc(fp);
	}
	for (int i = 0; i<size; i++)
		for (int j = 0; j<size; j++)
			fscanf_s(fp,"%d", (*(G.e + i)) + j);
}
void print(Graph G){
	printf("\t");
	for (int i = 0; i<G.size; i++)
		printf("%8c", G.v[i]);
	printf("\n");
	for (int i = 0; i<G.size; i++){
		printf("%8c", G.v[i]);
		for (int j = 0; j<G.size; j++)
			printf("%8d", *((*(G.e + i)) + j));
		printf("\n");
	}
}
void Dijkstra(Graph G, int u, int v){
	u--; v--; int size = G.size;
	Distance *dis = (Distance*)malloc(sizeof(Distance)*size);
	for (int i = 0; i < size; i++){		//初始化
		dis[i].start = u;
		dis[i].end = i;
		dis[i].length = LONGEST;
		dis[i].pathvex = (int*)malloc(sizeof(int)*size);
		dis[i].pathvex[0] = u;
		dis[i].state = false;
		dis[i].vexnum = 1;
	}
	dis[u].end = u;	dis[u].length = 0;	dis[u].state = true;
	int minvex, minlength, sum = 1;
	while (sum < size-1){
		minlength = LONGEST;
		for (int i = 0; i < size; i++){
			if (!dis[i].state){
				for (int j = 0; j < size; j++){
					if (dis[j].state && G.e[i][j] > 0 && dis[j].length + G.e[i][j] < dis[i].length){
						dis[i].length = dis[j].length + G.e[i][j];
						for (int k = 0; k < dis[j].vexnum; k++){
							dis[i].pathvex[k] = dis[j].pathvex[k];
						}
						dis[i].pathvex[dis[j].vexnum] = i;
						dis[i].vexnum = dis[j].vexnum + 1;
					}
				}
				if (!dis[i].state&&minlength > dis[i].length){
					minlength = dis[i].length;
					minvex = i;
				}
			}
		}
		dis[minvex].state = true;
		sum++;
	}
	printf("从%c到%c的路径:", G.v[u], G.v[v]);
	for (int i = 0; i < dis[v].vexnum-1; i++){
		printf("%c->", G.v[dis[v].pathvex[i]]);
	}
	printf("%c    总长%d\n", G.v[v], dis[v].length);
}

int main(){
	Graph G;
	FILE *fp;
	fopen_s(&fp, "F:\\Cprogram\\temp\\ShortestPath.txt", "r");
	init(G, 6, fp);
	print(G);
	for (int i = 1; i <= G.size ; i++){
		for (int j = 1; j <= G.size; j++){
			Dijkstra(G, i, j);
		}
	}
	system("pause");
	return 0;
}