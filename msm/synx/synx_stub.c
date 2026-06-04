// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include "synx_api.h"
#include "synx_hwfence.h"
#include "synx_internal.h"
#include "synx_compat_debug.h"
#include "synx_ioctl.h"
#include "synx_interop.h"


struct synx_interface_device *synx_dev;

struct synx_hwfence_interops hwfence_shared_ops = { NULL };
struct synx_hwfence_interops synx_shared_ops = { NULL };

static const struct file_operations synx_fops = {
	.owner = THIS_MODULE,
	.open  = synx_open,
	.read  = synx_read,
	.release = synx_close,
	.poll  = synx_poll,
	.write = synx_write,
	.unlocked_ioctl = synx_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = synx_ioctl,
#endif
};

#if IS_ENABLED(CONFIG_QTI_HW_FENCE)
int synx_hwfence_get_capability(u32 *caps, u32 num_dwords, bool is_interop)
{
	u32 i;

	if (IS_ERR_OR_NULL(caps) || num_dwords == 0)
		return -SYNX_INVALID;

	memset(caps, 0, num_dwords * sizeof(u32));

	for (i = 0; i < ARRAY_SIZE(synx_hwfence_caps); i++)
		SYNX_CAP_SET(caps, num_dwords, synx_hwfence_caps[i]);

	if (is_interop) {
		for (i = 0; i < ARRAY_SIZE(synx_hwfence_interop_clear); i++)
			SYNX_CAP_CLEAR(caps, num_dwords, synx_hwfence_interop_clear[i]);
	}

	return SYNX_SUCCESS;
}
#endif /* CONFIG_QTI_HW_FENCE */

static int __init synx_init(void)
{
	int rc;

	synx_dev = kzalloc(sizeof(*synx_dev), GFP_KERNEL);
	if (IS_ERR_OR_NULL(synx_dev)) {
		dprintk(SYNX_ERR, "synx_dev allocation failed\n");
		return -SYNX_NOMEM;
	}

	rc = alloc_chrdev_region(&synx_dev->dev, 0, 1, SYNX_DEVICE_NAME);
	if (rc < 0) {
		dprintk(SYNX_ERR, "alloc_chrdev_region failed, err=%d\n", rc);
		goto alloc_fail;
	}

	cdev_init(&synx_dev->cdev, &synx_fops);
	synx_dev->cdev.owner = THIS_MODULE;

	rc = cdev_add(&synx_dev->cdev, synx_dev->dev, 1);
	if (rc < 0) {
		dprintk(SYNX_ERR, "cdev_add failed, err=%d\n", rc);
		goto reg_fail;
	}

#if (KERNEL_VERSION(6, 4, 0) <= LINUX_VERSION_CODE)
	synx_dev->class = class_create(SYNX_DEVICE_NAME);
#else
	synx_dev->class = class_create(THIS_MODULE, SYNX_DEVICE_NAME);
#endif
	if (IS_ERR(synx_dev->class)) {
		rc = PTR_ERR(synx_dev->class);
		dprintk(SYNX_ERR, "class_create failed, err=%d\n", rc);
		goto err_class_create;
	}

	if (IS_ERR(device_create(synx_dev->class, NULL, synx_dev->dev,
				 NULL, SYNX_DEVICE_NAME))) {
		rc = -EINVAL;
		dprintk(SYNX_ERR, "device_create failed, err=%d\n", rc);
		goto err_device_create;
	}

#if IS_ENABLED(CONFIG_DEBUG_FS)
	synx_dev->debugfs_root = synx_interface_init_debugfs_dir(synx_dev);
#endif/* CONFIG_DEBUG_FS */
#if IS_ENABLED(CONFIG_QTI_HW_FENCE)
	rc = synx_hwfence_init_ops(&synx_hwfence_ops);
	if (rc)
		dprintk(SYNX_WARN, "synx-hwfence init failed, err=%d\n", rc);
#endif /* CONFIG_QTI_HW_FENCE */

#if IS_ENABLED(CONFIG_SYNX_IMPL)
	rc = synx_internal_init_ops(&synx_internal_ops);
	if (rc)
		dprintk(SYNX_WARN, "synx_impl driver init ops failed, err=%d\n", rc);

	/* Populate synx_shared_ops from synx_core if available */
	synx_populate_shared_ops(&synx_shared_ops);
	rc = synx_hwfence_init_interops(&synx_shared_ops, &hwfence_shared_ops);
	if (rc) {
		dprintk(SYNX_ERR, "Hw fence inter-op mapping failed, err %d\n", rc);
	} else {
		/* Pass hwfence operations to synx_core for compatibility */
		synx_set_hwfence_ops(&hwfence_shared_ops);
	}
#endif /* CONFIG_SYNX_IMPL */
	dprintk(SYNX_INFO, "synx interface module loaded\n");

	return 0;

err_device_create:
	class_destroy(synx_dev->class);
err_class_create:
	cdev_del(&synx_dev->cdev);
reg_fail:
	unregister_chrdev_region(synx_dev->dev, 1);
alloc_fail:
	kfree(synx_dev);
	synx_dev = NULL;
	return rc;
}

static void __exit synx_exit(void)
{
#if IS_ENABLED(CONFIG_DEBUG_FS)
	synx_interface_remove_debugfs_dir(synx_dev);
#endif /* CONFIG_DEBUG_FS */
	if (!IS_ERR_OR_NULL(synx_dev)) {
		device_destroy(synx_dev->class, synx_dev->dev);
		class_destroy(synx_dev->class);
		cdev_del(&synx_dev->cdev);
		unregister_chrdev_region(synx_dev->dev, 1);
		kfree(synx_dev);
		synx_dev = NULL;
	}
	dprintk(SYNX_INFO, "synx interface module unloaded\n");
}

module_init(synx_init);
module_exit(synx_exit);
MODULE_DESCRIPTION("Global Synx Driver");
MODULE_LICENSE("GPL");
