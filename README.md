# Operating Systems Course - Homework Assignments

This repository contains homework assignments for an Operating Systems course, covering fundamental OS concepts through practical programming exercises.

## Overview

The repository is organized into 7 homework assignments (hw1-hw7), each focusing on different aspects of operating systems:

- **hw1**: Basic file operations and memory management
- **hw2**: Text User Interface (TUI) with ncurses
- **hw3**: Multi-process programming and signal handling
- **hw4**: Inter-Process Communication (IPC) with message passing and shared memory
- **hw5**: Multi-thread programming with pthreads
- **hw6**: Virtual Memory operations
- **hw7**: File system operations (PNG ExifTool and custom ls implementation)

## Project Structure

```
os/
├── hw1/          # Basic file operations (fopen, malloc)
├── hw2/          # Text User Interface with ncurses
├── hw3/          # Multi-process programming and signals
├── hw4/          # IPC with message passing and shared memory
├── hw5/          # Multi-thread programming
├── hw6/          # Virtual Memory
├── hw7/          # File system operations
└── README.md     # This file
```

## Prerequisites

- **Compiler**: GCC (GNU Compiler Collection)
- **C Standard**: C11 or later
- **Libraries**:
  - `ncurses` (for hw2)
  - `pthread` (for hw5)
  - Standard POSIX libraries

### Installing Dependencies

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev
```

On Fedora/RHEL:
```bash
sudo dnf install gcc make ncurses-devel
```

## Homework Assignments

### Homework 1: Basic File Operations

**Topics**: File I/O, memory allocation

**Files**:
- `main.cpp` - Basic Hello World program
- `fopen.cpp` - File opening operations
- `malloc.cpp` - Memory allocation examples

**Compilation**:
```bash
cd hw1
gcc -o main main.cpp
gcc -o fopen fopen.cpp
gcc -o malloc malloc.cpp
```

### Homework 2: Text User Interface

**Topics**: Terminal control, ncurses library, keyboard input handling

**Files**:
- `main.cpp` - Interactive TUI application with colored clock display

**Features**:
- Real-time clock display
- Color cycling animation
- Arrow key navigation
- 'q' to quit

**Compilation**:
```bash
cd hw2
gcc -o main main.cpp -lncurses
```

**Usage**:
```bash
./main
# Use arrow keys to move the clock, 'q' to quit
```

### Homework 3: Multi-process Programming and Signal Handling

**Topics**: Process creation (fork), process synchronization (wait), signal handling

**Files**:
- `hw3_1.c` - Fork and process synchronization example
- `hw3_2.c` - Signal handling example

**Compilation**:
```bash
cd hw3
gcc -o hw3_1 hw3_1.c
gcc -o hw3_2 hw3_2.c
```

### Homework 4: IPC with Message Passing and Shared Memory

**Topics**: FIFOs (named pipes), shared memory, inter-process communication

**Files**:
- `hw4_1.cpp` - FIFO-based message passing
- `hw4_2.cpp` - Shared memory implementation
- `hw4_test1`, `hw4_test2.c` - Test programs

**Compilation**:
```bash
cd hw4
gcc -o hw4_1 hw4_1.cpp
gcc -o hw4_2 hw4_2.cpp
```

### Homework 5: Multi-thread Programming

**Topics**: POSIX threads (pthreads), thread synchronization, parallel algorithms

**Files**:
- `hw5_1.c` - Multi-threaded merge sort implementation
- `hw5_2.c` - Additional threading exercises
- `testcase.txt` - Test data

**Compilation**:
```bash
cd hw5
gcc -o hw5_1 hw5_1.c -lpthread
gcc -o hw5_2 hw5_2.c -lpthread
```

### Homework 6: Virtual Memory

**Topics**: Memory-mapped files, shared memory operations

**Files**:
- `hw6_write1.c`, `hw6_write2.c` - Programs that write to shared memory
- `hw6_read1.c`, `hw6_read2.c` - Programs that read from shared memory
- `Makefile` - Build automation

**Compilation and Usage**:
```bash
cd hw6
make              # Compile all programs
make run          # Run all programs in sequence
make clean        # Clean up executables and shared files
```

**Available Makefile targets**:
- `make all` - Compile all programs (default)
- `make run` - Compile and run all programs
- `make run-write1`, `make run-read1` - Run individual programs
- `make clean` - Remove executables and shared files
- `make help` - Show all available targets

### Homework 7: File System Operations

**Topics**: Binary file parsing, PNG format, directory operations, file metadata

**Files**:
- `hw7_1.c` - PNG ExifTool (extracts metadata from PNG files)
- `hw7_2.c` - Custom `ls` implementation
- `hw7_1_imgs/` - Test PNG images
- `Makefile` - Build automation
- `hw7_111550073.md` - Detailed report and documentation

**Compilation and Usage**:
```bash
cd hw7
make              # Compile both programs
make test1        # Test PNG ExifTool with all images
make test2        # Test custom ls on hw7_1_imgs directory
make compare      # Compare custom ls output with standard ls -l
make clean        # Clean up executables
```

**Part I - PNG ExifTool**:
```bash
./hw7_1 <image_file> <output_file>
# Example:
./hw7_1 ./hw7_1_imgs/1.png output.md
```

**Part II - Custom ls**:
```bash
./hw7_2 <directory>
# Example:
./hw7_2 ./hw7_1_imgs
```

**Features**:
- PNG file format parsing (magic number, IHDR chunk, metadata extraction)
- Endianness conversion (big-endian to little-endian)
- Directory listing with detailed file information
- File permissions display (including SUID, SGID, sticky bits)
- File type detection (regular files, directories, links, etc.)

## Building and Running

### Individual Homework

Each homework directory contains its own source files. Navigate to the specific homework directory and compile:

```bash
cd hw<number>
gcc -Wall -Wextra -o <executable> <source_file.c> [additional_flags]
```

### Using Makefiles

Some homeworks (hw6, hw7) include Makefiles for easier compilation:

```bash
cd hw<number>
make
```

## Common Compilation Flags

- `-Wall` - Enable all common warnings
- `-Wextra` - Enable extra warnings
- `-std=c11` - Use C11 standard
- `-lpthread` - Link pthread library (for hw5)
- `-lncurses` - Link ncurses library (for hw2)
- `-D_XOPEN_SOURCE=500` - Enable POSIX features (for hw6)

## Notes

- Most programs are written in C, with some C++ files in earlier assignments
- All programs follow POSIX standards for portability
- Test files and sample data are included where applicable

## Course Materials

The repository also includes PDF lecture slides:
- Environment Setup
- Text User Interface
- Multi-process Programming and Signal Handling
- IPC with Message Passing and Shared Memory
- Multi-thread Programming
- Virtual Memory
- File system operations
