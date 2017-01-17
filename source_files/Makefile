OBJS	= sort.o main.o
OUT		= sort
CC		= gcc
FLAGS	= -c -Wall
LIB 	= ../BF/linux/BF_64.a

# test HT
t1: $(OBJS)
	$(CC) $(OBJS) $(LIB) -o $(OUT)
sort.o: sort.c
	$(CC) $(FLAGS) sort.c
main.o: main.c
	$(CC) $(FLAGS) main.c

# clean up
clean:
	rm -f $(OBJS) $(OUT)
