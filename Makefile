# Copyright 2018 RnD Center "ELVEES", JSC

UMP_DRIVER_VERSION ?= r8p1-00rel0

export USING_UMP:=1
export UMP_SYMVERS_FILE:=$(CURDIR)/$(UMP_DRIVER_VERSION)/driver/src/devicedrv/ump/Module.symvers

TARGET_PLATFORM_MALI:=default
CONFIG_MALI:=mcom02-m300
BUILD_TYPE ?= release

build install clean: export CONFIG=$(CONFIG_MALI)
build install clean: export BUILD=$(BUILD_TYPE)
build install clean: export TARGET_PLATFORM=$(TARGET_PLATFORM_MALI)

build:
	$(MAKE) -C $(UMP_DRIVER_VERSION)/driver/src/devicedrv/ump
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/mali

install: build
	$(MAKE) -C $(UMP_DRIVER_VERSION)/driver/src/devicedrv/ump install
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/mali install

clean:
	$(MAKE) -C $(UMP_DRIVER_VERSION)/driver/src/devicedrv/ump clean
	$(MAKE) -C r3p0-04rel0/driver/src/devicedrv/mali clean
help:
	echo 'Variables:'
	echo '  KDIR - Path to Linux kernel source directory'
	echo '  UMP_DRIVER_VERSION - UMP driver version, r8p1-00rel0 by default'
	echo
	echo 'Goals:'
	echo
	echo '  build   - Build UMP and Mali kernel drivers'
	echo '  clean   - Clean UMP and Mali directories'
	echo '  install - Install UMP and Mali kernel drivers'
	echo '  help    - Show this help message'
	echo

.SILENT: help
.PHONY: build install clean help
