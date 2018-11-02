%define module_name mali
%define module_version 1.0
%define module_release alt1
%define flavour mcom02

%setup_kernel_module %flavour

%define udev_rules_dir /etc/udev/rules.d

Name: kernel-modules-%module_name-%flavour
Version: %module_version
Release: %module_release.%kcode.%kbuildrelease

Summary: Kernel drivers for MCom-02 Mali-300 GPU
License: GPLv2
Group: System/Kernel and hardware
ExclusiveArch: armh

Source: %name-%version.tar
URL: http://www.multicore.com

%description
Kernel part of Mali-300 GPU drivers that provides 3D acceleration on MCom-02 SoC

BuildRequires(pre): rpm-build-kernel
BuildRequires(pre): kernel-headers-modules-mcom02
BuildRequires: kernel-headers-modules-%flavour = %kversion-%krelease

Provides: kernel-modules-%module_name-%kversion-%flavour-%krelease = %version-%release
Conflicts: kernel-modules-%module_name-%kversion-%flavour-%krelease < %version-%release
Conflicts: kernel-modules-%module_name-%kversion-%flavour-%krelease > %version-%release

Requires: kernel-image-%flavour = %kversion-%krelease

%prep
%setup

%build
%make_build KDIR=%_usrsrc/linux-%kversion-%flavour

%install
%make_install KDIR=%_usrsrc/linux-%kversion-%flavour INSTALL_MOD_PATH=%buildroot install
mkdir -p %buildroot%udev_rules_dir
install -p -m644 udev/rules.d/61-mali.rules %buildroot%udev_rules_dir/61-mali.rules

%files
/lib/modules/%kversion-%flavour-alt%kbuildrelease/extra/*
%udev_rules_dir/61-mali.rules

%changelog
* Fri Nov 02 2018 RnD Center ELVEES <rnd_elvees@altlinux.org> 1.0-alt1
- Initial build for ALT
