# Compiler
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
SRCDIR = src
INCDIR = headers
OBJDIR = obj

# Source and header files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
LIB = $(wildcard $(INCDIR)/*.h)

# Executable
TARGET = assembler

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(LIB) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS) $(LIB)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

