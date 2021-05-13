# General definitions
TARGET = edl_cli
CFLAGS = -Wall -g -Og
LDFLAGS = -lrt

# Include OpenCCSDS sources
CCSDS_ROOT = OpenCCSDS
include $(CCSDS_ROOT)/ccsds.mk

# Define application sources
SRCDIR = src
EDL_INC = include
EDL_SRC = $(SRCDIR)/common.c	\
		  $(SRCDIR)/command.c	\
		  $(SRCDIR)/file.c		\
		  $(SRCDIR)/main.c

# All sources
INC := $(CCSDS_INC) $(EDL_INC)
CSRC := $(CCSDS_SRC) $(EDL_SRC)

# Output and dependency directories
BUILDDIR = build
DEPDIR := $(BUILDDIR)/.dep
VPATH := $(sort $(dir $(CSRC)))

OBJS := $(addprefix $(BUILDDIR)/, $(notdir $(CSRC:.c=.o)))
INCDIR := $(patsubst %,-I%,$(INC))

# Generate dependencies
CFLAGS += -MD -MP -MF $(DEPDIR)/$(@F).d

all: $(OBJS) $(TARGET)

$(OBJS): | $(BUILDDIR) $(DEPDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

$(OBJS): $(BUILDDIR)/%.o : %.c $(MAKEFILE_LIST)
	$(CC) -c $(CFLAGS) -I. $(INCDIR) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@rm -rf $(BUILDDIR) $(TARGET)

print-%:
	@echo '$*=$($*)'
