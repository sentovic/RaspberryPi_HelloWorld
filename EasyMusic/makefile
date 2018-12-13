.RECIPEPREFIX = >

NAME       := EasyMusic

CC         := gcc
CFLAGS     := -I $(INC_DIR) -Wall -O3
CXXFLAGS   := -I $(INC_DIR) -Wall -O3

OUTPUT_DIR := build
OBJ_DIR    := $(OUTPUT_DIR)/obj
LIB_DIR    := lib
LIBS        = -lpthread -lstdc++

INC_DIRS   += include \
              include/message

SRC_DIRS   += src \
              src/message

C_SRC_FILES   := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
CXX_SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
C_OBJ_FILES   := $(patsubst %, $(OBJ_DIR)/%, $(C_SRC_FILES:.c=.o))
CXX_OBJ_FILES := $(patsubst %, $(OBJ_DIR)/%, $(CXX_SRC_FILES:.cpp=.o))
C_DEP_FILES   := $(patsubst %, $(OBJ_DIR)/%, $(C_SRC_FILES:.c=.o.d))
CXX_DEP_FILES := $(patsubst %, $(OBJ_DIR)/%, $(CXX_SRC_FILES:.cpp=.o.d))
SRC_FILES     := $(C_SRC_FILES) $(CXX_SRC_FILES)
OBJ_FILES     := $(C_OBJ_FILES) $(CXX_OBJ_FILES)
DEP_FILES     := $(C_DEP_FILES) $(CXX_DEP_FILES)

all: $(NAME).exe
> @echo Building $(NAME)...
> @echo $(SRC_FILES)
> @echo $(CXX_SRC_FILES)
> @echo $(OBJ_FILES)

> @echo Compiling objs...
$(NAME) : $(OBJ_FILES)
> @echo $^

$(C_OBJ_FILES) : %.o : %.c
> @echo C_OBJ_FILES => $@

$(CXX_OBJ_FILES) : %.o : %.cpp
> @echo CXX_OBJ_FILES => $@

.PHONY: clean
clean:
> @echo Cleaning $(OBJ_DIR)...
> -rm -rf $(OBJ_DIR)
> @echo Cleaning done

