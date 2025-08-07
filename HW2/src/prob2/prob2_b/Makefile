# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -O2

# Linker flags
LDFLAGS = -lm

# Shared source file
UTILS_SRC = ../../utils/hw2.c

# Source files for each target
FS_SRCS = f_s.c $(UTILS_SRC)
JJN_SRCS = j_j_n.c $(UTILS_SRC)
STUCKI_SRCS = stucki.c $(UTILS_SRC)

# Object files
FS_OBJS = $(FS_SRCS:.c=.o)
JJN_OBJS = $(JJN_SRCS:.c=.o)
STUCKI_OBJS = $(STUCKI_SRCS:.c=.o)

# Header files
HEADERS =  ../../utils/hw2.h

# Targets
TARGETS = f_s j_j_n stucki

# Build prob1
fs: $(FS_OBJS)
	$(CC) $(FS_OBJS) -o f_s $(LDFLAGS)

# Build prob2
jjn: $(JJN_OBJS)
	$(CC) $(JJN_OBJS) -o j_j_n $(LDFLAGS)

# Build prob3
stucki: $(STUCKI_OBJS)
	$(CC) $(STUCKI_OBJS) -o stucki $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(FS_OBJS) $(JJN_OBJS) $(STUCKI_OBJS) $(TARGETS)

# Phony targets
.PHONY: clean f_s j_j_n stucki