#pragma once
// ͷ�ļ�
// �洢�ṹ���弰��������
// ����������	TSP
// ��������		C++

//-----------------------�������״̬����--------------------

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

typedef int Status;			//Status Ϊ�������ͣ�ֵΪ�������״̬����


//----------------------- Graph ���Ԫ�ض���-----------------------

#define INFINITY_MY -1			//�������У���ʾ���㲻����

//--------���㼯(˳���)------

typedef int VexElemType;		//�������ͣ�����Ϊ���ͣ�Ϊ�˼����1 2 3���������������
								//Ҳ���Զ���Ϊ�������ͣ��洢TSP�����еĳ������Ƶ�

struct VertexSet				//���㼯 �������� �洢����
{
	VexElemType *elem;
	int maxsize;
};

//-------�ڽӾ���(��ά��˳���)------

typedef int AdjMatElem;		//�������͵ľ���

typedef int** Mat;			//�ǽṹ��ľ������ͣ����ڼ������

struct AdjMat				//�ڽӾ���
{
	AdjMatElem **elem;
	int maxsize;			// AdjMat Ϊ����
};


//----------------���㼯,�ڽӾ���ռ����---------------

#define VEX_INIT_NUM 20
#define VEX_INCREMENT 10
#define MAT_INIT_SIZE 20
#define MAT_INCREMENT 10

Status VexSetInit(VertexSet &vexset);		//��ʼ�����㼯

Status VexSetDestroy(VertexSet &vexset);	//���ٶ��㼯

Status VexSetExtend(VertexSet & vexset, int NEWSIZE);

Status AdjMatInit(AdjMat &mat);				//��ʼ��ͼ���ڽӾ���

Status AdjMatDestroy(AdjMat &mat);			//����ͼ���ڽӾ���

Status AdjMatExtend(AdjMat & mat, int NEWSIZE);

Status SortVexSet(VexElemType *a, int n);	//�Ժ��� n ������Ķ��㼯����	//�ú������ڷ�˳������ʱ������ʵ���в���ȡ�ֶ��������Ƶķ�ʽ�������ô�����

//-----------------------ADT GraphM ����---------------------

struct GraphM			//�� TSP �����еĵ�ͼ�洢�ṹ
{
	int vexnum;
	int edgenum;
	VertexSet vexset;	//���㼯
	AdjMat mat;			//�ڽӾ���
};

//-----------------------GraphM ��������----------------------

Status InitGraphM(GraphM &G);			//��ʼ��Ϊ��ͼ,����ռ�󣬱����ֶ��������ݵȲ���

Status DestroyGraphM(GraphM &G);		//����ͼ

int LocateVex(GraphM G, VexElemType v);	//���ض��� v �ڶ��㼯�е�λ��(λ��)

Status FileCreatGraphM(GraphM &G);			//����ͼ TSP �������������Ĵ�����ʽ����Ϊ�ļ���ȡ

int FirstAdjVex(AdjMatElem** elem, int n, int v);			//���뷵�ؾ�Ϊ���㼯λ��,��һ���ڽӵ�

int NextAdjVex(AdjMatElem** elem, int n, int v, int w);		//���뷵�ؾ�Ϊ���㼯λ����� w �ĵ�һ���ڽӵ�

int LastAdjVex(AdjMatElem** elem, int n, int v);

int PriorAdjVex(AdjMatElem** elem, int n, int v, int w);




//--------------������ջ����--------------

#define STACK_INIT_SIZE 100 //ջ�洢�ռ� ��ʼ������
#define STACKINCREMENT 10   //ջ�洢�ռ� ��������

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

//------------������ջ�������------------


//----------------------�ؼ�����--------------------

int FindMinCost(int *lowcost, bool *finished, int N);			//������СԪ��λ��

void ChangeLowCost(int p, int *newcost, int* &adjvex, int* &lowcost, bool *finished, int N);

Status Prim(GraphM G, int* &prim);								//����С������,����洢һά���� prim ��

Status GetTreeMat(GraphM G, int *prim, int** &treemat);			//����С���������ת���ھ��� treemat ��

Status GetHCircuit_1(int** tree, int N, int p, int* &Hcircuit);	//�õ����ܶٻ�·

Status GetHCircuit_2(int** tree, int N, int p, int* &Hcircuit);	//�õ���һ�����ܶٻ�·

int	CountHCicuit(GraphM G, int* &Hcircuit);						//�����·����


//------------��������--------------

void PrintGraphMat(GraphM G);			//�����ͼ�����Ƿ���ȷ

int Min(int *a, int n);				//��������е���Сֵ