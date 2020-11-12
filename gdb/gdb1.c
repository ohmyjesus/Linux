#include<stdio.h>
#include"head.h"
int main()
{
	int i=0,c=0;
	int *p;
	int b[3]={5,9,1};
	num();
	p=b;
	for (i;i<3;i++)		
	{
		if(i<*(p+c))
		{ 
			printf("the number is %d\n",p[i]);
	        	c++;
		}
	}
	xyh();
	return 0;
}
