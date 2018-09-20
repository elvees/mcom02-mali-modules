/*
 * Copyright 2015-2018 RnD Center "ELVEES", JSC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

#ifndef USING_MEMORY
#define USING_MEMORY	1
#endif

#define ARCH_UMP_BACKEND_DEFAULT		USING_MEMORY
#if (USING_MEMORY == 0) /* Dedicated Memory */
	#define ARCH_UMP_MEMORY_ADDRESS_DEFAULT		0x78000000
#else
	#define ARCH_UMP_MEMORY_ADDRESS_DEFAULT		0
#endif
#define ARCH_UMP_MEMORY_SIZE_DEFAULT (256UL * 1024UL * 1024UL)

#endif /* __ARCH_CONFIG_H__ */
