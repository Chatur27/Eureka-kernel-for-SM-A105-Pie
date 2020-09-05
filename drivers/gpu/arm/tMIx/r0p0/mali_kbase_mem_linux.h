/*
 *
 * (C) COPYRIGHT 2010, 2012-2016 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */





/**
 * @file mali_kbase_mem_linux.h
 * Base kernel memory APIs, Linux implementation.
 */

#ifndef _KBASE_MEM_LINUX_H_
#define _KBASE_MEM_LINUX_H_

/** A HWC dump mapping */
struct kbase_hwc_dma_mapping {
	void       *cpu_va;
	dma_addr_t  dma_pa;
	size_t      size;
};
	/* MALI_SEC_INTEGRATION */
struct kbase_mem_phy_alloc;

struct kbase_va_region *kbase_mem_alloc(struct kbase_context *kctx, u64 va_pages, u64 commit_pages, u64 extent, u64 *flags, u64 *gpu_va, u16 *va_alignment);
int kbase_mem_query(struct kbase_context *kctx, u64 gpu_addr, int query, u64 *const pages);
int kbase_mem_import(struct kbase_context *kctx, enum base_mem_import_type type,
		void __user *phandle, u64 *gpu_va, u64 *va_pages,
		u64 *flags);
u64 kbase_mem_alias(struct kbase_context *kctx, u64 *flags, u64 stride, u64 nents, struct base_mem_aliasing_info *ai, u64 *num_pages);
int kbase_mem_flags_change(struct kbase_context *kctx, u64 gpu_addr, unsigned int flags, unsigned int mask);
int kbase_mem_commit(struct kbase_context *kctx, u64 gpu_addr, u64 new_pages, enum base_backing_threshold_status *failure_reason);
int kbase_mmap(struct file *file, struct vm_area_struct *vma);

/**
 * kbase_mem_evictable_init - Initialize the Ephemeral memory the eviction
 * mechanism.
 * @kctx: The kbase context to initialize.
 *
 * Return: Zero on success or -errno on failure.
 */
int kbase_mem_evictable_init(struct kbase_context *kctx);

/**
 * kbase_mem_evictable_deinit - De-initialize the Ephemeral memory eviction
 * mechanism.
 * @kctx: The kbase context to de-initialize.
 */
void kbase_mem_evictable_deinit(struct kbase_context *kctx);

/**
 * kbase_mem_grow_gpu_mapping - Grow the GPU mapping of an allocation
 * @kctx:      Context the region belongs to
 * @reg:       The GPU region
 * @new_pages: The number of pages after the grow
 * @old_pages: The number of pages before the grow
 *
 * Return: 0 on success, -errno on error.
 *
 * Expand the GPU mapping to encompass the new psychical pages which have
 * been added to the allocation.
 *
 * Note: Caller must be holding the region lock.
 */
int kbase_mem_grow_gpu_mapping(struct kbase_context *kctx,
		struct kbase_va_region *reg,
		u64 new_pages, u64 old_pages);

/**
 * kbase_mem_evictable_make - Make a physical allocation eligible for eviction
 * @gpu_alloc: The physical allocation to make evictable
 *
 * Return: 0 on success, -errno on error.
 *
 * Take the provided region and make all the physical pages within it
 * reclaimable by the kernel, updating the per-process VM stats as well.
 * Remove any CPU mappings (as these can't be removed in the shrinker callback
 * as mmap_sem might already be taken) but leave the GPU mapping intact as
 * and until the shrinker reclaims the allocation.
 *
 * Note: Must be called with the region lock of the containing context.
 */
int kbase_mem_evictable_make(struct kbase_mem_phy_alloc *gpu_alloc);

/**
 * kbase_mem_evictable_unmake - Remove a physical allocations eligibility for
 * eviction.
 * @alloc: The physical allocation to remove eviction eligibility from.
 *
 * Return: True if the allocation had its backing restored and false if
 * it hasn't.
 *
 * Make the physical pages in the region no longer reclaimable and update the
 * per-process stats, if the shrinker has already evicted the memory then
 * re-allocate it if the region is still alive.
 *
 * Note: Must be called with the region lock of the containing context.
 */
bool kbase_mem_evictable_unmake(struct kbase_mem_phy_alloc *alloc);

struct kbase_vmap_struct {
	u64 gpu_addr;
	struct kbase_mem_phy_alloc *cpu_alloc;
	struct kbase_mem_phy_alloc *gpu_alloc;
	phys_addr_t *cpu_pages;
	phys_addr_t *gpu_pages;
	void *addr;
	size_t size;
	bool is_cached;
};
void *kbase_vmap(struct kbase_context *kctx, u64 gpu_addr, size_t size,
		struct kbase_vmap_struct *map);
void kbase_vunmap(struct kbase_context *kctx, struct kbase_vmap_struct *map);

/** @brief Allocate memory from kernel space and map it onto the GPU
 *
 * @param kctx   The context used for the allocation/mapping
 * @param size   The size of the allocation in bytes
 * @param handle An opaque structure used to contain the state needed to free the memory
 * @return the VA for kernel space and GPU MMU
 */
void *kbase_va_alloc(struct kbase_context *kctx, u32 size, struct kbase_hwc_dma_mapping *handle);

/** @brief Free/unmap memory allocated by kbase_va_alloc
 *
 * @param kctx   The context used for the allocation/mapping
 * @param handle An opaque structure returned by the kbase_va_alloc function.
 */
void kbase_va_free(struct kbase_context *kctx, struct kbase_hwc_dma_mapping *handle);

#endif				/* _KBASE_MEM_LINUX_H_ */
