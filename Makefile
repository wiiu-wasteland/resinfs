#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

#-------------------------------------------------------------------------------
# build settings
#-------------------------------------------------------------------------------
PLATFORM	?=	wiiu
VERSION		:=	0.7

ifeq ($(PLATFORM), wiiu)
#-------------------------------------------------------------------------------
# wiiu build config
#-------------------------------------------------------------------------------
include $(DEVKITPRO)/wut/share/wut_rules
ARCH		:=	$(MACHDEP)
ARCH		+=	-D__WIIU__ -D__WUT__
LIBDIRS		:=	$(PORTLIBS) $(WUT_ROOT)
#-------------------------------------------------------------------------------
else
ifeq ($(PLATFORM), switch)
#-------------------------------------------------------------------------------
# switch build config
#-------------------------------------------------------------------------------
include $(DEVKITPRO)/libnx/switch_rules
ARCH		:=	-march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIC -ftls-model=local-exec
ARCH		+=	-D__SWITCH__ -D__LIBNX__
LIBDIRS		:=	$(PORTLIBS) $(LIBNX)
#-------------------------------------------------------------------------------
else
$(error "Specified TARGET unsupported or missing")
endif
endif

#-------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
# INSTALL is the directory where the library will be installed
#-------------------------------------------------------------------------------
TARGET		:=	ramfs
BUILD		:=	$(PLATFORM)-build
SOURCES		:=	source
INCLUDES	:=	include
INSTALL		:=	$(DEVKITPRO)/portlibs/$(PLATFORM)

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
CFLAGS		:=	-O2 -Wall -Werror \
			-ffunction-sections \
			-fdata-sections \
			$(ARCH)

CFLAGS		+=	$(INCLUDE)

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS		:=	$(ARCH)

#-------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(BUILD)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

#-------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#-------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#-------------------------------------------------------------------------------
	export LD	:=	$(CC)
#-------------------------------------------------------------------------------
else
#-------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

export SRCFILES		:=	$(CPPFILES) $(CFILES) $(SFILES)
export OFILES		:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
				$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
				-I$(CURDIR)/$(BUILD)

.PHONY: dist-bin dist-src dist install all clean

#-------------------------------------------------------------------------------

dist-bin: all
	@[ -d lib ] || mkdir -p lib
	@cp $(OUTPUT).a lib/
	@tar --exclude=*~ -cjf $(TARGET)-$(VERSION)-$(PLATFORM).tar.bz2 \
		include \
		share \
		lib
	@rm -rf lib

dist-src:
	@tar --exclude=*~ -cjf $(TARGET)-src-$(VERSION)-$(PLATFORM).tar.bz2 \
		include \
		samples \
		share \
		source \
		Makefile \
		LICENSE \
		README.md

dist: dist-src dist-bin

install: dist-bin
	mkdir -p $(DESTDIR)$(INSTALL)
	bzip2 -cd $(TARGET)-$(VERSION)-$(PLATFORM).tar.bz2 | tar -xf - -C $(DESTDIR)$(INSTALL)

all: $(BUILD)

$(BUILD): $(SRCFILES)
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#-------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -rf $(BUILD)

#-------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS		:=	$(OFILES:.o=.d)

#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
all		:	$(OUTPUT).a

$(OUTPUT).a	:	$(OFILES)

-include $(DEPENDS)

#-------------------------------------------------------------------------------
endif
#------------------------------------------------------------------------------- 
