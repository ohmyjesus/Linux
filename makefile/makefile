#obj=main.o add.o sub.o mul.o
target=app

#makefile中自己维护的变量
CC=gcc
#自动变量
# $<  $@  $^

src=$(wildcard ./*.c)
obj=$(patsubst ./%.c, ./%.o, $(src))

$(target):$(obj) 
	$(CC) $(obj) -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	-mkdir  /aa
	rm $(obj) $(target)
hello:
	echo "hello , makefile"
