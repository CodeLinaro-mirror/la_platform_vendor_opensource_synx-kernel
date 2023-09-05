// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2021-2023, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <asm/memory.h>
#include <linux/sizes.h>

#include "ipclite_client.h"
#include "ipclite.h"

#undef IPCLITE_OS_LOG

int ipclite_hw_mutex_acquire(void)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_hw_mutex_acquire);

int ipclite_hw_mutex_release(void)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_hw_mutex_release);

/* Atomic Functions Start */
void ipclite_atomic_init_u32(ipclite_atomic_uint32_t *addr, uint32_t data)
{
	BUG_ON(addr == NULL);

	atomic_set(addr, data);
}
EXPORT_SYMBOL(ipclite_atomic_init_u32);

void ipclite_atomic_init_i32(ipclite_atomic_int32_t *addr, int32_t data)
{
	BUG_ON(addr == NULL);

	atomic_set(addr, data);
}
EXPORT_SYMBOL(ipclite_atomic_init_i32);

void ipclite_global_atomic_store_u32(ipclite_atomic_uint32_t *addr, uint32_t data)
{
	BUG_ON(addr == NULL);

	atomic_set(addr, data);
}
EXPORT_SYMBOL(ipclite_global_atomic_store_u32);

void ipclite_global_atomic_store_i32(ipclite_atomic_int32_t *addr, int32_t data)
{
	BUG_ON(addr == NULL);

	atomic_set(addr, data);
}
EXPORT_SYMBOL(ipclite_global_atomic_store_i32);

uint32_t ipclite_global_atomic_load_u32(ipclite_atomic_uint32_t *addr)
{
	uint32_t ret = 0;

	BUG_ON(addr == NULL);

	ret = atomic_read(addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_atomic_load_u32);

int32_t ipclite_global_atomic_load_i32(ipclite_atomic_int32_t *addr)
{
	int32_t ret = 0;

	BUG_ON(addr == NULL);

	ret = atomic_read(addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_atomic_load_i32);

uint32_t ipclite_global_test_and_set_bit(uint32_t nr, ipclite_atomic_uint32_t *addr)
{
	uint32_t ret = 0, mask = (1 << nr);

	BUG_ON(addr == NULL);

	ret = atomic_fetch_or(mask, addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_test_and_set_bit);

uint32_t ipclite_global_test_and_clear_bit(uint32_t nr, ipclite_atomic_uint32_t *addr)
{
	uint32_t ret = 0, mask = (1 << nr);

	BUG_ON(addr == NULL);

	ret = atomic_fetch_and(~mask, addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_test_and_clear_bit);

int32_t ipclite_global_atomic_inc(ipclite_atomic_int32_t *addr)
{
	int32_t ret = 0;

	BUG_ON(addr == NULL);

	ret = atomic_fetch_add(1, addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_atomic_inc);

int32_t ipclite_global_atomic_dec(ipclite_atomic_int32_t *addr)
{
	int32_t ret = 0;

	BUG_ON(addr == NULL);

	ret = atomic_fetch_sub(1, addr);

	return ret;
}
EXPORT_SYMBOL(ipclite_global_atomic_dec);
/* Atomic Functions End */

int ipclite_register_client(IPCLite_Client cb_func_ptr, void *priv)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_register_client);

int ipclite_register_test_client(IPCLite_Client cb_func_ptr, void *priv)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_register_test_client);

int ipclite_msg_send(int32_t proc_id, uint64_t data)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_msg_send);

int ipclite_test_msg_send(int32_t proc_id, uint64_t data)
{
	return 0;
}
EXPORT_SYMBOL(ipclite_test_msg_send);

void ipclite_recover(enum ipcmem_host_type core_id)
{
	return;
}
EXPORT_SYMBOL(ipclite_recover);

int32_t get_global_partition_info(struct global_region_info *global_ipcmem)
{
	if (!global_ipcmem)
		return -EINVAL;

	global_ipcmem->virt_base = kzalloc(GLOBAL_REGION_SIZE, GFP_ATOMIC);
	if (!global_ipcmem->virt_base)
		return -ENOMEM;
	global_ipcmem->size = (size_t)(GLOBAL_REGION_SIZE);

	return 0;
}
EXPORT_SYMBOL(get_global_partition_info);


MODULE_DESCRIPTION("IPCLite Driver");
MODULE_LICENSE("GPL v2");
