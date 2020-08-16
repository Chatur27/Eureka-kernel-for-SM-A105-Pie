// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/ntfs3/fsntfs.c
 *
 * Copyright (C) 2019-2020 Paragon Software GmbH, All rights reserved.
 *
 */

#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/fs.h>
#include <linux/nls.h>
#include <linux/sched/signal.h>

#include "debug.h"
#include "ntfs.h"
#include "ntfs_fs.h"

const struct cpu_str NAME_MFT = { 4, 0, { '$', 'M', 'F', 'T' } };
const struct cpu_str NAME_MIRROR = { 8,
				     0,
				     { '$', 'M', 'F', 'T', 'M', 'i', 'r',
				       'r' } };
const struct cpu_str NAME_LOGFILE = { 8,
				      0,
				      { '$', 'L', 'o', 'g', 'F', 'i', 'l',
					'e' } };
const struct cpu_str NAME_VOLUME = { 7,
				     0,
				     { '$', 'V', 'o', 'l', 'u', 'm', 'e' } };
const struct cpu_str NAME_ATTRDEF = { 8,
				      0,
				      { '$', 'A', 't', 't', 'r', 'D', 'e',
					'f' } };
const struct cpu_str NAME_ROOT = { 1, 0, { '.' } };
const struct cpu_str NAME_BITMAP = { 7,
				     0,
				     { '$', 'B', 'i', 't', 'm', 'a', 'p' } };
const struct cpu_str NAME_BOOT = { 5, 0, { '$', 'B', 'o', 'o', 't' } };
const struct cpu_str NAME_BADCLUS = { 8,
				      0,
				      { '$', 'B', 'a', 'd', 'C', 'l', 'u',
					's' } };
const struct cpu_str NAME_QUOTA = { 6, 0, { '$', 'Q', 'u', 'o', 't', 'a' } };
const struct cpu_str NAME_SECURE = { 7,
				     0,
				     { '$', 'S', 'e', 'c', 'u', 'r', 'e' } };
const struct cpu_str NAME_UPCASE = { 7,
				     0,
				     { '$', 'U', 'p', 'C', 'a', 's', 'e' } };
const struct cpu_str NAME_EXTEND = { 7,
				     0,
				     { '$', 'E', 'x', 't', 'e', 'n', 'd' } };
const struct cpu_str NAME_OBJID = { 6, 0, { '$', 'O', 'b', 'j', 'I', 'd' } };
const struct cpu_str NAME_REPARSE = { 8,
				      0,
				      { '$', 'R', 'e', 'p', 'a', 'r', 's',
					'e' } };
const struct cpu_str NAME_USNJRNL = { 8,
				      0,
				      { '$', 'U', 's', 'n', 'J', 'r', 'n',
					'l' } };
const __le16 BAD_NAME[4] = { cpu_to_le16('$'), cpu_to_le16('B'),
			     cpu_to_le16('a'), cpu_to_le16('d') };
const __le16 I30_NAME[4] = { cpu_to_le16('$'), cpu_to_le16('I'),
			     cpu_to_le16('3'), cpu_to_le16('0') };
const __le16 SII_NAME[4] = { cpu_to_le16('$'), cpu_to_le16('S'),
			     cpu_to_le16('I'), cpu_to_le16('I') };
const __le16 SDH_NAME[4] = { cpu_to_le16('$'), cpu_to_le16('S'),
			     cpu_to_le16('D'), cpu_to_le16('H') };
const __le16 SDS_NAME[4] = { cpu_to_le16('$'), cpu_to_le16('S'),
			     cpu_to_le16('D'), cpu_to_le16('S') };
const __le16 SO_NAME[2] = { cpu_to_le16('$'), cpu_to_le16('O') };
const __le16 SQ_NAME[2] = { cpu_to_le16('$'), cpu_to_le16('Q') };
const __le16 SR_NAME[2] = { cpu_to_le16('$'), cpu_to_le16('R') };
//const __le16 EFS_NAME[4] = { '$', 'E', 'F', 'S' };

//const __le16 WOF_NAME[17] = { 'W', 'o', 'f', 'C', 'o', 'm', 'p', 'r', 'e',
//			      'S', 'S', 'e', 'd', 'D', 'a', 't', 'a' };
//const __le16 J_NAME[2] = { '$', 'J' };
//const __le16 MAX_NAME[4] = { '$', 'M', 'a', 'x' };

/*
 * ntfs_fix_pre_write
 *
 * inserts fixups into 'rhdr' before writing to disk
 */
bool ntfs_fix_pre_write(NTFS_RECORD_HEADER *rhdr, size_t bytes)
{
	u16 *fixup, *ptr;
	u16 sample;
	u16 fo = le16_to_cpu(rhdr->fix_off);
	u16 fn = le16_to_cpu(rhdr->fix_num);

	if ((fo & 1) || fo + fn * sizeof(short) > bytes || !fn-- ||
	    fn * SECTOR_SIZE > bytes) {
		return false;
	}

	/* Get fixup pointer */
	fixup = Add2Ptr(rhdr, fo);

	if (*fixup >= 0x7FFF)
		*fixup = 1;
	else
		*fixup += 1;

	sample = *fixup;

	ptr = Add2Ptr(rhdr, SECTOR_SIZE - sizeof(short));

	while (fn--) {
		*++fixup = *ptr;
		*ptr = sample;
		ptr += SECTOR_SIZE / sizeof(short);
	}
	return true;
}

/*
 * ntfs_fix_post_read
 *
 * remove fixups after reading from disk
 * Returns < 0 if error, 0 if ok, 1 if need to update fixups
 */
int ntfs_fix_post_read(NTFS_RECORD_HEADER *rhdr, size_t bytes, bool simple)
{
	int ret;
	u16 *fixup, *ptr;
	u16 sample, fo, fn;

	fo = le16_to_cpu(rhdr->fix_off);
	fn = simple ? ((bytes >> SECTOR_SHIFT) + 1) :
		      le16_to_cpu(rhdr->fix_num);

	/* Check errors */
	if ((fo & 1) || fo + fn * sizeof(short) > bytes || !fn-- ||
	    fn * SECTOR_SIZE > bytes) {
		return -EINVAL; /* native chkntfs returns ok! */
	}

	/* Get fixup pointer */
	fixup = Add2Ptr(rhdr, fo);
	sample = *fixup;
	ptr = Add2Ptr(rhdr, SECTOR_SIZE - sizeof(short));
	ret = 0;

	while (fn--) {
		/* Test current word */
		if (*ptr != sample)
			ret = 1;

		/* Replace fixup */
		*ptr = *++fixup;
		ptr += SECTOR_SIZE / sizeof(short);
	}

	return ret;
}

/*
 * ntfs_extend_init
 *
 * loads $Extend file
 */
int ntfs_extend_init(ntfs_sb_info *sbi)
{
	int err;
	struct super_block *sb = sbi->sb;
	struct inode *inode, *inode2;
	ntfs_inode *ni;
	MFT_REF ref;

	if (sbi->volume.major_ver < 3) {
		ntfs_trace(sb, "Skip $Extend 'cause NTFS version");
		return 0;
	}

	ref.low = cpu_to_le32(MFT_REC_EXTEND);
	ref.high = 0;
	ref.seq = cpu_to_le16(MFT_REC_EXTEND);
	inode = ntfs_iget5(sb, &ref, &NAME_EXTEND);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		ntfs_error(sbi->sb, "Failed to load $Extend.");
		inode = NULL;
		goto out;
	}

	/* if ntfs_iget5 reads from disk it never returns bad inode */
	ni = ntfs_i(inode);
	if (!S_ISDIR(inode->i_mode)) {
		err = -EINVAL;
		goto out;
	}

	/* Try to find $ObjId */
	inode2 = dir_search_u(inode, &NAME_OBJID, NULL);
	if (inode2 && !IS_ERR(inode2)) {
		if (is_bad_inode(inode2))
			iput(inode2);
		else {
			sbi->objid.ni = ntfs_i(inode2);
			sbi->objid_no = inode2->i_ino;
		}
	}

	/* Try to find $Quota */
	inode2 = dir_search_u(inode, &NAME_QUOTA, NULL);
	if (inode2 && !IS_ERR(inode2)) {
		sbi->quota_no = inode2->i_ino;
		iput(inode2);
	}

	/* Try to find $Reparse */
	inode2 = dir_search_u(inode, &NAME_REPARSE, NULL);
	if (inode2 && !IS_ERR(inode2)) {
		sbi->reparse.ni = ntfs_i(inode2);
		sbi->reparse_no = inode2->i_ino;
	}

	/* Try to find $UsnJrnl */
	inode2 = dir_search_u(inode, &NAME_USNJRNL, NULL);
	if (inode2 && !IS_ERR(inode2)) {
		sbi->usn_jrnl_no = inode2->i_ino;
		iput(inode2);
	}

	err = 0;
out:
	iput(inode);
	return err;
}

int ntfs_loadlog_and_replay(ntfs_inode *ni, ntfs_sb_info *sbi)
{
	int err = 0;
	struct super_block *sb = sbi->sb;
	struct inode *inode;
	MFT_REF ref;
	u32 idx, log_size;
	CLST lcn, len;

	inode = &ni->vfs_inode;

	/* Check for 4GB */
	if (inode->i_size >= 0x100000000ull) {
		ntfs_error(sb, "$LogFile is too big");
		err = -EINVAL;
		goto out;
	}

	log_size = inode->i_size;
	sbi->flags |= NTFS_FLAGS_LOG_REPLAING;

	ref.low = cpu_to_le32(MFT_REC_MFT);
	ref.high = 0;
	ref.seq = cpu_to_le16(1);

	inode = ntfs_iget5(sb, &ref, NULL);

	if (IS_ERR(inode))
		inode = NULL;

	if (!inode) {
		/* Try to use mft copy */
		u64 t64 = sbi->mft.lbo;

		sbi->mft.lbo = sbi->mft.lbo2;
		inode = ntfs_iget5(sb, &ref, NULL);
		sbi->mft.lbo = t64;
		if (IS_ERR(inode))
			inode = NULL;
	}

	if (!inode) {
		err = -EINVAL;
		ntfs_error(sb, "Failed to load $MFT.");
		goto out;
	}

	sbi->mft.ni = ntfs_i(inode);

	err = ni_load_all_mi(sbi->mft.ni);
	if (!err)
		err = log_replay(ni);

	iput(inode);
	sbi->mft.ni = NULL;

	sync_blockdev(sb->s_bdev);
	invalidate_bdev(sb->s_bdev);

	/* reinit MFT */
	if (sbi->flags & NTFS_FLAGS_NEED_REPLAY) {
		err = 0;
		goto out;
	}

	if (sb_rdonly(sb))
		goto out;

	idx = 0;
	while (run_get_entry(&ni->file.run, idx++, NULL, &lcn, &len)) {
		u64 lbo = (u64)lcn << sbi->cluster_bits;
		u64 bytes = (u64)len << sbi->cluster_bits;

		err = ntfs_sb_write(sb, lbo, bytes, NULL, 0);
		if (err)
			goto out;
	}

out:
	sbi->flags &= ~NTFS_FLAGS_LOG_REPLAING;

	return err;
}

/*
 * ntfs_query_def
 *
 * returns current ATTR_DEF_ENTRY for given attribute type
 */
const ATTR_DEF_ENTRY *ntfs_query_def(ntfs_sb_info *sbi, ATTR_TYPE type)
{
	int type_in = le32_to_cpu(type);
	size_t min_idx = 0;
	size_t max_idx = sbi->def_entries - 1;

	while (min_idx <= max_idx) {
		size_t i = min_idx + ((max_idx - min_idx) >> 1);
		const ATTR_DEF_ENTRY *entry = sbi->def_table + i;
		int diff = le32_to_cpu(entry->type) - type_in;

		if (!diff)
			return entry;
		if (diff < 0)
			min_idx = i + 1;
		else if (i)
			max_idx = i - 1;
		else
			return NULL;
	}
	return NULL;
}

/*
 * ntfs_look_for_free_space
 *
 * looks for a free space in bitmap
 */
int ntfs_look_for_free_space(ntfs_sb_info *sbi, CLST lcn, CLST len,
			     CLST *new_lcn, CLST *new_len,
			     enum ALLOCATE_OPT opt)
{
	int err;
	struct super_block *sb = sbi->sb;
	size_t a_lcn, zlen, zeroes, zlcn, zlen2, ztrim, new_zlen;
	wnd_bitmap *wnd = &sbi->used.bitmap;

	down_write_nested(&wnd->rw_lock, BITMAP_MUTEX_CLUSTERS);
	if (opt & ALLOCATE_MFT) {
		CLST alen;

		zlen = wnd_zone_len(wnd);

		if (!zlen) {
			err = ntfs_refresh_zone(sbi);
			if (err)
				goto out;

			zlen = wnd_zone_len(wnd);

			if (!zlen) {
				ntfs_error(sbi->sb,
					   "no free space to extend mft");
				err = -ENOSPC;
				goto out;
			}
		}

		lcn = wnd_zone_bit(wnd);
		alen = zlen > len ? len : zlen;

		wnd_zone_set(wnd, lcn + alen, zlen - alen);

		err = wnd_set_used(wnd, lcn, alen);
		if (err)
			goto out;

		*new_lcn = lcn;
		*new_len = alen;
		goto ok;
	}

	/*
	 * 'Cause cluster 0 is always used this value means that we should use
	 * cached value of 'next_free_lcn' to improve performance
	 */
	if (!lcn)
		lcn = sbi->used.next_free_lcn;

	if (lcn >= wnd->nbits)
		lcn = 0;

	*new_len = wnd_find(wnd, len, lcn, BITMAP_FIND_MARK_AS_USED, &a_lcn);
	if (*new_len) {
		*new_lcn = a_lcn;
		goto ok;
	}

	/* Try to use clusters from MftZone */
	zlen = wnd_zone_len(wnd);
	zeroes = wnd_zeroes(wnd);

	/* Check too big request */
	if (len > zeroes + zlen)
		goto no_space;

	if (zlen <= NTFS_MIN_MFT_ZONE)
		goto no_space;

	/* How many clusters to cat from zone */
	zlcn = wnd_zone_bit(wnd);
	zlen2 = zlen >> 1;
	ztrim = len > zlen ? zlen : (len > zlen2 ? len : zlen2);
	new_zlen = zlen - ztrim;

	if (new_zlen < NTFS_MIN_MFT_ZONE) {
		new_zlen = NTFS_MIN_MFT_ZONE;
		if (new_zlen > zlen)
			new_zlen = zlen;
	}

	wnd_zone_set(wnd, zlcn, new_zlen);

	/* allocate continues clusters */
	*new_len =
		wnd_find(wnd, len, 0,
			 BITMAP_FIND_MARK_AS_USED | BITMAP_FIND_FULL, &a_lcn);
	if (*new_len) {
		*new_lcn = a_lcn;
		goto ok;
	}

no_space:
	ntfs_trace(
		sb,
		"Can not allocate %x cluster(s), Zone: %zx, free %zx, flags %d",
		(u32)len, zlen, wnd_zeroes(wnd), (int)opt);

	up_write(&wnd->rw_lock);

	return -ENOSPC;

ok:
	err = 0;

	ntfs_unmap_meta(sb, *new_lcn, *new_len);

	if (opt & ALLOCATE_MFT)
		goto out;

	/* Set hint for next requests */
	sbi->used.next_free_lcn = *new_lcn + *new_len;

out:
	up_write(&wnd->rw_lock);
	return err;
}

/*
 * ntfs_extend_mft
 *
 * allocates additional MFT records
 * sbi->mft.bitmap is locked for write
 *
 * NOTE: recursive:
 *	ntfs_look_free_mft ->
 *	ntfs_extend_mft ->
 *	attr_set_size ->
 *	ni_insert_nonresident ->
 *	ni_insert_attr ->
 *	ni_ins_attr_ext ->
 *	ntfs_look_free_mft ->
 *	ntfs_extend_mft
 * To avoid recursive always allocate space for two new mft records
 * see attrib.c: "at least two mft to avoid recursive loop"
 */
static int ntfs_extend_mft(ntfs_sb_info *sbi)
{
	int err;
	ntfs_inode *ni = sbi->mft.ni;
	size_t new_mft_total;
	u64 new_mft_bytes, new_bitmap_bytes;
	ATTRIB *attr;
	wnd_bitmap *wnd = &sbi->mft.bitmap;

	new_mft_total = (wnd->nbits + MFT_INCREASE_CHUNK + 127) & (CLST)~127;
	new_mft_bytes = (u64)new_mft_total << sbi->record_bits;

	/* Step 1: Resize $MFT::DATA */
	down_write(&ni->file.run_lock);
	err = attr_set_size(ni, ATTR_DATA, NULL, 0, &ni->file.run,
			    new_mft_bytes, NULL, false, &attr);

	if (err) {
		up_write(&ni->file.run_lock);
		goto out;
	}

	attr->nres.valid_size = attr->nres.data_size;
	new_mft_total = le64_to_cpu(attr->nres.alloc_size) >> sbi->record_bits;
	ni->mi.dirty = true;

	/* Step 2: Resize $MFT::BITMAP */
	new_bitmap_bytes = bitmap_size(new_mft_total);

	err = attr_set_size(ni, ATTR_BITMAP, NULL, 0, &sbi->mft.bitmap.run,
			    new_bitmap_bytes, &new_bitmap_bytes, true, NULL);

	/* Refresh Mft Zone if necessary */
	down_write_nested(&sbi->used.bitmap.rw_lock, BITMAP_MUTEX_CLUSTERS);

	ntfs_refresh_zone(sbi);

	up_write(&sbi->used.bitmap.rw_lock);
	up_write(&ni->file.run_lock);

	if (err)
		goto out;

	err = wnd_extend(wnd, new_mft_total);

	if (err)
		goto out;

	ntfs_clear_mft_tail(sbi, sbi->mft.used, new_mft_total);

	err = _ni_write_inode(&ni->vfs_inode, 0);
out:
	return err;
}

/*
 * ntfs_look_free_mft
 *
 * looks for a free MFT record
 */
int ntfs_look_free_mft(ntfs_sb_info *sbi, CLST *rno, bool mft, ntfs_inode *ni,
		       mft_inode **mi)
{
	int err = 0;
	size_t zbit, zlen, from, to, fr;
	size_t mft_total;
	MFT_REF ref;
	struct super_block *sb = sbi->sb;
	wnd_bitmap *wnd = &sbi->mft.bitmap;
	u32 ir;

	static_assert(sizeof(sbi->mft.reserved_bitmap) * 8 >=
		      MFT_REC_FREE - MFT_REC_RESERVED);

	if (!mft)
		down_write_nested(&wnd->rw_lock, BITMAP_MUTEX_MFT);

	zlen = wnd_zone_len(wnd);

	/* Always reserve space for MFT */
	if (zlen) {
		if (mft) {
			zbit = wnd_zone_bit(wnd);
			*rno = zbit;
			wnd_zone_set(wnd, zbit + 1, zlen - 1);
		}
		goto found;
	}

	/* No MFT zone. find the nearest to '0' free MFT */
	if (!wnd_find(wnd, 1, MFT_REC_FREE, 0, &zbit)) {
		/* Resize MFT */
		mft_total = wnd->nbits;

		err = ntfs_extend_mft(sbi);
		if (!err) {
			zbit = mft_total;
			goto reserve_mft;
		}

		if (!mft || MFT_REC_FREE == sbi->mft.next_reserved)
			goto out;

		err = 0;

		/*
		 * Look for free record reserved area [11-16) ==
		 * [MFT_REC_RESERVED, MFT_REC_FREE ) MFT bitmap always
		 * marks it as used
		 */
		if (sbi->mft.reserved_bitmap)
			goto scan_zone;

		/* Once per session create internal bitmap for 5 bits */
		sbi->mft.reserved_bitmap = 0xFF;

		ref.high = 0;
		for (ir = MFT_REC_RESERVED; ir < MFT_REC_FREE; ir++) {
			struct inode *i;
			ntfs_inode *ni;
			MFT_REC *mrec;

			ref.low = cpu_to_le32(ir);
			ref.seq = cpu_to_le16(ir);

			i = ntfs_iget5(sb, &ref, NULL);
			if (IS_ERR(i)) {
next:
				ntfs_trace(sb, "Invalid reserved record %x",
					   ref.low);
				continue;
			}
			if (is_bad_inode(i)) {
				iput(i);
				goto next;
			}

			ni = ntfs_i(i);

			mrec = ni->mi.mrec;

			if (!is_rec_base(mrec))
				goto next;

			if (mrec->hard_links)
				goto next;

			if (!ni_std(ni))
				goto next;

			if (ni_find_attr(ni, NULL, NULL, ATTR_NAME, NULL, 0,
					 NULL, NULL))
				goto next;

			__clear_bit(ir - MFT_REC_RESERVED,
				    &sbi->mft.reserved_bitmap);
		}

scan_zone:
		/* Scan 5 bits for zero. Bit 0 == MFT_REC_RESERVED */
		zbit = find_next_zero_bit(&sbi->mft.reserved_bitmap,
					  MFT_REC_FREE, MFT_REC_RESERVED);
		if (zbit >= MFT_REC_FREE) {
			sbi->mft.next_reserved = MFT_REC_FREE;
			goto out;
		}

		zlen = 1;
		sbi->mft.next_reserved = zbit;
	} else {
reserve_mft:
		zlen = zbit == MFT_REC_FREE ? (MFT_REC_USER - MFT_REC_FREE) : 4;
		if (zbit + zlen > wnd->nbits)
			zlen = wnd->nbits - zbit;

		while (zlen > 1 && !wnd_is_free(wnd, zbit, zlen))
			zlen -= 1;

		/* [zbit, zbit + zlen) will be used for Mft itself */
		from = sbi->mft.used;
		if (from < zbit)
			from = zbit;
		to = zbit + zlen;
		if (from < to) {
			ntfs_clear_mft_tail(sbi, from, to);
			sbi->mft.used = to;
		}
	}

	if (mft) {
		*rno = zbit;
		zbit += 1;
		zlen -= 1;
	}

	wnd_zone_set(wnd, zbit, zlen);

found:
	if (mft)
		goto mark_used;

	/* The request to get record for general purpose */
	if (sbi->mft.next_free < MFT_REC_USER)
		sbi->mft.next_free = MFT_REC_USER;

	for (;;) {
		if (sbi->mft.next_free >= sbi->mft.bitmap.nbits)
			;
		else if (!wnd_find(wnd, 1, MFT_REC_USER, 0, &fr))
			sbi->mft.next_free = sbi->mft.bitmap.nbits;
		else {
			*rno = fr;
			sbi->mft.next_free = *rno + 1;
			break;
		}

		err = ntfs_extend_mft(sbi);
		if (err)
			goto out;
	}

mark_used:

	if (ni && !ni_add_subrecord(ni, *rno, mi)) {
		err = -ENOMEM;
		goto out;
	}

	/* We have found a record that are not reserved for next MFT */
	if (*rno >= MFT_REC_FREE)
		wnd_set_used(wnd, *rno, 1);
	else if (*rno >= MFT_REC_RESERVED && sbi->mft.reserved_bitmap_inited)
		__set_bit(*rno - MFT_REC_RESERVED, &sbi->mft.reserved_bitmap);

out:
	if (!mft)
		up_write(&wnd->rw_lock);

	return err;
}

/*
 * ntfs_mark_rec_free
 *
 * marks record as free
 */
void ntfs_mark_rec_free(ntfs_sb_info *sbi, CLST rno)
{
	wnd_bitmap *wnd = &sbi->mft.bitmap;

	down_write_nested(&wnd->rw_lock, BITMAP_MUTEX_MFT);
	if (rno >= wnd->nbits)
		goto out;

	if (rno >= MFT_REC_FREE) {
		if (!wnd_is_used(wnd, rno, 1))
			ntfs_set_state(sbi, NTFS_DIRTY_ERROR);
		else
			wnd_set_free(wnd, rno, 1);
	} else if (rno >= MFT_REC_RESERVED && sbi->mft.reserved_bitmap_inited)
		__clear_bit(rno - MFT_REC_RESERVED, &sbi->mft.reserved_bitmap);

	if (rno < wnd_zone_bit(wnd))
		wnd_zone_set(wnd, rno, 1);
	else if (rno < sbi->mft.next_free && rno >= MFT_REC_USER)
		sbi->mft.next_free = rno;

out:
	up_write(&wnd->rw_lock);
}

/*
 * ntfs_clear_mft_tail
 *
 * formats empty records [from, to)
 * sbi->mft.bitmap is locked for write
 */
int ntfs_clear_mft_tail(ntfs_sb_info *sbi, size_t from, size_t to)
{
	int err;
	u32 rs;
	u64 vbo;
	struct runs_tree *run;
	ntfs_inode *ni;

	if (from >= to)
		return 0;

	rs = sbi->record_size;
	ni = sbi->mft.ni;
	run = &ni->file.run;

	down_read(&ni->file.run_lock);
	vbo = (u64)from * rs;
	for (; from < to; from++, vbo += rs) {
		struct ntfs_buffers nb;

		err = ntfs_get_bh(sbi, run, vbo, rs, &nb);
		if (err)
			goto out;

		err = ntfs_write_bh_ex(sbi, &sbi->new_rec->rhdr, &nb, 0);
		nb_put(&nb);
		if (err)
			goto out;
	}

out:
	sbi->mft.used = from;
	up_read(&ni->file.run_lock);

	return err;
}

/*
 * ntfs_refresh_zone
 *
 * refreshes Mft zone
 * sbi->used.bitmap is locked for rw
 * sbi->mft.bitmap is locked for write
 * sbi->mft.ni->file.run_lock for write
 */
int ntfs_refresh_zone(ntfs_sb_info *sbi)
{
	CLST zone_limit, zone_max, lcn, vcn, len;
	size_t lcn_s, zlen;
	wnd_bitmap *wnd = &sbi->used.bitmap;
	ntfs_inode *ni = sbi->mft.ni;

	/* Do not change anything unless we have non empty Mft zone */
	if (wnd_zone_len(wnd))
		return 0;

	/*
	 * Compute the mft zone at two steps
	 * It would be nice if we are able to allocate
	 * 1/8 of total clusters for MFT but not more then 512 MB
	 */
	zone_limit = (512 * 1024 * 1024) >> sbi->cluster_bits;
	zone_max = wnd->nbits >> 3;
	if (zone_max > zone_limit)
		zone_max = zone_limit;

	vcn = bytes_to_cluster(sbi,
			       (u64)sbi->mft.bitmap.nbits << sbi->record_bits);

	if (!run_lookup_entry(&ni->file.run, vcn - 1, &lcn, &len, NULL))
		lcn = SPARSE_LCN;

	/* We should always find Last Lcn for MFT */
	if (lcn == SPARSE_LCN)
		return -EINVAL;

	lcn_s = lcn + 1;

	/* Try to allocate clusters after last MFT run */
	zlen = wnd_find(wnd, zone_max, lcn_s, 0, &lcn_s);
	if (!zlen) {
		ntfs_trace(sbi->sb, "MftZone: unavailable");
		return 0;
	}

	/* Truncate too large zone */
	wnd_zone_set(wnd, lcn_s, zlen);

	return 0;
}

/*
 * ntfs_update_mftmirr
 *
 * updates $MFTMirr data
 */
int ntfs_update_mftmirr(ntfs_sb_info *sbi, int wait)
{
	int err;
	struct super_block *sb = sbi->sb;
	u32 blocksize = sb->s_blocksize;
	struct buffer_head *bh1, *bh2;
	sector_t block1, block2;
	u32 bytes;

	if (!(sbi->flags & NTFS_FLAGS_MFTMIRR))
		return 0;

	bytes = sbi->mft.recs_mirr << sbi->record_bits;
	block1 = sbi->mft.lbo >> sb->s_blocksize_bits;
	block2 = sbi->mft.lbo2 >> sb->s_blocksize_bits;

next_bh:
	bh1 = sb_bread(sb, block1);
	if (!bh1) {
		err = -EIO;
		goto out;
	}

	bh2 = sb_getblk(sb, block2);
	if (!bh2) {
		err = -EIO;
		goto out;
	}

	memcpy(bh2->b_data, bh1->b_data, blocksize);

	put_bh(bh1);
	bh1 = NULL;

	set_buffer_uptodate(bh2);
	mark_buffer_dirty(bh2);

	if (wait) {
		err = sync_dirty_buffer(bh2);
		if (err)
			goto out;
	}

	put_bh(bh2);
	bh2 = NULL;

	if (bytes > blocksize) {
		bytes -= blocksize;
		block1 += 1;
		block2 += 1;
		goto next_bh;
	}

	err = 0;

	sbi->flags &= ~NTFS_FLAGS_MFTMIRR;

out:
	if (bh1)
		put_bh(bh1);
	if (bh2)
		put_bh(bh2);

	return err;
}

/*
 * ntfs_set_state
 *
 * mount: ntfs_set_state(NTFS_DIRTY_DIRTY)
 * umount: ntfs_set_state(NTFS_DIRTY_CLEAR)
 * ntfs error: ntfs_set_state(NTFS_DIRTY_ERROR)
 */
int ntfs_set_state(ntfs_sb_info *sbi, enum NTFS_DIRTY_FLAGS dirty)
{
	int err;
	ATTRIB *attr;
	VOLUME_INFO *info;
	mft_inode *mi;
	ntfs_inode *ni;

	/*
	 * do not change state if fs was real_dirty
	 * do not change state if fs already dirty(clear)
	 * do not change any thing if mounted read only
	 */
	if (sbi->volume.real_dirty || sb_rdonly(sbi->sb))
		return 0;

	/* Check cached value */
	if ((dirty == NTFS_DIRTY_CLEAR ? 0 : VOLUME_FLAG_DIRTY) ==
	    (sbi->volume.flags & VOLUME_FLAG_DIRTY))
		return 0;

	ni = sbi->volume.ni;
	if (!ni)
		return -EINVAL;

	inode_lock(&ni->vfs_inode);

	attr = ni_find_attr(ni, NULL, NULL, ATTR_VOL_INFO, NULL, 0, NULL, &mi);
	if (!attr) {
		err = -EINVAL;
		goto out;
	}

	info = resident_data_ex(attr, SIZEOF_ATTRIBUTE_VOLUME_INFO);
	if (!info) {
		err = -EINVAL;
		goto out;
	}

	switch (dirty) {
	case NTFS_DIRTY_ERROR:
		ntfs_trace(sbi->sb, "Mark volume as dirty due to NTFS errors");
		sbi->volume.real_dirty = true;
		fallthrough;
	case NTFS_DIRTY_DIRTY:
		info->flags |= VOLUME_FLAG_DIRTY;
		break;
	case NTFS_DIRTY_CLEAR:
		info->flags &= ~VOLUME_FLAG_DIRTY;
		break;
	}
	/* cache current volume flags*/
	sbi->volume.flags = info->flags;
	mi->dirty = true;
	err = 0;

out:
	inode_unlock(&ni->vfs_inode);
	if (err)
		return err;

	mark_inode_dirty(&ni->vfs_inode);
	/*verify(!ntfs_update_mftmirr()); */
	err = sync_inode_metadata(&ni->vfs_inode, 1);

	return err;
}

/*
 * security_hash
 *
 * calculates a hash of security descriptor
 */
static inline __le32 security_hash(const void *sd, size_t bytes)
{
	u32 hash = 0;
	const __le32 *ptr = sd;

	bytes >>= 2;
	while (bytes--)
		hash = ((hash >> 0x1D) | (hash << 3)) + le32_to_cpu(*ptr++);
	return cpu_to_le32(hash);
}

int ntfs_sb_read(struct super_block *sb, u64 lbo, size_t bytes, void *buffer)
{
	struct block_device *bdev = sb->s_bdev;
	u32 blocksize = sb->s_blocksize;
	u64 block = lbo >> sb->s_blocksize_bits;
	u32 off = lbo & (blocksize - 1);
	u32 op = blocksize - off;

	for (; bytes; block += 1, off = 0, op = blocksize) {
		struct buffer_head *bh = __bread(bdev, block, blocksize);

		if (!bh)
			return -EIO;

		if (op > bytes)
			op = bytes;

		memcpy(buffer, bh->b_data + off, op);

		put_bh(bh);

		bytes -= op;
		buffer = Add2Ptr(buffer, op);
	}

	return 0;
}

int ntfs_sb_write(struct super_block *sb, u64 lbo, size_t bytes,
		  const void *buf, int wait)
{
	u32 blocksize = sb->s_blocksize;
	struct block_device *bdev = sb->s_bdev;
	sector_t block = lbo >> sb->s_blocksize_bits;
	u32 off = lbo & (blocksize - 1);
	u32 op = blocksize - off;
	struct buffer_head *bh;

	if (!wait && (sb->s_flags & SB_SYNCHRONOUS))
		wait = 1;

	for (; bytes; block += 1, off = 0, op = blocksize) {
		if (op > bytes)
			op = bytes;

		if (op < blocksize) {
			bh = __bread(bdev, block, blocksize);
			if (!bh) {
				ntfs_error(sb, "failed to read block %llx",
					   (u64)block);
				return -EIO;
			}
		} else {
			bh = __getblk(bdev, block, blocksize);
			if (!bh)
				return -ENOMEM;
		}

		if (buffer_locked(bh))
			__wait_on_buffer(bh);

		lock_buffer(bh);
		if (buf) {
			memcpy(bh->b_data + off, buf, op);
			buf = Add2Ptr(buf, op);
		} else {
			memset(bh->b_data + off, -1, op);
		}

		set_buffer_uptodate(bh);
		mark_buffer_dirty(bh);
		unlock_buffer(bh);

		if (wait) {
			int err = sync_dirty_buffer(bh);

			if (err) {
				ntfs_error(
					sb,
					"failed to sync buffer at block %llx, error %d",
					(u64)block, err);
				put_bh(bh);
				return err;
			}
		}

		put_bh(bh);

		bytes -= op;
	}
	return 0;
}

int ntfs_sb_write_run(ntfs_sb_info *sbi, struct runs_tree *run, u64 vbo,
		      const void *buf, size_t bytes)
{
	struct super_block *sb = sbi->sb;
	u8 cluster_bits = sbi->cluster_bits;
	u32 off = vbo & sbi->cluster_mask;
	CLST lcn, clen;
	u64 lbo, len;
	size_t idx;

	if (!run_lookup_entry(run, vbo >> cluster_bits, &lcn, &clen, &idx))
		return -ENOENT;

	if (lcn == SPARSE_LCN)
		return -EINVAL;

	lbo = ((u64)lcn << cluster_bits) + off;
	len = ((u64)clen << cluster_bits) - off;

	for (;;) {
		u32 op = len < bytes ? len : bytes;
		int err = ntfs_sb_write(sb, lbo, op, buf, 0);

		if (err)
			return err;

		bytes -= op;
		if (!bytes)
			break;

		if (!run_get_entry(run, ++idx, NULL, &lcn, &clen))
			return -ENOENT;

		if (lcn == SPARSE_LCN)
			return -EINVAL;

		if (buf)
			buf = Add2Ptr(buf, op);

		lbo = ((u64)lcn << cluster_bits) + off;
		len = ((u64)clen << cluster_bits) - off;
	}

	return 0;
}

struct buffer_head *ntfs_bread_run(ntfs_sb_info *sbi, struct runs_tree *run,
				   u64 vbo)
{
	struct super_block *sb = sbi->sb;
	u8 cluster_bits = sbi->cluster_bits;
	CLST lcn;
	u64 lbo;

	if (!run_lookup_entry(run, vbo >> cluster_bits, &lcn, NULL, NULL))
		return ERR_PTR(-ENOENT);

	lbo = ((u64)lcn << cluster_bits) + (vbo & sbi->cluster_mask);

	return ntfs_bread(sb, lbo >> sb->s_blocksize_bits);
}

int ntfs_read_run_nb(ntfs_sb_info *sbi, struct runs_tree *run, u64 vbo,
		     void *buf, u32 bytes, struct ntfs_buffers *nb)
{
	int err;
	struct super_block *sb = sbi->sb;
	u32 blocksize = sb->s_blocksize;
	u8 cluster_bits = sbi->cluster_bits;
	u32 off = vbo & sbi->cluster_mask;
	u32 nbh = 0;
	CLST lcn, clen;
	u64 lbo, len;
	size_t idx;
	struct buffer_head *bh;

	if (!run) {
		/* first reading of $Volume + $MFTMirr + $LogFile goes here*/
		if (vbo > MFT_REC_VOL * sbi->record_size) {
			err = -ENOENT;
			goto out;
		}

		/* use absolute boot's 'MFTCluster' to read record */
		lbo = vbo + sbi->mft.lbo;
		len = sbi->record_size;
	} else if (!run_lookup_entry(run, vbo >> cluster_bits, &lcn, &clen,
				     &idx)) {
		err = -ENOENT;
		goto out;
	} else {
		if (lcn == SPARSE_LCN) {
			err = -EINVAL;
			goto out;
		}

		lbo = ((u64)lcn << cluster_bits) + off;
		len = ((u64)clen << cluster_bits) - off;
	}

	off = lbo & (blocksize - 1);
	if (nb) {
		nb->off = off;
		nb->bytes = bytes;
	}

	for (;;) {
		u32 len32 = len >= bytes ? bytes : len;
		sector_t block = lbo >> sb->s_blocksize_bits;

		do {
			u32 op = blocksize - off;

			if (op > len32)
				op = len32;

			bh = ntfs_bread(sb, block);
			if (!bh) {
				err = -EIO;
				goto out;
			}

			if (buf) {
				memcpy(buf, bh->b_data + off, op);
				buf = Add2Ptr(buf, op);
			}

			if (!nb) {
				put_bh(bh);
			} else if (nbh >= ARRAY_SIZE(nb->bh)) {
				err = -EINVAL;
				goto out;
			} else {
				nb->bh[nbh++] = bh;
				nb->nbufs = nbh;
			}

			bytes -= op;
			if (!bytes)
				return 0;
			len32 -= op;
			block += 1;
			off = 0;

		} while (len32);

		if (!run_get_entry(run, ++idx, NULL, &lcn, &clen)) {
			err = -ENOENT;
			goto out;
		}

		if (lcn == SPARSE_LCN) {
			err = -EINVAL;
			goto out;
		}

		lbo = ((u64)lcn << cluster_bits);
		len = ((u64)clen << cluster_bits);
	}

out:
	if (!nbh)
		return err;

	while (nbh) {
		put_bh(nb->bh[--nbh]);
		nb->bh[nbh] = NULL;
	}

	nb->nbufs = 0;
	return err;
}

/* Returns < 0 if error, 0 if ok, 1 if need to update fixups */
int ntfs_read_bh_ex(ntfs_sb_info *sbi, struct runs_tree *run, u64 vbo,
		    NTFS_RECORD_HEADER *rhdr, u32 bytes,
		    struct ntfs_buffers *nb)
{
	int err = ntfs_read_run_nb(sbi, run, vbo, rhdr, bytes, nb);

	if (err)
		return err;

	return ntfs_fix_post_read(rhdr, nb->bytes, true);
}

int ntfs_get_bh(ntfs_sb_info *sbi, struct runs_tree *run, u64 vbo, u32 bytes,
		struct ntfs_buffers *nb)
{
	int err = 0;
	struct super_block *sb = sbi->sb;
	u32 blocksize = sb->s_blocksize;
	u8 cluster_bits = sbi->cluster_bits;
	u32 off;
	u32 nbh = 0;
	CLST lcn, clen;
	u64 lbo, len;
	size_t idx;

	nb->bytes = bytes;

	if (!run_lookup_entry(run, vbo >> cluster_bits, &lcn, &clen, &idx)) {
		err = -ENOENT;
		goto out;
	}

	off = vbo & sbi->cluster_mask;
	lbo = ((u64)lcn << cluster_bits) + off;
	len = ((u64)clen << cluster_bits) - off;

	nb->off = off = lbo & (blocksize - 1);

	for (;;) {
		u32 len32 = len < bytes ? len : bytes;
		sector_t block = lbo >> sb->s_blocksize_bits;

		do {
			u32 op;
			struct buffer_head *bh;

			if (nbh >= ARRAY_SIZE(nb->bh)) {
				err = -EINVAL;
				goto out;
			}

			op = blocksize - off;
			if (op > len32)
				op = len32;

			if (op == blocksize) {
				bh = sb_getblk(sb, block);
				if (!bh) {
					err = -ENOMEM;
					goto out;
				}
				if (buffer_locked(bh))
					__wait_on_buffer(bh);
				set_buffer_uptodate(bh);
			} else {
				bh = ntfs_bread(sb, block);
				if (!bh) {
					err = -EIO;
					goto out;
				}
			}

			nb->bh[nbh++] = bh;
			bytes -= op;
			if (!bytes) {
				nb->nbufs = nbh;
				return 0;
			}

			block += 1;
			len32 -= op;
			off = 0;
		} while (len32);

		if (!run_get_entry(run, ++idx, NULL, &lcn, &clen)) {
			err = -ENOENT;
			goto out;
		}

		lbo = ((u64)lcn << cluster_bits);
		len = ((u64)clen << cluster_bits);
	}

out:
	while (nbh) {
		put_bh(nb->bh[--nbh]);
		nb->bh[nbh] = NULL;
	}

	nb->nbufs = 0;

	return err;
}

static int ntfs_write_bh(ntfs_sb_info *sbi, const void *buf,
			 struct ntfs_buffers *nb, int sync)

{
	int err = 0;
	struct super_block *sb = sbi->sb;
	u32 idx;
	u32 bytes = nb->bytes;
	u32 off = nb->off;

	for (idx = 0; bytes; idx += 1, off = 0) {
		u32 op = sb->s_blocksize - off;
		struct buffer_head *bh = nb->bh[idx];

		if (!bh)
			return err;

		if (op > bytes)
			op = bytes;
		if (buffer_locked(bh))
			__wait_on_buffer(bh);

		lock_buffer(bh);

		if (buf) {
			memcpy(Add2Ptr(bh->b_data, off), buf, op);
			buf = Add2Ptr(buf, op);
		} else {
			memset(Add2Ptr(bh->b_data, off), 0, op);
		}

		set_buffer_uptodate(bh);
		mark_buffer_dirty(bh);
		unlock_buffer(bh);

		if (sync) {
			int err2 = sync_dirty_buffer(bh);

			if (!err && err2)
				err = err2;
		}

		bytes -= op;
	}

	return err;
}

int ntfs_write_bh_ex(ntfs_sb_info *sbi, NTFS_RECORD_HEADER *rhdr,
		     struct ntfs_buffers *nb, int sync)
{
	int err;

	ntfs_fix_pre_write(rhdr, nb->bytes);

	err = ntfs_write_bh(sbi, rhdr, nb, sync);

	if (ntfs_fix_post_read(rhdr, nb->bytes, true))
		err = 1;

	return err;
}

int ntfs_vbo_to_pbo(ntfs_sb_info *sbi, struct runs_tree *run, u64 vbo, u64 *pbo,
		    u64 *bytes)
{
	u32 off;
	CLST lcn, len;
	u8 cluster_bits = sbi->cluster_bits;

	if (!run_lookup_entry(run, vbo >> cluster_bits, &lcn, &len, NULL))
		return -ENOENT;

	off = vbo & sbi->cluster_mask;
	*pbo = lcn == SPARSE_LCN ? -1 : (((u64)lcn << cluster_bits) + off);
	*bytes = ((u64)len << cluster_bits) - off;

	return 0;
}

ntfs_inode *ntfs_new_inode(ntfs_sb_info *sbi, CLST rno, bool dir)
{
	int err = 0;
	struct super_block *sb = sbi->sb;
	struct inode *inode = new_inode(sb);
	ntfs_inode *ni;

	if (!inode)
		return ERR_PTR(-ENOMEM);

	ni = ntfs_i(inode);

	err = mi_format_new(&ni->mi, sbi, rno, dir ? RECORD_FLAG_DIR : 0,
			    false);
	if (err)
		goto out;

	inode->i_ino = rno;
	__insert_inode_hash(inode, rno);

out:
	if (err) {
		iput(inode);
		ni = ERR_PTR(err);
	}
	return ni;
}

/* O:BAG:BAD:(A;OICI;FA;;;WD) */
const u8 s_dir_security[] __aligned(8) = {
	0x01, 0x00, 0x04, 0x80, 0x30, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x02, 0x00, 0x1C, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x14, 0x00, 0xFF, 0x01, 0x1F, 0x00,
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x20, 0x00, 0x00, 0x00,
	0x20, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
	0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00,
};

static_assert(sizeof(s_dir_security) == 0x50);

const u8 s_file_security[] __aligned(8) = {
	0x01, 0x00, 0x04, 0x94, 0x30, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x1c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0xff,
	0x01, 0x1f, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x05, 0x15, 0x00, 0x00, 0x00, 0x1b, 0x70, 0x7d, 0x76, 0x9d,
	0x84, 0xb0, 0xf7, 0x79, 0x42, 0x1c, 0xeb, 0x01, 0x02, 0x00, 0x00,
};

static_assert(sizeof(s_file_security) == 0x58);

/*
 * ntfs_security_init
 *
 * loads and parse $Secure
 */
int ntfs_security_init(ntfs_sb_info *sbi)
{
	int err;
	struct super_block *sb = sbi->sb;
	struct inode *inode;
	ntfs_inode *ni;
	MFT_REF ref;
	ATTRIB *attr;
	ATTR_LIST_ENTRY *le;
	u64 sds_size;
	size_t cnt, off;
	NTFS_DE *ne;
	NTFS_DE_SII *sii_e;
	struct ntfs_fnd *fnd_sii = NULL;
	const INDEX_ROOT *root_sii;
	const INDEX_ROOT *root_sdh;
	ntfs_index *indx_sdh = &sbi->security.index_sdh;
	ntfs_index *indx_sii = &sbi->security.index_sii;

	ref.low = cpu_to_le32(MFT_REC_SECURE);
	ref.high = 0;
	ref.seq = cpu_to_le16(MFT_REC_SECURE);

	inode = ntfs_iget5(sb, &ref, &NAME_SECURE);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		ntfs_error(sbi->sb, "Failed to load $Secure.");
		inode = NULL;
		goto out;
	}

	ni = ntfs_i(inode);

	le = NULL;

	attr = ni_find_attr(ni, NULL, &le, ATTR_ROOT, SDH_NAME,
			    ARRAY_SIZE(SDH_NAME), NULL, NULL);
	if (!attr) {
		err = -EINVAL;
		goto out;
	}

	root_sdh = resident_data(attr);
	if (root_sdh->type != ATTR_ZERO ||
	    root_sdh->rule != NTFS_COLLATION_TYPE_SECURITY_HASH) {
		err = -EINVAL;
		goto out;
	}

	err = indx_init(indx_sdh, sbi, attr, INDEX_MUTEX_SDH);
	if (err)
		goto out;

	attr = ni_find_attr(ni, attr, &le, ATTR_ROOT, SII_NAME,
			    ARRAY_SIZE(SII_NAME), NULL, NULL);
	if (!attr) {
		err = -EINVAL;
		goto out;
	}

	root_sii = resident_data(attr);
	if (root_sii->type != ATTR_ZERO ||
	    root_sii->rule != NTFS_COLLATION_TYPE_UINT) {
		err = -EINVAL;
		goto out;
	}

	err = indx_init(indx_sii, sbi, attr, INDEX_MUTEX_SII);
	if (err)
		goto out;

	fnd_sii = fnd_get(indx_sii);
	if (!fnd_sii) {
		err = -ENOMEM;
		goto out;
	}

	sds_size = inode->i_size;

	/* Find the last valid Id */
	sbi->security.next_id = SECURITY_ID_FIRST;
	/* Always write new security at the end of bucket */
	sbi->security.next_off =
		Quad2Align(sds_size - SecurityDescriptorsBlockSize);

	cnt = 0;
	off = 0;
	ne = NULL;

	for (;;) {
		u32 next_id;

		err = indx_find_raw(indx_sii, ni, root_sii, &ne, &off, fnd_sii);
		if (err || !ne)
			break;

		sii_e = (NTFS_DE_SII *)ne;
		if (le16_to_cpu(ne->View.data_size) < SIZEOF_SECURITY_HDR)
			continue;

		next_id = le32_to_cpu(sii_e->sec_id) + 1;
		if (next_id >= sbi->security.next_id)
			sbi->security.next_id = next_id;

		cnt += 1;
	}

	sbi->security.ni = ni;
	inode = NULL;
out:
	iput(inode);
	fnd_put(fnd_sii);

	return err;
}

/*
 * ntfs_get_security_by_id
 *
 * reads security descriptor by id
 */
int ntfs_get_security_by_id(ntfs_sb_info *sbi, u32 security_id, void **sd,
			    size_t *size)
{
	int err;
	int diff;
	ntfs_inode *ni = sbi->security.ni;
	ntfs_index *indx = &sbi->security.index_sii;
	void *p = NULL;
	NTFS_DE_SII *sii_e;
	struct ntfs_fnd *fnd_sii;
	SECURITY_HDR d_security;
	const INDEX_ROOT *root_sii;
	u32 t32;

	*sd = NULL;

	inode_lock_shared(&ni->vfs_inode);

	fnd_sii = fnd_get(indx);
	if (!fnd_sii) {
		err = -ENOMEM;
		goto out;
	}

	root_sii = indx_get_root(indx, ni, NULL, NULL);
	if (!root_sii) {
		err = -EINVAL;
		goto out;
	}

	/* Try to find this SECURITY descriptor in SII and SDH indexes */
	err = indx_find(indx, ni, root_sii, &security_id, sizeof(security_id),
			NULL, &diff, (NTFS_DE **)&sii_e, fnd_sii);
	if (err)
		goto out;

	if (diff)
		goto out;

	t32 = le32_to_cpu(sii_e->sec_hdr.size);
	if (t32 < SIZEOF_SECURITY_HDR) {
		err = -EINVAL;
		goto out;
	}

	*size = t32 - SIZEOF_SECURITY_HDR;

	p = ntfs_alloc(*size, 0);
	if (p) {
		err = -ENOMEM;
		goto out;
	}

	err = ntfs_read_run_nb(sbi, &ni->file.run,
			       le64_to_cpu(sii_e->sec_hdr.off), &d_security,
			       sizeof(d_security), NULL);
	if (err)
		goto out;

	if (memcmp(&d_security, &sii_e->sec_hdr, SIZEOF_SECURITY_HDR)) {
		err = -EINVAL;
		goto out;
	}

	err = ntfs_read_run_nb(sbi, &ni->file.run,
			       le64_to_cpu(sii_e->sec_hdr.off) +
				       SIZEOF_SECURITY_HDR,
			       p, *size, NULL);
	if (err)
		goto out;

	*sd = p;
	p = NULL;

out:
	ntfs_free(p);
	fnd_put(fnd_sii);
	inode_unlock_shared(&ni->vfs_inode);

	return err;
}

/*
 * ntfs_insert_security
 *
 * inserts security descriptor into $Secure::SDS
 *
 * SECURITY Descriptor Stream data is organized into chunks of 256K bytes
 * and it contains a mirror copy of each security descriptor.  When writing
 * to a security descriptor at location X, another copy will be written at
 * location (X+256K).
 * When writing a security descriptor that will cross the 256K boundary,
 * the pointer will be advanced by 256K to skip
 * over the mirror portion.
 */
int ntfs_insert_security(ntfs_sb_info *sbi, const void *sd, u32 size_sd,
			 __le32 *security_id, bool *inserted)
{
	int err, diff;
	ntfs_inode *ni = sbi->security.ni;
	ntfs_index *indx_sdh = &sbi->security.index_sdh;
	ntfs_index *indx_sii = &sbi->security.index_sii;
	NTFS_DE_SDH *e;
	NTFS_DE_SDH sdh_e;
	NTFS_DE_SII sii_e;
	SECURITY_HDR *d_security;
	u32 new_sec_size = size_sd + SIZEOF_SECURITY_HDR;
	u32 aligned_sec_size = Quad2Align(new_sec_size);
	SECURITY_KEY hash_key;
	struct ntfs_fnd *fnd_sdh = NULL;
	const INDEX_ROOT *root_sdh;
	const INDEX_ROOT *root_sii;
	u64 mirr_off, new_sds_size;
	u32 used, next, left;

	static_assert((1 << Log2OfSecurityDescriptorsBlockSize) ==
		      SecurityDescriptorsBlockSize);

	hash_key.hash = security_hash(sd, size_sd);
	hash_key.sec_id = SECURITY_ID_INVALID;

	if (inserted)
		*inserted = false;
	*security_id = SECURITY_ID_INVALID;

	/* Allocate a temporal buffer*/
	d_security = ntfs_alloc(aligned_sec_size, 1);
	if (!d_security)
		return -ENOMEM;

	inode_lock(&ni->vfs_inode);

	fnd_sdh = fnd_get(indx_sdh);
	if (!fnd_sdh) {
		err = -ENOMEM;
		goto out;
	}

	root_sdh = indx_get_root(indx_sdh, ni, NULL, NULL);
	if (!root_sdh) {
		err = -EINVAL;
		goto out;
	}

	root_sii = indx_get_root(indx_sii, ni, NULL, NULL);
	if (!root_sii) {
		err = -EINVAL;
		goto out;
	}

	err = indx_find(indx_sdh, ni, root_sdh, &hash_key, sizeof(hash_key),
			&d_security->key.sec_id, &diff, (NTFS_DE **)&e,
			fnd_sdh);
	if (err)
		goto out;

	if (!e)
		goto insert_security;

next_security:
	if (le32_to_cpu(e->sec_hdr.size) != new_sec_size)
		goto skip_read_sds;

	err = ntfs_read_run_nb(sbi, &ni->file.run, le64_to_cpu(e->sec_hdr.off),
			       d_security, new_sec_size, NULL);
	if (err)
		goto out;

	if (le32_to_cpu(d_security->size) == new_sec_size &&
	    d_security->key.hash == hash_key.hash &&
	    !memcmp(d_security + 1, sd, size_sd)) {
		*security_id = d_security->key.sec_id;
		err = 0;
		goto out;
	}

skip_read_sds:
	err = indx_find_sort(indx_sdh, ni, root_sdh, (NTFS_DE **)&e, fnd_sdh);
	if (err)
		goto out;

	if (!e || e->key.hash == hash_key.hash)
		goto next_security;

insert_security:

	/* Zero unused space */
	next = sbi->security.next_off & (SecurityDescriptorsBlockSize - 1);
	left = SecurityDescriptorsBlockSize - next;

	/* Zero gap until SecurityDescriptorsBlockSize */
	if (left < new_sec_size) {
		/* zero "left" bytes from sbi->security.next_off */
		sbi->security.next_off += SecurityDescriptorsBlockSize + left;
	}

	/* Zero tail of previous security */
	used = ni->vfs_inode.i_size & (SecurityDescriptorsBlockSize - 1);

	/*
	 * Example:
	 * 0x40438 == ni->vfs_inode.i_size
	 * 0x00440 == sbi->security.next_off
	 * need to zero [0x438-0x440)
	 * if (next > used) {
	 *  u32 tozero = next - used;
	 *  zero "tozero" bytes from sbi->security.next_off - tozero
	 */

	/* format new security descriptor */
	d_security->key.hash = hash_key.hash;
	d_security->key.sec_id = cpu_to_le32(sbi->security.next_id);
	d_security->off = cpu_to_le64(sbi->security.next_off);
	d_security->size = cpu_to_le32(new_sec_size);
	memcpy(d_security + 1, sd, size_sd);

	/* Write main SDS bucket */
	err = ntfs_sb_write_run(sbi, &ni->file.run, sbi->security.next_off,
				d_security, aligned_sec_size);

	if (err)
		goto out;

	mirr_off = sbi->security.next_off + SecurityDescriptorsBlockSize;
	new_sds_size = mirr_off + aligned_sec_size;

	if (new_sds_size > ni->vfs_inode.i_size) {
		err = attr_set_size(ni, ATTR_DATA, SDS_NAME,
				    ARRAY_SIZE(SDS_NAME), &ni->file.run,
				    new_sds_size, &new_sds_size, false, NULL);
		if (err)
			goto out;
	}

	/* Write copy SDS bucket */
	err = ntfs_sb_write_run(sbi, &ni->file.run, mirr_off, d_security,
				aligned_sec_size);
	if (err)
		goto out;

	/* Fill SII entry */
	sii_e.de.View.data_off = cpu_to_le16(offsetof(NTFS_DE_SII, sec_hdr));
	sii_e.de.View.data_size = cpu_to_le16(SIZEOF_SECURITY_HDR);
	sii_e.de.View.Res = 0; //??
	sii_e.de.size = cpu_to_le16(SIZEOF_SII_DIRENTRY);
	sii_e.de.key_size = cpu_to_le16(sizeof(d_security->key.sec_id));
	sii_e.de.flags = 0;
	sii_e.de.Reserved = 0;
	sii_e.sec_id = d_security->key.sec_id;
	memcpy(&sii_e.sec_hdr, d_security, SIZEOF_SECURITY_HDR);

	err = indx_insert_entry(indx_sii, ni, &sii_e.de, NULL, NULL);
	if (err)
		goto out;

	/* Fill SDH entry */
	sdh_e.de.View.data_off = cpu_to_le16(offsetof(NTFS_DE_SDH, sec_hdr));
	sdh_e.de.View.data_size = cpu_to_le16(SIZEOF_SECURITY_HDR);
	sdh_e.de.View.Res = 0;
	sdh_e.de.size = cpu_to_le16(SIZEOF_SDH_DIRENTRY);
	sdh_e.de.key_size = cpu_to_le16(sizeof(sdh_e.key));
	sdh_e.de.flags = 0;
	sdh_e.de.Reserved = 0;
	sdh_e.key.hash = d_security->key.hash;
	sdh_e.key.sec_id = d_security->key.sec_id;
	memcpy(&sdh_e.sec_hdr, d_security, SIZEOF_SECURITY_HDR);
	sdh_e.magic[0] = cpu_to_le16('I');
	sdh_e.magic[1] = cpu_to_le16('I');

	fnd_clear(fnd_sdh);
	err = indx_insert_entry(indx_sdh, ni, &sdh_e.de, (void *)(size_t)1,
				fnd_sdh);
	if (err)
		goto out;

	*security_id = d_security->key.sec_id;
	if (inserted)
		*inserted = true;

	/* Update Id and offset for next descriptor */
	sbi->security.next_id += 1;
	sbi->security.next_off += aligned_sec_size;

out:
	fnd_put(fnd_sdh);
	inode_unlock(&ni->vfs_inode);
	ntfs_free(d_security);

	return err;
}

/*
 * ntfs_reparse_init
 *
 * loads and parse $Extend/$Reparse
 */
int ntfs_reparse_init(ntfs_sb_info *sbi)
{
	int err;
	ntfs_inode *ni = sbi->reparse.ni;
	ntfs_index *indx = &sbi->reparse.index_r;
	ATTRIB *attr;
	ATTR_LIST_ENTRY *le;
	const INDEX_ROOT *root_r;

	if (!ni)
		return 0;

	le = NULL;
	attr = ni_find_attr(ni, NULL, &le, ATTR_ROOT, SR_NAME,
			    ARRAY_SIZE(SR_NAME), NULL, NULL);
	if (!attr) {
		err = -EINVAL;
		goto out;
	}

	root_r = resident_data(attr);
	if (root_r->type != ATTR_ZERO ||
	    root_r->rule != NTFS_COLLATION_TYPE_UINTS) {
		err = -EINVAL;
		goto out;
	}

	err = indx_init(indx, sbi, attr, INDEX_MUTEX_SR);
	if (err)
		goto out;

out:
	return err;
}

/*
 * ntfs_objid_init
 *
 * loads and parse $Extend/$ObjId
 */
int ntfs_objid_init(ntfs_sb_info *sbi)
{
	int err;
	ntfs_inode *ni = sbi->objid.ni;
	ntfs_index *indx = &sbi->objid.index_o;
	ATTRIB *attr;
	ATTR_LIST_ENTRY *le;
	const INDEX_ROOT *root;

	if (!ni)
		return 0;

	le = NULL;
	attr = ni_find_attr(ni, NULL, &le, ATTR_ROOT, SO_NAME,
			    ARRAY_SIZE(SO_NAME), NULL, NULL);
	if (!attr) {
		err = -EINVAL;
		goto out;
	}

	root = resident_data(attr);
	if (root->type != ATTR_ZERO ||
	    root->rule != NTFS_COLLATION_TYPE_UINTS) {
		err = -EINVAL;
		goto out;
	}

	err = indx_init(indx, sbi, attr, INDEX_MUTEX_SO);
	if (err)
		goto out;

out:
	return err;
}

int ntfs_objid_remove(ntfs_sb_info *sbi, GUID *guid)
{
	int err;
	ntfs_inode *ni = sbi->objid.ni;
	ntfs_index *indx = &sbi->objid.index_o;

	if (!ni)
		return -EINVAL;

	inode_lock(&ni->vfs_inode);

	err = indx_delete_entry(indx, ni, guid, sizeof(*guid), NULL);

	inode_unlock(&ni->vfs_inode);

	return err;
}

int ntfs_insert_reparse(ntfs_sb_info *sbi, __le32 rtag, const MFT_REF *ref)
{
	int err;
	ntfs_inode *ni = sbi->reparse.ni;
	ntfs_index *indx = &sbi->reparse.index_r;
	NTFS_DE_R re;

	if (!ni)
		return -EINVAL;

	memset(&re, 0, sizeof(re));

	re.Key.ReparseTag = rtag;
	memcpy(&re.Key.ref, ref, sizeof(*ref));

	re.de.View.data_off = cpu_to_le16(offsetof(NTFS_DE_R, Key));
	re.de.size = cpu_to_le16(QuadAlign(SIZEOF_R_DIRENTRY));
	re.de.key_size = cpu_to_le16(sizeof(re.Key));

	inode_lock(&ni->vfs_inode);

	err = indx_insert_entry(indx, ni, &re.de, NULL, NULL);

	inode_unlock(&ni->vfs_inode);

	return err;
}

int ntfs_remove_reparse(ntfs_sb_info *sbi, __le32 rtag, const MFT_REF *ref)
{
	int err;
	ntfs_inode *ni = sbi->reparse.ni;
	ntfs_index *indx = &sbi->reparse.index_r;
	REPARSE_KEY rkey;
	int diff;
	NTFS_DE_R *re;
	struct ntfs_fnd *fnd = NULL;
	INDEX_ROOT *root_r;

	if (!ni)
		return -EINVAL;

	rkey.ReparseTag = rtag;
	rkey.ref = *ref;

	inode_lock(&ni->vfs_inode);

	if (rtag) {
		err = indx_delete_entry(indx, ni, &rkey, sizeof(rkey), NULL);
		goto out1;
	}

	fnd = fnd_get(indx);
	if (!fnd) {
		err = -ENOMEM;
		goto out1;
	}

	root_r = indx_get_root(indx, ni, NULL, NULL);
	if (!root_r) {
		err = -EINVAL;
		goto out;
	}

	err = indx_find(indx, ni, root_r, &rkey, sizeof(rkey), NULL, &diff,
			(NTFS_DE **)&re, fnd);
	if (err)
		goto out;

	if (memcmp(&re->Key.ref, ref, sizeof(*ref)))
		goto out;

	memcpy(&rkey, &re->Key, sizeof(rkey));

	fnd_put(fnd);
	fnd = NULL;

	err = indx_delete_entry(indx, ni, &rkey, sizeof(rkey), NULL);
	if (err)
		goto out;

out:
	fnd_put(fnd);

out1:
	inode_unlock(&ni->vfs_inode);

	return err;
}

static inline void ntfs_unmap_and_discard(ntfs_sb_info *sbi, CLST lcn, CLST len)
{
	ntfs_unmap_meta(sbi->sb, lcn, len);
	ntfs_discard(sbi, lcn, len);
}

void mark_as_free_ex(ntfs_sb_info *sbi, CLST lcn, CLST len, bool trim)
{
	CLST end, i;
	wnd_bitmap *wnd = &sbi->used.bitmap;

	down_write_nested(&wnd->rw_lock, BITMAP_MUTEX_CLUSTERS);
	if (!wnd_is_used(wnd, lcn, len)) {
		ntfs_set_state(sbi, NTFS_DIRTY_ERROR);

		end = lcn + len;
		len = 0;
		for (i = lcn; i < end; i++) {
			if (wnd_is_used(wnd, i, 1)) {
				if (!len)
					lcn = i;
				len += 1;
				continue;
			}

			if (!len)
				continue;

			if (trim)
				ntfs_unmap_and_discard(sbi, lcn, len);

			wnd_set_free(wnd, lcn, len);
			len = 0;
		}

		if (!len)
			goto out;
	}

	if (trim)
		ntfs_unmap_and_discard(sbi, lcn, len);
	wnd_set_free(wnd, lcn, len);

out:
	up_write(&wnd->rw_lock);
}

/*
 * run_deallocate
 *
 * deallocate clusters
 */
int run_deallocate(ntfs_sb_info *sbi, struct runs_tree *run, bool trim)
{
	CLST lcn, len;
	size_t idx = 0;

	while (run_get_entry(run, idx++, NULL, &lcn, &len)) {
		if (lcn == SPARSE_LCN)
			continue;

		mark_as_free_ex(sbi, lcn, len, trim);
	}

	return 0;
}
