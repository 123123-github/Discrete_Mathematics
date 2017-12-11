#pragma once
// 头文件
// 存储结构定义及函数声明
// 旅行商问题	TSP
// 程序语言		C++

//-----------------------函数结果状态代码--------------------

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

typedef int Status;			//Status 为函数类型，值为函数结果状态代码


//----------------------- Graph 组成元素定义-----------------------

#define INFINITY_MY -1			//用在网中，表示两点不相邻

//--------顶点集(顺序表)------

typedef int VexElemType;		//顶点类型，定义为整型，为了简便以1 2 3……代表各个顶点
								//也可以定义为其它类型，存储TSP问题中的城市名称等

struct VertexSet				//顶点集 整型数组 存储名称
{
	VexElemType *elem;
	int maxsize;
};

//-------邻接矩阵(二维的顺序表)------

typedef int AdjMatElem;		//采用整型的距离

typedef int** Mat;			//非结构体的矩阵类型，用于计算过程

struct AdjMat				//邻接矩阵
{
	AdjMatElem **elem;
	int maxsize;			// AdjMat 为方阵
};


//----------------顶点集,邻接矩阵空间分配---------------

#define VEX_INIT_NUM 20
#define VEX_INCREMENT 10
#define MAT_INIT_SIZE 20
#define MAT_INCREMENT 10

Status VexSetInit(VertexSet &vexset);		//初始化顶点集

Status VexSetDestroy(VertexSet &vexset);	//销毁顶点集

Status VexSetExtend(VertexSet & vexset, int NEWSIZE);

Status AdjMatInit(AdjMat &mat);				//初始化图的邻接矩阵

Status AdjMatDestroy(AdjMat &mat);			//销毁图的邻接矩阵

Status AdjMatExtend(AdjMat & mat, int NEWSIZE);

Status SortVexSet(VexElemType *a, int n);	//对含有 n 个顶点的顶点集排序	//该函数用于非顺序输入时，本次实验中不采取手动输入名称的方式，所以用处不大

//-----------------------ADT GraphM 定义---------------------

struct GraphM			//即 TSP 问题中的地图存储结构
{
	int vexnum;
	int edgenum;
	VertexSet vexset;	//顶点集
	AdjMat mat;			//邻接矩阵
};

//-----------------------GraphM 基本操作----------------------

Status InitGraphM(GraphM &G);			//初始化为空图,分配空间后，便于手动输入数据等操作

Status DestroyGraphM(GraphM &G);		//销毁图

int LocateVex(GraphM G, VexElemType v);	//返回顶点 v 在顶点集中的位置(位序)

Status FileCreatGraphM(GraphM &G);			//创建图 TSP 问题中无向网的创建方式，且为文件读取

int FirstAdjVex(AdjMatElem** elem, int n, int v);			//输入返回均为顶点集位序,第一个邻接点

int NextAdjVex(AdjMatElem** elem, int n, int v, int w);		//输入返回均为顶点集位序，相对 w 的第一个邻接点

int LastAdjVex(AdjMatElem** elem, int n, int v);

int PriorAdjVex(AdjMatElem** elem, int n, int v, int w);




//--------------遍历用栈定义--------------

#define STACK_INIT_SIZE 100 //栈存储空间 初始分配量
#define STACKINCREMENT 10   //栈存储空间 分配增量

typedef int stackelem;

struct Stack_t
{
	stackelem* base;
	stackelem* top;
	int stacksize;
};

Status InitStack_t(Stack_t &S);

Status DestroyStack_t(Stack_t &S);

bool StackEmpty_t(Stack_t S);

Status GetTop(Stack_t S, stackelem &t);

Status Pop_t(Stack_t &S, stackelem &t);

Status Push_t(Stack_t &S, stackelem t);

//------------遍历用栈定义结束------------


//----------------------关键函数--------------------

int FindMinCost(int *lowcost, bool *finished, int N);			//返回最小元素位序

void ChangeLowCost(int p, int *newcost, int* &adjvex, int* &lowcost, bool *finished, int N);

Status Prim(GraphM G, int* &prim);								//求最小生成树,结果存储一维向量 prim 中

Status GetTreeMat(GraphM G, int *prim, int** &treemat);			//将最小生成树结果转存在矩阵 treemat 中

Status GetHCircuit_1(int** tree, int N, int p, int* &Hcircuit);	//得到哈密顿回路

Status GetHCircuit_2(int** tree, int N, int p, int* &Hcircuit);	//得到另一条哈密顿回路

int	CountHCicuit(GraphM G, int* &Hcircuit);						//计算回路长度


//------------辅助函数--------------

void PrintGraphMat(GraphM G);			//检验地图创建是否正确

int Min(int *a, int n);				//获得数列中的最小值