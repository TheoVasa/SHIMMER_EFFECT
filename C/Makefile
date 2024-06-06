# C flags
CFLAGS = -Wall -std=c99 -g 
CC = gcc
# C++ flags
CXX = g++
# static flags 
STATIC_FLAGS = -static-libgcc -static-libstdc++
# Define project directories
SOURCE_DIR = source
AUDIO_DIR = audio
BUILD_DIR = build
LIBRARY_DIR = lib
# where the asio sdk is located
ASIO_DIR = common/asio
# If ASIO is used, set this value to 1 if you want to use ASIO as host API, 0 otherwise (it uses WMME)
ASIO = 1

#library managment
LIB_INC := $(wildcard $(LIBRARY_DIR)/*.h)
LDFLAGS = -I.$(LIB_INC) -L./$(LIBRARY_DIR) -ltinywav -lfilterlib
ifeq ($(ASIO), 1)
# take the asio portaudio build
LDFLAGS += -I.$(wildcard $(LIBRARY_DIR)/portaudio_asio/*.h) -L./$(LIBRARY_DIR)/portaudio_asio -lportaudio -lole32 -lwinmm -lsetupapi
else
# take the wmme portaudio build
LDFLAGS += -I.$(wildcard $(LIBRARY_DIR)/portaudio_wmme/*.h) -L./$(LIBRARY_DIR)/portaudio_wmme -lportaudio -lole32 -lwinmm -lsetupapi
endif

# output name
ifeq ($(ASIO), 1)
OUT_NAME = shimmer_asio.exe
else
OUT_NAME = shimmer.exe
endif

# Define C source files and header files
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
# Define C++ source files and header files (used for ASIO)
CXX_SRCS := $(wildcard $(ASIO_DIR)/*.cpp)
CXX_HEADERS := $(wildcard $(ASIO_DIR)/*.h)

# Define C object files
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/c/%.o,$(SRCS))

# Define C++ object files
CXX_OBJS := $(patsubst $(ASIO_DIR)/%.cpp,$(BUILD_DIR)/c++/%.o,$(CXX_SRCS))

# Define targets
all: copy_audio $(OUT_NAME) 

# Rule to build the executable (depending on the configuration)
ifeq ($(ASIO), 1)
$(OUT_NAME): $(OBJS) $(CXX_OBJS)
	$(CXX) $(STATIC_FLAGS) $(LIB_INC) $(LIB_DIR) $^ -o $@ $(LDFLAGS)
else
$(OUT_NAME): $(OBJS)
	$(CC) $(STATIC_FLAGS) $(LIB_INC) $(LIB_DIR) $^ -o $@ $(LDFLAGS)
endif

# Rule to compile C source files into object files
$(BUILD_DIR)/c/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile C++ source files into object files
$(BUILD_DIR)/c++/%.o: $(ASIO_DIR)/%.cpp $(CXX_HEADERS)
	$(CXX) -c $< -o $@

# Rule to copy audio files to the build directory
copy_audio:
	cp -r $(AUDIO_DIR) $(BUILD_DIR)

# Phony targets
.PHONY: all copy_audio clean

# Clean targets
clean:
	rm -rf $(BUILD_DIR)/c/*
	rm -rf $(BUILD_DIR)/c++/*
	rm -f $(OUT_NAME)
# End of Makefile