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

#define BASE_ADDR 0x37210000

/* Configuration for the Mcom-02 platform */

static _mali_osk_resource_t arch_configuration [] =
{
	{
		.type = MALI300GP,
		.description = "Mali-300 GP",
		.base = BASE_ADDR | 0x0000,
		.irq = -1,
		.irq_name = "IRQGP",
		.mmu_id = 1
	},
	{
		.type = MALI300PP,
		.description = "Mali-300 PP",
		.base = BASE_ADDR | 0x8000,
		.irq = -1,
		.irq_name = "IRQPP0",
		.mmu_id = 2
	},
	{
		.type = MMU,
		.base = BASE_ADDR | 0x3000,
		.irq = -1,
		.irq_name = "IRQGPMMU",
		.description = "Mali-300 MMU for GP",
		.mmu_id = 1
	},
	{
		.type = MMU,
		.base = BASE_ADDR | 0x4000,
		.irq = -1,
		.irq_name = "IRQPPMMU0",
		.description = "Mali-300 MMU for PP",
		.mmu_id = 2
	},
	{
		.type = OS_MEMORY,
		.description = "OS Memory",
		.alloc_order = 10, /* Lowest preference for this memory */
		.size = 128 * 1024 * 1024,
		.flags = _MALI_CPU_WRITEABLE | _MALI_CPU_READABLE | _MALI_PP_READABLE | _MALI_PP_WRITEABLE |_MALI_GP_READABLE | _MALI_GP_WRITEABLE
	},
	{
		.type = MEM_VALIDATION,
		.description = "Framebuffer",
		.base = 0x7f000000,
		.size = 0x01000000,
		.flags = _MALI_CPU_WRITEABLE | _MALI_CPU_READABLE | _MALI_PP_WRITEABLE | _MALI_PP_READABLE
	},
	{
		.type = MALI300L2,
		.base = BASE_ADDR | 0x1000,
		.description = "Mali-300 L2 cache"
	},
};

#endif /* __ARCH_CONFIG_H__ */
