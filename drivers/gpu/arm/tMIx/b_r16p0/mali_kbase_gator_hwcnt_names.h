/*
 *
 * (C) COPYRIGHT 2014-2017 ARM Limited. All rights reserved.
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

#ifndef _KBASE_GATOR_HWCNT_NAMES_H_
#define _KBASE_GATOR_HWCNT_NAMES_H_

/*
 * "Short names" for hardware counters used by Streamline. Counters names are
 * stored in accordance with their memory layout in the binary counter block
 * emitted by the Mali GPU. Each "master" in the GPU emits a fixed-size block
 * of 64 counters, and each GPU implements the same set of "masters" although
 * the counters each master exposes within its block of 64 may vary.
 *
 * Counters which are an empty string are simply "holes" in the counter memory
 * where no counter exists.
 */

static const char * const hardware_counters_mali_t60x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T60x_MESSAGES_SENT",
	"T60x_MESSAGES_RECEIVED",
	"T60x_GPU_ACTIVE",
	"T60x_IRQ_ACTIVE",
	"T60x_JS0_JOBS",
	"T60x_JS0_TASKS",
	"T60x_JS0_ACTIVE",
	"",
	"T60x_JS0_WAIT_READ",
	"T60x_JS0_WAIT_ISSUE",
	"T60x_JS0_WAIT_DEPEND",
	"T60x_JS0_WAIT_FINISH",
	"T60x_JS1_JOBS",
	"T60x_JS1_TASKS",
	"T60x_JS1_ACTIVE",
	"",
	"T60x_JS1_WAIT_READ",
	"T60x_JS1_WAIT_ISSUE",
	"T60x_JS1_WAIT_DEPEND",
	"T60x_JS1_WAIT_FINISH",
	"T60x_JS2_JOBS",
	"T60x_JS2_TASKS",
	"T60x_JS2_ACTIVE",
	"",
	"T60x_JS2_WAIT_READ",
	"T60x_JS2_WAIT_ISSUE",
	"T60x_JS2_WAIT_DEPEND",
	"T60x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T60x_TI_JOBS_PROCESSED",
	"T60x_TI_TRIANGLES",
	"T60x_TI_QUADS",
	"T60x_TI_POLYGONS",
	"T60x_TI_POINTS",
	"T60x_TI_LINES",
	"T60x_TI_VCACHE_HIT",
	"T60x_TI_VCACHE_MISS",
	"T60x_TI_FRONT_FACING",
	"T60x_TI_BACK_FACING",
	"T60x_TI_PRIM_VISIBLE",
	"T60x_TI_PRIM_CULLED",
	"T60x_TI_PRIM_CLIPPED",
	"T60x_TI_LEVEL0",
	"T60x_TI_LEVEL1",
	"T60x_TI_LEVEL2",
	"T60x_TI_LEVEL3",
	"T60x_TI_LEVEL4",
	"T60x_TI_LEVEL5",
	"T60x_TI_LEVEL6",
	"T60x_TI_LEVEL7",
	"T60x_TI_COMMAND_1",
	"T60x_TI_COMMAND_2",
	"T60x_TI_COMMAND_3",
	"T60x_TI_COMMAND_4",
	"T60x_TI_COMMAND_4_7",
	"T60x_TI_COMMAND_8_15",
	"T60x_TI_COMMAND_16_63",
	"T60x_TI_COMMAND_64",
	"T60x_TI_COMPRESS_IN",
	"T60x_TI_COMPRESS_OUT",
	"T60x_TI_COMPRESS_FLUSH",
	"T60x_TI_TIMESTAMPS",
	"T60x_TI_PCACHE_HIT",
	"T60x_TI_PCACHE_MISS",
	"T60x_TI_PCACHE_LINE",
	"T60x_TI_PCACHE_STALL",
	"T60x_TI_WRBUF_HIT",
	"T60x_TI_WRBUF_MISS",
	"T60x_TI_WRBUF_LINE",
	"T60x_TI_WRBUF_PARTIAL",
	"T60x_TI_WRBUF_STALL",
	"T60x_TI_ACTIVE",
	"T60x_TI_LOADING_DESC",
	"T60x_TI_INDEX_WAIT",
	"T60x_TI_INDEX_RANGE_WAIT",
	"T60x_TI_VERTEX_WAIT",
	"T60x_TI_PCACHE_WAIT",
	"T60x_TI_WRBUF_WAIT",
	"T60x_TI_BUS_READ",
	"T60x_TI_BUS_WRITE",
	"",
	"",
	"",
	"",
	"",
	"T60x_TI_UTLB_STALL",
	"T60x_TI_UTLB_REPLAY_MISS",
	"T60x_TI_UTLB_REPLAY_FULL",
	"T60x_TI_UTLB_NEW_MISS",
	"T60x_TI_UTLB_HIT",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T60x_FRAG_ACTIVE",
	"T60x_FRAG_PRIMITIVES",
	"T60x_FRAG_PRIMITIVES_DROPPED",
	"T60x_FRAG_CYCLES_DESC",
	"T60x_FRAG_CYCLES_PLR",
	"T60x_FRAG_CYCLES_VERT",
	"T60x_FRAG_CYCLES_TRISETUP",
	"T60x_FRAG_CYCLES_RAST",
	"T60x_FRAG_THREADS",
	"T60x_FRAG_DUMMY_THREADS",
	"T60x_FRAG_QUADS_RAST",
	"T60x_FRAG_QUADS_EZS_TEST",
	"T60x_FRAG_QUADS_EZS_KILLED",
	"T60x_FRAG_THREADS_LZS_TEST",
	"T60x_FRAG_THREADS_LZS_KILLED",
	"T60x_FRAG_CYCLES_NO_TILE",
	"T60x_FRAG_NUM_TILES",
	"T60x_FRAG_TRANS_ELIM",
	"T60x_COMPUTE_ACTIVE",
	"T60x_COMPUTE_TASKS",
	"T60x_COMPUTE_THREADS",
	"T60x_COMPUTE_CYCLES_DESC",
	"T60x_TRIPIPE_ACTIVE",
	"T60x_ARITH_WORDS",
	"T60x_ARITH_CYCLES_REG",
	"T60x_ARITH_CYCLES_L0",
	"T60x_ARITH_FRAG_DEPEND",
	"T60x_LS_WORDS",
	"T60x_LS_ISSUES",
	"T60x_LS_RESTARTS",
	"T60x_LS_REISSUES_MISS",
	"T60x_LS_REISSUES_VD",
	"T60x_LS_REISSUE_ATTRIB_MISS",
	"T60x_LS_NO_WB",
	"T60x_TEX_WORDS",
	"T60x_TEX_BUBBLES",
	"T60x_TEX_WORDS_L0",
	"T60x_TEX_WORDS_DESC",
	"T60x_TEX_ISSUES",
	"T60x_TEX_RECIRC_FMISS",
	"T60x_TEX_RECIRC_DESC",
	"T60x_TEX_RECIRC_MULTI",
	"T60x_TEX_RECIRC_PMISS",
	"T60x_TEX_RECIRC_CONF",
	"T60x_LSC_READ_HITS",
	"T60x_LSC_READ_MISSES",
	"T60x_LSC_WRITE_HITS",
	"T60x_LSC_WRITE_MISSES",
	"T60x_LSC_ATOMIC_HITS",
	"T60x_LSC_ATOMIC_MISSES",
	"T60x_LSC_LINE_FETCHES",
	"T60x_LSC_DIRTY_LINE",
	"T60x_LSC_SNOOPS",
	"T60x_AXI_TLB_STALL",
	"T60x_AXI_TLB_MISS",
	"T60x_AXI_TLB_TRANSACTION",
	"T60x_LS_TLB_MISS",
	"T60x_LS_TLB_HIT",
	"T60x_AXI_BEATS_READ",
	"T60x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T60x_MMU_HIT",
	"T60x_MMU_NEW_MISS",
	"T60x_MMU_REPLAY_FULL",
	"T60x_MMU_REPLAY_MISS",
	"T60x_MMU_TABLE_WALK",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T60x_UTLB_HIT",
	"T60x_UTLB_NEW_MISS",
	"T60x_UTLB_REPLAY_FULL",
	"T60x_UTLB_REPLAY_MISS",
	"T60x_UTLB_STALL",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T60x_L2_EXT_WRITE_BEATS",
	"T60x_L2_EXT_READ_BEATS",
	"T60x_L2_ANY_LOOKUP",
	"T60x_L2_READ_LOOKUP",
	"T60x_L2_SREAD_LOOKUP",
	"T60x_L2_READ_REPLAY",
	"T60x_L2_READ_SNOOP",
	"T60x_L2_READ_HIT",
	"T60x_L2_CLEAN_MISS",
	"T60x_L2_WRITE_LOOKUP",
	"T60x_L2_SWRITE_LOOKUP",
	"T60x_L2_WRITE_REPLAY",
	"T60x_L2_WRITE_SNOOP",
	"T60x_L2_WRITE_HIT",
	"T60x_L2_EXT_READ_FULL",
	"T60x_L2_EXT_READ_HALF",
	"T60x_L2_EXT_WRITE_FULL",
	"T60x_L2_EXT_WRITE_HALF",
	"T60x_L2_EXT_READ",
	"T60x_L2_EXT_READ_LINE",
	"T60x_L2_EXT_WRITE",
	"T60x_L2_EXT_WRITE_LINE",
	"T60x_L2_EXT_WRITE_SMALL",
	"T60x_L2_EXT_BARRIER",
	"T60x_L2_EXT_AR_STALL",
	"T60x_L2_EXT_R_BUF_FULL",
	"T60x_L2_EXT_RD_BUF_FULL",
	"T60x_L2_EXT_R_RAW",
	"T60x_L2_EXT_W_STALL",
	"T60x_L2_EXT_W_BUF_FULL",
	"T60x_L2_EXT_R_W_HAZARD",
	"T60x_L2_TAG_HAZARD",
	"T60x_L2_SNOOP_FULL",
	"T60x_L2_REPLAY_FULL"
};
static const char * const hardware_counters_mali_t62x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T62x_MESSAGES_SENT",
	"T62x_MESSAGES_RECEIVED",
	"T62x_GPU_ACTIVE",
	"T62x_IRQ_ACTIVE",
	"T62x_JS0_JOBS",
	"T62x_JS0_TASKS",
	"T62x_JS0_ACTIVE",
	"",
	"T62x_JS0_WAIT_READ",
	"T62x_JS0_WAIT_ISSUE",
	"T62x_JS0_WAIT_DEPEND",
	"T62x_JS0_WAIT_FINISH",
	"T62x_JS1_JOBS",
	"T62x_JS1_TASKS",
	"T62x_JS1_ACTIVE",
	"",
	"T62x_JS1_WAIT_READ",
	"T62x_JS1_WAIT_ISSUE",
	"T62x_JS1_WAIT_DEPEND",
	"T62x_JS1_WAIT_FINISH",
	"T62x_JS2_JOBS",
	"T62x_JS2_TASKS",
	"T62x_JS2_ACTIVE",
	"",
	"T62x_JS2_WAIT_READ",
	"T62x_JS2_WAIT_ISSUE",
	"T62x_JS2_WAIT_DEPEND",
	"T62x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T62x_TI_JOBS_PROCESSED",
	"T62x_TI_TRIANGLES",
	"T62x_TI_QUADS",
	"T62x_TI_POLYGONS",
	"T62x_TI_POINTS",
	"T62x_TI_LINES",
	"T62x_TI_VCACHE_HIT",
	"T62x_TI_VCACHE_MISS",
	"T62x_TI_FRONT_FACING",
	"T62x_TI_BACK_FACING",
	"T62x_TI_PRIM_VISIBLE",
	"T62x_TI_PRIM_CULLED",
	"T62x_TI_PRIM_CLIPPED",
	"T62x_TI_LEVEL0",
	"T62x_TI_LEVEL1",
	"T62x_TI_LEVEL2",
	"T62x_TI_LEVEL3",
	"T62x_TI_LEVEL4",
	"T62x_TI_LEVEL5",
	"T62x_TI_LEVEL6",
	"T62x_TI_LEVEL7",
	"T62x_TI_COMMAND_1",
	"T62x_TI_COMMAND_2",
	"T62x_TI_COMMAND_3",
	"T62x_TI_COMMAND_4",
	"T62x_TI_COMMAND_5_7",
	"T62x_TI_COMMAND_8_15",
	"T62x_TI_COMMAND_16_63",
	"T62x_TI_COMMAND_64",
	"T62x_TI_COMPRESS_IN",
	"T62x_TI_COMPRESS_OUT",
	"T62x_TI_COMPRESS_FLUSH",
	"T62x_TI_TIMESTAMPS",
	"T62x_TI_PCACHE_HIT",
	"T62x_TI_PCACHE_MISS",
	"T62x_TI_PCACHE_LINE",
	"T62x_TI_PCACHE_STALL",
	"T62x_TI_WRBUF_HIT",
	"T62x_TI_WRBUF_MISS",
	"T62x_TI_WRBUF_LINE",
	"T62x_TI_WRBUF_PARTIAL",
	"T62x_TI_WRBUF_STALL",
	"T62x_TI_ACTIVE",
	"T62x_TI_LOADING_DESC",
	"T62x_TI_INDEX_WAIT",
	"T62x_TI_INDEX_RANGE_WAIT",
	"T62x_TI_VERTEX_WAIT",
	"T62x_TI_PCACHE_WAIT",
	"T62x_TI_WRBUF_WAIT",
	"T62x_TI_BUS_READ",
	"T62x_TI_BUS_WRITE",
	"",
	"",
	"",
	"",
	"",
	"T62x_TI_UTLB_STALL",
	"T62x_TI_UTLB_REPLAY_MISS",
	"T62x_TI_UTLB_REPLAY_FULL",
	"T62x_TI_UTLB_NEW_MISS",
	"T62x_TI_UTLB_HIT",

	/* Shader Core */
	"",
	"",
	"",
	"T62x_SHADER_CORE_ACTIVE",
	"T62x_FRAG_ACTIVE",
	"T62x_FRAG_PRIMITIVES",
	"T62x_FRAG_PRIMITIVES_DROPPED",
	"T62x_FRAG_CYCLES_DESC",
	"T62x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T62x_FRAG_CYCLES_VERT",
	"T62x_FRAG_CYCLES_TRISETUP",
	"T62x_FRAG_CYCLES_EZS_ACTIVE",
	"T62x_FRAG_THREADS",
	"T62x_FRAG_DUMMY_THREADS",
	"T62x_FRAG_QUADS_RAST",
	"T62x_FRAG_QUADS_EZS_TEST",
	"T62x_FRAG_QUADS_EZS_KILLED",
	"T62x_FRAG_THREADS_LZS_TEST",
	"T62x_FRAG_THREADS_LZS_KILLED",
	"T62x_FRAG_CYCLES_NO_TILE",
	"T62x_FRAG_NUM_TILES",
	"T62x_FRAG_TRANS_ELIM",
	"T62x_COMPUTE_ACTIVE",
	"T62x_COMPUTE_TASKS",
	"T62x_COMPUTE_THREADS",
	"T62x_COMPUTE_CYCLES_DESC",
	"T62x_TRIPIPE_ACTIVE",
	"T62x_ARITH_WORDS",
	"T62x_ARITH_CYCLES_REG",
	"T62x_ARITH_CYCLES_L0",
	"T62x_ARITH_FRAG_DEPEND",
	"T62x_LS_WORDS",
	"T62x_LS_ISSUES",
	"T62x_LS_RESTARTS",
	"T62x_LS_REISSUES_MISS",
	"T62x_LS_REISSUES_VD",
	"T62x_LS_REISSUE_ATTRIB_MISS",
	"T62x_LS_NO_WB",
	"T62x_TEX_WORDS",
	"T62x_TEX_BUBBLES",
	"T62x_TEX_WORDS_L0",
	"T62x_TEX_WORDS_DESC",
	"T62x_TEX_ISSUES",
	"T62x_TEX_RECIRC_FMISS",
	"T62x_TEX_RECIRC_DESC",
	"T62x_TEX_RECIRC_MULTI",
	"T62x_TEX_RECIRC_PMISS",
	"T62x_TEX_RECIRC_CONF",
	"T62x_LSC_READ_HITS",
	"T62x_LSC_READ_MISSES",
	"T62x_LSC_WRITE_HITS",
	"T62x_LSC_WRITE_MISSES",
	"T62x_LSC_ATOMIC_HITS",
	"T62x_LSC_ATOMIC_MISSES",
	"T62x_LSC_LINE_FETCHES",
	"T62x_LSC_DIRTY_LINE",
	"T62x_LSC_SNOOPS",
	"T62x_AXI_TLB_STALL",
	"T62x_AXI_TLB_MISS",
	"T62x_AXI_TLB_TRANSACTION",
	"T62x_LS_TLB_MISS",
	"T62x_LS_TLB_HIT",
	"T62x_AXI_BEATS_READ",
	"T62x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T62x_MMU_HIT",
	"T62x_MMU_NEW_MISS",
	"T62x_MMU_REPLAY_FULL",
	"T62x_MMU_REPLAY_MISS",
	"T62x_MMU_TABLE_WALK",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T62x_UTLB_HIT",
	"T62x_UTLB_NEW_MISS",
	"T62x_UTLB_REPLAY_FULL",
	"T62x_UTLB_REPLAY_MISS",
	"T62x_UTLB_STALL",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T62x_L2_EXT_WRITE_BEATS",
	"T62x_L2_EXT_READ_BEATS",
	"T62x_L2_ANY_LOOKUP",
	"T62x_L2_READ_LOOKUP",
	"T62x_L2_SREAD_LOOKUP",
	"T62x_L2_READ_REPLAY",
	"T62x_L2_READ_SNOOP",
	"T62x_L2_READ_HIT",
	"T62x_L2_CLEAN_MISS",
	"T62x_L2_WRITE_LOOKUP",
	"T62x_L2_SWRITE_LOOKUP",
	"T62x_L2_WRITE_REPLAY",
	"T62x_L2_WRITE_SNOOP",
	"T62x_L2_WRITE_HIT",
	"T62x_L2_EXT_READ_FULL",
	"T62x_L2_EXT_READ_HALF",
	"T62x_L2_EXT_WRITE_FULL",
	"T62x_L2_EXT_WRITE_HALF",
	"T62x_L2_EXT_READ",
	"T62x_L2_EXT_READ_LINE",
	"T62x_L2_EXT_WRITE",
	"T62x_L2_EXT_WRITE_LINE",
	"T62x_L2_EXT_WRITE_SMALL",
	"T62x_L2_EXT_BARRIER",
	"T62x_L2_EXT_AR_STALL",
	"T62x_L2_EXT_R_BUF_FULL",
	"T62x_L2_EXT_RD_BUF_FULL",
	"T62x_L2_EXT_R_RAW",
	"T62x_L2_EXT_W_STALL",
	"T62x_L2_EXT_W_BUF_FULL",
	"T62x_L2_EXT_R_W_HAZARD",
	"T62x_L2_TAG_HAZARD",
	"T62x_L2_SNOOP_FULL",
	"T62x_L2_REPLAY_FULL"
};

static const char * const hardware_counters_mali_t72x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T72x_GPU_ACTIVE",
	"T72x_IRQ_ACTIVE",
	"T72x_JS0_JOBS",
	"T72x_JS0_TASKS",
	"T72x_JS0_ACTIVE",
	"T72x_JS1_JOBS",
	"T72x_JS1_TASKS",
	"T72x_JS1_ACTIVE",
	"T72x_JS2_JOBS",
	"T72x_JS2_TASKS",
	"T72x_JS2_ACTIVE",
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

	/*Tiler */
	"",
	"",
	"",
	"T72x_TI_JOBS_PROCESSED",
	"T72x_TI_TRIANGLES",
	"T72x_TI_QUADS",
	"T72x_TI_POLYGONS",
	"T72x_TI_POINTS",
	"T72x_TI_LINES",
	"T72x_TI_FRONT_FACING",
	"T72x_TI_BACK_FACING",
	"T72x_TI_PRIM_VISIBLE",
	"T72x_TI_PRIM_CULLED",
	"T72x_TI_PRIM_CLIPPED",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T72x_TI_ACTIVE",
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
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T72x_FRAG_ACTIVE",
	"T72x_FRAG_PRIMITIVES",
	"T72x_FRAG_PRIMITIVES_DROPPED",
	"T72x_FRAG_THREADS",
	"T72x_FRAG_DUMMY_THREADS",
	"T72x_FRAG_QUADS_RAST",
	"T72x_FRAG_QUADS_EZS_TEST",
	"T72x_FRAG_QUADS_EZS_KILLED",
	"T72x_FRAG_THREADS_LZS_TEST",
	"T72x_FRAG_THREADS_LZS_KILLED",
	"T72x_FRAG_CYCLES_NO_TILE",
	"T72x_FRAG_NUM_TILES",
	"T72x_FRAG_TRANS_ELIM",
	"T72x_COMPUTE_ACTIVE",
	"T72x_COMPUTE_TASKS",
	"T72x_COMPUTE_THREADS",
	"T72x_TRIPIPE_ACTIVE",
	"T72x_ARITH_WORDS",
	"T72x_ARITH_CYCLES_REG",
	"T72x_LS_WORDS",
	"T72x_LS_ISSUES",
	"T72x_LS_RESTARTS",
	"T72x_LS_REISSUES_MISS",
	"T72x_TEX_WORDS",
	"T72x_TEX_BUBBLES",
	"T72x_TEX_ISSUES",
	"T72x_LSC_READ_HITS",
	"T72x_LSC_READ_MISSES",
	"T72x_LSC_WRITE_HITS",
	"T72x_LSC_WRITE_MISSES",
	"T72x_LSC_ATOMIC_HITS",
	"T72x_LSC_ATOMIC_MISSES",
	"T72x_LSC_LINE_FETCHES",
	"T72x_LSC_DIRTY_LINE",
	"T72x_LSC_SNOOPS",
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

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T72x_L2_EXT_WRITE_BEAT",
	"T72x_L2_EXT_READ_BEAT",
	"T72x_L2_READ_SNOOP",
	"T72x_L2_READ_HIT",
	"T72x_L2_WRITE_SNOOP",
	"T72x_L2_WRITE_HIT",
	"T72x_L2_EXT_WRITE_SMALL",
	"T72x_L2_EXT_BARRIER",
	"T72x_L2_EXT_AR_STALL",
	"T72x_L2_EXT_W_STALL",
	"T72x_L2_SNOOP_FULL",
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
	""
};

static const char * const hardware_counters_mali_t76x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T76x_MESSAGES_SENT",
	"T76x_MESSAGES_RECEIVED",
	"T76x_GPU_ACTIVE",
	"T76x_IRQ_ACTIVE",
	"T76x_JS0_JOBS",
	"T76x_JS0_TASKS",
	"T76x_JS0_ACTIVE",
	"",
	"T76x_JS0_WAIT_READ",
	"T76x_JS0_WAIT_ISSUE",
	"T76x_JS0_WAIT_DEPEND",
	"T76x_JS0_WAIT_FINISH",
	"T76x_JS1_JOBS",
	"T76x_JS1_TASKS",
	"T76x_JS1_ACTIVE",
	"",
	"T76x_JS1_WAIT_READ",
	"T76x_JS1_WAIT_ISSUE",
	"T76x_JS1_WAIT_DEPEND",
	"T76x_JS1_WAIT_FINISH",
	"T76x_JS2_JOBS",
	"T76x_JS2_TASKS",
	"T76x_JS2_ACTIVE",
	"",
	"T76x_JS2_WAIT_READ",
	"T76x_JS2_WAIT_ISSUE",
	"T76x_JS2_WAIT_DEPEND",
	"T76x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T76x_TI_JOBS_PROCESSED",
	"T76x_TI_TRIANGLES",
	"T76x_TI_QUADS",
	"T76x_TI_POLYGONS",
	"T76x_TI_POINTS",
	"T76x_TI_LINES",
	"T76x_TI_VCACHE_HIT",
	"T76x_TI_VCACHE_MISS",
	"T76x_TI_FRONT_FACING",
	"T76x_TI_BACK_FACING",
	"T76x_TI_PRIM_VISIBLE",
	"T76x_TI_PRIM_CULLED",
	"T76x_TI_PRIM_CLIPPED",
	"T76x_TI_LEVEL0",
	"T76x_TI_LEVEL1",
	"T76x_TI_LEVEL2",
	"T76x_TI_LEVEL3",
	"T76x_TI_LEVEL4",
	"T76x_TI_LEVEL5",
	"T76x_TI_LEVEL6",
	"T76x_TI_LEVEL7",
	"T76x_TI_COMMAND_1",
	"T76x_TI_COMMAND_2",
	"T76x_TI_COMMAND_3",
	"T76x_TI_COMMAND_4",
	"T76x_TI_COMMAND_5_7",
	"T76x_TI_COMMAND_8_15",
	"T76x_TI_COMMAND_16_63",
	"T76x_TI_COMMAND_64",
	"T76x_TI_COMPRESS_IN",
	"T76x_TI_COMPRESS_OUT",
	"T76x_TI_COMPRESS_FLUSH",
	"T76x_TI_TIMESTAMPS",
	"T76x_TI_PCACHE_HIT",
	"T76x_TI_PCACHE_MISS",
	"T76x_TI_PCACHE_LINE",
	"T76x_TI_PCACHE_STALL",
	"T76x_TI_WRBUF_HIT",
	"T76x_TI_WRBUF_MISS",
	"T76x_TI_WRBUF_LINE",
	"T76x_TI_WRBUF_PARTIAL",
	"T76x_TI_WRBUF_STALL",
	"T76x_TI_ACTIVE",
	"T76x_TI_LOADING_DESC",
	"T76x_TI_INDEX_WAIT",
	"T76x_TI_INDEX_RANGE_WAIT",
	"T76x_TI_VERTEX_WAIT",
	"T76x_TI_PCACHE_WAIT",
	"T76x_TI_WRBUF_WAIT",
	"T76x_TI_BUS_READ",
	"T76x_TI_BUS_WRITE",
	"",
	"",
	"",
	"",
	"",
	"T76x_TI_UTLB_HIT",
	"T76x_TI_UTLB_NEW_MISS",
	"T76x_TI_UTLB_REPLAY_FULL",
	"T76x_TI_UTLB_REPLAY_MISS",
	"T76x_TI_UTLB_STALL",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T76x_FRAG_ACTIVE",
	"T76x_FRAG_PRIMITIVES",
	"T76x_FRAG_PRIMITIVES_DROPPED",
	"T76x_FRAG_CYCLES_DESC",
	"T76x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T76x_FRAG_CYCLES_VERT",
	"T76x_FRAG_CYCLES_TRISETUP",
	"T76x_FRAG_CYCLES_EZS_ACTIVE",
	"T76x_FRAG_THREADS",
	"T76x_FRAG_DUMMY_THREADS",
	"T76x_FRAG_QUADS_RAST",
	"T76x_FRAG_QUADS_EZS_TEST",
	"T76x_FRAG_QUADS_EZS_KILLED",
	"T76x_FRAG_THREADS_LZS_TEST",
	"T76x_FRAG_THREADS_LZS_KILLED",
	"T76x_FRAG_CYCLES_NO_TILE",
	"T76x_FRAG_NUM_TILES",
	"T76x_FRAG_TRANS_ELIM",
	"T76x_COMPUTE_ACTIVE",
	"T76x_COMPUTE_TASKS",
	"T76x_COMPUTE_THREADS",
	"T76x_COMPUTE_CYCLES_DESC",
	"T76x_TRIPIPE_ACTIVE",
	"T76x_ARITH_WORDS",
	"T76x_ARITH_CYCLES_REG",
	"T76x_ARITH_CYCLES_L0",
	"T76x_ARITH_FRAG_DEPEND",
	"T76x_LS_WORDS",
	"T76x_LS_ISSUES",
	"T76x_LS_REISSUE_ATTR",
	"T76x_LS_REISSUES_VARY",
	"T76x_LS_VARY_RV_MISS",
	"T76x_LS_VARY_RV_HIT",
	"T76x_LS_NO_UNPARK",
	"T76x_TEX_WORDS",
	"T76x_TEX_BUBBLES",
	"T76x_TEX_WORDS_L0",
	"T76x_TEX_WORDS_DESC",
	"T76x_TEX_ISSUES",
	"T76x_TEX_RECIRC_FMISS",
	"T76x_TEX_RECIRC_DESC",
	"T76x_TEX_RECIRC_MULTI",
	"T76x_TEX_RECIRC_PMISS",
	"T76x_TEX_RECIRC_CONF",
	"T76x_LSC_READ_HITS",
	"T76x_LSC_READ_OP",
	"T76x_LSC_WRITE_HITS",
	"T76x_LSC_WRITE_OP",
	"T76x_LSC_ATOMIC_HITS",
	"T76x_LSC_ATOMIC_OP",
	"T76x_LSC_LINE_FETCHES",
	"T76x_LSC_DIRTY_LINE",
	"T76x_LSC_SNOOPS",
	"T76x_AXI_TLB_STALL",
	"T76x_AXI_TLB_MISS",
	"T76x_AXI_TLB_TRANSACTION",
	"T76x_LS_TLB_MISS",
	"T76x_LS_TLB_HIT",
	"T76x_AXI_BEATS_READ",
	"T76x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T76x_MMU_HIT",
	"T76x_MMU_NEW_MISS",
	"T76x_MMU_REPLAY_FULL",
	"T76x_MMU_REPLAY_MISS",
	"T76x_MMU_TABLE_WALK",
	"T76x_MMU_REQUESTS",
	"",
	"",
	"T76x_UTLB_HIT",
	"T76x_UTLB_NEW_MISS",
	"T76x_UTLB_REPLAY_FULL",
	"T76x_UTLB_REPLAY_MISS",
	"T76x_UTLB_STALL",
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
	"T76x_L2_EXT_WRITE_BEATS",
	"T76x_L2_EXT_READ_BEATS",
	"T76x_L2_ANY_LOOKUP",
	"T76x_L2_READ_LOOKUP",
	"T76x_L2_SREAD_LOOKUP",
	"T76x_L2_READ_REPLAY",
	"T76x_L2_READ_SNOOP",
	"T76x_L2_READ_HIT",
	"T76x_L2_CLEAN_MISS",
	"T76x_L2_WRITE_LOOKUP",
	"T76x_L2_SWRITE_LOOKUP",
	"T76x_L2_WRITE_REPLAY",
	"T76x_L2_WRITE_SNOOP",
	"T76x_L2_WRITE_HIT",
	"T76x_L2_EXT_READ_FULL",
	"",
	"T76x_L2_EXT_WRITE_FULL",
	"T76x_L2_EXT_R_W_HAZARD",
	"T76x_L2_EXT_READ",
	"T76x_L2_EXT_READ_LINE",
	"T76x_L2_EXT_WRITE",
	"T76x_L2_EXT_WRITE_LINE",
	"T76x_L2_EXT_WRITE_SMALL",
	"T76x_L2_EXT_BARRIER",
	"T76x_L2_EXT_AR_STALL",
	"T76x_L2_EXT_R_BUF_FULL",
	"T76x_L2_EXT_RD_BUF_FULL",
	"T76x_L2_EXT_R_RAW",
	"T76x_L2_EXT_W_STALL",
	"T76x_L2_EXT_W_BUF_FULL",
	"T76x_L2_EXT_R_BUF_FULL",
	"T76x_L2_TAG_HAZARD",
	"T76x_L2_SNOOP_FULL",
	"T76x_L2_REPLAY_FULL"
};

static const char * const hardware_counters_mali_t82x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T82x_MESSAGES_SENT",
	"T82x_MESSAGES_RECEIVED",
	"T82x_GPU_ACTIVE",
	"T82x_IRQ_ACTIVE",
	"T82x_JS0_JOBS",
	"T82x_JS0_TASKS",
	"T82x_JS0_ACTIVE",
	"",
	"T82x_JS0_WAIT_READ",
	"T82x_JS0_WAIT_ISSUE",
	"T82x_JS0_WAIT_DEPEND",
	"T82x_JS0_WAIT_FINISH",
	"T82x_JS1_JOBS",
	"T82x_JS1_TASKS",
	"T82x_JS1_ACTIVE",
	"",
	"T82x_JS1_WAIT_READ",
	"T82x_JS1_WAIT_ISSUE",
	"T82x_JS1_WAIT_DEPEND",
	"T82x_JS1_WAIT_FINISH",
	"T82x_JS2_JOBS",
	"T82x_JS2_TASKS",
	"T82x_JS2_ACTIVE",
	"",
	"T82x_JS2_WAIT_READ",
	"T82x_JS2_WAIT_ISSUE",
	"T82x_JS2_WAIT_DEPEND",
	"T82x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T82x_TI_JOBS_PROCESSED",
	"T82x_TI_TRIANGLES",
	"T82x_TI_QUADS",
	"T82x_TI_POLYGONS",
	"T82x_TI_POINTS",
	"T82x_TI_LINES",
	"T82x_TI_FRONT_FACING",
	"T82x_TI_BACK_FACING",
	"T82x_TI_PRIM_VISIBLE",
	"T82x_TI_PRIM_CULLED",
	"T82x_TI_PRIM_CLIPPED",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T82x_TI_ACTIVE",
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
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T82x_FRAG_ACTIVE",
	"T82x_FRAG_PRIMITIVES",
	"T82x_FRAG_PRIMITIVES_DROPPED",
	"T82x_FRAG_CYCLES_DESC",
	"T82x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T82x_FRAG_CYCLES_VERT",
	"T82x_FRAG_CYCLES_TRISETUP",
	"T82x_FRAG_CYCLES_EZS_ACTIVE",
	"T82x_FRAG_THREADS",
	"T82x_FRAG_DUMMY_THREADS",
	"T82x_FRAG_QUADS_RAST",
	"T82x_FRAG_QUADS_EZS_TEST",
	"T82x_FRAG_QUADS_EZS_KILLED",
	"T82x_FRAG_THREADS_LZS_TEST",
	"T82x_FRAG_THREADS_LZS_KILLED",
	"T82x_FRAG_CYCLES_NO_TILE",
	"T82x_FRAG_NUM_TILES",
	"T82x_FRAG_TRANS_ELIM",
	"T82x_COMPUTE_ACTIVE",
	"T82x_COMPUTE_TASKS",
	"T82x_COMPUTE_THREADS",
	"T82x_COMPUTE_CYCLES_DESC",
	"T82x_TRIPIPE_ACTIVE",
	"T82x_ARITH_WORDS",
	"T82x_ARITH_CYCLES_REG",
	"T82x_ARITH_CYCLES_L0",
	"T82x_ARITH_FRAG_DEPEND",
	"T82x_LS_WORDS",
	"T82x_LS_ISSUES",
	"T82x_LS_REISSUE_ATTR",
	"T82x_LS_REISSUES_VARY",
	"T82x_LS_VARY_RV_MISS",
	"T82x_LS_VARY_RV_HIT",
	"T82x_LS_NO_UNPARK",
	"T82x_TEX_WORDS",
	"T82x_TEX_BUBBLES",
	"T82x_TEX_WORDS_L0",
	"T82x_TEX_WORDS_DESC",
	"T82x_TEX_ISSUES",
	"T82x_TEX_RECIRC_FMISS",
	"T82x_TEX_RECIRC_DESC",
	"T82x_TEX_RECIRC_MULTI",
	"T82x_TEX_RECIRC_PMISS",
	"T82x_TEX_RECIRC_CONF",
	"T82x_LSC_READ_HITS",
	"T82x_LSC_READ_OP",
	"T82x_LSC_WRITE_HITS",
	"T82x_LSC_WRITE_OP",
	"T82x_LSC_ATOMIC_HITS",
	"T82x_LSC_ATOMIC_OP",
	"T82x_LSC_LINE_FETCHES",
	"T82x_LSC_DIRTY_LINE",
	"T82x_LSC_SNOOPS",
	"T82x_AXI_TLB_STALL",
	"T82x_AXI_TLB_MISS",
	"T82x_AXI_TLB_TRANSACTION",
	"T82x_LS_TLB_MISS",
	"T82x_LS_TLB_HIT",
	"T82x_AXI_BEATS_READ",
	"T82x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T82x_MMU_HIT",
	"T82x_MMU_NEW_MISS",
	"T82x_MMU_REPLAY_FULL",
	"T82x_MMU_REPLAY_MISS",
	"T82x_MMU_TABLE_WALK",
	"T82x_MMU_REQUESTS",
	"",
	"",
	"T82x_UTLB_HIT",
	"T82x_UTLB_NEW_MISS",
	"T82x_UTLB_REPLAY_FULL",
	"T82x_UTLB_REPLAY_MISS",
	"T82x_UTLB_STALL",
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
	"T82x_L2_EXT_WRITE_BEATS",
	"T82x_L2_EXT_READ_BEATS",
	"T82x_L2_ANY_LOOKUP",
	"T82x_L2_READ_LOOKUP",
	"T82x_L2_SREAD_LOOKUP",
	"T82x_L2_READ_REPLAY",
	"T82x_L2_READ_SNOOP",
	"T82x_L2_READ_HIT",
	"T82x_L2_CLEAN_MISS",
	"T82x_L2_WRITE_LOOKUP",
	"T82x_L2_SWRITE_LOOKUP",
	"T82x_L2_WRITE_REPLAY",
	"T82x_L2_WRITE_SNOOP",
	"T82x_L2_WRITE_HIT",
	"T82x_L2_EXT_READ_FULL",
	"",
	"T82x_L2_EXT_WRITE_FULL",
	"T82x_L2_EXT_R_W_HAZARD",
	"T82x_L2_EXT_READ",
	"T82x_L2_EXT_READ_LINE",
	"T82x_L2_EXT_WRITE",
	"T82x_L2_EXT_WRITE_LINE",
	"T82x_L2_EXT_WRITE_SMALL",
	"T82x_L2_EXT_BARRIER",
	"T82x_L2_EXT_AR_STALL",
	"T82x_L2_EXT_R_BUF_FULL",
	"T82x_L2_EXT_RD_BUF_FULL",
	"T82x_L2_EXT_R_RAW",
	"T82x_L2_EXT_W_STALL",
	"T82x_L2_EXT_W_BUF_FULL",
	"T82x_L2_EXT_R_BUF_FULL",
	"T82x_L2_TAG_HAZARD",
	"T82x_L2_SNOOP_FULL",
	"T82x_L2_REPLAY_FULL"
};

static const char * const hardware_counters_mali_t83x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T83x_MESSAGES_SENT",
	"T83x_MESSAGES_RECEIVED",
	"T83x_GPU_ACTIVE",
	"T83x_IRQ_ACTIVE",
	"T83x_JS0_JOBS",
	"T83x_JS0_TASKS",
	"T83x_JS0_ACTIVE",
	"",
	"T83x_JS0_WAIT_READ",
	"T83x_JS0_WAIT_ISSUE",
	"T83x_JS0_WAIT_DEPEND",
	"T83x_JS0_WAIT_FINISH",
	"T83x_JS1_JOBS",
	"T83x_JS1_TASKS",
	"T83x_JS1_ACTIVE",
	"",
	"T83x_JS1_WAIT_READ",
	"T83x_JS1_WAIT_ISSUE",
	"T83x_JS1_WAIT_DEPEND",
	"T83x_JS1_WAIT_FINISH",
	"T83x_JS2_JOBS",
	"T83x_JS2_TASKS",
	"T83x_JS2_ACTIVE",
	"",
	"T83x_JS2_WAIT_READ",
	"T83x_JS2_WAIT_ISSUE",
	"T83x_JS2_WAIT_DEPEND",
	"T83x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T83x_TI_JOBS_PROCESSED",
	"T83x_TI_TRIANGLES",
	"T83x_TI_QUADS",
	"T83x_TI_POLYGONS",
	"T83x_TI_POINTS",
	"T83x_TI_LINES",
	"T83x_TI_FRONT_FACING",
	"T83x_TI_BACK_FACING",
	"T83x_TI_PRIM_VISIBLE",
	"T83x_TI_PRIM_CULLED",
	"T83x_TI_PRIM_CLIPPED",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"T83x_TI_ACTIVE",
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
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T83x_FRAG_ACTIVE",
	"T83x_FRAG_PRIMITIVES",
	"T83x_FRAG_PRIMITIVES_DROPPED",
	"T83x_FRAG_CYCLES_DESC",
	"T83x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T83x_FRAG_CYCLES_VERT",
	"T83x_FRAG_CYCLES_TRISETUP",
	"T83x_FRAG_CYCLES_EZS_ACTIVE",
	"T83x_FRAG_THREADS",
	"T83x_FRAG_DUMMY_THREADS",
	"T83x_FRAG_QUADS_RAST",
	"T83x_FRAG_QUADS_EZS_TEST",
	"T83x_FRAG_QUADS_EZS_KILLED",
	"T83x_FRAG_THREADS_LZS_TEST",
	"T83x_FRAG_THREADS_LZS_KILLED",
	"T83x_FRAG_CYCLES_NO_TILE",
	"T83x_FRAG_NUM_TILES",
	"T83x_FRAG_TRANS_ELIM",
	"T83x_COMPUTE_ACTIVE",
	"T83x_COMPUTE_TASKS",
	"T83x_COMPUTE_THREADS",
	"T83x_COMPUTE_CYCLES_DESC",
	"T83x_TRIPIPE_ACTIVE",
	"T83x_ARITH_WORDS",
	"T83x_ARITH_CYCLES_REG",
	"T83x_ARITH_CYCLES_L0",
	"T83x_ARITH_FRAG_DEPEND",
	"T83x_LS_WORDS",
	"T83x_LS_ISSUES",
	"T83x_LS_REISSUE_ATTR",
	"T83x_LS_REISSUES_VARY",
	"T83x_LS_VARY_RV_MISS",
	"T83x_LS_VARY_RV_HIT",
	"T83x_LS_NO_UNPARK",
	"T83x_TEX_WORDS",
	"T83x_TEX_BUBBLES",
	"T83x_TEX_WORDS_L0",
	"T83x_TEX_WORDS_DESC",
	"T83x_TEX_ISSUES",
	"T83x_TEX_RECIRC_FMISS",
	"T83x_TEX_RECIRC_DESC",
	"T83x_TEX_RECIRC_MULTI",
	"T83x_TEX_RECIRC_PMISS",
	"T83x_TEX_RECIRC_CONF",
	"T83x_LSC_READ_HITS",
	"T83x_LSC_READ_OP",
	"T83x_LSC_WRITE_HITS",
	"T83x_LSC_WRITE_OP",
	"T83x_LSC_ATOMIC_HITS",
	"T83x_LSC_ATOMIC_OP",
	"T83x_LSC_LINE_FETCHES",
	"T83x_LSC_DIRTY_LINE",
	"T83x_LSC_SNOOPS",
	"T83x_AXI_TLB_STALL",
	"T83x_AXI_TLB_MISS",
	"T83x_AXI_TLB_TRANSACTION",
	"T83x_LS_TLB_MISS",
	"T83x_LS_TLB_HIT",
	"T83x_AXI_BEATS_READ",
	"T83x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T83x_MMU_HIT",
	"T83x_MMU_NEW_MISS",
	"T83x_MMU_REPLAY_FULL",
	"T83x_MMU_REPLAY_MISS",
	"T83x_MMU_TABLE_WALK",
	"T83x_MMU_REQUESTS",
	"",
	"",
	"T83x_UTLB_HIT",
	"T83x_UTLB_NEW_MISS",
	"T83x_UTLB_REPLAY_FULL",
	"T83x_UTLB_REPLAY_MISS",
	"T83x_UTLB_STALL",
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
	"T83x_L2_EXT_WRITE_BEATS",
	"T83x_L2_EXT_READ_BEATS",
	"T83x_L2_ANY_LOOKUP",
	"T83x_L2_READ_LOOKUP",
	"T83x_L2_SREAD_LOOKUP",
	"T83x_L2_READ_REPLAY",
	"T83x_L2_READ_SNOOP",
	"T83x_L2_READ_HIT",
	"T83x_L2_CLEAN_MISS",
	"T83x_L2_WRITE_LOOKUP",
	"T83x_L2_SWRITE_LOOKUP",
	"T83x_L2_WRITE_REPLAY",
	"T83x_L2_WRITE_SNOOP",
	"T83x_L2_WRITE_HIT",
	"T83x_L2_EXT_READ_FULL",
	"",
	"T83x_L2_EXT_WRITE_FULL",
	"T83x_L2_EXT_R_W_HAZARD",
	"T83x_L2_EXT_READ",
	"T83x_L2_EXT_READ_LINE",
	"T83x_L2_EXT_WRITE",
	"T83x_L2_EXT_WRITE_LINE",
	"T83x_L2_EXT_WRITE_SMALL",
	"T83x_L2_EXT_BARRIER",
	"T83x_L2_EXT_AR_STALL",
	"T83x_L2_EXT_R_BUF_FULL",
	"T83x_L2_EXT_RD_BUF_FULL",
	"T83x_L2_EXT_R_RAW",
	"T83x_L2_EXT_W_STALL",
	"T83x_L2_EXT_W_BUF_FULL",
	"T83x_L2_EXT_R_BUF_FULL",
	"T83x_L2_TAG_HAZARD",
	"T83x_L2_SNOOP_FULL",
	"T83x_L2_REPLAY_FULL"
};

static const char * const hardware_counters_mali_t86x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T86x_MESSAGES_SENT",
	"T86x_MESSAGES_RECEIVED",
	"T86x_GPU_ACTIVE",
	"T86x_IRQ_ACTIVE",
	"T86x_JS0_JOBS",
	"T86x_JS0_TASKS",
	"T86x_JS0_ACTIVE",
	"",
	"T86x_JS0_WAIT_READ",
	"T86x_JS0_WAIT_ISSUE",
	"T86x_JS0_WAIT_DEPEND",
	"T86x_JS0_WAIT_FINISH",
	"T86x_JS1_JOBS",
	"T86x_JS1_TASKS",
	"T86x_JS1_ACTIVE",
	"",
	"T86x_JS1_WAIT_READ",
	"T86x_JS1_WAIT_ISSUE",
	"T86x_JS1_WAIT_DEPEND",
	"T86x_JS1_WAIT_FINISH",
	"T86x_JS2_JOBS",
	"T86x_JS2_TASKS",
	"T86x_JS2_ACTIVE",
	"",
	"T86x_JS2_WAIT_READ",
	"T86x_JS2_WAIT_ISSUE",
	"T86x_JS2_WAIT_DEPEND",
	"T86x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T86x_TI_JOBS_PROCESSED",
	"T86x_TI_TRIANGLES",
	"T86x_TI_QUADS",
	"T86x_TI_POLYGONS",
	"T86x_TI_POINTS",
	"T86x_TI_LINES",
	"T86x_TI_VCACHE_HIT",
	"T86x_TI_VCACHE_MISS",
	"T86x_TI_FRONT_FACING",
	"T86x_TI_BACK_FACING",
	"T86x_TI_PRIM_VISIBLE",
	"T86x_TI_PRIM_CULLED",
	"T86x_TI_PRIM_CLIPPED",
	"T86x_TI_LEVEL0",
	"T86x_TI_LEVEL1",
	"T86x_TI_LEVEL2",
	"T86x_TI_LEVEL3",
	"T86x_TI_LEVEL4",
	"T86x_TI_LEVEL5",
	"T86x_TI_LEVEL6",
	"T86x_TI_LEVEL7",
	"T86x_TI_COMMAND_1",
	"T86x_TI_COMMAND_2",
	"T86x_TI_COMMAND_3",
	"T86x_TI_COMMAND_4",
	"T86x_TI_COMMAND_5_7",
	"T86x_TI_COMMAND_8_15",
	"T86x_TI_COMMAND_16_63",
	"T86x_TI_COMMAND_64",
	"T86x_TI_COMPRESS_IN",
	"T86x_TI_COMPRESS_OUT",
	"T86x_TI_COMPRESS_FLUSH",
	"T86x_TI_TIMESTAMPS",
	"T86x_TI_PCACHE_HIT",
	"T86x_TI_PCACHE_MISS",
	"T86x_TI_PCACHE_LINE",
	"T86x_TI_PCACHE_STALL",
	"T86x_TI_WRBUF_HIT",
	"T86x_TI_WRBUF_MISS",
	"T86x_TI_WRBUF_LINE",
	"T86x_TI_WRBUF_PARTIAL",
	"T86x_TI_WRBUF_STALL",
	"T86x_TI_ACTIVE",
	"T86x_TI_LOADING_DESC",
	"T86x_TI_INDEX_WAIT",
	"T86x_TI_INDEX_RANGE_WAIT",
	"T86x_TI_VERTEX_WAIT",
	"T86x_TI_PCACHE_WAIT",
	"T86x_TI_WRBUF_WAIT",
	"T86x_TI_BUS_READ",
	"T86x_TI_BUS_WRITE",
	"",
	"",
	"",
	"",
	"",
	"T86x_TI_UTLB_HIT",
	"T86x_TI_UTLB_NEW_MISS",
	"T86x_TI_UTLB_REPLAY_FULL",
	"T86x_TI_UTLB_REPLAY_MISS",
	"T86x_TI_UTLB_STALL",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T86x_FRAG_ACTIVE",
	"T86x_FRAG_PRIMITIVES",
	"T86x_FRAG_PRIMITIVES_DROPPED",
	"T86x_FRAG_CYCLES_DESC",
	"T86x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T86x_FRAG_CYCLES_VERT",
	"T86x_FRAG_CYCLES_TRISETUP",
	"T86x_FRAG_CYCLES_EZS_ACTIVE",
	"T86x_FRAG_THREADS",
	"T86x_FRAG_DUMMY_THREADS",
	"T86x_FRAG_QUADS_RAST",
	"T86x_FRAG_QUADS_EZS_TEST",
	"T86x_FRAG_QUADS_EZS_KILLED",
	"T86x_FRAG_THREADS_LZS_TEST",
	"T86x_FRAG_THREADS_LZS_KILLED",
	"T86x_FRAG_CYCLES_NO_TILE",
	"T86x_FRAG_NUM_TILES",
	"T86x_FRAG_TRANS_ELIM",
	"T86x_COMPUTE_ACTIVE",
	"T86x_COMPUTE_TASKS",
	"T86x_COMPUTE_THREADS",
	"T86x_COMPUTE_CYCLES_DESC",
	"T86x_TRIPIPE_ACTIVE",
	"T86x_ARITH_WORDS",
	"T86x_ARITH_CYCLES_REG",
	"T86x_ARITH_CYCLES_L0",
	"T86x_ARITH_FRAG_DEPEND",
	"T86x_LS_WORDS",
	"T86x_LS_ISSUES",
	"T86x_LS_REISSUE_ATTR",
	"T86x_LS_REISSUES_VARY",
	"T86x_LS_VARY_RV_MISS",
	"T86x_LS_VARY_RV_HIT",
	"T86x_LS_NO_UNPARK",
	"T86x_TEX_WORDS",
	"T86x_TEX_BUBBLES",
	"T86x_TEX_WORDS_L0",
	"T86x_TEX_WORDS_DESC",
	"T86x_TEX_ISSUES",
	"T86x_TEX_RECIRC_FMISS",
	"T86x_TEX_RECIRC_DESC",
	"T86x_TEX_RECIRC_MULTI",
	"T86x_TEX_RECIRC_PMISS",
	"T86x_TEX_RECIRC_CONF",
	"T86x_LSC_READ_HITS",
	"T86x_LSC_READ_OP",
	"T86x_LSC_WRITE_HITS",
	"T86x_LSC_WRITE_OP",
	"T86x_LSC_ATOMIC_HITS",
	"T86x_LSC_ATOMIC_OP",
	"T86x_LSC_LINE_FETCHES",
	"T86x_LSC_DIRTY_LINE",
	"T86x_LSC_SNOOPS",
	"T86x_AXI_TLB_STALL",
	"T86x_AXI_TLB_MISS",
	"T86x_AXI_TLB_TRANSACTION",
	"T86x_LS_TLB_MISS",
	"T86x_LS_TLB_HIT",
	"T86x_AXI_BEATS_READ",
	"T86x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T86x_MMU_HIT",
	"T86x_MMU_NEW_MISS",
	"T86x_MMU_REPLAY_FULL",
	"T86x_MMU_REPLAY_MISS",
	"T86x_MMU_TABLE_WALK",
	"T86x_MMU_REQUESTS",
	"",
	"",
	"T86x_UTLB_HIT",
	"T86x_UTLB_NEW_MISS",
	"T86x_UTLB_REPLAY_FULL",
	"T86x_UTLB_REPLAY_MISS",
	"T86x_UTLB_STALL",
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
	"T86x_L2_EXT_WRITE_BEATS",
	"T86x_L2_EXT_READ_BEATS",
	"T86x_L2_ANY_LOOKUP",
	"T86x_L2_READ_LOOKUP",
	"T86x_L2_SREAD_LOOKUP",
	"T86x_L2_READ_REPLAY",
	"T86x_L2_READ_SNOOP",
	"T86x_L2_READ_HIT",
	"T86x_L2_CLEAN_MISS",
	"T86x_L2_WRITE_LOOKUP",
	"T86x_L2_SWRITE_LOOKUP",
	"T86x_L2_WRITE_REPLAY",
	"T86x_L2_WRITE_SNOOP",
	"T86x_L2_WRITE_HIT",
	"T86x_L2_EXT_READ_FULL",
	"",
	"T86x_L2_EXT_WRITE_FULL",
	"T86x_L2_EXT_R_W_HAZARD",
	"T86x_L2_EXT_READ",
	"T86x_L2_EXT_READ_LINE",
	"T86x_L2_EXT_WRITE",
	"T86x_L2_EXT_WRITE_LINE",
	"T86x_L2_EXT_WRITE_SMALL",
	"T86x_L2_EXT_BARRIER",
	"T86x_L2_EXT_AR_STALL",
	"T86x_L2_EXT_R_BUF_FULL",
	"T86x_L2_EXT_RD_BUF_FULL",
	"T86x_L2_EXT_R_RAW",
	"T86x_L2_EXT_W_STALL",
	"T86x_L2_EXT_W_BUF_FULL",
	"T86x_L2_EXT_R_BUF_FULL",
	"T86x_L2_TAG_HAZARD",
	"T86x_L2_SNOOP_FULL",
	"T86x_L2_REPLAY_FULL"
};

static const char * const hardware_counters_mali_t88x[] = {
	/* Job Manager */
	"",
	"",
	"",
	"",
	"T88x_MESSAGES_SENT",
	"T88x_MESSAGES_RECEIVED",
	"T88x_GPU_ACTIVE",
	"T88x_IRQ_ACTIVE",
	"T88x_JS0_JOBS",
	"T88x_JS0_TASKS",
	"T88x_JS0_ACTIVE",
	"",
	"T88x_JS0_WAIT_READ",
	"T88x_JS0_WAIT_ISSUE",
	"T88x_JS0_WAIT_DEPEND",
	"T88x_JS0_WAIT_FINISH",
	"T88x_JS1_JOBS",
	"T88x_JS1_TASKS",
	"T88x_JS1_ACTIVE",
	"",
	"T88x_JS1_WAIT_READ",
	"T88x_JS1_WAIT_ISSUE",
	"T88x_JS1_WAIT_DEPEND",
	"T88x_JS1_WAIT_FINISH",
	"T88x_JS2_JOBS",
	"T88x_JS2_TASKS",
	"T88x_JS2_ACTIVE",
	"",
	"T88x_JS2_WAIT_READ",
	"T88x_JS2_WAIT_ISSUE",
	"T88x_JS2_WAIT_DEPEND",
	"T88x_JS2_WAIT_FINISH",
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

	/*Tiler */
	"",
	"",
	"",
	"T88x_TI_JOBS_PROCESSED",
	"T88x_TI_TRIANGLES",
	"T88x_TI_QUADS",
	"T88x_TI_POLYGONS",
	"T88x_TI_POINTS",
	"T88x_TI_LINES",
	"T88x_TI_VCACHE_HIT",
	"T88x_TI_VCACHE_MISS",
	"T88x_TI_FRONT_FACING",
	"T88x_TI_BACK_FACING",
	"T88x_TI_PRIM_VISIBLE",
	"T88x_TI_PRIM_CULLED",
	"T88x_TI_PRIM_CLIPPED",
	"T88x_TI_LEVEL0",
	"T88x_TI_LEVEL1",
	"T88x_TI_LEVEL2",
	"T88x_TI_LEVEL3",
	"T88x_TI_LEVEL4",
	"T88x_TI_LEVEL5",
	"T88x_TI_LEVEL6",
	"T88x_TI_LEVEL7",
	"T88x_TI_COMMAND_1",
	"T88x_TI_COMMAND_2",
	"T88x_TI_COMMAND_3",
	"T88x_TI_COMMAND_4",
	"T88x_TI_COMMAND_5_7",
	"T88x_TI_COMMAND_8_15",
	"T88x_TI_COMMAND_16_63",
	"T88x_TI_COMMAND_64",
	"T88x_TI_COMPRESS_IN",
	"T88x_TI_COMPRESS_OUT",
	"T88x_TI_COMPRESS_FLUSH",
	"T88x_TI_TIMESTAMPS",
	"T88x_TI_PCACHE_HIT",
	"T88x_TI_PCACHE_MISS",
	"T88x_TI_PCACHE_LINE",
	"T88x_TI_PCACHE_STALL",
	"T88x_TI_WRBUF_HIT",
	"T88x_TI_WRBUF_MISS",
	"T88x_TI_WRBUF_LINE",
	"T88x_TI_WRBUF_PARTIAL",
	"T88x_TI_WRBUF_STALL",
	"T88x_TI_ACTIVE",
	"T88x_TI_LOADING_DESC",
	"T88x_TI_INDEX_WAIT",
	"T88x_TI_INDEX_RANGE_WAIT",
	"T88x_TI_VERTEX_WAIT",
	"T88x_TI_PCACHE_WAIT",
	"T88x_TI_WRBUF_WAIT",
	"T88x_TI_BUS_READ",
	"T88x_TI_BUS_WRITE",
	"",
	"",
	"",
	"",
	"",
	"T88x_TI_UTLB_HIT",
	"T88x_TI_UTLB_NEW_MISS",
	"T88x_TI_UTLB_REPLAY_FULL",
	"T88x_TI_UTLB_REPLAY_MISS",
	"T88x_TI_UTLB_STALL",

	/* Shader Core */
	"",
	"",
	"",
	"",
	"T88x_FRAG_ACTIVE",
	"T88x_FRAG_PRIMITIVES",
	"T88x_FRAG_PRIMITIVES_DROPPED",
	"T88x_FRAG_CYCLES_DESC",
	"T88x_FRAG_CYCLES_FPKQ_ACTIVE",
	"T88x_FRAG_CYCLES_VERT",
	"T88x_FRAG_CYCLES_TRISETUP",
	"T88x_FRAG_CYCLES_EZS_ACTIVE",
	"T88x_FRAG_THREADS",
	"T88x_FRAG_DUMMY_THREADS",
	"T88x_FRAG_QUADS_RAST",
	"T88x_FRAG_QUADS_EZS_TEST",
	"T88x_FRAG_QUADS_EZS_KILLED",
	"T88x_FRAG_THREADS_LZS_TEST",
	"T88x_FRAG_THREADS_LZS_KILLED",
	"T88x_FRAG_CYCLES_NO_TILE",
	"T88x_FRAG_NUM_TILES",
	"T88x_FRAG_TRANS_ELIM",
	"T88x_COMPUTE_ACTIVE",
	"T88x_COMPUTE_TASKS",
	"T88x_COMPUTE_THREADS",
	"T88x_COMPUTE_CYCLES_DESC",
	"T88x_TRIPIPE_ACTIVE",
	"T88x_ARITH_WORDS",
	"T88x_ARITH_CYCLES_REG",
	"T88x_ARITH_CYCLES_L0",
	"T88x_ARITH_FRAG_DEPEND",
	"T88x_LS_WORDS",
	"T88x_LS_ISSUES",
	"T88x_LS_REISSUE_ATTR",
	"T88x_LS_REISSUES_VARY",
	"T88x_LS_VARY_RV_MISS",
	"T88x_LS_VARY_RV_HIT",
	"T88x_LS_NO_UNPARK",
	"T88x_TEX_WORDS",
	"T88x_TEX_BUBBLES",
	"T88x_TEX_WORDS_L0",
	"T88x_TEX_WORDS_DESC",
	"T88x_TEX_ISSUES",
	"T88x_TEX_RECIRC_FMISS",
	"T88x_TEX_RECIRC_DESC",
	"T88x_TEX_RECIRC_MULTI",
	"T88x_TEX_RECIRC_PMISS",
	"T88x_TEX_RECIRC_CONF",
	"T88x_LSC_READ_HITS",
	"T88x_LSC_READ_OP",
	"T88x_LSC_WRITE_HITS",
	"T88x_LSC_WRITE_OP",
	"T88x_LSC_ATOMIC_HITS",
	"T88x_LSC_ATOMIC_OP",
	"T88x_LSC_LINE_FETCHES",
	"T88x_LSC_DIRTY_LINE",
	"T88x_LSC_SNOOPS",
	"T88x_AXI_TLB_STALL",
	"T88x_AXI_TLB_MISS",
	"T88x_AXI_TLB_TRANSACTION",
	"T88x_LS_TLB_MISS",
	"T88x_LS_TLB_HIT",
	"T88x_AXI_BEATS_READ",
	"T88x_AXI_BEATS_WRITTEN",

	/*L2 and MMU */
	"",
	"",
	"",
	"",
	"T88x_MMU_HIT",
	"T88x_MMU_NEW_MISS",
	"T88x_MMU_REPLAY_FULL",
	"T88x_MMU_REPLAY_MISS",
	"T88x_MMU_TABLE_WALK",
	"T88x_MMU_REQUESTS",
	"",
	"",
	"T88x_UTLB_HIT",
	"T88x_UTLB_NEW_MISS",
	"T88x_UTLB_REPLAY_FULL",
	"T88x_UTLB_REPLAY_MISS",
	"T88x_UTLB_STALL",
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
	"T88x_L2_EXT_WRITE_BEATS",
	"T88x_L2_EXT_READ_BEATS",
	"T88x_L2_ANY_LOOKUP",
	"T88x_L2_READ_LOOKUP",
	"T88x_L2_SREAD_LOOKUP",
	"T88x_L2_READ_REPLAY",
	"T88x_L2_READ_SNOOP",
	"T88x_L2_READ_HIT",
	"T88x_L2_CLEAN_MISS",
	"T88x_L2_WRITE_LOOKUP",
	"T88x_L2_SWRITE_LOOKUP",
	"T88x_L2_WRITE_REPLAY",
	"T88x_L2_WRITE_SNOOP",
	"T88x_L2_WRITE_HIT",
	"T88x_L2_EXT_READ_FULL",
	"",
	"T88x_L2_EXT_WRITE_FULL",
	"T88x_L2_EXT_R_W_HAZARD",
	"T88x_L2_EXT_READ",
	"T88x_L2_EXT_READ_LINE",
	"T88x_L2_EXT_WRITE",
	"T88x_L2_EXT_WRITE_LINE",
	"T88x_L2_EXT_WRITE_SMALL",
	"T88x_L2_EXT_BARRIER",
	"T88x_L2_EXT_AR_STALL",
	"T88x_L2_EXT_R_BUF_FULL",
	"T88x_L2_EXT_RD_BUF_FULL",
	"T88x_L2_EXT_R_RAW",
	"T88x_L2_EXT_W_STALL",
	"T88x_L2_EXT_W_BUF_FULL",
	"T88x_L2_EXT_R_BUF_FULL",
	"T88x_L2_TAG_HAZARD",
	"T88x_L2_SNOOP_FULL",
	"T88x_L2_REPLAY_FULL"
};

#include "mali_kbase_gator_hwcnt_names_tmix.h"

#include "mali_kbase_gator_hwcnt_names_thex.h"

#include "mali_kbase_gator_hwcnt_names_tsix.h"

#include "mali_kbase_gator_hwcnt_names_tnox.h"

#include "mali_kbase_gator_hwcnt_names_tgox.h"

#include "mali_kbase_gator_hwcnt_names_tkax.h"

#include "mali_kbase_gator_hwcnt_names_ttrx.h"

#endif
