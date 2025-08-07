# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -O2

# Linker flags
LDFLAGS = -lm

# Shared source file
UTILS_SRC = ../../utils/hw2.c

# Source files for each target
FIXED_SRCS = fixed_thresholding.c $(UTILS_SRC)
RANDOM_SRCS = random_thresholding.c $(UTILS_SRC)
DIHERING_SRCS = dithering_matrix.c $(UTILS_SRC)

# Object files
FIXED_OBJS = $(FIXED_SRCS:.c=.o)
RANDOM_OBJS = $(RANDOM_SRCS:.c=.o)
DITHERING_OBJS = $(DIHERING_SRCS:.c=.o)

# Header files
HEADERS =  ../../utils/hw2.h

# Targets
TARGETS = fixed_thresholding random_thresholding dithering_matrix

# Build prob1
fixed: $(FIXED_OBJS)
	$(CC) $(FIXED_OBJS) -o fixed_thresholding $(LDFLAGS)

# Build prob2
random: $(RANDOM_OBJS)
	$(CC) $(RANDOM_OBJS) -o random_thresholding $(LDFLAGS)

# Build prob3
dithering: $(DITHERING_OBJS)
	$(CC) $(DITHERING_OBJS) -o dithering_matrix $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(FIXED_OBJS) $(RANDOM_OBJS) $(DITHERING_OBJS) $(TARGETS)

# Phony targets
.PHONY: clean random_thresholding random_thresholding dithering_matrix