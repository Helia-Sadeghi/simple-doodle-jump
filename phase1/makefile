CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
TARGET_NAME = SimpleDoodleJump

SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

vpath %.cpp $(SRC_DIR) $(SRC_DIR)/core $(SRC_DIR)/entities $(SRC_DIR)/managers

ifeq ($(OS),Windows_NT)
    TARGET = $(TARGET_NAME).exe
    MKDIR = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    RM = if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR) & if exist $(TARGET) del /Q $(TARGET)
else
    TARGET = $(TARGET_NAME)
    MKDIR = mkdir -p $(OBJ_DIR)
    RM = rm -rf $(OBJ_DIR) $(TARGET)
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR)

clean:
	$(RM)

ifeq ($(OS),Windows_NT)
run: all
	$(TARGET)
else
run: all
	./$(TARGET)
endif

.PHONY: all clean run
