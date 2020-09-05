/*
 *
 * (C) COPYRIGHT 2016-2018 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 */

/*
 * This header was autogenerated, it should not be edited.
 */

#ifndef _KBASE_GATOR_HWCNT_NAMES_TKAX_H_
#define _KBASE_GATOR_HWCNT_NAMES_TKAX_H_

static const char * const hardware_counters_mali_tKAx[] = {
	/* Performance counters for the Job Manager */
	"",
	"",
	"",
	"",
	"TKAx_MESSAGES_SENT",
	"TKAx_MESSAGES_RECEIVED",
	"TKAx_GPU_ACTIVE",
	"TKAx_IRQ_ACTIVE",
	"TKAx_JS0_JOBS",
	"TKAx_JS0_TASKS",
	"TKAx_JS0_ACTIVE",
	"",
	"TKAx_JS0_WAIT_READ",
	"TKAx_JS0_WAIT_ISSUE",
	"TKAx_JS0_WAIT_DEPEND",
	"TKAx_JS0_WAIT_FINISH",
	"TKAx_JS1_JOBS",
	"TKAx_JS1_TASKS",
	"TKAx_JS1_ACTIVE",
	"",
	"TKAx_JS1_WAIT_READ",
	"TKAx_JS1_WAIT_ISSUE",
	"TKAx_JS1_WAIT_DEPEND",
	"TKAx_JS1_WAIT_FINISH",
	"TKAx_JS2_JOBS",
	"TKAx_JS2_TASKS",
	"TKAx_JS2_ACTIVE",
	"",
	"TKAx_JS2_WAIT_READ",
	"TKAx_JS2_WAIT_ISSUE",
	"TKAx_JS2_WAIT_DEPEND",
	"TKAx_JS2_WAIT_FINISH",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	/* Performance counters for the Tiler */
	"",
	"",
	"",
	"",
	"TKAx_TILER_ACTIVE",
	"TKAx_JOBS_PROCESSED",
	"TKAx_TRIANGLES",
	"TKAx_LINES",
	"TKAx_POINTS",
	"TKAx_FRONT_FACING",
	"TKAx_BACK_FACING",
	"TKAx_PRIM_VISIBLE",
	"TKAx_PRIM_CULLED",
	"TKAx_PRIM_CLIPPED",
	"TKAx_PRIM_SAT_CULLED",
	"TKAx_BIN_ALLOC_INIT",
	"TKAx_BIN_ALLOC_OVERFLOW",
	"TKAx_BUS_READ",
	"",
	"TKAx_BUS_WRITE",
	"TKAx_LOADING_DESC",
	"TKAx_IDVS_POS_SHAD_REQ",
	"TKAx_IDVS_POS_SHAD_WAIT",
	"TKAx_IDVS_POS_SHAD_STALL",
	"TKAx_IDVS_POS_FIFO_FULL",
	"TKAx_PREFETCH_STALL",
	"TKAx_VCACHE_HIT",
	"TKAx_VCACHE_MISS",
	"TKAx_VCACHE_LINE_WAIT",
	"TKAx_VFETCH_POS_READ_WAIT",
	"TKAx_VFETCH_VERTEX_WAIT",
	"TKAx_VFETCH_STALL",
	"TKAx_PRIMASSY_STALL",
	"TKAx_BBOX_GEN_STALL",
	"TKAx_IDVS_VBU_HIT",
	"TKAx_IDVS_VBU_MISS",
	"TKAx_IDVS_VBU_LINE_DEALLOCATE",
	"TKAx_IDVS_VAR_SHAD_REQ",
	"TKAx_IDVS_VAR_SHAD_STALL",
	"TKAx_BINNER_STALL",
	"TKAx_ITER_STALL",
	"TKAx_COMPRESS_MISS",
	"TKAx_COMPRESS_STALL",
	"TKAx_PCACHE_HIT",
	"TKAx_PCACHE_MISS",
	"TKAx_PCACHE_MISS_STALL",
	"TKAx_PCACHE_EVICT_STALL",
	"TKAx_PMGR_PTR_WR_STALL",
	"TKAx_PMGR_PTR_RD_STALL",
	"TKAx_PMGR_CMD_WR_STALL",
	"TKAx_WRBUF_ACTIVE",
	"TKAx_WRBUF_HIT",
	"TKAx_WRBUF_MISS",
	"TKAx_WRBUF_NO_FREE_LINE_STALL",
	"TKAx_WRBUF_NO_AXI_ID_STALL",
	"TKAx_WRBUF_AXI_STALL",
	"",
	"",
	"",
	"TKAx_UTLB_TRANS",
	"TKAx_UTLB_TRANS_HIT",
	"TKAx_UTLB_TRANS_STALL",
	"TKAx_UTLB_TRANS_MISS_DELAY",
	"TKAx_UTLB_MMU_REQ",

	/* Performance counters for the Shader Core */
	"",
	"",
	"",
	"",
	"TKAx_FRAG_ACTIVE",
	"TKAx_FRAG_PRIMITIVES",
	"TKAx_FRAG_PRIM_RAST",
	"TKAx_FRAG_FPK_ACTIVE",
	"TKAx_FRAG_STARVING",
	"TKAx_FRAG_WARPS",
	"TKAx_FRAG_PARTIAL_WARPS",
	"TKAx_FRAG_QUADS_RAST",
	"TKAx_FRAG_QUADS_EZS_TEST",
	"TKAx_FRAG_QUADS_EZS_UPDATE",
	"TKAx_FRAG_QUADS_EZS_KILL",
	"TKAx_FRAG_LZS_TEST",
	"TKAx_FRAG_LZS_KILL",
	"TKAx_WARP_REG_SIZE_64",
	"TKAx_FRAG_PTILES",
	"TKAx_FRAG_TRANS_ELIM",
	"TKAx_QUAD_FPK_KILLER",
	"TKAx_FULL_QUAD_WARPS",
	"TKAx_COMPUTE_ACTIVE",
	"TKAx_COMPUTE_TASKS",
	"TKAx_COMPUTE_WARPS",
	"TKAx_COMPUTE_STARVING",
	"TKAx_EXEC_CORE_ACTIVE",
	"TKAx_EXEC_ACTIVE",
	"TKAx_EXEC_INSTR_COUNT",
	"TKAx_EXEC_INSTR_DIVERGED",
	"TKAx_EXEC_INSTR_STARVING",
	"TKAx_ARITH_INSTR_SINGLE_FMA",
	"TKAx_ARITH_INSTR_DOUBLE",
	"TKAx_ARITH_INSTR_MSG",
	"TKAx_ARITH_INSTR_MSG_ONLY",
	"TKAx_TEX_MSGI_NUM_QUADS",
	"TKAx_TEX_DFCH_NUM_PASSES",
	"TKAx_TEX_DFCH_NUM_PASSES_MISS",
	"TKAx_TEX_DFCH_NUM_PASSES_MIP_MAP",
	"TKAx_TEX_TIDX_NUM_SPLIT_MIP_MAP",
	"TKAx_TEX_TFCH_NUM_LINES_FETCHED",
	"TKAx_TEX_TFCH_NUM_LINES_FETCHED_BLOCK",
	"TKAx_TEX_TFCH_NUM_OPERATIONS",
	"TKAx_TEX_FILT_NUM_OPERATIONS",
	"TKAx_LS_MEM_READ_FULL",
	"TKAx_LS_MEM_READ_SHORT",
	"TKAx_LS_MEM_WRITE_FULL",
	"TKAx_LS_MEM_WRITE_SHORT",
	"TKAx_LS_MEM_ATOMIC",
	"TKAx_VARY_INSTR",
	"TKAx_VARY_SLOT_32",
	"TKAx_VARY_SLOT_16",
	"TKAx_ATTR_INSTR",
	"TKAx_ARITH_INSTR_FP_MUL",
	"TKAx_BEATS_RD_FTC",
	"TKAx_BEATS_RD_FTC_EXT",
	"TKAx_BEATS_RD_LSC",
	"TKAx_BEATS_RD_LSC_EXT",
	"TKAx_BEATS_RD_TEX",
	"TKAx_BEATS_RD_TEX_EXT",
	"TKAx_BEATS_RD_OTHER",
	"TKAx_BEATS_WR_LSC_OTHER",
	"TKAx_BEATS_WR_TIB",
	"TKAx_BEATS_WR_LSC_WB",

	/* Performance counters for the Memory System */
	"",
	"",
	"",
	"",
	"TKAx_MMU_REQUESTS",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"TKAx_L2_RD_MSG_IN",
	"TKAx_L2_RD_MSG_IN_STALL",
	"TKAx_L2_WR_MSG_IN",
	"TKAx_L2_WR_MSG_IN_STALL",
	"TKAx_L2_SNP_MSG_IN",
	"TKAx_L2_SNP_MSG_IN_STALL",
	"TKAx_L2_RD_MSG_OUT",
	"TKAx_L2_RD_MSG_OUT_STALL",
	"TKAx_L2_WR_MSG_OUT",
	"TKAx_L2_ANY_LOOKUP",
	"TKAx_L2_READ_LOOKUP",
	"TKAx_L2_WRITE_LOOKUP",
	"TKAx_L2_EXT_SNOOP_LOOKUP",
	"TKAx_L2_EXT_READ",
	"TKAx_L2_EXT_READ_NOSNP",
	"TKAx_L2_EXT_READ_UNIQUE",
	"TKAx_L2_EXT_READ_BEATS",
	"TKAx_L2_EXT_AR_STALL",
	"TKAx_L2_EXT_AR_CNT_Q1",
	"TKAx_L2_EXT_AR_CNT_Q2",
	"TKAx_L2_EXT_AR_CNT_Q3",
	"TKAx_L2_EXT_RRESP_0_127",
	"TKAx_L2_EXT_RRESP_128_191",
	"TKAx_L2_EXT_RRESP_192_255",
	"TKAx_L2_EXT_RRESP_256_319",
	"TKAx_L2_EXT_RRESP_320_383",
	"TKAx_L2_EXT_WRITE",
	"TKAx_L2_EXT_WRITE_NOSNP_FULL",
	"TKAx_L2_EXT_WRITE_NOSNP_PTL",
	"TKAx_L2_EXT_WRITE_SNP_FULL",
	"TKAx_L2_EXT_WRITE_SNP_PTL",
	"TKAx_L2_EXT_WRITE_BEATS",
	"TKAx_L2_EXT_W_STALL",
	"TKAx_L2_EXT_AW_CNT_Q1",
	"TKAx_L2_EXT_AW_CNT_Q2",
	"TKAx_L2_EXT_AW_CNT_Q3",
	"TKAx_L2_EXT_SNOOP",
	"TKAx_L2_EXT_SNOOP_STALL",
	"TKAx_L2_EXT_SNOOP_RESP_CLEAN",
	"TKAx_L2_EXT_SNOOP_RESP_DATA",
	"TKAx_L2_EXT_SNOOP_INTERNAL",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
};

#endif /* _KBASE_GATOR_HWCNT_NAMES_TKAX_H_ */
