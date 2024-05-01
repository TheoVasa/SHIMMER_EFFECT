CFLAGS = -Wall -std=c99 -g
CC = gcc
#library directory 
LDFLAGS = -I/c/msys64/ucrt64/include -L/c/msys64/ucrt64/lib -llibportaudio -ltinywav

# Define project directories
SOURCE_DIR = source
DATA_DIR = data
AUDIO_DIR = audio
BUILD_DIR = build
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

# Rule to copy data files to the build directory
copy_data:
	cp -r $(DATA_DIR) $(BUILD_DIR)

# Rule to copy audio files to the build directory
copy_audio:
	cp -r $(AUDIO_DIR) $(BUILD_DIR)

# Phony targets
.PHONY: all copy_data copy_audio clean

# Clean targets
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUT_NAME)
# End of Makefile