CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
OBJ_DIR = obj
TARGET_NAME = SimpleDoodleJump

SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

vpath %.cpp $(SRC_DIR) $(SRC_DIR)/core $(SRC_DIR)/entities $(SRC_DIR)/managers

TARGET = $(TARGET_NAME).exe
MKDIR = mkdir -p $(OBJ_DIR)
RM = rm -rf $(OBJ_DIR) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR)

clean:
	$(RM)

run: all
	./$(TARGET)

.PHONY: all clean run