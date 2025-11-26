# Francis-Compiler

![License](https://img.shields.io/badge/License-Custom-blue.svg)
A complete, multi-stage compiler implemented in C++ for educational purposes or processing a specific "Francis" language variant.

##  Project Introduction

`Francis-Compiler` is a self-contained compiler pipeline written entirely in **C++**.

The main objective of this project is to provide a working model of a compiler, translating source code into an Intermediate Representation (IR) or a target code format suitable for execution. The implementation covers both the analytical front-end and the code synthesis back-end stages.

##  Key Features

Based on the structure and classes found in the `Francis_Compiler.cpp` source file, the compiler implements the following core functionalities:

| Feature | Description | Implementation Details |
| :--- | :--- | :--- |
| **Implementation Language** | The entire compiler is built using C++. | |
| **Lexical Analysis (Lexer)** | Responsible for tokenizing the input source code. | Handled by the `Lexer` class and `Token` structure. |
| **Symbol Table Management** | Manages identifiers, types, and scope. | Implemented using the `Table_S` and `AllTable` structures. |
| **Intermediate Code (IR) Generation** | Generates a lower-level representation of the source code. | The `Code` class utilizes the `imt` structure (Intermediate Code Table) for code generation, typically Three-Address Code. |
| **Interactive CLI** | Provides a simple, command-line driven interface for user interaction. | Managed within the `main` function loop. |

##  Getting Started

Follow these steps to build and run the `Francis-Compiler` on your local machine.

### Prerequisites

You need a standard C++ development environment installed:

* **C++ Compiler:** `g++` (or equivalent)
* **Operating System:** Any system supporting standard C++ compilation (Linux, macOS, Windows/WSL).

### Building the Project

Since the project is compiled from a single source file, the build process is straightforward.

1.  **Clone the Repository** (If you haven't already):
    ```bash
    git clone [https://github.com/pinwei123/Francis-Compiler.git](https://github.com/pinwei123/Francis-Compiler.git)
    cd Francis-Compiler
    ```

2.  **Compile the Source Code:**
    ```bash
    g++ Francis_Compiler.cpp -o francis_compiler
    ```
    This command compiles the source file and creates an executable named `francis_compiler`.

## ⚙️ Usage

The compiler runs in an interactive command-line mode.

1.  **Execute the Compiler:**
    ```bash
    ./francis_compiler
    ```

2.  **Interactive Interface:** You will be greeted by the following menu:

    ```
    **** Francis Compiler ****
    * 0. Exit                *
    * 1. Implementation      *
    **************************
    Input a command(0, 1): 
    ```

3.  **Start Compilation:**
    * Enter `1` and press Enter to begin the compilation workflow.

4.  **Provide Input File:**
    * The program will prompt you: `Input file name: `
    * Enter the name of your source code file (e.g., `input.txt`). Ensure this file exists in the same directory as the executable.

5.  **Provide Output File:**
    * The program will then prompt you: `Output file name: `
    * Enter the desired name for the output file (e.g., `output.txt`). This is where the generated Intermediate Code will be saved.

The compiler will process the input file and write the result to the specified output file.

