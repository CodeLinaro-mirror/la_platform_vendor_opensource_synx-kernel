// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#include <linux/types.h>
#include "synx_api.h"
#include "synx_hwfence.h"
#include "synx_internal.h"
#include "synx_compat_debug.h"

struct synx_ops synx_hwfence_ops = {
	.uninitialize = NULL,
	.create = NULL,
	.release = NULL,
	.release_n = NULL,
	.signal = NULL,
	.signal_n = NULL,
	.async_wait = NULL,
	.async_wait_n = NULL,
	.get_fence = NULL,
	.import = NULL,
	.get_status = NULL,
	.merge = NULL,
	.merge_n = NULL,
	.wait = NULL,
	.cancel_async_wait = NULL,
	.cancel_async_wait_n = NULL
};

struct synx_ops synx_internal_ops = {
	.uninitialize = NULL,
	.create = NULL,
	.release = NULL,
	.release_n = NULL,
	.signal = NULL,
	.signal_n = NULL,
	.async_wait = NULL,
	.async_wait_n = NULL,
	.get_fence = NULL,
	.import = NULL,
	.get_status = NULL,
	.merge = NULL,
	.merge_n = NULL,
	.wait = NULL,
	.cancel_async_wait = NULL,
	.cancel_async_wait_n = NULL,
	.get = NULL
};

static bool is_hw_fence_client(enum synx_client_id synx_client_id)
{
	return synx_client_id >= SYNX_HW_FENCE_CLIENT_START
		&& synx_client_id < SYNX_HW_FENCE_CLIENT_END;
}

struct synx_session *synx_initialize(struct synx_initialization_params *params)
{
	struct synx_session *session = NULL;

	if (IS_ERR_OR_NULL(params)) {
		dprintk(SYNX_ERR, "invalid params\n");
		return ERR_PTR(-SYNX_INVALID);
	}

	if (is_hw_fence_client(params->id)) {
		session = synx_hwfence_initialize(params);
		if (IS_ERR_OR_NULL(session)) {
			dprintk(SYNX_ERR, "invalid session\n");
			return session;
		}
		session->ops = &synx_hwfence_ops;
	} else {
		session = synx_internal_initialize(params);
		if (IS_ERR_OR_NULL(session)) {
			dprintk(SYNX_ERR, "invalid session\n");
			return session;
		}
		session->ops = &synx_internal_ops;
	}
	return session;
}
EXPORT_SYMBOL(synx_initialize);

int synx_uninitialize(struct synx_session *session)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->uninitialize) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->uninitialize(session);
}
EXPORT_SYMBOL(synx_uninitialize);

int synx_create(struct synx_session *session, struct synx_create_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->create) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->create(session, params);
}
EXPORT_SYMBOL(synx_create);

int synx_release(struct synx_session *session, u32 h_synx)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->release) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->release(session, h_synx);
}
EXPORT_SYMBOL(synx_release);

int synx_release_n(struct synx_session *session, struct synx_release_n_params *pParams)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->release_n)
		return -SYNX_INVALID;
	return session->ops->release_n(session, pParams);
}
EXPORT_SYMBOL(synx_release_n);

int synx_signal(struct synx_session *session, u32 h_synx, enum synx_signal_status status)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->signal) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->signal(session, h_synx, status);
}
EXPORT_SYMBOL(synx_signal);

int synx_async_wait(struct synx_session *session, struct synx_callback_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->async_wait) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->async_wait(session, params);
}
EXPORT_SYMBOL(synx_async_wait);

int synx_async_wait_n(struct synx_session *session, struct synx_callback_n_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->async_wait_n) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->async_wait_n(session, params);
}
EXPORT_SYMBOL_GPL(synx_async_wait_n);

int synx_recover(enum synx_client_id id)
{
	int ret = 0;

	dprintk(SYNX_WARN, "Subsystem restart for core_id: %d\n", id);

	if (is_hw_fence_client(id))
		ret = synx_hwfence_recover(id);
	else
		ret = synx_internal_recover(id);
	return ret;
}
EXPORT_SYMBOL(synx_recover);

void *synx_get_fence(struct synx_session *session, u32 h_synx)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->get_fence) {
		dprintk(SYNX_ERR, "invalid session\n");
		return ERR_PTR(-SYNX_INVALID);
	}
	return session->ops->get_fence(session, h_synx);
}
EXPORT_SYMBOL(synx_get_fence);

int synx_import(struct synx_session *session, struct synx_import_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->import) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->import(session, params);
}
EXPORT_SYMBOL(synx_import);

int synx_get_status(struct synx_session *session, u32 h_synx)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->get_status) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->get_status(session, h_synx);
}
EXPORT_SYMBOL(synx_get_status);

int synx_merge(struct synx_session *session, struct synx_merge_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->merge) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->merge(session, params);
}
EXPORT_SYMBOL(synx_merge);

int synx_merge_n(struct synx_session *session, struct synx_merge_n_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->merge_n) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->merge_n(session, params);
}
EXPORT_SYMBOL_GPL(synx_merge_n);

int synx_wait(struct synx_session *session, u32 h_synx, u64 timeout_ms)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->wait) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->wait(session, h_synx, timeout_ms);
}
EXPORT_SYMBOL(synx_wait);

int synx_cancel_async_wait(struct synx_session *session,
	struct synx_callback_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->cancel_async_wait) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->cancel_async_wait(session, params);
}
EXPORT_SYMBOL(synx_cancel_async_wait);

int synx_cancel_async_wait_n(struct synx_session *session,
	struct synx_callback_n_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->cancel_async_wait_n) {
		dprintk(SYNX_ERR, "invalid session\n");
		return -SYNX_INVALID;
	}
	return session->ops->cancel_async_wait_n(session, params);
}
EXPORT_SYMBOL_GPL(synx_cancel_async_wait_n);

int synx_enable_resources(enum synx_client_id id, enum synx_resource_type resource, bool enable)
{
	int ret = 0;

	if (is_hw_fence_client(id))
		ret = synx_hwfence_enable_resources(id, resource, enable);
	else
		ret = SYNX_SUCCESS; /* no resources to enable for native synx clients */
	return ret;
}
EXPORT_SYMBOL_GPL(synx_enable_resources);

int synx_get(struct synx_session *session, struct synx_get_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->get) {
		dprintk(SYNX_ERR, "invalid session or operation or function pointer\n");
		return -SYNX_INVALID;
	}
	return session->ops->get(session, params);
}
EXPORT_SYMBOL_GPL(synx_get);

int synx_signal_n(struct synx_session *session, struct synx_signal_n_params *params)
{
	if (IS_ERR_OR_NULL(session) || !session->ops || !session->ops->signal_n) {
		dprintk(SYNX_ERR, "invalid session or operation or function pointer\n");
		return -SYNX_INVALID;
	}
	return session->ops->signal_n(session, params);
}
EXPORT_SYMBOL_GPL(synx_signal_n);

int synx_get_sys_info(enum synx_client_type type, struct synx_get_sys_info_params *params)
{
	int ret = SYNX_SUCCESS;
	u32 num_dwords, i;
	u32 hwfence_caps[SYNX_CAPABILITY_DWORDS] = {0};

	if (!params) {
		dprintk(SYNX_ERR, "invalid params\n");
		return -SYNX_INVALID;
	}

	if (params->type != SYNX_GET_CAPABILITY) {
		dprintk(SYNX_ERR, "unsupported sys info type: %d\n", params->type);
		return -SYNX_INVALID;
	}
	if (params->type == SYNX_GET_CAPABILITY) {
		if (!params->caps || !params->num_dwords) {
			dprintk(SYNX_ERR, "invalid capability params\n");
			return -SYNX_INVALID;
		}

		num_dwords = (u32)params->num_dwords;
		if (num_dwords > SYNX_CAPABILITY_DWORDS) {
			dprintk(SYNX_ERR, "num_dwords %u exceeds SYNX_CAPABILITY_DWORDS %u\n",
				num_dwords, SYNX_CAPABILITY_DWORDS);
			return -SYNX_INVALID;
		}

		switch (type) {
		case SYNX_CLIENT:
			ret = synx_internal_get_capability(params->caps, num_dwords, false);
			if (ret)
				dprintk(SYNX_ERR, "synx_internal_get_capability failed: %d\n", ret);
			break;

		case HW_FENCE_CLIENT:
			ret = synx_hwfence_get_capability(params->caps, num_dwords, false);
			if (ret)
				dprintk(SYNX_ERR, "synx_hwfence_get_capability failed: %d\n", ret);
			break;

		case SYNX_INTEROP_CLIENT: {
			/*
			 * Get combined capabilities (AND of synx and hw-fence).
			 * Only capabilities supported by BOTH implementations are returned.
			 * Each implementation clears its own interop-incompatible bits before
			 * the AND is applied.
			 */
			ret = synx_internal_get_capability(params->caps, num_dwords, true);
			if (ret) {
				dprintk(SYNX_ERR, "synx_internal_get_capability failed: %d\n", ret);
				break;
			}
			ret = synx_hwfence_get_capability(hwfence_caps, num_dwords, true);
			if (ret) {
				dprintk(SYNX_ERR, "synx_hwfence_get_capability failed: %d\n", ret);
				break;
			}
			for (i = 0; i < num_dwords; i++)
				params->caps[i] &= hwfence_caps[i];
			break;
		}

		default:
			dprintk(SYNX_ERR, "invalid client type: %d\n", type);
			ret = -SYNX_INVALID;
			break;
		}

		if (!ret) {
			for (i = 0; i < num_dwords; i++)
				dprintk(SYNX_DBG, "synx capability: caps[%u]=0x%08x\n",
					i, params->caps[i]);
		}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(synx_get_sys_info);
