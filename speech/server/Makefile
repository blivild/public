#target_dest:target_src
.PHONY:clean

Q=@
CC?=gcc
OBJ?=server
OBJS= server.o project.o threadpool.o
LDFLAGS=-lpthread
CFLAGS=-c -o


$(OBJ):$(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
%.o:%.c
	$(CC) $^ $(CFLAGS) $@

clean:
	@for var in $(OBJS) $(OBJ) ;do\
		if [ -f $$var ] ;then\
		rm $$var;\
		fi;\
		done
