# ==========================
# Compiler and flags
# ==========================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -m64
CPPFLAGS = -I include
LDFLAGS =
LDLIBS =

# ==========================
# Discord Social SDK
# ==========================
DISCORD_SDK_ROOT = lib/discord_social_sdk
DISCORD_SDK_INCLUDE = $(DISCORD_SDK_ROOT)/include
DISCORD_SDK_LIB_RELEASE = $(DISCORD_SDK_ROOT)/lib/release
DISCORD_SDK_BIN_RELEASE = $(DISCORD_SDK_ROOT)/bin/release

CPPFLAGS += -I $(DISCORD_SDK_INCLUDE)

# ==========================
# Source and target
# ==========================
SRCS = $(shell find src -name '*.cpp')
TARGET_NAME = main

# Default to debug build
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), release)
    CXXFLAGS += -O3
    BUILD_DIR = bin/release
else
    CXXFLAGS += -g
    BUILD_DIR = bin/debug
endif

OBJS = $(patsubst src/%.cpp, $(BUILD_DIR)/obj/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/$(TARGET_NAME)

# ==========================
# Platform-specific settings
# ==========================
ifeq ($(OS), Windows_NT)

    # -------- Windows --------
    SDL2_PATH = C:/SDL2-w64
    CPPFLAGS += -I $(SDL2_PATH)/include
    LDFLAGS += -L $(SDL2_PATH)/lib
    LDLIBS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

    # Discord SDK (Windows)
    LDFLAGS += -L $(DISCORD_SDK_LIB_RELEASE)
    LDLIBS += -ldiscord_partner_sdk
    DISCORD_RUNTIME = $(DISCORD_SDK_BIN_RELEASE)/discord_partner_sdk.dll

    RUN_CMD = start $(TARGET)
    RM = del /Q /F
    MKDIR_P = mkdir

    TARGET_WIN = $(subst /,\,$(TARGET))
    BUILD_DIR_WIN = $(subst /,\,$(BUILD_DIR))

else

    # -------- macOS / Linux --------
    SDL2_PATH ?= /opt/homebrew/opt/sdl2
    CPPFLAGS += -I $(SDL2_PATH)/include
    LDFLAGS += -L $(SDL2_PATH)/lib
    LDLIBS += -lSDL2 -lSDL2_image

    # Discord SDK (macOS / Linux)
    LDFLAGS += -L $(DISCORD_SDK_LIB_RELEASE)
    LDLIBS += -ldiscord_partner_sdk

    UNAME_S := $(shell uname)
    ifeq ($(UNAME_S), Darwin)
        DISCORD_RUNTIME = $(DISCORD_SDK_LIB_RELEASE)/libdiscord_partner_sdk.dylib
    else
        DISCORD_RUNTIME = $(DISCORD_SDK_LIB_RELEASE)/libdiscord_partner_sdk.so
    endif

    RUN_CMD = ./$(TARGET)
    RM = rm -f
    MKDIR_P = mkdir -p

endif

# ==========================
# Phony targets
# ==========================
.PHONY: all clean run debug release

# ==========================
# Default target
# ==========================
all: $(TARGET)

# ==========================
# Link executable
# ==========================
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "Copying Discord SDK runtime..."
	@cp $(DISCORD_RUNTIME) $(dir $@) 2>/dev/null || echo "Discord runtime already present"

# ==========================
# Compile objects
# ==========================
$(BUILD_DIR)/obj/%.o: src/%.cpp
	@echo "Compiling $<..."
	@$(MKDIR_P) $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CPPFLAGS)

# ==========================
# Build & run
# ==========================
run: all
	@echo "Running $(TARGET)..."
	$(RUN_CMD)

debug:
	$(MAKE) all BUILD_TYPE=debug

release:
	$(MAKE) all BUILD_TYPE=release

# ==========================
# Clean
# ==========================
clean:
ifeq ($(OS), Windows_NT)
	@if exist bin rmdir /S /Q bin
else
	@echo "Cleaning up..."
	$(RM) -r bin
endif
