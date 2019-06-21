/**
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_kernel_pm.c
 * Linux Power Management integration
 */

#include <linux/sched.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <asm/current.h>
#include <linux/suspend.h>
#include <linux/module.h>
#ifdef CONFIG_PM_RUNTIME
#include <linux/pm_runtime.h>
#endif
#include <linux/clk.h>
#include "mali_osk.h"
#include "mali_uk_types.h"
#include "mali_kernel_common.h"
#include "mali_kernel_license.h"
#include "mali_linux_pm.h"
#include "mali_pm.h"
#include "mali_platform.h"

#if ! MALI_LICENSE_IS_GPL
#undef CONFIG_PM_RUNTIME
#endif

static struct clk *mali_clk;
static struct platform_device *mali_pdev;

static int mali_probe(struct platform_device *pdev);
static int mali_remove(struct platform_device *pdev);

#ifdef CONFIG_PM_RUNTIME
static int mali_runtime_suspend(struct device *dev);
static int mali_runtime_resume(struct device *dev);
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))
static int mali_os_suspend(struct platform_device *pdev, pm_message_t state);
static int mali_os_resume(struct platform_device *pdev);
#else
static int mali_os_suspend(struct device *dev);
static int mali_os_resume(struct device *dev);
#endif


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
static const struct dev_pm_ops mali_dev_pm_ops =
{
#ifdef CONFIG_PM_RUNTIME
	.runtime_suspend = mali_runtime_suspend,
	.runtime_resume = mali_runtime_resume,
	.runtime_idle = NULL,
#else
	.suspend = mali_os_suspend,
	.resume = mali_os_resume,
#endif

	.freeze = mali_os_suspend,
	.poweroff = mali_os_suspend,
	.thaw = mali_os_resume,
	.restore = mali_os_resume,
};
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))
struct pm_ext_ops mali_ext_pm_operations =
{
	.base =
	{
		.freeze = mali_os_suspend,
		.thaw =   mali_os_resume,
		.poweroff = mali_os_suspend,
		.restore = mali_os_resume,
	},
};
#endif

static const struct of_device_id mali_dt_ids[] = {
	{ .compatible = "arm,mali-dev" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, mali_dt_ids);


static struct platform_driver mali_plat_driver =
{
	.probe  = mali_probe,
	.remove = mali_remove,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))
	.suspend = mali_os_suspend,
	.resume  = mali_os_resume,
	.pm = &mali_ext_pm_operations,
#endif

	.driver =
	{
		.name   = "mali_dev",
		.owner  = THIS_MODULE,
#if MALI_LICENSE_IS_GPL
		.bus = &platform_bus_type,
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
		.pm = &mali_dev_pm_ops,
#endif
		.of_match_table = mali_dt_ids,
	},
};

#ifdef CONFIG_PM_RUNTIME
static int mali_pwr_suspend_notifier(struct notifier_block *nb,unsigned long event,void* dummy);

static struct notifier_block mali_pwr_notif_block =
{
	.notifier_call = mali_pwr_suspend_notifier
};
#endif

/** This function is called when platform device is unregistered. This function
 * is necessary when the platform device is unregistered.
 */
static void _mali_release_pm(struct device *device)
{
}
struct platform_device mali_gpu_device =
{
	.name = "mali_dev",
	.id = 0,
	.dev.release = _mali_release_pm
};

/* this function is called when resources are initialized */
void mali_osk_resource_update(_mali_osk_resource_t *res, u32 num)
{
	int idx, irq;

	if (!mali_pdev)
		return;

	for (idx = 0; idx < num; idx++) {
		if (!res[idx].irq_name)
			continue;

		irq = platform_get_irq_byname(mali_pdev, res[idx].irq_name);

		if (irq > 0) {
			MALI_DEBUG_PRINT(2, ("Mali. IRQ found %s=%d\n",
					     res[idx].irq_name, irq));
			res[idx].irq = irq;
		}
	}
}

/** This function is called when the device is probed */
static int mali_probe(struct platform_device *pdev)
{
	struct resource *regs;
	int err;

	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	mali_pdev = pdev;

	/* Try to get and enable clock */
	mali_clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(mali_clk)) {
		err = PTR_ERR(mali_clk);
		dev_err(&pdev->dev,
			"failed to get clk (%u)\n", err);
		return err;
	}

	err = clk_prepare_enable(mali_clk);
	if (err) {
		dev_err(&pdev->dev,
			"failed to enable clk (%u)\n", err);
		return err;
	}

	return 0;
}

static int mali_remove(struct platform_device *pdev)
{
#ifdef CONFIG_PM_RUNTIME
	pm_runtime_disable(&pdev->dev);
#endif
	clk_disable_unprepare(mali_clk);

	mali_pdev = NULL;
	return 0;
}

#ifdef CONFIG_PM_RUNTIME
static int mali_pwr_suspend_notifier(struct notifier_block *nb,unsigned long event,void* dummy)
{
	switch (event)
	{
		case PM_SUSPEND_PREPARE:
			MALI_DEBUG_PRINT(2, ("mali_pwr_suspend_notifier(PM_SUSPEND_PREPARE) called\n"));
			mali_pm_os_suspend();
			break;
		case PM_POST_SUSPEND:
			MALI_DEBUG_PRINT(2, ("mali_pwr_suspend_notifier(PM_SUSPEND_PREPARE) called\n"));
			mali_pm_os_resume();
			break;
		default:
			break;
	}
	return 0;
}
#endif


#ifdef CONFIG_PM_RUNTIME

static int mali_runtime_suspend(struct device *dev)
{
	MALI_DEBUG_PRINT(3, ("mali_runtime_suspend() called\n"));
	mali_pm_runtime_suspend();
	return 0; /* all ok */
}

static int mali_runtime_resume(struct device *dev)
{
	MALI_DEBUG_PRINT(3, ("mali_runtime_resume() called\n"));
	mali_pm_runtime_resume();
	return 0; /* all ok */
}

#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))

static int mali_os_suspend(struct platform_device *pdev, pm_message_t state)
{
	MALI_DEBUG_PRINT(3, ("mali_os_suspend(old) called\n"));
	mali_pm_os_suspend();
	return 0; /* all ok */
}

static int mali_os_resume(struct platform_device *pdev)
{
	MALI_DEBUG_PRINT(3, ("mali_os_resume(old) called\n"));
	mali_pm_os_resume();
	return 0; /* all ok */
}

#else

static int mali_os_suspend(struct device *dev)
{
	MALI_DEBUG_PRINT(3, ("mali_os_suspend(new) called\n"));
	mali_pm_os_suspend();
	return 0; /* all ok */
}

static int mali_os_resume(struct device *dev)
{
	MALI_DEBUG_PRINT(3, ("mali_os_resume(new) called\n"));
	mali_pm_os_resume();
	return 0; /* all ok */
}

#endif

/** This function is called when Mali GPU device is initialized
 */
int _mali_dev_platform_register(void)
{
	int err;

#ifdef CONFIG_PM_RUNTIME
	set_mali_parent_power_domain((void *)&mali_gpu_device);
#endif

#ifdef CONFIG_PM_RUNTIME
	err = register_pm_notifier(&mali_pwr_notif_block);
	if (err)
	{
		return err;
	}
#endif

#if MALI_LICENSE_IS_GPL
	err = platform_driver_register(&mali_plat_driver);
#ifdef CONFIG_PM_RUNTIME
	if (err)
		unregister_pm_notifier(&mali_pwr_notif_block);
#endif
#endif

	return err;
}

/** This function is called when Mali GPU device is unloaded
 */
void _mali_dev_platform_unregister(void)
{
#ifdef CONFIG_PM_RUNTIME
	unregister_pm_notifier(&mali_pwr_notif_block);
#endif

#if MALI_LICENSE_IS_GPL
	platform_driver_unregister(&mali_plat_driver);
#endif
}
