OBJS	= sort.o main.o util.o
OUT		= sort
CC		= gcc
FLAGS	= -c -Wall
LIB 	= ../BF/linux/BF_64.a

# test HT
t1: $(OBJS)
	$(CC) $(OBJS) $(LIB) -o $(OUT)
sort.o: sort.c
	$(CC) $(FLAGS) -std=c99 sort.c
main.o: main.c
	$(CC) $(FLAGS) main.c
util.o: util.c
	$(CC) $(FLAGS) -std=c99 util.c

# clean up
clean:
	rm -f $(OBJS) $(OUT)
