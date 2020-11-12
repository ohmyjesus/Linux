 int main()
{
	int fd = open("tempfile",O_RDWR | O_CREAT ,0777);
	if(fd==-1)
	{
		perror("open");
		exit(1);
	}
	int ret = unlink("tempfile");

	//写操作
	write(fd, "Hello\n", 6);

	//重置文件指针
	lseek(fd, 0, SEEK_SET);

	//读操作
	char buf[24]={0};
	int len = read(fd, buf, sizeof(buf));
	

	//输出到屏幕上
	write(1, buf,len);
	//关闭文件
	close(fd);
	return 0;
}
