# Set project directory one level above of Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(realpath $(CURDIR))
SOURCEDIR := $(PROJDIR)/src
BUILDDIR := $(PROJDIR)/build
PINCLUDES := $(PROJDIR)/include
TESTDIR := $(PROJDIR)/tests

# Name of the final executable
TARGET = libhp54600b.a

# Decide whether the commands will be shwon or not
VERBOSE = TRUE

# Create the list of directories
DIRS = utils rs232 command
SOURCEDIRS = $(foreach dir, $(DIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
TARGETDIRS = $(foreach dir, $(DIRS), $(addprefix $(BUILDDIR)/, $(dir)))

# Generate the GCC includes parameters by adding -I before each source folder
# INCLUDES = $(foreach dir, $(SOURCEDIRS), $(addprefix -I, $(dir)))
INCLUDES = $(addprefix -I, $(SOURCEDIR))

# Add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Create a list of *.c sources in DIRS
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.c=.o))

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CC = gcc

# OS specific part
RM = rm -f
RMDIR = rm -rf
MKDIR = mkdir -p
ERRIGNORE = 2>/dev/null
SEP=/

# Remove space after separator
PSEP = $(strip $(SEP))

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

# Define the function that will generate each rule
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$(HIDE)$(CC) -c $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef

.PHONY: all clean directories

all: directories $(TARGET)

test: all $(TESTDIR)/demo.c
	@echo Building test
	$(HIDE)$(CC) -I$(PINCLUDES) -o $(TESTDIR)/demo.out -L $(BUILDDIR)  $(TESTDIR)/demo.c -lhp54600b
	$(HIDE)$(CC) -I$(PINCLUDES) -o $(TESTDIR)/demo2.out -L $(BUILDDIR)  $(TESTDIR)/demo2.c -lhp54600b

$(TARGET): $(OBJS)
	@echo Linking $@
	$(HIDE)ar rcs $(BUILDDIR)/$(TARGET) $(OBJS)

# Include dependencies
-include $(DEPS)

# Generate rules
$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)

# Remove all objects, dependencies and executable files generated during the build
clean:
	$(HIDE)$(RMDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
	$(HIDE)$(RM) $(BUILDDIR)/$(TARGET) $(ERRIGNORE)
	$(HIDE)$(RM) $(TESTDIR)/demo.out $(ERRIGNORE)
	@echo Cleaning done !
