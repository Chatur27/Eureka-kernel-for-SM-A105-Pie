/*
 * Based on arch/arm/include/asm/tlb.h
 *
 * Copyright (C) 2002 Russell King
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ASM_TLB_H
#define __ASM_TLB_H

#include <linux/pagemap.h>
#include <linux/swap.h>

#ifdef CONFIG_HAVE_RCU_TABLE_FREE

#define tlb_remove_entry(tlb, entry)	tlb_remove_table(tlb, entry)
static inline void __tlb_remove_table(void *_table)
{
	free_page_and_swap_cache((struct page *)_table);
}
#else
#define tlb_remove_entry(tlb, entry)	tlb_remove_page(tlb, entry)
#endif /* CONFIG_HAVE_RCU_TABLE_FREE */

#include <asm-generic/tlb.h>

static inline void tlb_flush(struct mmu_gather *tlb)
{
	struct vm_area_struct vma = { .vm_mm = tlb->mm, };

	/*
	 * The ASID allocator will either invalidate the ASID or mark
	 * it as used.
	 */
	if (tlb->fullmm)
		return;

	/*
	 * The intermediate page table levels are already handled by
	 * the __(pte|pmd|pud)_free_tlb() functions, so last level
	 * TLBI is sufficient here.
	 */
	__flush_tlb_range(&vma, tlb->start, tlb->end, true);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t pte,
				  unsigned long addr)
{
	__flush_tlb_pgtable(tlb->mm, addr);
	pgtable_page_dtor(pte);
	tlb_remove_entry(tlb, pte);
}

#if CONFIG_PGTABLE_LEVELS > 2
#ifndef CONFIG_UH_RKP
static inline void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmdp,
				  unsigned long addr)
{
	__flush_tlb_pgtable(tlb->mm, addr);
	tlb_remove_entry(tlb, virt_to_page(pmdp));
}
#else
static inline void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmdp,
				  unsigned long addr)
{
	int rkp_do = 0;
#ifdef CONFIG_KNOX_KAP
	if (boot_mode_security)
#endif	//CONFIG_KNOX_KAP
		rkp_do = 1;
	if (rkp_do && is_rkp_ro_page((unsigned long)pmdp)) {
		__flush_tlb_pgtable(tlb->mm, addr);
		rkp_ro_free((void*)pmdp);
	}
	else {
		__flush_tlb_pgtable(tlb->mm, addr);
		tlb_remove_entry(tlb, virt_to_page(pmdp));
	}
}
#endif
#endif

#if CONFIG_PGTABLE_LEVELS > 3
#ifndef CONFIG_UH_RKP
static inline void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pudp,
				  unsigned long addr)
{
	__flush_tlb_pgtable(tlb->mm, addr);
	tlb_remove_entry(tlb, virt_to_page(pudp));
}
#else
static inline void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pudp,
				  unsigned long addr)
{
	if (is_rkp_ro_page((unsigned long)pudp)) {
		__flush_tlb_pgtable(tlb->mm, addr);
		rkp_ro_free((void*)pudp);
	}
	else {
		__flush_tlb_pgtable(tlb->mm, addr);
		tlb_remove_entry(tlb, virt_to_page(pudp));
	}
}
#endif
#endif

#endif
