/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __SYNX_COMPAT_DEBUG_H__
#define __SYNX_COMPAT_DEBUG_H__

#include <linux/printk.h>
#include <linux/string.h>
#include <linux/stdarg.h>
#include "synx_internal.h"

/* Debug level definitions */
enum synx_debug_level {
	SYNX_ERR      = 0x0001,
	SYNX_WARN     = 0x0002,
	SYNX_INFO     = 0x0004,
	SYNX_DBG      = 0x0008,
	SYNX_VERB     = 0x0010,
	SYNX_IPCL     = 0x0020,
	SYNX_GSM      = 0x0040,
	SYNX_MEM      = 0x0080,
	SYNX_ALL      = SYNX_ERR | SYNX_WARN | SYNX_INFO |
					SYNX_DBG | SYNX_IPCL | SYNX_GSM  | SYNX_MEM,
	SYNX_TRACE    = 0x0100,
	SYNX_OUTPUT_KERNEL = 0x0200,
	SYNX_OUTPUT_TRACE  = 0x0400,
};

#ifndef SYNX_DBG_LABEL
#define SYNX_DBG_LABEL "synx"
#endif

#define SYNX_DBG_TAG SYNX_DBG_LABEL ": %4s: "

#define synx_ftrace_print(prefix, fmt, ...) do {                              \
	char _sfp_buf[512];                                                       \
	snprintf(_sfp_buf, sizeof(_sfp_buf), "%s: " fmt, prefix, ##__VA_ARGS__); \
	__trace_puts(_THIS_IP_, _sfp_buf, strlen(_sfp_buf));                      \
} while (0)

extern int synx_debug;

/* Used for accessing synx_test ioctl nodes*/
extern const struct file_operations synx_test_fops;

static inline char *synx_debug_str(int level)
{
	switch (level) {
	case SYNX_ERR:
		return "err";
	case SYNX_WARN:
		return "warn";
	case SYNX_INFO:
		return "info";
	case SYNX_DBG:
		return "dbg";
	case SYNX_VERB:
		return "verb";
	case SYNX_IPCL:
		return "ipcl";
	case SYNX_GSM:
		return "gmem";
	case SYNX_MEM:
		return "mem";
	default:
		return "???";
	}
}

static noinline __maybe_unused void tracing_mark_write(const char *fmt, ...)
{
	char buf[256];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	__trace_puts(_THIS_IP_, buf, strlen(buf));
}

#define dprintk(__level, __fmt, arg...)                                  \
	do {                                                                 \
		if (synx_debug & __level) {                                      \
			if (__level == SYNX_TRACE) {                                 \
				if (synx_debug & SYNX_OUTPUT_KERNEL) {                   \
					pr_info(__fmt, ##arg);                               \
				} else {                                                 \
					tracing_mark_write(__fmt, ##arg);               \
				}                                                        \
			} else {                                                     \
				if (synx_debug & SYNX_OUTPUT_TRACE) {                    \
					synx_ftrace_print(synx_debug_str(__level),           \
						__fmt, ##arg);                                   \
				} else {                                                 \
					pr_info(SYNX_DBG_TAG "%s: %d: " __fmt,               \
						synx_debug_str(__level),                         \
						__func__, __LINE__, ##arg);                      \
				}                                                        \
			}                                                            \
		}                                                                \
	} while (0)

/**
 * synx_interface_init_debugfs_dir - Initializes debugfs
 *
 * @param dev : Pointer to synx interface device structure
 */
struct dentry *synx_interface_init_debugfs_dir(struct synx_interface_device *dev);

/**
 * synx_interface_remove_debugfs_dir - Remove debugfs directory
 *
 * @param dev : Pointer to synx interface device structure
 */
void synx_interface_remove_debugfs_dir(struct synx_interface_device *dev);

#endif /* __SYNX_COMPAT_DEBUG_H__ */
