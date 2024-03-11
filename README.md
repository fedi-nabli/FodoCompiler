![cpp workflow](https://github.com/fedi-nabli/FodoCompiler/actions/workflows/c-cpp-build.yml/badge.svg)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

# FodoCompiler

Fodo Compiler is a simple C project made for compiling C projects and file into x86 32-bit machine code.
This compiler is made fully in C. It is 32-bit compatible.
<br />
At this stage the compiler performs lexical analysis and created a AST (Abstract Syntax Tree) for some types (still under developemnt).
<br />
For more information about the progress and stages check the 'Overview' section.

## Setup
- To be able to run this locally, you need to install make, gcc and gdb, to do this run:
``` bash
sudo apt-get install make
sudo apt-get install gcc
sudo apt-get install gdb
```

- To run th project locally, you need `gcc` and `make` installed. Works better on linux.
- Step 1: Clone the repository locally `git clone https://github.com/FodoOS/FodoCompiler.git`.
- Step 2: Run `make` or `make clean` then `make` for clean build.
- Step 3: Run `./main` and the output should be `'Everything compiled ok'`.

## Overview
- The compiler is still in the development stage.
- The compiler is divided into 6 parts:
  - Lexer: Done
  - Parser: Done
  - Code generator: In Progress
  - Preprocessor: Not started
  - Macro system: Not started
  - Semantic validator: Not started

## Other
For any information or question contact me via email at `fedinabli@gmail.com` or `fedinabli@ieee.org`.
