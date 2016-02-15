#Custom make file

PROJECT := ktrie

# Define directories
CURDIR := .
TARGET_DIR := bin

INCLUDES := -I$(CURDIR)/include/
SRC_DIR := $(CURDIR)/src/
OBJ_DIR := $(CURDIR)/obj/


BIOINF_DIR := $(CURDIR)/src/bioinf/
CONSTRUCTION_DIR := $(CURDIR)/src/construction/
TREE_DIR := $(CURDIR)/src/tree/

# Object files directories
SRC_OBJ_DIR := $(OBJ_DIR)/src/
BIOINF_OBJ_DIR := $(OBJ_DIR)/src/bioinf/
CONSTRUCTION_OBJ_DIR := $(OBJ_DIR)/src/construction/
TREE_OBJ_DIR := $(OBJ_DIR)/src/tree/

# Define files
SRC_H_FILES := $(wildcard $(SRC_DIR)/*.h)
SRC_CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ_FILES := $(addprefix $(SRC_OBJ_DIR),$(notdir $(SRC_CPP_FILES:.cpp=.o)))

BIOINF_H_FILES := $(wildcard $(BIOINF_DIR)/*.h)
BIOINF_CPP_FILES := $(wildcard $(BIOINF_DIR)/*.cpp)
BIOINF_OBJ_FILES := $(addprefix $(BIOINF_OBJ_DIR),$(notdir $(BIOINF_CPP_FILES:.cpp=.o)))

CONSTRUCTION_H_FILES := $(wildcard $(CONSTRUCTION_DIR)/*.h)
CONSTRUCTION_CPP_FILES := $(wildcard $(CONSTRUCTION_DIR)/*.cpp)
CONSTRUCTION_OBJ_FILES := $(addprefix $(CONSTRUCTION_OBJ_DIR),$(notdir $(CONSTRUCTION_CPP_FILES:.cpp=.o)))

TREE_H_FILES := $(wildcard $(TREE_DIR)/*.h)
TREE_CPP_FILES := $(wildcard $(TREE_DIR)/*.cpp)
TREE_OBJ_FILES := $(addprefix $(TREE_OBJ_DIR),$(notdir $(TREE_CPP_FILES:.cpp=.o)))


# Compiler flags
CXX := g++
CXXFLAGS := -g -Wall -O2 -std=c++11 -Wno-unused-function -w $(INCLUDES)

CC := $(CXX)
CC_FLAGS := $(CXXFLAGS)

LD_FLAGS :=
LD_LIBS := $(INCLUDES)



all: $(TARGET_DIR)/$(PROJECT)

$(TARGET_DIR)/$(PROJECT): $(BIOINF_OBJ_FILES) $(CONSTRUCTION_OBJ_FILES) $(TREE_OBJ_FILES) $(SRC_OBJ_FILES)
	@mkdir -p $(TARGET_DIR)
	$(CC) -o $@ $^  $(LD_FLAGS) $(LD_LIBS)

$(SRC_OBJ_FILES): $(SRC_CPP_FILES) $(SRC_H_FILES)
	@mkdir -p $(SRC_OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $(SRC_DIR)/$(notdir $(patsubst %.o, %.cpp, $@))

$(BIOINF_OBJ_FILES): $(BIOINF_CPP_FILES) $(BIOINF_H_FILES)
	@mkdir -p $(BIOINF_OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $(BIOINF_DIR)/$(notdir $(patsubst %.o, %.cpp, $@))

$(CONSTRUCTION_OBJ_FILES): $(CONSTRUCTION_CPP_FILES) $(CONSTRUCTION_H_FILES)
	@mkdir -p $(CONSTRUCTION_OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $(CONSTRUCTION_DIR)/$(notdir $(patsubst %.o, %.cpp, $@))

$(TREE_OBJ_FILES): $(TREE_CPP_FILES) $(TREE_H_FILES)
	@mkdir -p $(TREE_OBJ_DIR)
	$(CC) $(CC_FLAGS) -c -o $@ $(TREE_DIR)/$(notdir $(patsubst %.o, %.cpp, $@))


run: $(TARGET_DIR)/$(PROJECT)
	./$(TARGET_DIR)/$(PROJECT) $(COMMANDLINE_OPTIONS)

.PHONY: clean

clean:
	rm -ri $(OBJ_DIR)
	rm -ri $(TARGET_DIR)
	rm -fi $(BIN)
