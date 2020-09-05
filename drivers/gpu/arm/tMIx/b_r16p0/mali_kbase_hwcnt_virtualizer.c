/*
 *
 * (C) COPYRIGHT 2018 ARM Limited. All rights reserved.
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

#include "mali_kbase_hwcnt_virtualizer.h"
#include "mali_kbase_hwcnt_accumulator.h"
#include "mali_kbase_hwcnt_context.h"
#include "mali_kbase_hwcnt_types.h"
#include "mali_malisw.h"
#include "mali_kbase_debug.h"
#include "mali_kbase_linux.h"

#include <linux/mutex.h>
#include <linux/slab.h>

/**
 * struct kbase_hwcnt_virtualizer - Hardware counter virtualizer structure.
 * @hctx:         Hardware counter context being virtualized.
 * @metadata:     Hardware counter metadata.
 * @lock:         Lock acquired at all entrypoints, to protect mutable state.
 * @client_count: Current number of virtualizer clients.
 * @clients:      List of virtualizer clients.
 * @accum:        Hardware counter accumulator. NULL if no clients.
 * @scratch_map:  Enable map used as scratch space during counter changes.
 * @scratch_buf:  Dump buffer used as scratch space during dumps.
 */
struct kbase_hwcnt_virtualizer {
	struct kbase_hwcnt_context *hctx;
	const struct kbase_hwcnt_metadata *metadata;
	struct mutex lock;
	size_t client_count;
	struct list_head clients;
	struct kbase_hwcnt_accumulator *accum;
	struct kbase_hwcnt_enable_map scratch_map;
	struct kbase_hwcnt_dump_buffer scratch_buf;
};

/**
 * struct kbase_hwcnt_virtualizer_client - Virtualizer client structure.
 * @node:        List node used for virtualizer client list.
 * @hvirt:       Hardware counter virtualizer.
 * @enable_map:  Enable map with client's current enabled counters.
 * @accum_buf:   Dump buffer with client's current accumulated counters.
 * @has_accum:   True if accum_buf contains any accumulated counters.
 * @ts_start_ns: Counter collection start time of current dump.
 */
struct kbase_hwcnt_virtualizer_client {
	struct list_head node;
	struct kbase_hwcnt_virtualizer *hvirt;
	struct kbase_hwcnt_enable_map enable_map;
	struct kbase_hwcnt_dump_buffer accum_buf;
	bool has_accum;
	u64 ts_start_ns;
};

const struct kbase_hwcnt_metadata *kbase_hwcnt_virtualizer_metadata(
	struct kbase_hwcnt_virtualizer *hvirt)
{
	if (!hvirt)
		return NULL;

	return hvirt->metadata;
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_metadata);

/**
 * kbasep_hwcnt_virtualizer_client_free - Free a virtualizer client's memory.
 * @hvcli: Pointer to virtualizer client.
 *
 * Will safely free a client in any partial state of construction.
 */
static void kbasep_hwcnt_virtualizer_client_free(
	struct kbase_hwcnt_virtualizer_client *hvcli)
{
	if (!hvcli)
		return;

	kbase_hwcnt_dump_buffer_free(&hvcli->accum_buf);
	kbase_hwcnt_enable_map_free(&hvcli->enable_map);
	kfree(hvcli);
}

/**
 * kbasep_hwcnt_virtualizer_client_alloc - Allocate memory for a virtualizer
 *                                         client.
 * @metadata:  Non-NULL pointer to counter metadata.
 * @out_hvcli: Non-NULL pointer to where created client will be stored on
 *             success.
 *
 * Return: 0 on success, else error code.
 */
static int kbasep_hwcnt_virtualizer_client_alloc(
	const struct kbase_hwcnt_metadata *metadata,
	struct kbase_hwcnt_virtualizer_client **out_hvcli)
{
	int errcode;
	struct kbase_hwcnt_virtualizer_client *hvcli = NULL;

	WARN_ON(!metadata);
	WARN_ON(!out_hvcli);

	hvcli = kzalloc(sizeof(*hvcli), GFP_KERNEL);
	if (!hvcli)
		return -ENOMEM;

	errcode = kbase_hwcnt_enable_map_alloc(metadata, &hvcli->enable_map);
	if (errcode)
		goto error;

	errcode = kbase_hwcnt_dump_buffer_alloc(metadata, &hvcli->accum_buf);
	if (errcode)
		goto error;

	*out_hvcli = hvcli;
	return 0;
error:
	kbasep_hwcnt_virtualizer_client_free(hvcli);
	return errcode;
}

/**
 * kbasep_hwcnt_virtualizer_client_accumulate - Accumulate a dump buffer into a
 *                                              client's accumulation buffer.
 * @hvcli:    Non-NULL pointer to virtualizer client.
 * @dump_buf: Non-NULL pointer to dump buffer to accumulate from.
 */
static void kbasep_hwcnt_virtualizer_client_accumulate(
	struct kbase_hwcnt_virtualizer_client *hvcli,
	const struct kbase_hwcnt_dump_buffer *dump_buf)
{
	WARN_ON(!hvcli);
	WARN_ON(!dump_buf);
	lockdep_assert_held(&hvcli->hvirt->lock);

	if (hvcli->has_accum) {
		/* If already some accumulation, accumulate */
		kbase_hwcnt_dump_buffer_accumulate(
			&hvcli->accum_buf, dump_buf, &hvcli->enable_map);
	} else {
		/* If no accumulation, copy */
		kbase_hwcnt_dump_buffer_copy(
			&hvcli->accum_buf, dump_buf, &hvcli->enable_map);
	}
	hvcli->has_accum = true;
}

/**
 * kbasep_hwcnt_virtualizer_accumulator_term - Terminate the hardware counter
 *                                             accumulator after final client
 *                                             removal.
 * @hvirt: Non-NULL pointer to the hardware counter virtualizer.
 *
 * Will safely terminate the accumulator in any partial state of initialisation.
 */
static void kbasep_hwcnt_virtualizer_accumulator_term(
	struct kbase_hwcnt_virtualizer *hvirt)
{
	WARN_ON(!hvirt);
	lockdep_assert_held(&hvirt->lock);
	WARN_ON(hvirt->client_count);

	kbase_hwcnt_dump_buffer_free(&hvirt->scratch_buf);
	kbase_hwcnt_enable_map_free(&hvirt->scratch_map);
	kbase_hwcnt_accumulator_release(hvirt->accum);
	hvirt->accum = NULL;
}

/**
 * kbasep_hwcnt_virtualizer_accumulator_init - Initialise the hardware counter
 *                                             accumulator before first client
 *                                             addition.
 * @hvirt: Non-NULL pointer to the hardware counter virtualizer.
 *
 * Return: 0 on success, else error code.
 */
static int kbasep_hwcnt_virtualizer_accumulator_init(
	struct kbase_hwcnt_virtualizer *hvirt)
{
	int errcode;

	WARN_ON(!hvirt);
	lockdep_assert_held(&hvirt->lock);
	WARN_ON(hvirt->client_count);
	WARN_ON(hvirt->accum);

	errcode = kbase_hwcnt_accumulator_acquire(
		hvirt->hctx, &hvirt->accum);
	if (errcode)
		goto error;

	errcode = kbase_hwcnt_enable_map_alloc(
		hvirt->metadata, &hvirt->scratch_map);
	if (errcode)
		goto error;

	errcode = kbase_hwcnt_dump_buffer_alloc(
		hvirt->metadata, &hvirt->scratch_buf);
	if (errcode)
		goto error;

	return 0;
error:
	kbasep_hwcnt_virtualizer_accumulator_term(hvirt);
	return errcode;
}

/**
 * kbasep_hwcnt_virtualizer_client_add - Add a newly allocated client to the
 *                                       virtualizer.
 * @hvirt:      Non-NULL pointer to the hardware counter virtualizer.
 * @hvcli:      Non-NULL pointer to the virtualizer client to add.
 * @enable_map: Non-NULL pointer to client's initial enable map.
 *
 * Return: 0 on success, else error code.
 */
static int kbasep_hwcnt_virtualizer_client_add(
	struct kbase_hwcnt_virtualizer *hvirt,
	struct kbase_hwcnt_virtualizer_client *hvcli,
	const struct kbase_hwcnt_enable_map *enable_map)
{
	int errcode = 0;
	u64 ts_start_ns;
	u64 ts_end_ns;

	WARN_ON(!hvirt);
	WARN_ON(!hvcli);
	WARN_ON(!enable_map);
	lockdep_assert_held(&hvirt->lock);

	if (hvirt->client_count == 0)
		/* First client added, so initialise the accumulator */
		errcode = kbasep_hwcnt_virtualizer_accumulator_init(hvirt);
	if (errcode)
		return errcode;

	hvirt->client_count += 1;

	if (hvirt->client_count == 1) {
		/* First client, so just pass the enable map onwards as is */
		errcode = kbase_hwcnt_accumulator_set_counters(hvirt->accum,
			enable_map, &ts_start_ns, &ts_end_ns, NULL);
	} else {
		struct kbase_hwcnt_virtualizer_client *pos;

		/* Make the scratch enable map the union of all enable maps */
		kbase_hwcnt_enable_map_copy(
			&hvirt->scratch_map, enable_map);
		list_for_each_entry(pos, &hvirt->clients, node)
			kbase_hwcnt_enable_map_union(
				&hvirt->scratch_map, &pos->enable_map);

		/* Set the counters with the new union enable map */
		errcode = kbase_hwcnt_accumulator_set_counters(hvirt->accum,
			&hvirt->scratch_map,
			&ts_start_ns, &ts_end_ns,
			&hvirt->scratch_buf);
		/* Accumulate into only existing clients' accumulation bufs */
		if (!errcode)
			list_for_each_entry(pos, &hvirt->clients, node)
				kbasep_hwcnt_virtualizer_client_accumulate(
					pos, &hvirt->scratch_buf);
	}
	if (errcode)
		goto error;

	list_add(&hvcli->node, &hvirt->clients);
	hvcli->hvirt = hvirt;
	kbase_hwcnt_enable_map_copy(&hvcli->enable_map, enable_map);
	hvcli->has_accum = false;
	hvcli->ts_start_ns = ts_end_ns;

	return 0;
error:
	hvirt->client_count -= 1;
	if (hvirt->client_count == 0)
		kbasep_hwcnt_virtualizer_accumulator_term(hvirt);
	return errcode;
}

/**
 * kbasep_hwcnt_virtualizer_client_remove - Remove a client from the
 *                                          virtualizer.
 * @hvirt:      Non-NULL pointer to the hardware counter virtualizer.
 * @hvcli:      Non-NULL pointer to the virtualizer client to remove.
 */
static void kbasep_hwcnt_virtualizer_client_remove(
	struct kbase_hwcnt_virtualizer *hvirt,
	struct kbase_hwcnt_virtualizer_client *hvcli)
{
	int errcode = 0;
	u64 ts_start_ns;
	u64 ts_end_ns;

	WARN_ON(!hvirt);
	WARN_ON(!hvcli);
	lockdep_assert_held(&hvirt->lock);

	list_del(&hvcli->node);
	hvirt->client_count -= 1;

	if (hvirt->client_count == 0) {
		/* Last client removed, so terminate the accumulator */
		kbasep_hwcnt_virtualizer_accumulator_term(hvirt);
	} else {
		struct kbase_hwcnt_virtualizer_client *pos;
		/* Make the scratch enable map the union of all enable maps */
		kbase_hwcnt_enable_map_disable_all(&hvirt->scratch_map);
		list_for_each_entry(pos, &hvirt->clients, node)
			kbase_hwcnt_enable_map_union(
				&hvirt->scratch_map, &pos->enable_map);
		/* Set the counters with the new union enable map */
		errcode = kbase_hwcnt_accumulator_set_counters(hvirt->accum,
			&hvirt->scratch_map,
			&ts_start_ns, &ts_end_ns,
			&hvirt->scratch_buf);
		/* Accumulate into remaining clients' accumulation bufs */
		if (!errcode)
			list_for_each_entry(pos, &hvirt->clients, node)
				kbasep_hwcnt_virtualizer_client_accumulate(
					pos, &hvirt->scratch_buf);
	}
	WARN_ON(errcode);
}

/**
 * kbasep_hwcnt_virtualizer_client_set_counters - Perform a dump of the client's
 *                                                currently enabled counters,
 *                                                and enable a new set of
 *                                                counters that will be used for
 *                                                subsequent dumps.
 * @hvirt:       Non-NULL pointer to the hardware counter virtualizer.
 * @hvcli:       Non-NULL pointer to the virtualizer client.
 * @enable_map:  Non-NULL pointer to the new counter enable map for the client.
 *               Must have the same metadata as the virtualizer.
 * @ts_start_ns: Non-NULL pointer where the start timestamp of the dump will
 *               be written out to on success.
 * @ts_end_ns:   Non-NULL pointer where the end timestamp of the dump will
 *               be written out to on success.
 * @dump_buf:    Pointer to the buffer where the dump will be written out to on
 *               success. If non-NULL, must have the same metadata as the
 *               accumulator. If NULL, the dump will be discarded.
 *
 * Return: 0 on success or error code.
 */
static int kbasep_hwcnt_virtualizer_client_set_counters(
	struct kbase_hwcnt_virtualizer *hvirt,
	struct kbase_hwcnt_virtualizer_client *hvcli,
	const struct kbase_hwcnt_enable_map *enable_map,
	u64 *ts_start_ns,
	u64 *ts_end_ns,
	struct kbase_hwcnt_dump_buffer *dump_buf)
{
	int errcode;
	struct kbase_hwcnt_virtualizer_client *pos;

	WARN_ON(!hvirt);
	WARN_ON(!hvcli);
	WARN_ON(!enable_map);
	WARN_ON(!ts_start_ns);
	WARN_ON(!ts_end_ns);
	WARN_ON(enable_map->metadata != hvirt->metadata);
	WARN_ON(dump_buf && (dump_buf->metadata != hvirt->metadata));
	lockdep_assert_held(&hvirt->lock);

	/* Make the scratch enable map the union of all enable maps */
	kbase_hwcnt_enable_map_copy(&hvirt->scratch_map, enable_map);
	list_for_each_entry(pos, &hvirt->clients, node)
		/* Ignore the enable map of the selected client */
		if (pos != hvcli)
			kbase_hwcnt_enable_map_union(
				&hvirt->scratch_map, &pos->enable_map);

	/* Set the counters with the new union enable map */
	errcode = kbase_hwcnt_accumulator_set_counters(hvirt->accum,
		&hvirt->scratch_map, ts_start_ns, ts_end_ns,
		&hvirt->scratch_buf);
	if (errcode)
		return errcode;

	/* Accumulate into all accumulation bufs except the selected client's */
	list_for_each_entry(pos, &hvirt->clients, node)
		if (pos != hvcli)
			kbasep_hwcnt_virtualizer_client_accumulate(
				pos, &hvirt->scratch_buf);

	/* Finally, write into the dump buf */
	if (dump_buf) {
		const struct kbase_hwcnt_dump_buffer *src = &hvirt->scratch_buf;

		if (hvcli->has_accum) {
			kbase_hwcnt_dump_buffer_accumulate(
				&hvcli->accum_buf, src, &hvcli->enable_map);
			src = &hvcli->accum_buf;
		}
		kbase_hwcnt_dump_buffer_copy(dump_buf, src, &hvcli->enable_map);
	}
	hvcli->has_accum = false;

	/* Update the selected client's enable map */
	kbase_hwcnt_enable_map_copy(&hvcli->enable_map, enable_map);

	/* Fix up the timestamps */
	*ts_start_ns = hvcli->ts_start_ns;
	hvcli->ts_start_ns = *ts_end_ns;

	return errcode;
}

int kbase_hwcnt_virtualizer_client_set_counters(
	struct kbase_hwcnt_virtualizer_client *hvcli,
	const struct kbase_hwcnt_enable_map *enable_map,
	u64 *ts_start_ns,
	u64 *ts_end_ns,
	struct kbase_hwcnt_dump_buffer *dump_buf)
{
	int errcode;
	struct kbase_hwcnt_virtualizer *hvirt;

	if (!hvcli || !enable_map || !ts_start_ns || !ts_end_ns)
		return -EINVAL;

	hvirt = hvcli->hvirt;

	if ((enable_map->metadata != hvirt->metadata) ||
	    (dump_buf && (dump_buf->metadata != hvirt->metadata)))
		return -EINVAL;

	mutex_lock(&hvirt->lock);

	if ((hvirt->client_count == 1) && (!hvcli->has_accum)) {
		/*
		 * If there's only one client with no prior accumulation, we can
		 * completely skip the virtualize and just pass through the call
		 * to the accumulator, saving a fair few copies and
		 * accumulations.
		 */
		errcode = kbase_hwcnt_accumulator_set_counters(
			hvirt->accum, enable_map,
			ts_start_ns, ts_end_ns, dump_buf);

		if (!errcode) {
			/* Update the selected client's enable map */
			kbase_hwcnt_enable_map_copy(
				&hvcli->enable_map, enable_map);

			/* Fix up the timestamps */
			*ts_start_ns = hvcli->ts_start_ns;
			hvcli->ts_start_ns = *ts_end_ns;
		}
	} else {
		/* Otherwise, do the full virtualize */
		errcode = kbasep_hwcnt_virtualizer_client_set_counters(
			hvirt, hvcli, enable_map,
			ts_start_ns, ts_end_ns, dump_buf);
	}

	mutex_unlock(&hvirt->lock);

	return errcode;
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_client_set_counters);

/**
 * kbasep_hwcnt_virtualizer_client_dump - Perform a dump of the client's
 *                                        currently enabled counters.
 * @hvirt:       Non-NULL pointer to the hardware counter virtualizer.
 * @hvcli:       Non-NULL pointer to the virtualizer client.
 * @ts_start_ns: Non-NULL pointer where the start timestamp of the dump will
 *               be written out to on success.
 * @ts_end_ns:   Non-NULL pointer where the end timestamp of the dump will
 *               be written out to on success.
 * @dump_buf:    Pointer to the buffer where the dump will be written out to on
 *               success. If non-NULL, must have the same metadata as the
 *               accumulator. If NULL, the dump will be discarded.
 *
 * Return: 0 on success or error code.
 */
static int kbasep_hwcnt_virtualizer_client_dump(
	struct kbase_hwcnt_virtualizer *hvirt,
	struct kbase_hwcnt_virtualizer_client *hvcli,
	u64 *ts_start_ns,
	u64 *ts_end_ns,
	struct kbase_hwcnt_dump_buffer *dump_buf)
{
	int errcode;
	struct kbase_hwcnt_virtualizer_client *pos;

	WARN_ON(!hvirt);
	WARN_ON(!hvcli);
	WARN_ON(!ts_start_ns);
	WARN_ON(!ts_end_ns);
	WARN_ON(dump_buf && (dump_buf->metadata != hvirt->metadata));
	lockdep_assert_held(&hvirt->lock);

	/* Perform the dump */
	errcode = kbase_hwcnt_accumulator_dump(hvirt->accum,
		ts_start_ns, ts_end_ns, &hvirt->scratch_buf);
	if (errcode)
		return errcode;

	/* Accumulate into all accumulation bufs except the selected client's */
	list_for_each_entry(pos, &hvirt->clients, node)
		if (pos != hvcli)
			kbasep_hwcnt_virtualizer_client_accumulate(
				pos, &hvirt->scratch_buf);

	/* Finally, write into the dump buf */
	if (dump_buf) {
		const struct kbase_hwcnt_dump_buffer *src = &hvirt->scratch_buf;

		if (hvcli->has_accum) {
			kbase_hwcnt_dump_buffer_accumulate(
				&hvcli->accum_buf, src, &hvcli->enable_map);
			src = &hvcli->accum_buf;
		}
		kbase_hwcnt_dump_buffer_copy(dump_buf, src, &hvcli->enable_map);
	}
	hvcli->has_accum = false;

	/* Fix up the timestamps */
	*ts_start_ns = hvcli->ts_start_ns;
	hvcli->ts_start_ns = *ts_end_ns;

	return errcode;
}

int kbase_hwcnt_virtualizer_client_dump(
	struct kbase_hwcnt_virtualizer_client *hvcli,
	u64 *ts_start_ns,
	u64 *ts_end_ns,
	struct kbase_hwcnt_dump_buffer *dump_buf)
{
	int errcode;
	struct kbase_hwcnt_virtualizer *hvirt;

	if (!hvcli || !ts_start_ns || !ts_end_ns)
		return -EINVAL;

	hvirt = hvcli->hvirt;

	if (dump_buf && (dump_buf->metadata != hvirt->metadata))
		return -EINVAL;

	mutex_lock(&hvirt->lock);

	if ((hvirt->client_count == 1) && (!hvcli->has_accum)) {
		/*
		 * If there's only one client with no prior accumulation, we can
		 * completely skip the virtualize and just pass through the call
		 * to the accumulator, saving a fair few copies and
		 * accumulations.
		 */
		errcode = kbase_hwcnt_accumulator_dump(
			hvirt->accum, ts_start_ns, ts_end_ns, dump_buf);

		if (!errcode) {
			/* Fix up the timestamps */
			*ts_start_ns = hvcli->ts_start_ns;
			hvcli->ts_start_ns = *ts_end_ns;
		}
	} else {
		/* Otherwise, do the full virtualize */
		errcode = kbasep_hwcnt_virtualizer_client_dump(
			hvirt, hvcli, ts_start_ns, ts_end_ns, dump_buf);
	}

	mutex_unlock(&hvirt->lock);

	return errcode;
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_client_dump);

int kbase_hwcnt_virtualizer_client_create(
	struct kbase_hwcnt_virtualizer *hvirt,
	const struct kbase_hwcnt_enable_map *enable_map,
	struct kbase_hwcnt_virtualizer_client **out_hvcli)
{
	int errcode;
	struct kbase_hwcnt_virtualizer_client *hvcli;

	if (!hvirt || !enable_map || !out_hvcli ||
	    (enable_map->metadata != hvirt->metadata))
		return -EINVAL;

	errcode = kbasep_hwcnt_virtualizer_client_alloc(
		hvirt->metadata, &hvcli);
	if (errcode)
		return errcode;

	mutex_lock(&hvirt->lock);

	errcode = kbasep_hwcnt_virtualizer_client_add(hvirt, hvcli, enable_map);

	mutex_unlock(&hvirt->lock);

	if (errcode) {
		kbasep_hwcnt_virtualizer_client_free(hvcli);
		return errcode;
	}

	*out_hvcli = hvcli;
	return 0;
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_client_create);

void kbase_hwcnt_virtualizer_client_destroy(
	struct kbase_hwcnt_virtualizer_client *hvcli)
{
	if (!hvcli)
		return;

	mutex_lock(&hvcli->hvirt->lock);

	kbasep_hwcnt_virtualizer_client_remove(hvcli->hvirt, hvcli);

	mutex_unlock(&hvcli->hvirt->lock);

	kbasep_hwcnt_virtualizer_client_free(hvcli);
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_client_destroy);

int kbase_hwcnt_virtualizer_init(
	struct kbase_hwcnt_context *hctx,
	struct kbase_hwcnt_virtualizer **out_hvirt)
{
	struct kbase_hwcnt_virtualizer *virt;
	const struct kbase_hwcnt_metadata *metadata;

	if (!hctx || !out_hvirt)
		return -EINVAL;

	metadata = kbase_hwcnt_context_metadata(hctx);
	if (!metadata)
		return -EINVAL;

	virt = kzalloc(sizeof(*virt), GFP_KERNEL);
	if (!virt)
		return -ENOMEM;

	virt->hctx = hctx;
	virt->metadata = metadata;

	mutex_init(&virt->lock);
	INIT_LIST_HEAD(&virt->clients);

	*out_hvirt = virt;
	return 0;
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_init);

void kbase_hwcnt_virtualizer_term(
	struct kbase_hwcnt_virtualizer *hvirt)
{
	if (!hvirt)
		return;

	/* Non-zero client count implies client leak */
	if (WARN_ON(hvirt->client_count != 0)) {
		struct kbase_hwcnt_virtualizer_client *pos, *n;

		list_for_each_entry_safe(pos, n, &hvirt->clients, node)
			kbase_hwcnt_virtualizer_client_destroy(pos);
	}

	WARN_ON(hvirt->client_count != 0);
	WARN_ON(hvirt->accum);

	kfree(hvirt);
}
KBASE_EXPORT_TEST_API(kbase_hwcnt_virtualizer_term);
