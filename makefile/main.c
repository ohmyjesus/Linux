#include<stdio.h>
#include"head.h"
int main()
{
	int a,b,c,d,e;
	a=1;b=2;
	c=add(a,b);
	d=sub(b,c);
	e=mul(c,d);
	printf("the final result is %d\n" , e);

	return 0;
}

