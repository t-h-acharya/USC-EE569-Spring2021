# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -O2

# Linker flags
LDFLAGS = -lm

# Shared source file
UTILS_SRC = ../../utils/hw2.c

# Source files for each target
SEPARABLE_ERROR_SRCS = separable_error.c $(UTILS_SRC)

# Object files
SEPARABLE_ERROR_OBJS = $(SEPARABLE_ERROR_SRCS:.c=.o)

# Header files
HEADERS =  ../../utils/hw2.h

# Targets
TARGETS = separable_error

separable_error: $(SEPARABLE_ERROR_OBJS)
	$(CC) $(SEPARABLE_ERROR_OBJS) -o separable_error $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(SEPARABLE_ERROR_OBJS) $(TARGETS)

# Phony targets
.PHONY: clean separable_error