# dir.mk

# List of source files
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# List of object files
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
