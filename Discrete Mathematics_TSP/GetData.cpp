#include <stdio.h>
#include <stdlib.h>

int main()
{
	char sourcefile[20];
	printf("请输入读取文件名\n");
	scanf_s("%s", sourcefile,20);

	FILE *fp;
	fopen_s(&fp, sourcefile, "r");
	if (!fp)
	{
		printf("ERROR");
		fclose(fp);
	}
	int a[50000];
	int count = 0;

	fscanf_s(fp, "%d", a + count);
	count++;
	while (!feof(fp))
	{
		fscanf_s(fp, "%d", a + count);
		count++;
	}
	printf("%d\n", count);

	fclose(fp);



	char newfile[20];
	printf("请输入输出文件名\n");
	scanf_s("%s", newfile, 20);

	FILE *out;
	fopen_s(&out, newfile, "w");
	if (!out)
	{
		printf("ERROR");
		fclose(out);
	}
	fprintf(out, "%5d\n", a[0]);
	for (int i = 1;i < count-1;i++)
	{
		fprintf(out,"%5d", a[i]);
		if (a[i]==0)
			fprintf(fp,"\n");
	}
	fclose(out);

	system("pause");

	return 0;
}