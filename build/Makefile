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

# Nombre del archivo DOT y PNG
DOT_FILE := ast.dot
PNG_FILE := ast.png

# Comando para abrir archivos según el SO
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    OPEN_CMD := xdg-open
endif
ifeq ($(UNAME_S),Darwin)
    OPEN_CMD := open
endif
ifndef OPEN_CMD
    OPEN_CMD := start
endif

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

# Generar imagen del AST desde archivo DOT
dot: $(DOT_FILE)
	dot -Tpng $(DOT_FILE) -o $(PNG_FILE)
	@echo "AST generado como $(PNG_FILE)"

# Generar imagen del AST desde archivo DOT y abrirlo
view: dot
	$(OPEN_CMD) $(PNG_FILE)
	@echo "AST abierto en el visor de imágenes predeterminado"

# Ejecutar programa principal
run: all
	@./$(TARGET) $(FILE)

# Ejecutar tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Limpiar
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(PNG_FILE)

.PHONY: all clean run test dot view