@echo off
REM ================================
REM Build script for CompilerProject
REM ================================

REM Go to project directory (optional if you run from project folder)
REM cd /d C:\lab1\CompilerProject
REM Using current directory instead

REM ----------------
REM 1. Clean old generated files
REM ----------------
del src\scanner.cpp 2>nul
del src\parser.cpp 2>nul
del src\parser.hpp 2>nul

REM ----------------  
REM 2. Generate parser with Bison
REM ----------------  
set "BISON_PKGDATADIR=C:\tools\GnuWin32\share\bison"
bison -d -o src\parser.cpp src\parser.y

REM ----------------
REM 3. Generate scanner with Flex
REM ----------------
flex src\scanner.l
if exist lex.yy.c move lex.yy.c src\scanner.cpp

REM ----------------  
REM 4. Compile all source files
REM ----------------  
g++ -std=c++17 src\main.cpp src\ast.cpp src\globals.cpp src\parser.cpp src\scanner.cpp -o compiler.exe

REM ----------------
REM 5. Run the compiler on test.mc (optional)
REM ----------------
if exist test.mc (
    echo Running compiler on test.mc...
    compiler.exe test.mc
) else (
    echo test.mc not found. Compilation finished.
)

REM pause
