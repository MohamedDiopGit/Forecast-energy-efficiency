CC = gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CFLAGS = -lm
EXEC = myApp

all: $(EXEC) 
	
$(EXEC): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $(EXEC)
	
%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	@echo "removing object files..."
	rm *.o 
	@echo "complete."

mrproper:
	@echo "removing object and app files..."
	rm *.o $(EXEC)
	@echo "complete."
