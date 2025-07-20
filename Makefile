# Makefile for Load Balancer Project (Unix/Linux)

# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude -Wall -Wextra

# Folders
SRC = src
INC = include
OBJ = obj

# Files
OBJS = $(OBJ)/main.o $(OBJ)/request.o $(OBJ)/webserver.o $(OBJ)/loadbalancer.o
TARGET = loadbalancer

# Default target
all: $(TARGET)

# Compile target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object file compilation
$(OBJ)/main.o: main.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	@rm -rf $(OBJ)
	@rm -f $(TARGET)
	@rm -f log.txt
	@rm -f loadbalancer_log.csv
	@echo "Cleanup complete!"

# Install target (optional)
install: $(TARGET)
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Load balancer installed to /usr/local/bin/"

# Uninstall target
uninstall:
	@sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Load balancer uninstalled"

# Run the program
run: all
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)

# Doxygen docs
docs:
	@doxygen Doxyfile 2>/dev/null || echo "Doxygen not found. Install with: sudo apt-get install doxygen"

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the load balancer (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the load balancer"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build with optimization"
	@echo "  install  - Install to /usr/local/bin/"
	@echo "  uninstall- Remove from /usr/local/bin/"
	@echo "  docs     - Generate documentation"
	@echo "  help     - Show this help message"

.PHONY: all clean install uninstall run debug release docs help
