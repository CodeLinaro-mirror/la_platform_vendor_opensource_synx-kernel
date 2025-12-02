// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2019, 2021, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/io.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#include "synx_compat_debug.h"

#if IS_ENABLED(CONFIG_SYNX_IMPL)
extern int synx_debug;
#else
int synx_debug = SYNX_ERR | SYNX_WARN | SYNX_INFO;
#endif /* CONFIG_SYNX_IMPL */

struct dentry *synx_interface_init_debugfs_dir(struct synx_interface_device *dev)
{
	struct dentry *dir = NULL;

	dir = debugfs_create_dir("synx_debug", NULL);
	if (!dir) {
		dprintk(SYNX_ERR, "Failed to create debugfs for synx interface\n");
		return NULL;
	}

	debugfs_create_u32("debug_level", 0644, dir, &synx_debug);

	if (!debugfs_create_file("synx_test_ioctl",
		0644, dir, dev, &synx_test_fops)) {
		dprintk(SYNX_ERR, "Failed to create debugfs test ioctl file for synx\n");
		return NULL;
	}

	return dir;
}

void synx_interface_remove_debugfs_dir(struct synx_interface_device *dev)
{
	debugfs_remove_recursive(dev->debugfs_root);
}
