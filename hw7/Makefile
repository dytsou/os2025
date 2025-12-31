# Makefile for Homework 7

STUDENT_ID = 
HW7_1_SRC = hw7_1_$(STUDENT_ID).c
HW7_2_SRC = hw7_2_$(STUDENT_ID).c
HW7_1_BIN = hw7_1
HW7_2_BIN = hw7_2
CC = gcc
CFLAGS = -Wall -Wextra

# Default target
all: $(HW7_1_BIN) $(HW7_2_BIN)

# Compile Part I
$(HW7_1_BIN): $(HW7_1_SRC)
	@echo "Compiling Part I (PNG ExifTool)..."
	$(CC) $(CFLAGS) -o $(HW7_1_BIN) $(HW7_1_SRC)
	@echo "✓ Part I compiled successfully"

# Compile Part II
$(HW7_2_BIN): $(HW7_2_SRC)
	@echo "Compiling Part II (Custom ls)..."
	$(CC) $(CFLAGS) -o $(HW7_2_BIN) $(HW7_2_SRC)
	@echo "✓ Part II compiled successfully"

# Test Part I with all images
test1: $(HW7_1_BIN)
	@echo "Testing Part I (PNG ExifTool)..."
	@rm -f test_output.md
	@for img in hw7_1_imgs/*.png; do \
		if [ -f "$$img" ]; then \
			echo "Processing: $$img"; \
			./$(HW7_1_BIN) "$$img" test_output.md; \
		fi \
	done
	@echo ""
	@echo "Output written to: test_output.md"
	@echo "---"
	@cat test_output.md
	@echo "---"

# Test Part II on hw7_1_imgs directory
test2: $(HW7_2_BIN)
	@echo "Testing Part II (Custom ls)..."
	@./$(HW7_2_BIN) hw7_1_imgs

# Compare Part II output with ls -l
compare: $(HW7_2_BIN)
	@echo "=== Your program output ==="
	@./$(HW7_2_BIN) hw7_1_imgs
	@echo ""
	@echo "=== Standard ls -l output ==="
	@ls -l hw7_1_imgs

# Test both programs
test: test1 test2

# Clean compiled binaries and test outputs
clean:
	@echo "Cleaning..."
	@rm -f $(HW7_1_BIN) $(HW7_2_BIN) test_output.md output.md 1_output.md
	@echo "✓ Clean completed"

# Show help
help:
	@echo "Homework 7 Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make all      - Compile both programs (default)"
	@echo "  make test1    - Compile and test Part I with all images"
	@echo "  make test2    - Compile and test Part II on hw7_1_imgs"
	@echo "  make compare  - Compare Part II output with 'ls -l'"
	@echo "  make test     - Test both programs"
	@echo "  make clean    - Remove compiled binaries and test outputs"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Manual execution:"
	@echo "  ./$(HW7_1_BIN) <image_file> <output_file>"
	@echo "  ./$(HW7_2_BIN) <directory>"

.PHONY: all test1 test2 compare test clean help


