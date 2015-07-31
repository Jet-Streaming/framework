# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := core
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
	$(obj).target/$(TARGET)/src/plugins/jet/core/plugin.o \
	$(obj).target/$(TARGET)/src/plugins/jet/core/identity.o \
	$(obj).target/$(TARGET)/src/plugins/jet/core/fakesink.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# Make sure our dependencies are built before any of us.
$(OBJS): | $(builddir)/lib.target/libjet.so $(obj).target/libjet.so

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
	-pthread \
	-pthread \
	-Wl,-rpath=\$$ORIGIN/lib.target/ \
	-Wl,-rpath-link=\$(builddir)/lib.target/

LIBS := \
	-lrt \
	-ldl

$(obj).target/libcore.so: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/libcore.so: LIBS := $(LIBS)
$(obj).target/libcore.so: LD_INPUTS := $(OBJS) $(obj).target/libjet.so
$(obj).target/libcore.so: TOOLSET := $(TOOLSET)
$(obj).target/libcore.so: $(OBJS) $(obj).target/libjet.so FORCE_DO_CMD
	$(call do_cmd,solink)

all_deps += $(obj).target/libcore.so
# Add target alias
.PHONY: core
core: $(builddir)/lib.target/libcore.so

# Copy this to the shared library output path.
$(builddir)/lib.target/libcore.so: TOOLSET := $(TOOLSET)
$(builddir)/lib.target/libcore.so: $(obj).target/libcore.so FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/lib.target/libcore.so
# Short alias for building this shared library.
.PHONY: libcore.so
libcore.so: $(obj).target/libcore.so $(builddir)/lib.target/libcore.so

# Add shared library to "all" target.
.PHONY: all
all: $(builddir)/lib.target/libcore.so
