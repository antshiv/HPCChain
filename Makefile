# Makefile

# Check for the existence of icc and set the compiler accordingly
ifneq ($(shell which icc 2>/dev/null),)
    CC = icc
    CFLAGS = -O3 -fopenmp -march=native -xHost -qopt-report=5 -qopt-report-phase=vec
else
    CC = gcc
    CFLAGS = -O3 -fopenmp -march=native -ftree-vectorize -fopt-info-vec
endif

LDFLAGS = -lm -lmpi -lnuma

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIRS = include include/server include/utils

# Source and Object files
include dir.mk

# Add the include directories to the CFLAGS
CFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

# Target executable
TARGET = $(BIN_DIR)/hpc_server

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
