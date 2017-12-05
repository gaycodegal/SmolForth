INC=./headers
SRC=./source
OBJS = $(patsubst %.c,%.o,$(wildcard $(SRC)/*.c))
CC = gcc
CFLAGS = -I$(INC)
OBJ_NAME = main
all: $(OBJS)
	$(CC) $(OBJS) $(CPPFLAGS) -o $(OBJ_NAME)
clean:
	rm -f $(OBJS) $(wildcard *~) $(wildcard $(SRC)/*~) $(wildcard $(INC)/*~)
fclean:
	rm -f $(OBJS) $(OBJ_NAME) $(wildcard *~)
re:
	make clean all
