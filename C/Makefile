CFLAGS = -Wall -std=c99 -g
CC = gcc
# Define project directories
SOURCE_DIR = source
AUDIO_DIR = audio
BUILD_DIR = build
LIBRARY_DIR = lib

#library managment
LIB_INC := $(wildcard $(LIBRARY_DIR)/*.h)
LDFLAGS = -I.$(LIB_INC) -L./$(LIBRARY_DIR) -lportaudio -ltinywav -lfilterlib -lole32 -lwinmm -lsetupapi

# output name
OUT_NAME = shimmer.exe

# Define source files and header files
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)

# Define object files
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Define targets
all: copy_audio $(OUT_NAME) 

# Rule to build the executable
$(OUT_NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(LIB_INC) $(LIB_DIR) $^ -o $@ $(LDFLAGS)

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS)  -c $< -o $@ 


# Rule to copy audio files to the build directory
copy_audio:
	cp -r $(AUDIO_DIR) $(BUILD_DIR)

# Phony targets
.PHONY: all copy_audio clean

# Clean targets
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUT_NAME)
# End of Makefile