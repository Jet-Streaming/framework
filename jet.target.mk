# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := jet
DEFS_Default :=

# Flags passed to all source files.
CFLAGS_Default := \
	-fPIC \
	-O0 \
	-g3

# Flags passed to only C files.
CFLAGS_C_Default :=

# Flags passed to only C++ files.
CFLAGS_CC_Default :=

INCS_Default := \
	-Isrc/core

OBJS := \
	$(obj).target/$(TARGET)/src/core/jet/jet.o \
	$(obj).target/$(TARGET)/src/core/jet/log.o \
	$(obj).target/$(TARGET)/src/core/jet/object.o \
	$(obj).target/$(TARGET)/src/core/jet/element.o \
	$(obj).target/$(TARGET)/src/core/jet/bin.o \
	$(obj).target/$(TARGET)/src/core/jet/pipeline.o \
	$(obj).target/$(TARGET)/src/core/jet/pad.o \
	$(obj).target/$(TARGET)/src/core/jet/capability.o \
	$(obj).target/$(TARGET)/src/core/jet/plugin.o \
	$(obj).target/$(TARGET)/src/core/jet/videocap.o \
	$(obj).target/$(TARGET)/src/core/jet/videocap_xraw.o \
	$(obj).target/$(TARGET)/src/core/jet/basesrc.o \
	$(obj).target/$(TARGET)/src/core/jet/basetransform.o \
	$(obj).target/$(TARGET)/src/core/jet/basesink.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# Make sure our dependencies are built before any of us.
$(OBJS): | $(obj).target/libst.a

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Default := \
	-pthread

LIBS := \
	-ldl \
	-lrt

$(obj).target/libjet.so: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/libjet.so: LIBS := $(LIBS)
$(obj).target/libjet.so: LD_INPUTS := $(OBJS) $(obj).target/libst.a
$(obj).target/libjet.so: TOOLSET := $(TOOLSET)
$(obj).target/libjet.so: $(OBJS) $(obj).target/libst.a FORCE_DO_CMD
	$(call do_cmd,solink)

all_deps += $(obj).target/libjet.so
# Add target alias
.PHONY: jet
jet: $(builddir)/lib.target/libjet.so

# Copy this to the shared library output path.
$(builddir)/lib.target/libjet.so: TOOLSET := $(TOOLSET)
$(builddir)/lib.target/libjet.so: $(obj).target/libjet.so FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/lib.target/libjet.so
# Short alias for building this shared library.
.PHONY: libjet.so
libjet.so: $(obj).target/libjet.so $(builddir)/lib.target/libjet.so

# Add shared library to "all" target.
.PHONY: all
all: $(builddir)/lib.target/libjet.so
