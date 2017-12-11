#include "TSP.h"
#include <iostream>
#include <fstream>
using namespace std;

//----------------基本元素空间分配---------------

//-----顶点集向量-----

Status VexSetInit(VertexSet  &vexset)			//初始化顶点向量组
{
	vexset.elem = new VexElemType[VEX_INIT_NUM + 1];	//vertex[0]不使用
	if (!vexset.elem)
		return ERROR;

	vexset.maxsize = VEX_INIT_NUM;

	return OK;
}

Status VexSetDestroy(VertexSet &vexset)		//销毁顶点集向量组
{
	delete[] vexset.elem;
	vexset.elem = NULL;

	vexset.maxsize = -1;

	return OK;
}

Status VexSetExtend(VertexSet & vexset, int NEWSIZE)	//将顶点集(无原始数据)空间扩大至NEWSIZE
{
	VexElemType *newbase;
	newbase = new VexElemType[NEWSIZE + 1];		//申请新空间
	if (!newbase)
		return ERROR;

	delete[] vexset.elem;						//释放旧空间

	vexset.elem = newbase;						//指向新空间

	vexset.maxsize = NEWSIZE;					//修改顶点集大小	

	return OK;
}

Status SortVexSet(VexElemType * a, int n)	//升序排列
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

//------邻接矩阵------

Status AdjMatInit(AdjMat &mat)				//初始化图的邻接矩阵
{
	mat.elem = new AdjMatElem*[MAT_INIT_SIZE + 1];	//数组 0 空间不使用 
	if (!mat.elem)
		return ERROR;
	for (int i = 0; i <= MAT_INIT_SIZE; i++)
	{
		mat.elem[i] = new AdjMatElem[MAT_INIT_SIZE + 1];
		if (!mat.elem[i])
			return ERROR;
	}
	//得到了大小为 (MAT_INIT_SIZE + 1) × (MAT_INIT_SIZE + 1) 的二维矩阵

	mat.maxsize = MAT_INIT_SIZE;

	for (int i = 0; i <= MAT_INIT_SIZE; i++)		//为矩阵赋初值
		for (int j = 0; j <= MAT_INIT_SIZE; j++)
			mat.elem[i][j] = 0;

	return OK;
}

Status AdjMatDestroy(AdjMat &mat)			//销毁图的邻接矩阵
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

	newbase = new AdjMatElem*[NEWSIZE + 1];			//申请新空间
	if (!newbase)
		return ERROR;
	for (int i = 0; i < NEWSIZE + 1; i++)
	{
		newbase[i] = new AdjMatElem[NEWSIZE + 1];
		if (!newbase[i])
			exit(OVERFLOW);
	}
	for (int i = 0; i <= NEWSIZE; i++)				//为矩阵赋初值
		for (int j = 0; j <= NEWSIZE; j++)
			newbase[i][j] = 0;

	for (int i = 0; i <= mat.maxsize; i++)			//释放旧空间
	{
		delete[] mat.elem[i];
	}
	delete[] mat.elem;

	mat.elem = newbase;								//指向新空间

	mat.maxsize = NEWSIZE;							//修改顶点集大小

	return OK;
}


//-----------------------GraphM 基本操作----------------------

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

Status FileCreatGraphM(GraphM &G)		//文件读取方式创建，针对整理后的 gr17 文档的结构读取
{
	char filename[20];
	ifstream in;
	
	cout << "请输入要读取的文件名\n";
	cin >> filename;

	in.open(filename);
	if (!in)
	{
		cout << "文件打开失败！";
		in.close();
		system("pause");
		return ERROR;
	}
	int N;
	in >> N;
	if (G.vexset.maxsize < N)		//如果空间不足，则扩展空间
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
		if (elem[v][j] > 0)		//表示邻接，不是 0 或 INFINITY_MY
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
		if (elem[v][j] > 0)		//表示邻接，不是 0 或 INFINITY_MY
			return j;
	return 0;
}

int PriorAdjVex(AdjMatElem** elem, int n, int v, int w)
{
	for (int j = w - 1; j >= 1; j--)
		if (elem[v][j] > 0)			//表示邻接，不是 0 或 INFINITY_MY
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
			p = i;		//记下最小元素位置
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


//-------------遍历用栈定义-------------

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

//----------遍历用栈定义结束-----------


//----------------------关键函数------------------

Status Prim(GraphM G, int* &prim)	//返回 Prim 算法执行得到的结果矩阵
{
	int N = G.vexnum;				// 初始化执行过程所需要用到的辅助向量
	VexElemType *adjvex;
	int *lowcost;
	bool *finished;

	adjvex = new VexElemType[N + 1];
	lowcost = new int[N + 1];
	finished = new bool[N + 1];				//最初以 v1 开始

	for (int i = 0;i <= N;i++)				//初始化 finished
		finished[i] = false;				
	Mat mat = G.mat.elem;
	finished[1] = true;
	for (int i = 1;i <= N;i++)
		lowcost[i] = G.mat.elem[1][i];		//初始化 lowcost
	for (int i = 1; i <= N; i++)
		adjvex[i] = 1;						//初始化 adjvex

	//执行 N-1 次可以得到结果
	int p;
	for (int i = 1;i <= N - 1;i++)
	{
		p = FindMinCost(lowcost, finished, N);						//找到最小值对应的顶点
		finished[p] = true;											//赋值为真
		ChangeLowCost(p, mat[p], adjvex, lowcost, finished, N);		//修改最低花费,同时标明路径来源
	}

	//使用一维数组返回结果
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

	treemat = new int*[N + 1];				//分配空间
	if (!treemat)
		return ERROR;
	for (int i = 1;i <= N;i++)
	{
		treemat[i] = new int[N + 1];
		if (!treemat[i])
			return ERROR;
	}

	for (int i = 1;i <= N;i++)				//初始化生成树矩阵
		for (int j = 1;j <= N;j++)
			treemat[i][j] = INFINITY_MY;
	for (int i = 1;i <= N;i++)
		for (int j = 1;j <= N;j++)
			treemat[i][j] = 0;

	AdjMatElem** sourcemat = G.mat.elem;
	int p = 0;
	for (int i = 1; i <= N; i++)			//生成生成树矩阵
	{
		p = prim[i];
		treemat[i][p] = sourcemat[i][p];
		treemat[p][i] = treemat[i][p];
	}

	return OK;
}

Status GetHCircuit_1(int** tree,int N, int p, int* &Hcircuit)	//利用最小生成树，类似深度遍历的过程得到一条哈密顿回路 (从指定点开始)
{
	Hcircuit = new int[N + 2];	//初始化回路存储向量
	if (!Hcircuit)
		return ERROR;

	bool *visited;				//初始化标志
	visited = new bool[N + 1];
	for (int i = 1;i <= N;i++)
		visited[i] = false;

	Stack_t S;					//使用栈进行深度优先遍历，得到欧拉回路顺序所经过的点序列
	InitStack_t(S);

	visited[p] = true;
	Push_t(S, p);

	int i = 1;
	Hcircuit[N + 1] = p;		//回路最后一个点与初始相同
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

Status GetHCircuit_2(int** tree, int N, int p, int* &Hcircuit)	//另一条哈密顿回路
{
	Hcircuit = new int[N + 2];	//初始化回路存储向量
	if (!Hcircuit)
		return ERROR;

	bool *visited;				//初始化标志
	visited = new bool[N + 1];
	for (int i = 1;i <= N;i++)
		visited[i] = false;

	Stack_t S;					//使用栈进行深度优先遍历，得到欧拉回路顺序所经过的点序列
	InitStack_t(S);

	visited[p] = true;
	Push_t(S, p);

	int i = 1;
	Hcircuit[N + 1] = p;		//回路最后一个点与初始相同
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

//------------辅助检测函数--------------

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
