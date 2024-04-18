CFLAGS = -Wall -Wextra -Isource
LDFLAGS = -L/path/to/external/libraries -llibrary1 -llibrary2

# Define directories
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
all: $(BUILD_DIR)/executable

# Rule to build the executable
$(BUILD_DIR)/executable: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

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