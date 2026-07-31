/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef __SYNX_INTEROP_API_H__
#define __SYNX_INTEROP_API_H__

#include "synx_api.h"
#include <linux/dma-fence.h>

/**
 * SYNX_CAP_WORD  - Get the u32 array index for a capability bit
 * SYNX_CAP_BIT   - Get the bit position within the u32 word for a capability
 * SYNX_CAP_SET   - Set a capability bit in the caps array
 *                  No-op if cap >= SYNX_CAP_MAX or dword index >= num_dwords
 * SYNX_CAP_CLEAR - Clear a capability bit in the caps array
 *                  No-op if cap >= SYNX_CAP_MAX or dword index >= num_dwords
 */
#define SYNX_CAP_WORD(cap)        ((cap) / 32)
#define SYNX_CAP_BIT(cap)         ((cap) % 32)
#define SYNX_CAP_SET(caps, num_dwords, cap) \
	do { \
		if ((cap) < SYNX_CAP_MAX && SYNX_CAP_WORD(cap) < (num_dwords)) \
			((caps)[SYNX_CAP_WORD(cap)] |= (1U << SYNX_CAP_BIT(cap))); \
	} while (0)
#define SYNX_CAP_CLEAR(caps, num_dwords, cap) \
	do { \
		if ((cap) < SYNX_CAP_MAX && SYNX_CAP_WORD(cap) < (num_dwords)) \
			((caps)[SYNX_CAP_WORD(cap)] &= ~(1U << SYNX_CAP_BIT(cap))); \
	} while (0)

/**
 * enum synx_core_id - Synx core IDs
 *
 * SYNX_CORE_APSS     : APSS core
 * SYNX_CORE_NSP      : NSP core
 * SYNX_CORE_EVA      : EVA core
 * SYNX_CORE_IRIS     : IRIS core
 * SYNX_CORE_ICP      : ICP core
 * SYNX_CORE_SOCCP    : SoCCP core
 * SYNX_CORE_ICP1     : OFE ICP core
 * SYNX_CORE_GMU      : GMU core
 * SYNX_CORE_ADSP     : ADSP (LPASS) core
 */
enum synx_core_id {
	SYNX_CORE_APSS = 0,
	SYNX_CORE_NSP,
	SYNX_CORE_EVA,
	SYNX_CORE_IRIS,
	SYNX_CORE_ICP,
	SYNX_CORE_SOCCP,
	SYNX_CORE_ICP1,
	SYNX_CORE_GMU,
	SYNX_CORE_ADSP,
	SYNX_CORE_MAX,
};

/**
 * SYNX_IMPORT_SKIP_SOCCP_SIGNAL : if set in import_flags, then skip the
 * signal via soccp path.
 * Note: this is set as maximum bit value to avoid overlapping with
 * new import flags. And restricted only to GFX synx client.
 */
#define SYNX_IMPORT_SKIP_SOCCP_SIGNAL  0x80000000

/**
 * SYNX_NATIVE_FENCE_FLAG_ENABLED_BIT - synx-native fence is enabled for the dma_fence
 *
 * This flag is set in dma-fence flags when fences are backed by a native synx fence.
 */
#define SYNX_NATIVE_FENCE_FLAG_ENABLED_BIT 29

/**
 * SYNX_HW_FENCE_HANDLE_FLAG - synx hw-fence is enabled for this handle
 *
 * This bit is set to one in hw-fence handles and set to zero in synx-native handles.
 */
#define SYNX_HW_FENCE_HANDLE_FLAG BIT(31)

/**
 * struct synx_hwfence_interops - internal ops used by hw-fence and synx drivers
 *
 * @share_handle_status: provide caller driver's handle and receive callee driver's handle
 *                       and signaling status
 * @get_fence: gets native fence backing handle of other driver
 * @notify_recover: performs recovery for given synx core
 * @signal_fence: signal h_synx from hlos on behalf of given synx core
 * @dma_add_cb_no_enable_sig: adds callback without calling enable_signaling
 */
struct synx_hwfence_interops {
	int (*share_handle_status)(struct synx_import_indv_params *params, u32 h_caller,
		u32 *signal_status);
	void *(*get_fence)(u32 h_callee);
	int (*notify_recover)(enum synx_core_id id);
	int (*signal_fence)(enum synx_core_id id, bool is_core_ssr, u32 h_synx,
		enum synx_signal_status status);
	int (*dma_add_cb_no_enable_sig)(struct dma_fence *fence,
		struct dma_fence_cb *cb, dma_fence_func_t func);
};

#if IS_ENABLED(CONFIG_QTI_HW_FENCE)
/**
 * synx_hwfence_init_interops - Get/Set Synx callback interops
 *
 * Function receives the internal callback functions for hw-fence driver
 * to call into synx native, and returns internal callback functions for
 * synx native to call into hw-fence driver.
 *
 * @param synx_ops    : pointer to callback functions that HW Fence Driver
 *                      will call into synx-native
 * @param hwfence_ops : pointer to callback functions (filled by the function)
 *                      with the function pointers that synx native will call
 *
 * @return Negative in case of error. SYNX_SUCCESS otherwise.
 */
int synx_hwfence_init_interops(struct synx_hwfence_interops *synx_ops,
	struct synx_hwfence_interops *hwfence_ops);
#else

static inline int synx_hwfence_init_interops(struct synx_hwfence_interops *synx_ops,
	struct synx_hwfence_interops *hwfence_ops)
{
	return -SYNX_INVALID;
}
#endif /* CONFIG_QTI_HW_FENCE */
#endif /* __SYNX_INTEROP_API_H__ */
