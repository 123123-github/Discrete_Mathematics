#include "TSP.h"
#include <iostream>
#include <fstream>
using namespace std;

//----------------����Ԫ�ؿռ����---------------

//-----���㼯����-----

Status VexSetInit(VertexSet  &vexset)			//��ʼ������������
{
	vexset.elem = new VexElemType[VEX_INIT_NUM + 1];	//vertex[0]��ʹ��
	if (!vexset.elem)
		return ERROR;

	vexset.maxsize = VEX_INIT_NUM;

	return OK;
}

Status VexSetDestroy(VertexSet &vexset)		//���ٶ��㼯������
{
	delete[] vexset.elem;
	vexset.elem = NULL;

	vexset.maxsize = -1;

	return OK;
}

Status VexSetExtend(VertexSet & vexset, int NEWSIZE)	//�����㼯(��ԭʼ����)�ռ�������NEWSIZE
{
	VexElemType *newbase;
	newbase = new VexElemType[NEWSIZE + 1];		//�����¿ռ�
	if (!newbase)
		return ERROR;

	delete[] vexset.elem;						//�ͷžɿռ�

	vexset.elem = newbase;						//ָ���¿ռ�

	vexset.maxsize = NEWSIZE;					//�޸Ķ��㼯��С	

	return OK;
}

Status SortVexSet(VexElemType * a, int n)	//��������
{
	int p;
	VexElemType t;
	for (int i = 1; i <= n - 1; i++)
	{
		p = i;
		for (int j = i + 1; j <= n; j++)
		{
			if (a[p] > a[j])
				p = j;
		}
		if (p != i)
		{
			t = a[p];
			a[p] = a[i];
			a[i] = t;
		}
	}
	return OK;
}

//------�ڽӾ���------

Status AdjMatInit(AdjMat &mat)				//��ʼ��ͼ���ڽӾ���
{
	mat.elem = new AdjMatElem*[MAT_INIT_SIZE + 1];	//���� 0 �ռ䲻ʹ�� 
	if (!mat.elem)
		return ERROR;
	for (int i = 0; i <= MAT_INIT_SIZE; i++)
	{
		mat.elem[i] = new AdjMatElem[MAT_INIT_SIZE + 1];
		if (!mat.elem[i])
			return ERROR;
	}
	//�õ��˴�СΪ (MAT_INIT_SIZE + 1) �� (MAT_INIT_SIZE + 1) �Ķ�ά����

	mat.maxsize = MAT_INIT_SIZE;

	for (int i = 0; i <= MAT_INIT_SIZE; i++)		//Ϊ���󸳳�ֵ
		for (int j = 0; j <= MAT_INIT_SIZE; j++)
			mat.elem[i][j] = 0;

	return OK;
}

Status AdjMatDestroy(AdjMat &mat)			//����ͼ���ڽӾ���
{
	for (int i = 0; i <= mat.maxsize; i++)
	{
		delete[] mat.elem[i];
	}
	delete[] mat.elem;
	mat.elem = NULL;

	mat.maxsize = -1;

	return OK;
}

Status AdjMatExtend(AdjMat & mat, int NEWSIZE)
{
	AdjMatElem **newbase;

	newbase = new AdjMatElem*[NEWSIZE + 1];			//�����¿ռ�
	if (!newbase)
		return ERROR;
	for (int i = 0; i < NEWSIZE + 1; i++)
	{
		newbase[i] = new AdjMatElem[NEWSIZE + 1];
		if (!newbase[i])
			exit(OVERFLOW);
	}
	for (int i = 0; i <= NEWSIZE; i++)				//Ϊ���󸳳�ֵ
		for (int j = 0; j <= NEWSIZE; j++)
			newbase[i][j] = 0;

	for (int i = 0; i <= mat.maxsize; i++)			//�ͷžɿռ�
	{
		delete[] mat.elem[i];
	}
	delete[] mat.elem;

	mat.elem = newbase;								//ָ���¿ռ�

	mat.maxsize = NEWSIZE;							//�޸Ķ��㼯��С

	return OK;
}


//-----------------------GraphM ��������----------------------

Status InitGraphM(GraphM & G)
{
	G.vexnum = 0;
	G.edgenum = 0;
	VexSetInit(G.vexset);
	AdjMatInit(G.mat);

	return OK;
}

Status DestroyGraphM(GraphM & G)
{
	G.vexnum = -1;
	G.edgenum = -1;

	VexSetDestroy(G.vexset);
	AdjMatDestroy(G.mat);

	return OK;
}

Status FileCreatGraphM(GraphM &G)		//�ļ���ȡ��ʽ��������������� gr17 �ĵ��Ľṹ��ȡ
{
	char filename[20];
	ifstream in;
	
	cout << "������Ҫ��ȡ���ļ���\n";
	cin >> filename;

	in.open(filename);
	if (!in)
	{
		cout << "�ļ���ʧ�ܣ�";
		in.close();
		system("pause");
		return ERROR;
	}
	int N;
	in >> N;
	if (G.vexset.maxsize < N)		//����ռ䲻�㣬����չ�ռ�
	{
		AdjMatExtend(G.mat, N);
		VexSetExtend(G.vexset, N);
	}
	
	G.vexnum = N;
	G.edgenum = N*N;
	for (int i = 1;i <= N;i++)
	{
		G.vexset.elem[i] = i;
	}
	for (int i = 1;i <= N;i++)
	{
		for (int j = 1;j <= i;j++)
		{
			in >> G.mat.elem[i][j];
			G.mat.elem[j][i] = G.mat.elem[i][j];
		}
	}

	in.close();

	return OK;
}

int LocateVex(GraphM G, VexElemType v)
{
	VexElemType *elem = G.vexset.elem;
	for (int i = 1; i <= G.vexnum; i++)
	{
		if (*(elem + i) == v)
			return i;
	}

	return 0;
}

int FirstAdjVex(AdjMatElem** elem, int n, int v)
{
	for (int j = 1; j <= n; j++)
		if (elem[v][j] > 0)		//��ʾ�ڽӣ����� 0 �� INFINITY_MY
			return j;

	return 0;
}

int NextAdjVex(AdjMatElem** elem, int n, int v, int w)
{
	for (int j = w + 1; j <= n; j++)
		if (elem[v][j] > 0)
			return j;

	return 0;
}

int LastAdjVex(AdjMatElem** elem, int n, int v)
{
	for (int j = n; j >= 1; j--)
		if (elem[v][j] > 0)		//��ʾ�ڽӣ����� 0 �� INFINITY_MY
			return j;
	return 0;
}

int PriorAdjVex(AdjMatElem** elem, int n, int v, int w)
{
	for (int j = w - 1; j >= 1; j--)
		if (elem[v][j] > 0)			//��ʾ�ڽӣ����� 0 �� INFINITY_MY
			return j;

	return 0;
}

int FindMinCost(int *lowcost, bool *finished, int N)
{
	int min = INT_MAX;
	int p = 0;
	
	for (int i = 1;i <= N;i++)
	{
		if (!finished[i] && lowcost[i] < min)
		{
			min = lowcost[i];
			p = i;		//������СԪ��λ��
		}
	}

	return p;
}

void ChangeLowCost(int p, int *newcost, int* &adjvex, int* &lowcost, bool *finished, int N)
{
	for (int i = 1; i <= N; i++)
	{
		if (!finished[i] && newcost[i]<lowcost[i])
		{
			lowcost[i] = newcost[i];
			adjvex[i] = p;
		}
	}

	return;
}


//-------------������ջ����-------------

Status InitStack_t(Stack_t &S)
{
	S.base = new stackelem[STACK_INIT_SIZE];
	if (!S.base)
		return ERROR;

	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;

	return OK;
}

Status DestroyStack_t(Stack_t &S)
{
	delete[] S.base;
	S.top = S.base = NULL;
	S.stacksize = -1;

	return OK;
}

bool StackEmpty_t(Stack_t S)
{
	if (S.top == S.base)
		return true;
	else
		return false;
}

Status GetTop(Stack_t S, stackelem &t)
{
	if (S.top == S.base)
		return FALSE;

	stackelem s;
	s = *(S.top - 1);
	t = s;

	return OK;
}

Status Pop_t(Stack_t &S, stackelem &t)
{
	if (S.base == S.top)
		return ERROR;

	stackelem s;
	s = *(S.top - 1);
	t = s;

	S.top--;

	return OK;
}

Status Push_t(Stack_t &S, stackelem t)
{
	if (S.top - S.base >= S.stacksize)
	{
		stackelem *newbase;
		newbase = new stackelem[S.stacksize + STACKINCREMENT];
		if (!newbase)
			exit(OVERFLOW);

		for (int i = 0;i < S.stacksize;i++)
			newbase[i] = S.base[i];

		S.base = newbase;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}

	stackelem s;
	s = t;
	*S.top = s;
	S.top++;

	return OK;
}

//----------������ջ�������-----------


//----------------------�ؼ�����------------------

Status Prim(GraphM G, int* &prim)	//���� Prim �㷨ִ�еõ��Ľ������
{
	int N = G.vexnum;				// ��ʼ��ִ�й�������Ҫ�õ��ĸ�������
	VexElemType *adjvex;
	int *lowcost;
	bool *finished;

	adjvex = new VexElemType[N + 1];
	lowcost = new int[N + 1];
	finished = new bool[N + 1];				//����� v1 ��ʼ

	for (int i = 0;i <= N;i++)				//��ʼ�� finished
		finished[i] = false;				
	Mat mat = G.mat.elem;
	finished[1] = true;
	for (int i = 1;i <= N;i++)
		lowcost[i] = G.mat.elem[1][i];		//��ʼ�� lowcost
	for (int i = 1; i <= N; i++)
		adjvex[i] = 1;						//��ʼ�� adjvex

	//ִ�� N-1 �ο��Եõ����
	int p;
	for (int i = 1;i <= N - 1;i++)
	{
		p = FindMinCost(lowcost, finished, N);						//�ҵ���Сֵ��Ӧ�Ķ���
		finished[p] = true;											//��ֵΪ��
		ChangeLowCost(p, mat[p], adjvex, lowcost, finished, N);		//�޸���ͻ���,ͬʱ����·����Դ
	}

	//ʹ��һά���鷵�ؽ��
	prim = new int[N + 1];
	for (int i = 1;i <= N;i++)
		prim[i] = adjvex[i];

	delete[] adjvex;
	delete[] lowcost;
	delete[] finished;

	return OK;
}

Status GetTreeMat(GraphM G, int * prim, int** &treemat)
{
	int N = G.vexnum;

	treemat = new int*[N + 1];				//����ռ�
	if (!treemat)
		return ERROR;
	for (int i = 1;i <= N;i++)
	{
		treemat[i] = new int[N + 1];
		if (!treemat[i])
			return ERROR;
	}

	for (int i = 1;i <= N;i++)				//��ʼ������������
		for (int j = 1;j <= N;j++)
			treemat[i][j] = INFINITY_MY;
	for (int i = 1;i <= N;i++)
		for (int j = 1;j <= N;j++)
			treemat[i][j] = 0;

	AdjMatElem** sourcemat = G.mat.elem;
	int p = 0;
	for (int i = 1; i <= N; i++)			//��������������
	{
		p = prim[i];
		treemat[i][p] = sourcemat[i][p];
		treemat[p][i] = treemat[i][p];
	}

	return OK;
}

Status GetHCircuit_1(int** tree,int N, int p, int* &Hcircuit)	//������С��������������ȱ����Ĺ��̵õ�һ�����ܶٻ�· (��ָ���㿪ʼ)
{
	Hcircuit = new int[N + 2];	//��ʼ����·�洢����
	if (!Hcircuit)
		return ERROR;

	bool *visited;				//��ʼ����־
	visited = new bool[N + 1];
	for (int i = 1;i <= N;i++)
		visited[i] = false;

	Stack_t S;					//ʹ��ջ����������ȱ������õ�ŷ����·˳���������ĵ�����
	InitStack_t(S);

	visited[p] = true;
	Push_t(S, p);

	int i = 1;
	Hcircuit[N + 1] = p;		//��·���һ�������ʼ��ͬ
	while (!StackEmpty_t(S))
	{
		Pop_t(S, p);
		Hcircuit[i++] = p;

		for (int q = LastAdjVex(tree, N, p); q > 0; q = PriorAdjVex(tree, N, p, q))
		{
			if (!visited[q])
			{
				visited[q] = true;
				Push_t(S, q);
			}
		}
	}

	DestroyStack_t(S);

	return OK;
}

Status GetHCircuit_2(int** tree, int N, int p, int* &Hcircuit)	//��һ�����ܶٻ�·
{
	Hcircuit = new int[N + 2];	//��ʼ����·�洢����
	if (!Hcircuit)
		return ERROR;

	bool *visited;				//��ʼ����־
	visited = new bool[N + 1];
	for (int i = 1;i <= N;i++)
		visited[i] = false;

	Stack_t S;					//ʹ��ջ����������ȱ������õ�ŷ����·˳���������ĵ�����
	InitStack_t(S);

	visited[p] = true;
	Push_t(S, p);

	int i = 1;
	Hcircuit[N + 1] = p;		//��·���һ�������ʼ��ͬ
	while (!StackEmpty_t(S))
	{
		Pop_t(S, p);
		Hcircuit[i++] = p;

		for (int q = FirstAdjVex(tree, N, p); q > 0; q = NextAdjVex(tree, N, p, q))
		{
			if (!visited[q])
			{
				visited[q] = true;
				Push_t(S, q);
			}
		}
	}

	DestroyStack_t(S);

	return OK;
}

int	CountHCicuit(GraphM G,int* &Hcircuit)
{
	int sum = 0;
	int N = G.vexnum;
	int p, q;
	AdjMatElem** mat = G.mat.elem;

	for (int i = 1; i <= N; i++)
	{
		p = Hcircuit[i];
		q = Hcircuit[i + 1];
		sum += mat[p][q];
	}

	delete[] Hcircuit;
	Hcircuit = NULL;
	
	return sum;
}

//------------������⺯��--------------

void PrintGraphMat(GraphM G)
{
	VexElemType *v = G.vexset.elem;
	for (int i = 1; i <= G.vexnum; i++)
	{
		cout << '\t' << v[i];
	}
	cout << endl;

	AdjMatElem **mat = G.mat.elem;
	for (int i = 1; i <= G.vexnum; i++)
	{
		cout << v[i] << '\t';
		for (int j = 1; j <= G.vexnum; j++)
		{
			cout << mat[i][j] << '\t';
		}
		cout << endl;
	}

	return;
}

int Min(int * a, int n)
{
	int min = a[0];
	for (int i = 1;i < n;i++)
	{
		if (a[i] < min)
			min = a[i];
	}

	return min;
}
