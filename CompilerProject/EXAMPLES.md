# MiniC Compiler - Complete Examples Documentation

## Table of Contents
1. [Data Types](#data-types)
2. [Arithmetic Operators](#arithmetic-operators)
3. [Comparison Operators](#comparison-operators)
4. [Logical Operators](#logical-operators)
5. [Control Flow](#control-flow)
6. [Variable Scoping](#variable-scoping)
7. [Comments](#comments)
8. [Complex Examples](#complex-examples)

---

## Data Types

### Integer (int)

**Input:**
```mc
int x = 42;
print(x);
```

**Syntax Tree:**
```
Program
  Var x = 
    Integer: 42
  Print
    Identifier: x
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int x;
    x = 42;
    printf("%d\n", x);
return 0;
}
```

---

### Float

**Input:**
```mc
float pi = 3.14159;
float e = 2.71828;
print(pi);
print(e);
```

**Syntax Tree:**
```
Program
  Var pi = 
    Float: 3.141590
  Var e = 
    Float: 2.718280
  Print
    Identifier: pi
  Print
    Identifier: e
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    float pi;
    pi = 3.141590;
    float e;
    e = 2.718280;
    printf("%f\n", pi);
    printf("%f\n", e);
return 0;
}
```

---

### Character (char)

**Input:**
```mc
char grade = 'A';
char symbol = '+';
print(grade);
print(symbol);
```

**Syntax Tree:**
```
Program
  Var grade = 
    Char: 'A'
  Var symbol = 
    Char: '+'
  Print
    Identifier: grade
  Print
    Identifier: symbol
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    char grade;
    grade = 'A';
    char symbol;
    symbol = '+';
    printf("%c\n", grade);
    printf("%c\n", symbol);
return 0;
}
```

---

### String

**Input:**
```mc
string message = "Hello World";
string name = "MiniC";
print(message);
print(name);
```

**Syntax Tree:**
```
Program
  Var message = 
    String: "Hello World"
  Var name = 
    String: "MiniC"
  Print
    Identifier: message
  Print
    Identifier: name
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    const char* message;
    message = "Hello World";
    const char* name;
    name = "MiniC";
    printf("%s\n", message);
    printf("%s\n", name);
return 0;
}
```

---

## Arithmetic Operators

### Addition (+)

**Input:**
```mc
int a = 10;
int b = 20;
int sum = a + b;
print(sum);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int a;
    a = 10;
    int b;
    b = 20;
    int sum;
    sum = (a + b);
    printf("%d\n", sum);
return 0;
}
```

---

### Subtraction (-)

**Input:**
```mc
int x = 50;
int y = 30;
int diff = x - y;
print(diff);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int x;
    x = 50;
    int y;
    y = 30;
    int diff;
    diff = (x - y);
    printf("%d\n", diff);
return 0;
}
```

---

### Multiplication (*)

**Input:**
```mc
int width = 5;
int height = 10;
int area = width * height;
print(area);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int width;
    width = 5;
    int height;
    height = 10;
    int area;
    area = (width * height);
    printf("%d\n", area);
return 0;
}
```

---

### Division (/)

**Input:**
```mc
int total = 100;
int parts = 4;
int each = total / parts;
print(each);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int total;
    total = 100;
    int parts;
    parts = 4;
    int each;
    each = (total / parts);
    printf("%d\n", each);
return 0;
}
```

---

### Modulo (%)

**Input:**
```mc
int number = 17;
int divisor = 5;
int remainder = number % divisor;
print(remainder);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int number;
    number = 17;
    int divisor;
    divisor = 5;
    int remainder;
    remainder = (number % divisor);
    printf("%d\n", remainder);
return 0;
}
```

---

### Unary Minus (-)

**Input:**
```mc
int positive = 42;
int negative = -positive;
print(negative);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int positive;
    positive = 42;
    int negative;
    negative = (-positive);
    printf("%d\n", negative);
return 0;
}
```

---

## Comparison Operators

### Less Than (<)

**Input:**
```mc
int a = 5;
int b = 10;
if (a < b) {
    print(a);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int a;
    a = 5;
    int b;
    b = 10;
    if((a < b)) {
    printf("%d\n", a);
    }
return 0;
}
```

---

### Greater Than (>)

**Input:**
```mc
int score = 95;
int passing = 60;
if (score > passing) {
    print(score);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int score;
    score = 95;
    int passing;
    passing = 60;
    if((score > passing)) {
    printf("%d\n", score);
    }
return 0;
}
```

---

### Less Than or Equal (<=)

**Input:**
```mc
int age = 18;
int limit = 18;
if (age <= limit) {
    print(age);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int age;
    age = 18;
    int limit;
    limit = 18;
    if((age <= limit)) {
    printf("%d\n", age);
    }
return 0;
}
```

---

### Greater Than or Equal (>=)

**Input:**
```mc
int balance = 100;
int minimum = 50;
if (balance >= minimum) {
    print(balance);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int balance;
    balance = 100;
    int minimum;
    minimum = 50;
    if((balance >= minimum)) {
    printf("%d\n", balance);
    }
return 0;
}
```

---

### Equal To (==)

**Input:**
```mc
int answer = 42;
int guess = 42;
if (answer == guess) {
    print(answer);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int answer;
    answer = 42;
    int guess;
    guess = 42;
    if((answer == guess)) {
    printf("%d\n", answer);
    }
return 0;
}
```

---

### Not Equal To (!=)

**Input:**
```mc
int x = 10;
int y = 20;
if (x != y) {
    print(x);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int x;
    x = 10;
    int y;
    y = 20;
    if((x != y)) {
    printf("%d\n", x);
    }
return 0;
}
```

---

## Logical Operators

### Logical AND (&&)

**Input:**
```mc
int age = 25;
int score = 80;
if (age >= 18 && score >= 70) {
    print(score);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int age;
    age = 25;
    int score;
    score = 80;
    if(((age >= 18) && (score >= 70))) {
    printf("%d\n", score);
    }
return 0;
}
```

---

### Logical OR (||)

**Input:**
```mc
int option1 = 1;
int option2 = 0;
if (option1 == 1 || option2 == 1) {
    print(option1);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int option1;
    option1 = 1;
    int option2;
    option2 = 0;
    if(((option1 == 1) || (option2 == 1))) {
    printf("%d\n", option1);
    }
return 0;
}
```

---

### Logical NOT (!)

**Input:**
```mc
int flag = 0;
if (!flag) {
    print(flag);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int flag;
    flag = 0;
    if((!flag)) {
    printf("%d\n", flag);
    }
return 0;
}
```

---

## Control Flow

### If Statement

**Input:**
```mc
int temperature = 30;
if (temperature > 25) {
    print(temperature);
}
```

**Syntax Tree:**
```
Program
  Var temperature = 
    Integer: 30
  If
    Comparison: >
      Identifier: temperature
      Integer: 25
Program
      Print
        Identifier: temperature
```

---

### If-Else Statement

**Input:**
```mc
int number = 15;
if (number > 20) {
    int big = 1;
    print(big);
} else {
    int small = 0;
    print(small);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int number;
    number = 15;
    if((number > 20)) {
    int big;
    big = 1;
    printf("%d\n", big);
    } else {
    int small;
    small = 0;
    printf("%d\n", small);
    }
return 0;
}
```

---

### While Loop

**Input:**
```mc
int counter = 0;
while (counter < 5) {
    print(counter);
    counter = counter + 1;
}
```

**Syntax Tree:**
```
Program
  Var counter = 
    Integer: 0
  While
    Comparison: <
      Identifier: counter
      Integer: 5
Program
      Print
        Identifier: counter
      Assign counter = 
        BinaryOp: +
          Identifier: counter
          Integer: 1
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int counter;
    counter = 0;
    while((counter < 5)) {
    printf("%d\n", counter);
    counter = (counter + 1);
    }
return 0;
}
```

---

### Nested While Loops

**Input:**
```mc
int i = 0;
while (i < 3) {
    int j = 0;
    while (j < 2) {
        print(j);
        j = j + 1;
    }
    i = i + 1;
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int i;
    i = 0;
    while((i < 3)) {
    int j;
    j = 0;
    while((j < 2)) {
    printf("%d\n", j);
    j = (j + 1);
    }
    i = (i + 1);
    }
return 0;
}
```

---

## Variable Scoping

### Block Scope with Braces

**Input:**
```mc
int x = 10;
{
    int y = 20;
    print(y);
}
print(x);
```

**Symbol Table:**
```
--- Symbol Table Scopes (inner to outer) ---
  Scope
    x : Type = int
-------------------------------------------
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int x;
    x = 10;
    int y;
    y = 20;
    printf("%d\n", y);
    printf("%d\n", x);
return 0;
}
```

---

### Variable Shadowing

**Input:**
```mc
int value = 100;
if (value > 50) {
    int value = 200;
    print(value);
}
print(value);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int value;
    value = 100;
    if((value > 50)) {
    int value;
    value = 200;
    printf("%d\n", value);
    }
    printf("%d\n", value);
return 0;
}
```

---

## Comments

### Single-Line Comments

**Input:**
```mc
// This is a comment
int x = 5;
print(x); // Print the value
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int x;
    x = 5;
    printf("%d\n", x);
return 0;
}
```

---

### Multi-Line Comments

**Input:**
```mc
/*
   This is a
   multi-line comment
*/
int result = 42;
print(result);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int result;
    result = 42;
    printf("%d\n", result);
return 0;
}
```

---

## Complex Examples

### Factorial Calculation

**Input:**
```mc
int n = 5;
int factorial = 1;
int i = 1;
while (i <= n) {
    factorial = factorial * i;
    i = i + 1;
}
print(factorial);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int n;
    n = 5;
    int factorial;
    factorial = 1;
    int i;
    i = 1;
    while((i <= n)) {
    factorial = (factorial * i);
    i = (i + 1);
    }
    printf("%d\n", factorial);
return 0;
}
```

---

### Even/Odd Checker

**Input:**
```mc
int number = 17;
int remainder = number % 2;
if (remainder == 0) {
    string result = "even";
    print(result);
} else {
    string result = "odd";
    print(result);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int number;
    number = 17;
    int remainder;
    remainder = (number % 2);
    if((remainder == 0)) {
    const char* result;
    result = "even";
    printf("%s\n", result);
    } else {
    const char* result;
    result = "odd";
    printf("%s\n", result);
    }
return 0;
}
```

---

### Mixed Type Operations

**Input:**
```mc
float price = 19.99;
int quantity = 3;
float total = price * quantity;
print(total);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    float price;
    price = 19.990000;
    int quantity;
    quantity = 3;
    float total;
    total = (price * quantity);
    printf("%f\n", total);
return 0;
}
```

---

### Complex Conditional Logic

**Input:**
```mc
int age = 25;
int income = 50000;
int credit_score = 720;

if ((age >= 21 && income >= 30000) || credit_score >= 700) {
    string status = "approved";
    print(status);
} else {
    string status = "denied";
    print(status);
}
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int age;
    age = 25;
    int income;
    income = 50000;
    int credit_score;
    credit_score = 720;
    if((((age >= 21) && (income >= 30000)) || (credit_score >= 700))) {
    const char* status;
    status = "approved";
    printf("%s\n", status);
    } else {
    const char* status;
    status = "denied";
    printf("%s\n", status);
    }
return 0;
}
```

---

### Sum of Numbers

**Input:**
```mc
int sum = 0;
int num = 1;
while (num <= 10) {
    sum = sum + num;
    num = num + 1;
}
print(sum);
```

**Output C Code:**
```c
#include <stdio.h>
int main() {
    int sum;
    sum = 0;
    int num;
    num = 1;
    while((num <= 10)) {
    sum = (sum + num);
    num = (num + 1);
    }
    printf("%d\n", sum);
return 0;
}
```

---

## Semantic Error Examples

### Redeclaration Error

**Input:**
```mc
int x = 5;
int x = 10;
```

**Error Output:**
```
Semantic error at line 2: redeclaration of 'x'
=== Compilation Failed (Semantic Errors) ===
```

---

### Use Before Declaration

**Input:**
```mc
print(y);
int y = 10;
```

**Error Output:**
```
Semantic error at line 1: identifier 'y' used before declaration
=== Compilation Failed (Semantic Errors) ===
```

---

### Type Mismatch

**Input:**
```mc
int number = 5;
number = "hello";
```

**Error Output:**
```
Semantic error at line 2: cannot assign expression of type 'string' to variable 'number' of type 'int'
=== Compilation Failed (Semantic Errors) ===
```

---

## How to Compile

1. **Create your MiniC source file** (e.g., `program.mc`)
2. **Run the compiler:**
   ```bash
   .\compiler.exe program.mc
   ```
3. **Check outputs:**
   - `syntax_tree.txt` - AST representation
   - `output.c` - Generated C code
   - Console output - Symbol table and compilation status

4. **Compile the generated C code:**
   ```bash
   gcc output.c -o output.exe
   .\output.exe
   ```

---

## Language Grammar Summary

```
Program         → StatementList
StatementList   → Statement*
Statement       → VarDeclaration | Assignment | Print | If | While
VarDeclaration  → Type IDENTIFIER [= Expression] ;
Assignment      → IDENTIFIER = Expression ;
Print           → print Expression ; | print(Expression) ;
If              → if Expression Block [else Block]
                | if (Expression) Block [else Block]
While           → while Expression Block | while (Expression) Block
Block           → { StatementList } | : StatementList end
Expression      → OrExpr
OrExpr          → AndExpr (|| AndExpr)*
AndExpr         → EqualityExpr (&& EqualityExpr)*
EqualityExpr    → ComparisonExpr ((== | !=) ComparisonExpr)*
ComparisonExpr  → AddExpr ((< | > | <= | >=) AddExpr)*
AddExpr         → MulExpr ((+ | -) MulExpr)*
MulExpr         → UnaryExpr ((* | / | %) UnaryExpr)*
UnaryExpr       → (! | -) UnaryExpr | PrimaryExpr
PrimaryExpr     → IDENTIFIER | INTEGER | FLOAT | CHAR | STRING | (Expression)
Type            → int | float | char | string
```

---

## Supported Features

✅ **Data Types:** int, float, char, string  
✅ **Operators:** +, -, *, /, %, ==, !=, <, >, <=, >=, &&, ||, !  
✅ **Control Flow:** if, if-else, while  
✅ **Variables:** Declaration, assignment, scoping  
✅ **Comments:** Single-line (//), Multi-line (/* */)  
✅ **Semantic Analysis:** Type checking, redeclaration detection, use-before-declaration  
✅ **Code Generation:** Generates valid C code  
✅ **Print:** Multiple formats based on type (%d, %f, %c, %s)  

---

**MiniC Compiler - Complete Implementation**  
*Supporting multiple types, operators, control flow, and semantic analysis*
