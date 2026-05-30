# Custom Programming Language Compiler

## Project Overview

This project implements a complete compiler for a custom programming language using **FLEX** (Scanner) and **BISON** (Parser). The compiler performs lexical analysis, syntax analysis, builds a syntax tree, and generates target C code.

## Language Specification

### Language Name
**MiniC** - A simplified programming language

### Features

#### 1. Variable Declarations
```javascript
var identifier = expression;
```
Example:
```javascript
var x = 5;
var result = 10 + 20;
```

#### 2. Variable Assignments
```javascript
identifier = expression;
```
Example:
```javascript
x = 10;
result = x + 5;
```

#### 3. Print Statements
```javascript
print identifier;
```
Example:
```javascript
print result;
```

#### 4. Conditional Statements
```javascript
if expression {
    statements
} else {
    statements
}
```
Example:
```javascript
if (a > b) {
    print a;
} else {
    print b;
}
```

#### 5. Expressions
- **Arithmetic Operations**: `+`, `-`, `*`, `/`
- **Comparison Operations**: `==`, `<`, `>`
- **Identifiers**: Variable names (letters and digits, starting with letter)
- **Integers**: Whole numbers
- **Parentheses**: For grouping expressions

Example:
```javascript
var sum = (a + b) * 2;
var isGreater = a > b;
```

### Keywords
- `var` - Variable declaration
- `if` - Conditional statement
- `else` - Alternative branch
- `print` - Output statement

### Operators
- `=` - Assignment
- `==` - Equality comparison
- `<` - Less than
- `>` - Greater than
- `+` - Addition
- `-` - Subtraction
- `*` - Multiplication
- `/` - Division

### Comments
Single-line comments using `//`:
```javascript
// This is a comment
var x = 5; // This is also a comment
```

## Compiler Components

### 1. Scanner (Lexical Analyzer)
- **Tool**: FLEX
- **File**: `src/scanner.l`
- **Features**:
  - Token recognition
  - Symbol table integration
  - Error detection and reporting
  - Line number tracking

### 2. Parser (Syntax Analyzer)
- **Tool**: BISON
- **File**: `src/parser.y`
- **Features**:
  - Grammar validation
  - Abstract Syntax Tree (AST) construction
  - Error recovery and reporting

### 3. Symbol Table
- **File**: `src/symbol_table.hpp`
- **Purpose**: Tracks identifiers and their types
- **Features**:
  - Automatic insertion of new identifiers
  - Type information storage
  - Lookup functionality

### 4. Abstract Syntax Tree (AST)
- **Files**: `src/ast.hpp`, `src/ast.cpp`
- **Purpose**: Represents program structure
- **Output**: Saved to `syntax_tree.txt`

### 5. Code Generator
- **File**: `src/code_generator.hpp`
- **Purpose**: Generates target C code
- **Output**: Saved to `output.c`

## Error Handling

### Scanner Errors
The scanner detects and reports:
- Invalid characters
- Unexpected tokens
- Line number information

Example error message:
```
=== Scanner Error ===
Lexical error: Unexpected character '@' at line 5.
Valid characters are letters, digits, operators, and whitespace.
```

### Parser Errors
The parser detects and reports:
- Syntax errors
- Unexpected tokens
- Expected constructs

Example error message:
```
=== Parser Error ===
Syntax error: syntax error at line 3
Expected: variable declaration (var identifier = expression;), 
assignment (identifier = expression;), 
print statement (print identifier;), 
or if statement (if expression { ... })
```

## Building the Compiler

### Prerequisites
- **FLEX** (Lexical Analyzer Generator)
- **BISON** (Parser Generator)
- **G++** (C++ Compiler) with C++17 support

### Build Instructions

#### Windows
```batch
build2.bat
```

#### Manual Build
```bash
# 1. Generate parser
bison -d -o src/parser.cpp src/parser.y

# 2. Generate scanner
flex src/scanner.l
move lex.yy.c src/scanner.cpp

# 3. Compile
g++ -std=c++17 src/main.cpp src/ast.cpp src/globals.cpp src/parser.cpp src/scanner.cpp -o compiler.exe
```

## Usage

### Compile a Source File
```bash
compiler.exe input.mc
```

### Output Files
- `syntax_tree.txt` - Abstract Syntax Tree representation
- `output.c` - Generated C code

## Example Program

**Input** (`test.mc`):
```javascript
var a = 5;
var b = 10;
var result = 0;
result = a + b;
print result;
```

**Output** (`output.c`):
```c
#include <stdio.h>
int main() {
    int a;
    a = 5;
    int b;
    b = 10;
    int result;
    result = 0;
    result = (a + b);
    printf("%d\n", result);
    return 0;
}
```

## Project Structure

```
CompilerProject/
├── src/
│   ├── scanner.l          # FLEX scanner specification
│   ├── parser.y           # BISON parser grammar
│   ├── ast.hpp            # AST node definitions
│   ├── ast.cpp            # AST implementations
│   ├── symbol_table.hpp   # Symbol table class
│   ├── code_generator.hpp # Code generator
│   ├── main.cpp           # Main entry point
│   └── globals.cpp        # Global variables
├── build2.bat             # Build script
├── test.mc                 # Test input file
├── syntax_tree.txt        # Generated syntax tree
├── output.c               # Generated C code
└── README.md              # This file
```

## Team Members
[ 
 eslam gamal
 mohamed ali 
 mahmoud elnagar 
 mohamed abdelrhman
 mohamed elsayad 
 mohamed ashry
]



