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
debug: CPPFLAGS += -g
debug: LDFLAGS += -g

MKDIR_P ?= mkdir -p

.PHONY: default release test clean

default: release test

release: $(BUILD_DIR)/$(TARGET_EXEC)
debug: $(BUILD_DIR)/$(TARGET_EXEC)

test:
	$(MKDIR_P) "post"
	@echo "\n\n\nRunning program...\n"
	mpirun -n 2 $(BUILD_DIR)/main

clean:
	$(RM) -r $(BUILD_DIR)

post:
	$(MKDIR_P) $@

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) post
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)
