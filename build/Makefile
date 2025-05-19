# Ruta base del proyecto
ROOT_DIR := ..

# Compilador
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g -fsanitize=address -I$(ROOT_DIR)/include -I../lib

# Directorios
SRC_DIR := $(ROOT_DIR)/src
TEST_DIR := $(ROOT_DIR)/test
BUILD_DIR := .
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

# Archivos fuente del proyecto principal
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Archivos fuente de pruebas
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.test.o,$(TEST_SRCS))

# Ejecutables
TARGET := $(BIN_DIR)/main
TEST_TARGET := $(BIN_DIR)/test

# Reglas
all: $(TARGET)

# Compilar ejecutable principal
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar ejecutable de test
$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar archivos fuente
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar archivos de test
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ejecutar programa principal
run: all
	./$(TARGET)

# Ejecutar tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Limpiar
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean run test
