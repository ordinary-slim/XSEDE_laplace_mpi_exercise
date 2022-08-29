CC=mpic++

TARGET_EXEC ?= main

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

MKDIR_P ?= mkdir -p

.PHONY: default release test clean

default: release test

release: $(BUILD_DIR)/$(TARGET_EXEC)

test:
	@echo "\n\n\nRunning program...\n"
	$(BUILD_DIR)/main

clean:
	$(RM) -r $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)
