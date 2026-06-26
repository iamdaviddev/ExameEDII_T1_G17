CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
TARGET  = sigtag
SRCS    = main.c bst.c avl.c ordenacao.c grafo.c menu.c utils.c sistema.c ficheiros.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
