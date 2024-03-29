#
# Copyright (C) 2010-2012 ARM Limited. All rights reserved.
#
# This program is free software and is provided to you under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
#
# A copy of the licence is included with the program, and can also be obtained from Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

USE_UMPV2=0

# The Makefile sets up "arch" based on the CONFIG, creates the version info
# string and the __malidrv_build_info.c file, and then call the Linux build
# system to actually build the driver. After that point the Kbuild file takes
# over.

OSKOS=linux
FILES_PREFIX=

# This conditional makefile exports the global definition ARM_INTERNAL_BUILD. Customer releases will not include arm_internal.mak
-include ../../../arm_internal.mak

# Check that required parameters are supplied.
ifeq ($(CONFIG),)
$(error "CONFIG must be specified.")
endif
ifeq ($(CPU)$(KDIR),)
$(error "KDIR or CPU must be specified.")
endif

ifeq ($(USING_UMP),1)
ifeq ($(USE_UMPV2),1)
UMP_SYMVERS_FILE ?= ../umpv2/Module.symvers
else
UMP_SYMVERS_FILE ?= ../ump/Module.symvers
endif
KBUILD_EXTRA_SYMBOLS = $(realpath $(UMP_SYMVERS_FILE))
$(warning $(KBUILD_EXTRA_SYMBOLS))
endif

# Get any user defined KDIR-<names> or maybe even a hardcoded KDIR
-include KDIR_CONFIGURATION

# Define host system directory
KDIR-$(shell uname -m):=/lib/modules/$(shell uname -r)/build

# look up KDIR based om CPU selection
KDIR ?= $(KDIR-$(CPU))

# validate lookup result
ifeq ($(KDIR),)
$(error No KDIR found for platform $(CPU))
endif

# report detected/selected settings
ifdef ARM_INTERNAL_BUILD
$(warning Config $(CONFIG))
$(warning Host CPU $(CPU))
$(warning OS_MEMORY $(USING_OS_MEMORY))
endif

# Set up build config
export CONFIG_MALI400=m

ifeq ($(USING_GPU_UTILIZATION),1)
export EXTRA_DEFINES += -DCONFIG_MALI400_GPU_UTILIZATION=1
export CONFIG_MALI400_GPU_UTILIZATION := y
endif

build: $(UMP_SYMVERS_FILE)
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules
	@rm $(FILES_PREFIX)__malidrv_build_info.c $(FILES_PREFIX)__malidrv_build_info.o

install: build
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules_install

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean

kernelrelease:
	$(MAKE) -C $(KDIR) kernelrelease

export CONFIG KBUILD_EXTRA_SYMBOLS
