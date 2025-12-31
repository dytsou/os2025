# Makefile for Virtual Memory Homework
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=500

# Executable targets
WRITE1 = hw6_write1
WRITE2 = hw6_write2
READ1 = hw6_read1
READ2 = hw6_read2

# Source files
WRITE1_SRC = hw6_write1.c
WRITE2_SRC = hw6_write2.c
READ1_SRC = hw6_read1.c
READ2_SRC = hw6_read2.c

# Generated files (shared memory files)
SHARED_FILES = shared_file1 shared_file2

# Default target: compile all programs
all: $(WRITE1) $(WRITE2) $(READ1) $(READ2)

# Compile write programs (need sys/stat.h for S_* constants)
$(WRITE1): $(WRITE1_SRC)
	$(CC) $(CFLAGS) -include sys/stat.h -o $(WRITE1) $(WRITE1_SRC)

$(WRITE2): $(WRITE2_SRC)
	$(CC) $(CFLAGS) -include sys/stat.h -o $(WRITE2) $(WRITE2_SRC)

# Compile read programs
$(READ1): $(READ1_SRC)
	$(CC) $(CFLAGS) -o $(READ1) $(READ1_SRC)

$(READ2): $(READ2_SRC)
	$(CC) $(CFLAGS) -o $(READ2) $(READ2_SRC)

# Run all programs in sequence (write then read)
run: all
	@echo "=== Running hw6_write1 ==="
	./$(WRITE1)
	@echo "=== Running hw6_read1 ==="
	./$(READ1)
	@echo ""
	@echo "=== Running hw6_write2 ==="
	./$(WRITE2)
	@echo "=== Running hw6_read2 ==="
	./$(READ2)

# Run individual programs
run-write1: $(WRITE1)
	./$(WRITE1)

run-read1: $(READ1)
	./$(READ1)

run-write2: $(WRITE2)
	./$(WRITE2)

run-read2: $(READ2)
	./$(READ2)

# Clean up compiled executables and generated shared files
clean:
	rm -f $(WRITE1) $(WRITE2) $(READ1) $(READ2) $(SHARED_FILES)
	@echo "Cleaned up executables and shared files"

# Clean only executables (keep shared files)
clean-exec:
	rm -f $(WRITE1) $(WRITE2) $(READ1) $(READ2)
	@echo "Cleaned up executables only"

# Clean only shared files (keep executables)
clean-shared:
	rm -f $(SHARED_FILES)
	@echo "Cleaned up shared files only"

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Compile all programs (default)"
	@echo "  run          - Compile and run all programs"
	@echo "  run-write1   - Compile and run hw6_write1"
	@echo "  run-read1    - Compile and run hw6_read1"
	@echo "  run-write2   - Compile and run hw6_write2"
	@echo "  run-read2    - Compile and run hw6_read2"
	@echo "  clean        - Remove all executables and shared files"
	@echo "  clean-exec   - Remove only executables"
	@echo "  clean-shared - Remove only shared files"
	@echo "  help         - Show this help message"

.PHONY: all run run-write1 run-read1 run-write2 run-read2 clean clean-exec clean-shared help

