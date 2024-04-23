CFLAGS = -Wall -std=c99 -g
CC = gcc
LDFLAGS = -I/c/msys64/ucrt64/include -L/c/msys64/ucrt64/lib -llibportaudio

# Define project directories
SOURCE_DIR = source
DATA_DIR = data
AUDIO_DIR = audio
BUILD_DIR = build

# Define source files and header files
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)

# Define object files
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Define targets
all: executable.exe

# Rule to build the executable
executable.exe: $(OBJS)
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

# Clean target
clean:
	rm -rf $(BUILD_DIR)/*

# End of Makefile