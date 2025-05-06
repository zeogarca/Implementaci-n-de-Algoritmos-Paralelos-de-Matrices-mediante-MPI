CC = mpicc
CFLAGS = -g -O3 -I./include
LDFLAGS = -lm
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/matmul_mpi

.PHONY: all clean

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    @mkdir -p $(OBJ_DIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
    @mkdir -p $(BIN_DIR)
    $(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)
