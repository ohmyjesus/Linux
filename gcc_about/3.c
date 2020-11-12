#include<stdio.h>
#include"head.h"
#define DEBUG
int main()
{
   int a,b,sum;
   a=NUM1;
   b=NUM2;
   sum=NUM1+NUM2;
#ifdef DEBUG
   printf("The sum value is: %d+%d=%d\n",a,b,sum);
#endif
   return 0;
}
