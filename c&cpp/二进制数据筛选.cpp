#include <stdio.h>
#include <stdlib.h> 

//----结果状态码定义----
#define OK 1
#define ERROR 0
typedef int Status; 

//------结构体定义------
typedef struct 
{
	long id;
	char name[20];
	float score;
} STUD; 

//-----函数声明------- 
Status Save(STUD *student,int N,int *list,int count);		//存储指定文件数据 
Status Load(STUD *student,int *Np); 	//读取指定文件数据 
void SortByScore(STUD *student,int N,int *list,int *pcount);	//选出成绩小于60的数据，并记录总个数 


//-------------------主函数-------------------- 
int main()
{
	STUD student[80];
	int N;

	//---------读取文件-----------
	Load(student,&N);
	
	//输出文件 
	printf("%d\n",N);
	for(int i=0;i<N;i++)
		printf("%d %s %15.2f\n",student[i].id,student[i].name,student[i].score);

	//---------处理文件------------
	int count=0;
	int list[80];
	SortByScore(student,N,list,&count);
	printf("符合条件的数据数为%d\n",count);
	
	//---------存储文件-----------
	Save(student,N,list,count); 
	int p;
	for(int i=0;i<count;i++)
	{
		p=list[i];
		printf("%d %s %15.2f\n",student[p].id,student[p].name,student[p].score);
	}
	return 0;
}

Status Save(STUD *student,int N,int *list,int count)		//生成二进制文件 
{
	char savefile[20];
	printf("请输入输出文件名\n");
	scanf("%s",savefile);

	FILE *save;
	save=fopen(savefile,"wb");
	if(!save)
	{
		printf("打开失败！\n");
		exit(0);
	}
	
	int status=0;
	for(int i=0;i<count;i++)
	{
		status = fwrite(student+list[i],sizeof(STUD),1,save);
		if(status != 1)
		{
			printf("写入失败！\n");
			fclose(save);
			return ERROR;
		}
	}
	
	fclose(save);
	
	return OK;
}

Status Load(STUD *student,int *Np)		//读取二进制文件 
{
	char loadfile[20];
	printf("请输入读取文件名\n");
	scanf("%s",loadfile);
	
	FILE *load;
	load=fopen(loadfile,"rb");
	if(!load)
	{
		printf("打开失败！\n");
		exit(0);
	}
	
	int i=0;
	int status=0;
	for(i=0;;i++) 
	{
		status = fread(&student[i],sizeof(STUD),1,load) ;
		if(feof(load))	
		{
			fclose(load);
			break;
		}
		if(status != 1)
		{
			printf("读取失败！ %d \n",i);
			fclose(load);
			return ERROR;
		}
	}
	*Np = i;
	
	fclose(load);
	
	return OK;
}

void SortByScore(STUD *student,int N,int *list,int *pcount)
{
	*pcount=0;
	
	for(int i=0;i<N;i++)
	{
		if(student[i].score<60)
		{	
			list[(*pcount)]=i;
			(*pcount)++;
		}
	}
	return;
}










