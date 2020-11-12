int main()
{
	//打开一个已经存在的文件
	int fd = open("english.txt",O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		exit(1);
	}
	//创建一个新文件
	int fd1 = open("new file",O_WRONLY | O_CREAT, o777);
	if(fd1==-1)
	{
		perror("open new");
		exit(1);
	}
	//读操作
	char buf[2048]={0};
	int count=read(fd,buf,sizeof(buf));
	if(count==-1)
	{
		perror("read");
		exit(1);
	}
	while(count)
	{
		//写操作
		int ret = write(fd1, buf, count);
		printf("write bytes are %d",ret);
		count = read(fd,buf,sizeof(buf));
	}
	//关闭文件
	close(fd);
	close(fd1);
                            