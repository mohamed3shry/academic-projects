# --- Makefile for FLEX/BISON Compiler Project ---

# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lfl # Link with the Flex library

# Source files
SRC_DIR = src
MAIN_SRC = $(SRC_DIR)/main.cpp
AST_SRC = $(SRC_DIR)/ast.cpp
GLOBALS_SRC = $(SRC_DIR)/globals.cpp
PARSER_SRC = $(SRC_DIR)/parser.cpp
SCANNER_SRC = $(SRC_DIR)/scanner.cpp
OBJ = $(MAIN_SRC:.cpp=.o) $(AST_SRC:.cpp=.o) $(GLOBALS_SRC:.cpp=.o) $(PARSER_SRC:.cpp=.o) $(SCANNER_SRC:.cpp=.o)

# Target executable name
TARGET = compiler

# --- Main Target ---
$(TARGET): $(PARSER_SRC) $(SCANNER_SRC) $(MAIN_SRC) $(AST_SRC) $(GLOBALS_SRC)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(AST_SRC) $(GLOBALS_SRC) $(PARSER_SRC) $(SCANNER_SRC) -o $(TARGET) $(LDFLAGS)

# --- BISON Rule ---
# Generates parser.cpp and parser.hpp from parser.y
$(PARSER_SRC): $(SRC_DIR)/parser.y
	bison -d -o $(PARSER_SRC) $(SRC_DIR)/parser.y

# --- FLEX Rule ---
# Generates scanner.cpp from scanner.l
# Note: On Windows, flex might not support -o option, so we generate lex.yy.c and rename it
$(SCANNER_SRC): $(SRC_DIR)/scanner.l
	@if [ -f lex.yy.c ]; then rm -f lex.yy.c; fi
	flex $(SRC_DIR)/scanner.l
	@if [ -f lex.yy.c ]; then mv lex.yy.c $(SCANNER_SRC); echo "Scanner generated successfully"; fi

# --- Clean Rule ---
clean:
	rm -f $(TARGET) $(SRC_DIR)/*.o $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.hpp $(SRC_DIR)/scanner.cpp syntax_tree.txt output.c

.PHONY: all clean
