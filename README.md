Francis-Compiler
Project Introduction
Francis-Compiler is a compiler project primarily implemented in C++.

It is designed to serve as a complete compilation pipeline, responsible for processing a specific source code language (presumed to be a Francis language) and transforming it into a target code format.

This compiler adopts a traditional multi-stage design, encompassing front-end analysis and back-end code generation.

Key Features
Based on the classes and structures found in the source code, this compiler implements the following key functionalities:

Implementation Language: Written in C++.

Lexical Analysis (Lexer): Responsible for breaking the input source code down into a sequence of Tokens.

Symbol Table Management: Uses the Table_S and AllTable structures to manage identifiers and associated information within the program.

Intermediate Code Generation: The core Code class and the imt structure are used to generate and manage the intermediate representation (IR), such as Three-Address Code.

Code Output: Capable of writing the final generated code to a specified output file.

How to Run
Since the project is provided as C++ source code, you will need to compile it first and then execute it via an interactive interface.

1. Build the Project
Compile: Use a C++ compiler (such as g++) to compile the Francis_Compiler.cpp file.

Bash

# Example compilation command
g++ Francis_Compiler.cpp -o francis_compiler
Execute: Run the generated executable file.

Bash

./francis_compiler
2. Using the Compiler
After execution, you will see a simple command-line interface:

**** Francis Compiler ****
* 0. Exit                *
* 1. Implementation      *
**************************
Input a command(0, 1): 
Input Command: Type 1 to start the compilation process.

Input File Name: The system will prompt you to enter the source code file name (e.g., test.fran). Ensure the file is in the same directory as the executable.

Output File Name: After successfully reading the file, the compiler will prompt you to enter the output file name (e.g., output.asm or output.imt) to save the generated code.

Upon completion, you will find the generated code file in the current directory.
