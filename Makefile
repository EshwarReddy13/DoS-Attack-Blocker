# Makefile for Load Balancer Project
# CSCE 412 Project 3

# Compiler and flags
CC = g++
CFLAGS = -Wall -Werror -Iinclude -Wno-unused-parameter -Wno-unused-variable -std=c++11

# Directories
SRC = src
INC = include
OBJ = obj

# Target executable
TARGET = loadbalancer.exe

# Object files (in obj/ directory)
OBJS = $(OBJ)/main.o $(OBJ)/loadbalancer.o $(OBJ)/webserver.o $(OBJ)/request.o

# Default target
all: $(TARGET)

# Main target - build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Object file rules (generate in obj/ directory)
$(OBJ)/main.o: main.cpp $(INC)/loadbalancer.h $(INC)/webserver.h $(INC)/request.h
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c main.cpp -o $@

$(OBJ)/loadbalancer.o: $(SRC)/loadbalancer.cpp $(INC)/loadbalancer.h $(INC)/request.h
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $(SRC)/loadbalancer.cpp -o $@

$(OBJ)/webserver.o: $(SRC)/webserver.cpp $(INC)/webserver.h $(INC)/request.h
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $(SRC)/webserver.cpp -o $@

$(OBJ)/request.o: $(SRC)/request.cpp $(INC)/request.h
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $(SRC)/request.cpp -o $@

# Clean target
clean:
	@rm -rf $(OBJ)
	@rm -f $(TARGET)
	@rm -f log.txt loadbalancer_log.csv assignment_log.txt
	@echo "Cleanup complete!"

# Run target
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the loadbalancer executable (default)"
	@echo "  clean   - Remove executable and object files"
	@echo "  run     - Build and run the program"
	@echo "  help    - Show this help message"

.PHONY: all clean run help
