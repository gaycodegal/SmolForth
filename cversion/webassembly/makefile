INC=../headers
SRC=../source
OUT=../../docs
SRC_FILES = $(wildcard $(SRC)/*.c)
CC = emcc
#CFLAGS = -s WASM=1 -I$(INC)
CFLAGS = -I$(INC) 
OBJ_NAME = hello.html
all:
	rm -rf $(OUT)
	mkdir $(OUT)
	$(CC) $(SRC_FILES) $(CFLAGS) -o $(OUT)/$(OBJ_NAME)
#run:
#	emrun --no_browser --port 8080 .
clean:
	rm -rf $(OUT)
re:
	make clean all
