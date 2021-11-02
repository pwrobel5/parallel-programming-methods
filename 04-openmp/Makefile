CC := gcc
CFLAGS := -g -Wall
LIBS := -fopenmp

OBJ_DIR = obj
SRC_DIR = src
OBJ = $(addprefix $(OBJ_DIR)/, bucket_sort.o bucket.o)
INC = -I./include/

all: $(OBJ)
	$(CC) $(CFLAGS) $? -o program.x $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $< $(LIBS)

clean:
	rm -rf $(OBJ)
	rm -rf *.x
