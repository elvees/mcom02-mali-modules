# Copyright 2018 RnD Center "ELVEES", JSC

export USING_UMP:=1

TARGET_PLATFORM_MALI:=default
CONFIG_MALI:=mcom02-m300
BUILD_TYPE ?= release

build: ump mali
clean: ump-clean mali-clean

mali ump: export CONFIG=$(CONFIG_MALI)
mali ump: export BUILD=$(BUILD_TYPE)
mali ump: export TARGET_PLATFORM=$(TARGET_PLATFORM_MALI)

ifeq ($(USING_UMP),1)
mali: ump
endif

mali:
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/mali

ump:
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/ump

ump-clean mali-clean: export CONFIG=$(CONFIG_MALI)
ump-clean mali-clean: export BUILD=$(BUILD_TYPE)
ump-clean mali-clean: export TARGET_PLATFORM=$(TARGET_PLATFORM_MALI)

ump-clean:
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/ump clean

mali-clean:
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/mali clean

help:
	echo 'Variables:'
	echo '  KDIR - Path to Linux kernel source directory'
	echo
	echo 'Goals:'
	echo
	echo '  build   - Build ump, mali'
	echo '  clean   - Clean ump, mali'
	echo '  help    - Show this help message'
	echo
	echo '  mali    - Build Mali kernel driver'
	echo '  ump     - Build Universal Memory Provider kernel driver'
	echo
	echo '  mali-clean   - Clean Mali kernel driver'
	echo '  ump-clean    - Clean Universal Memory Provider kernel driver'
	echo

.SILENT: help
