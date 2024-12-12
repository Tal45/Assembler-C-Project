# Two-Pass Assembler

## Table of Contents

1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
4. [Usage](#usage)
    - [Running the Assembler](#running-the-assembler)
    - [Input File Format](#input-file-format)
    - [Output Files](#output-files)
5. [Project Structure](#project-structure)
6. [Contributing](#contributing)
7. [License](#license)
8. [Acknowledgements](#acknowledgements)
9. [Contact](#contact)

## Project Overview

This project implements a two-pass assembler for a hypothetical assembly language. The assembler translates assembly language source code into machine code, performing lexical analysis, syntax analysis, and code generation in two passes.

## Features

- **Two-Pass Processing**: Ensures accurate resolution of symbols and addresses.
- **Error Handling**: Detects and reports syntax and semantic errors.
- **Memory and Symbol Management**: Efficient handling of labels and memory.
- **Output Files**: Generates machine code, extern labels usage, entry labels definitions, macro-extended file

## Getting Started

### Prerequisites

- GCC (GNU Compiler Collection)
- Make (build automation)

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/Tal45/Assembler-C-Project.git
    cd Assembler-C-Project
    ```

2. Compile the assembler:
    ```sh
    make
    ```

## Usage

### Running the Assembler

To assemble a source file, run the following command for '.as' source files:
```sh
./assembler source1 source2 source3 ...
```

### Input File Format

The assembler expects the source file to be in a specific format. Below are the details of the expected input:


#### Registers
The assembler supports 8 registers:
- **r0, r1, r2, r3, r4, r5, r6, r7**

#### Supported Commands and Instructions
   - **mov**: (Move data)
   - **cmp**: (Compare)
   - **add**: (Addition)
   - **sub**: (Subtraction)
   - **lea**: (Load effective address)
   - **clr**: (Clear register)
   - **inc**: (Increment)
   - **dec**: (Decrement)
   - **jmp**: (Jump)
   - **bne**: (Branch if not equal)
   - **red**: (Read input)
   - **prn**: (Print)
   - **jsr**: (Jump to subroutine)
   - **rts**: (Return from subroutine)
   - **stop**: (Stop execution)
     
#### Addressing Modes
- **Immediate**: #<value> (e.g., mov #5, r1)
- **Direct**: <label> (e.g., mov label, r1)
- **Register Direct**: r<register_number> (e.g., mov r2, r1)
- **Register Indirect**: *r<register_number> (e.g., mov *r3, r1)

#### Directives
1. **Data Definition:**
   - `.data <value1>, <value2>, ...`
   - Example: `.data 10, 20, -30`
2. **String Definition:**
   - `.string "<text>"`
   - Example: `.string "hello"`
3. **Entry Definition:**
   - `.entry <label>`
   - Example: `.entry LOOP`
4. **Extern Definition:**
   - `.extern <label>`
   - Example: `.extern GLOBAL`

#### Memory Layout
- Addressable memory: 0 to 4095 (decimal).
- Each instruction is encoded into 15 bits:
  - Bits 14-11: Opcode.
  - Bits 10-7: Source operand addressing method.
  - Bits 6-3: Destination operand addressing method.
  - Bits 2-0: Additional flags (A, R, E).

 #### Symbols and Labels
- Labels must start with an alphabetic character and be followed by alphanumeric characters.
- Labels end with a colon (`:`) and can appear before an instruction or directive.
  - Example: `LOOP: mov r1, r2`
- Symbols must be defined before use unless declared as `.extern`.

#### Macros
- Define macros using the syntax:
  ```sh
  macr <macro name>
  <command #1>
  <command #2>
  .
  .
  .
  endmacr
  ```
- To invoke the macro block in the code simply put the macro name


### Output Files

- **Macro-Extended File**: `source.am`
- **Machine Code File**: `source.ob`
- **Extern Labels**: `source.ext`
- **Entry Labels**: `source.ent`

## Project Structure

The project directory structure is as follows:

`Assembler-C-Project/`                        
│                                        
├── `src/`                    
│   ├── main.c                                    
│   ├── preprocess.c                        
│   ├── firstpass.c                        
│   ├── secondpass.c                        
│   ├── filehandler.c                        
│   ├── valdation.c                            
│   ├── marcohandler.c                            
│   ├── labelhandler.c                        
│   └── virtualmemory.c                    
│                                
├── `headers/`                    
│   ├── main.h                
│   ├── virtualmemory.h                
│   ├── validation.h                    
│   ├── structs.h                    
│   ├── labelhandler.h                    
│   ├── macrohandler.h                    
│   ├── constants.h                    
│   └── filehandler.h                    
│                        
├── `inputfiles/`                            
│   ├──                    
│   └── ...                
│                
├── `outputfiles/`                    
│   ├──                    
│   └── ...                        
│                    
├── README.md                            
├── makefile                
└── LICENSE            
    
## Contributing

We are not accepting any contributions at the moment.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Made by **Vadim Volkov** and **Tal Levi** @ Open University as final project for C-Programming Labratory Course semester 2024b.
- Studies based on the book The C Programming Language (ANSI-C) 2nd Edition by Brian W.Kernighan and Dennis M. Ritchie.

## Contact

For any questions or feedback, please reach out to [tal4456@gmail.com] or [vadvol1337@gmail.com].

