#include "TSP.h"
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
	GraphM G;						//创建TSP问题中的地图
	InitGraphM(G);
	FileCreatGraphM(G);		
			

	clock_t start, end;
	double totaltime;
	start = clock();


	int N = G.vexnum;
	int *prim, **tree,*Hcircuit;
	
	Prim(G, prim);					//计算 G 的最小生成树

	GetTreeMat(G, prim, tree);		//将求得的最小生成树结果转为矩阵存储
	
	int sum = 0;
	int min, min1, min2;
	int *distance;
	distance = new int[N + 1];

	for (int i = 1;i <= N;i++)		//记录从每一个顶点出发所得的回路长度
	{
		GetHCircuit_1(tree, G.vexnum, i, Hcircuit);
		distance[i] = CountHCicuit(G, Hcircuit);
	}
	min1 = Min(distance + 1, N);		

	for (int i = 1;i <= N;i++)		//另一种回路计算方法
	{
		GetHCircuit_2(tree, G.vexnum, i, Hcircuit);
		distance[i] = CountHCicuit(G, Hcircuit);
	}
	min2 = Min(distance + 1, N);		

	min = min1 < min2 ? min1 : min2;

	cout << min << endl;


	end = clock();
	totaltime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "程序运行时间为：" << totaltime << 's' << endl;

	delete[] prim;					//释放空间
	for (int i = 1;i <= N;i++)
		delete[] tree[i];
	delete[] tree;

	system("pause");

	return 0;
}
