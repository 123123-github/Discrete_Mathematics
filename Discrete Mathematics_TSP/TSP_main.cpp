#include "TSP.h"
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
	GraphM G;						//����TSP�����еĵ�ͼ
	InitGraphM(G);
	FileCreatGraphM(G);		
			

	clock_t start, end;
	double totaltime;
	start = clock();


	int N = G.vexnum;
	int *prim, **tree,*Hcircuit;
	
	Prim(G, prim);					//���� G ����С������

	GetTreeMat(G, prim, tree);		//����õ���С���������תΪ����洢
	
	int sum = 0;
	int min, min1, min2;
	int *distance;
	distance = new int[N + 1];

	for (int i = 1;i <= N;i++)		//��¼��ÿһ������������õĻ�·����
	{
		GetHCircuit_1(tree, G.vexnum, i, Hcircuit);
		distance[i] = CountHCicuit(G, Hcircuit);
	}
	min1 = Min(distance + 1, N);		

	for (int i = 1;i <= N;i++)		//��һ�ֻ�·���㷽��
	{
		GetHCircuit_2(tree, G.vexnum, i, Hcircuit);
		distance[i] = CountHCicuit(G, Hcircuit);
	}
	min2 = Min(distance + 1, N);		

	min = min1 < min2 ? min1 : min2;

	cout << min << endl;


	end = clock();
	totaltime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "��������ʱ��Ϊ��" << totaltime << 's' << endl;

	delete[] prim;					//�ͷſռ�
	for (int i = 1;i <= N;i++)
		delete[] tree[i];
	delete[] tree;

	system("pause");

	return 0;
}
