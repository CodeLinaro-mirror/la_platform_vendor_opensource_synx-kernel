/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __SYNX_INTERNAL_H__
#define __SYNX_INTERNAL_H__

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/debugfs.h>
#include <linux/dma-fence.h>
#include "synx_err.h"
#include "synx_api.h"
#include "synx_interop.h"

/* Forward declarations */
struct synx_interface_device;
struct synx_hwfence_interops;

extern struct synx_ops synx_hwfence_ops;
extern struct synx_ops synx_internal_ops;

/* synx handle encoding */
#define SYNX_HANDLE_INDEX_BITS         16
#define SYNX_HANDLE_CORE_BITS          4
#define SYNX_HANDLE_GLOBAL_FLAG_BIT    1

#define SYNX_MAX_OBJS               SYNX_GLOBAL_MAX_OBJS

#define SYNX_GLOBAL_MAX_OBJS           4096
#define SYNX_GLOBAL_MAX_PARENTS        4

#define SYNX_HANDLE_INDEX_MASK         ((1UL<<SYNX_HANDLE_INDEX_BITS)-1)

/* internal signal states */
#define SYNX_STATE_INVALID             0
#define SYNX_STATE_ACTIVE              1
#define SYNX_STATE_SIGNALED_ERROR      3
#define SYNX_STATE_SIGNALED_EXTERNAL   5
#define SYNX_STATE_SIGNALED_SSR        6
#define SYNX_STATE_TIMEOUT             7

/* Memory type definitions */
enum synx_queue_mem_type {
	SYNX_MEM_DEFAULT = 0x00,
	SYNX_MEM_MAX,
};


/* Interface device structure for synx-kernel module (user interface) */
struct synx_interface_device {
	struct cdev cdev;
	dev_t dev;
	struct class *class;
	struct dentry *debugfs_root;
};

/* Device name constants */
#define SYNX_NAME                   "synx"
#define SYNX_DEVICE_NAME            "synx_device"

#if IS_ENABLED(CONFIG_SYNX_IMPL)
// Function pointers for synx-impl implementations

int synx_internal_init_ops(struct synx_ops *ops);

struct synx_session *synx_internal_initialize(struct synx_initialization_params *params);

int synx_internal_recover(enum synx_client_id id);

void synx_util_default_user_callback(u32 h_synx,
	int status, void *data);

void synx_util_user_callback_v2(struct synx_callback_response *cb_response);

int synx_bind(struct synx_session *session,
	u32 h_synx,
	struct synx_external_desc_v2 external_sync);

// Function pointers for file operations
ssize_t synx_read(struct file *filep,
	char __user *buf, size_t size, loff_t *f_pos);

unsigned int synx_poll(struct file *filep,
	struct poll_table_struct *poll_table);

// Function declaration for synx interop operations
void synx_populate_shared_ops(struct synx_hwfence_interops *shared_ops);
void synx_set_hwfence_ops(struct synx_hwfence_interops *hw_ops);

// Wrapper functions that use function pointers if available, otherwise stubs
#else
// Pure stub implementations when CONFIG_SYNX_IMPL is not defined
static inline struct synx_session *synx_internal_initialize(
	struct synx_initialization_params *params)
{
	return ERR_PTR(-SYNX_INVALID);
}

static inline int synx_internal_recover(enum synx_client_id id)
{
	return -SYNX_INVALID;
}

static inline void synx_util_default_user_callback(u32 h_synx,
	int status, void *data)
{
}

static inline void synx_util_user_callback_v2(struct synx_callback_response *cb_response)
{
}

static inline int synx_bind(struct synx_session *session,
	u32 h_synx,
	struct synx_external_desc_v2 external_sync)
{
	return -SYNX_INVALID;
}

// Stub wrapper for read operation
static inline ssize_t synx_read(struct file *filep,
	char __user *buf, size_t size, loff_t *f_pos)
{
	return -SYNX_INVALID;
}

// Stub wrapper for poll operation
static inline unsigned int synx_poll(struct file *filep,
	struct poll_table_struct *poll_table)
{
	return -SYNX_INVALID;
}
#endif /* CONFIG_SYNX_IMPL */

int synx_open(struct inode *inode, struct file *filep);
int synx_close(struct inode *inode, struct file *filep);
void synx_dma_fence_callback(struct dma_fence *fence, struct dma_fence_cb *cb);
void synx_test_fence_release(struct dma_fence *fence);
#endif /* __SYNX_INTERNAL_H__ */
