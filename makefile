# c++ --std=c++17 models/weight.cpp file_list.cpp json_parser.cpp weights.cpp main.cpp $(pkg-config --cflags --libs libmongocxx) -o mongo

CC        := g++
LD        := g++

MODULES   := .
SRC_DIR   := $(MODULES) 
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst %.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))
DEPS      := $(OBJ:.o=.d)


vpath %.c $(SRC_DIR)

CFLAGS= -g -Wall --std=c++17 -MMD

-include $(DEPS)

define make-goal
$1/%.o: %.d
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

muh:
	@echo "SRC: " $(SRC)
	@echo "OBJ: " $(OBJ)
	@echo "DEP: " $(DEP)
	@echo "BUILD_DIR:" $(BUILD_DIR)


all: checkdirs run

run: $(OBJ)
	$(LD) $^ $(CFLAGS) -pthread -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build/*

rebuild: clean all

$(foreach bdir,$(BUILD_DIR), $(eval $(call make-goal,$(bdir))))

