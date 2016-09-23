/*
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_osk_mali.c
 * Implementation of the OS abstraction layer which is specific for the Mali kernel device driver
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <asm/uaccess.h>

#include "mali_kernel_common.h" /* MALI_xxx macros */
#include "mali_osk.h"           /* kernel side OS functions */
#include "mali_uk_types.h"
#include "arch/config.h"        /* contains the configuration of the arch we are compiling for */

static int mem_set(const char *val, const struct kernel_param *kp)
{
	unsigned long long m = memparse(val, NULL);

	if (m < SZ_16M || m > SZ_2G)
		return -EINVAL;

	*(u32 *)kp->arg = m;

	return 0;
}

static int mem_get(char *buffer, const struct kernel_param *kp)
{
	return scnprintf(buffer, PAGE_SIZE, "%u", *(u32 *)kp->arg);
}

static const struct kernel_param_ops param_mem_ops = {
	.set = mem_set,
	.get = mem_get
};

module_param_cb(mem, &param_mem_ops, &arch_configuration[4].size, S_IRUGO);
MODULE_PARM_DESC(mem, "Mali system memory size");

_mali_osk_errcode_t _mali_osk_resources_init( _mali_osk_resource_t **arch_config, u32 *num_resources )
{
    *num_resources = sizeof(arch_configuration) / sizeof(arch_configuration[0]);
    *arch_config = arch_configuration;
    return _MALI_OSK_ERR_OK;
}

void _mali_osk_resources_term( _mali_osk_resource_t **arch_config, u32 num_resources )
{
    /* Nothing to do */
}
