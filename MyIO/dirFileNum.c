#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int getFileNum(char *root)
{
	//open dir
	DIR *dir = NULL;
	dir = opendir(root);  //DIR结构体  打开目录失败时返回一个空指针
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}

	//遍历当前打开的目录
	struct dirent *ptr = NULL;  //dirent结构体  结构体成员有 d_name , d_type 等
	char path[1024] = {0};
	int total = 0;
	while((ptr = readdir(dir)) != NULL)
	{
		//过滤..和.
		if(strcmp(ptr->d_name,".")==0|| strcmp(ptr->d_name,"..") == 0)
		{
			continue;
		}
		//如果是目录
		if(ptr->d_type == DT_DIR)
		{
			//递归 读目录
			sprintf(path, "%s/%s", root , ptr->d_name);
			total += getFileNum(path);
		}
		//如果是普通文件
		if(ptr->d_type == DT_REG)
		{
			total++;
		}
	}
	closedir(dir);

	return total;
}

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("wrong");
		exit(1);
	}
	int total = getFileNum(argv[1]);
	printf("the %s has REG file number are %d",argv[1] ,total);

	return 0;
}
