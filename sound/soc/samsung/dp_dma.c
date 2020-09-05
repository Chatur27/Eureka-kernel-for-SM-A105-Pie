/*
 * dma.c  --  ALSA Soc Audio Layer
 *
 * (c) 2006 Wolfson Microelectronics PLC.
 * Graeme Gregory graeme.gregory@wolfsonmicro.com or linux@wolfsonmicro.com
 *
 * Copyright 2004-2005 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/iommu.h>
#include <linux/dma/dma-pl330.h>

#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/exynos.h>

#include "dma.h"
#include "dp_dma.h"

#define PERIOD_MIN		4
#define ST_RUNNING		(1<<0)
#define ST_OPENED		(1<<1)

#define DP_FIFO			(0x11090838)

#define SRAM_END		(0x04000000)
#define RX_SRAM_SIZE		(0x2000)	/* 8 KB */
#define MAX_DEEPBUF_SIZE	(0xA000)	/* 40 KB */

static struct device *g_debug_dev;

static const struct snd_pcm_hardware dma_hardware = {
	.info			= SNDRV_PCM_INFO_INTERLEAVED |
				  SNDRV_PCM_INFO_BLOCK_TRANSFER |
				  SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID,
	.formats		= SNDRV_PCM_FMTBIT_S24_LE |
				  SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_U16_LE |
				  SNDRV_PCM_FMTBIT_U8 |
				  SNDRV_PCM_FMTBIT_S8,
	.channels_min		= 1,
	.channels_max		= 8,
	.buffer_bytes_max	= 256*1024,
	.period_bytes_min	= 128,
	.period_bytes_max	= 64*1024,
	.periods_min		= 2,
	.periods_max		= 128,
	.fifo_size		= 32,
};

struct runtime_data {
	spinlock_t lock;
	int state;
	unsigned int dma_loaded;
	unsigned int dma_period;
	dma_addr_t dma_start;
	dma_addr_t dma_pos;
	dma_addr_t dma_end;
	struct s3c_dma_params *params;
	struct snd_pcm_hardware hw;
	struct displayport_audio_config_data dp_config;
	bool cap_dram_used;
	dma_addr_t irq_pos;
	u32 irq_cnt;
};

#ifdef CONFIG_SND_SAMSUNG_IOMMU
struct dma_iova {
	dma_addr_t		iova;
	dma_addr_t		pa;
	unsigned char		*va;
	struct list_head	node;
};

static LIST_HEAD(iova_list);
#endif

static void audio_buffdone(void *data);

/* dma_enqueue
 *
 * place a dma buffer onto the queue for the dma system
 * to handle.
 */
static void dma_enqueue(struct snd_pcm_substream *substream)
{
	struct runtime_data *prtd = substream->runtime->private_data;
	dma_addr_t pos = prtd->dma_pos;
	unsigned int limit;
	struct samsung_dma_prep dma_info;

	pr_info("Entered %s\n", __func__);

	limit = (prtd->dma_end - prtd->dma_start) / prtd->dma_period;

	pr_debug("%s: loaded %d, limit %d\n",
				__func__, prtd->dma_loaded, limit);

	dma_info.cap = DMA_CYCLIC;
	dma_info.direction = DMA_MEM_TO_DEV;
	dma_info.fp = audio_buffdone;
	dma_info.fp_param = substream;
	dma_info.period = prtd->dma_period;
	dma_info.len = prtd->dma_period*limit;

	if (prtd->params->esa_dma || samsung_dma_has_infiniteloop()) {
		dma_info.buf = prtd->dma_pos;
		dma_info.infiniteloop = limit;
		prtd->params->ops->prepare(prtd->params->ch, &dma_info);
	} else {
		dma_info.infiniteloop = 0;
		while (prtd->dma_loaded < limit) {
			pr_debug("dma_loaded: %d\n", prtd->dma_loaded);

			if ((pos + dma_info.period) > prtd->dma_end) {
				dma_info.period  = prtd->dma_end - pos;
				pr_debug("%s: corrected dma len %ld\n",
						__func__, dma_info.period);
			}

			dma_info.buf = pos;
			prtd->params->ops->prepare(prtd->params->ch, &dma_info);

			prtd->dma_loaded++;
			pos += prtd->dma_period;
			if (pos >= prtd->dma_end)
				pos = prtd->dma_start;
		}
		prtd->dma_pos = pos;
	}
}

static void audio_buffdone(void *data)
{
	struct snd_pcm_substream *substream = data;
	struct runtime_data *prtd;
	dma_addr_t src, dst, pos;

	pr_debug("Entered %s\n", __func__);

	if (!substream)
		return;

	prtd = substream->runtime->private_data;
	if (prtd->state & ST_RUNNING) {
		prtd->params->ops->getposition(prtd->params->ch, &src, &dst);
		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			pos = dst - prtd->dma_start;
		else
			pos = src - prtd->dma_start;

		prtd->irq_cnt++;
		prtd->irq_pos = pos;
		pos /= prtd->dma_period;
		pos = prtd->dma_start + (pos * prtd->dma_period);
		if (pos >= prtd->dma_end)
			pos = prtd->dma_start;

		prtd->dma_pos = pos;
		snd_pcm_period_elapsed(substream);

		if (!prtd->params->esa_dma && !samsung_dma_has_circular()) {
			spin_lock(&prtd->lock);
			prtd->dma_loaded--;
			if (!samsung_dma_has_infiniteloop())
				dma_enqueue(substream);
			spin_unlock(&prtd->lock);
		}
	}
}

static int dma_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct runtime_data *prtd = runtime->private_data;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	unsigned long totbytes = params_buffer_bytes(params);
	struct samsung_dma_req req;
	struct samsung_dma_config config;

	pr_debug("Entered %s\n", __func__);

	/* this may get called several times by oss emulation
	 * with different params -HW */
	if (prtd->params == NULL) {
		prtd->params = kzalloc(sizeof(struct s3c_dma_params), GFP_KERNEL);

		pr_debug("params %p, client %p, channel %d\n", prtd->params,
			prtd->params->client, prtd->params->channel);

		prtd->params->ops = samsung_dma_get_ops();
		req.cap = DMA_CYCLIC;
		req.client = prtd->params->client;

		config.direction = DMA_MEM_TO_DEV;
		config.width = 2;
		config.maxburst = 1; /* fixed */
		config.fifo = DP_FIFO;

		prtd->params->ch = prtd->params->ops->request(prtd->params->channel,
				&req, g_debug_dev, "tx");

		pr_info("dma_request: ch %d, req %p, dev %p, ch_name [%s]\n",
			prtd->params->channel, &req, rtd->cpu_dai->dev,
			prtd->params->ch_name);
		prtd->params->ops->config(prtd->params->ch, &config);
	}

	if (params != NULL) {
		runtime->access = params_access(params);
		runtime->format = params_format(params);
		runtime->subformat = params_subformat(params);
		runtime->period_size = params_period_bytes(params);
		runtime->rate = params_rate(params);
		runtime->channels = params_channels(params);
		runtime->sample_bits = snd_pcm_format_physical_width(params_format(params));
	}

	pr_info("[AUDIO] %s: period_size: %lu\n", __func__, runtime->period_size);
	pr_info("[AUDIO] %s: rate: %u\n", __func__, runtime->rate);
	pr_info("[AUDIO] %s: channels: %u\n", __func__, runtime->channels);
	pr_info("[AUDIO] %s: sample_bits: %u\n", __func__, runtime->sample_bits);

	switch (runtime->rate) {
	case 32000:
		prtd->dp_config.audio_fs = FS_32KHZ;
		break;
	case 44100:
		prtd->dp_config.audio_fs = FS_44KHZ;
		break;
	case 48000:
		prtd->dp_config.audio_fs = FS_48KHZ;
		break;
	case 88000:
		prtd->dp_config.audio_fs = FS_88KHZ;
		break;
	case 96000:
		prtd->dp_config.audio_fs = FS_96KHZ;
		break;
	case 176000:
		prtd->dp_config.audio_fs = FS_176KHZ;
		break;
	case 192000:
		prtd->dp_config.audio_fs = FS_192KHZ;
		break;
	default:
		pr_debug("[AUDIO] Not supported sample rate: %u\n", runtime->rate);
		return -EINVAL;
	}

	switch (runtime->sample_bits) {
	case 16:
		prtd->dp_config.audio_bit = AUDIO_16_BIT;
		break;
	case 20:
		prtd->dp_config.audio_bit = AUDIO_20_BIT;
		break;
	case 24:
		prtd->dp_config.audio_bit = AUDIO_24_BIT;
		break;
	default:
		pr_debug("[AUDIO] Not supported sample bits: %u\n", runtime->sample_bits);
		return -EINVAL;
	}

	prtd->dp_config.audio_channel_cnt = runtime->channels;
	prtd->dp_config.audio_packed_mode = NORMAL_MODE;
	prtd->dp_config.audio_word_length = WORD_LENGTH_1;

	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);

	runtime->dma_bytes = totbytes;

	spin_lock_irq(&prtd->lock);
	prtd->dma_loaded = 0;
	prtd->dma_period = params_period_bytes(params);
	prtd->dma_start = runtime->dma_addr;
	prtd->dma_pos = prtd->dma_start;
	prtd->dma_end = prtd->dma_start + totbytes;
	prtd->cap_dram_used = runtime->dma_addr < SRAM_END ? false : true;
	while ((totbytes / prtd->dma_period) < PERIOD_MIN)
		prtd->dma_period >>= 1;
	spin_unlock_irq(&prtd->lock);

	pr_info("ADMA:%s:DmaAddr=@%x Total=%d PrdSz=%d(%d) #Prds=%d dma_area=0x%p\n",
		(substream->stream == SNDRV_PCM_STREAM_PLAYBACK) ? "P" : "C",
		(u32)prtd->dma_start, (u32)runtime->dma_bytes,
		params_period_bytes(params),(u32) prtd->dma_period,
		params_periods(params), runtime->dma_area);
	return 0;
}

static int dma_hw_free(struct snd_pcm_substream *substream)
{
	struct runtime_data *prtd = substream->runtime->private_data;

	pr_debug("Entered %s\n", __func__);

	snd_pcm_set_runtime_buffer(substream, NULL);

	if (prtd->params) {
		prtd->params->ops->flush(prtd->params->ch);
		prtd->params->ops->release(prtd->params->ch,
					prtd->params->client);
		kfree(prtd->params);
		prtd->params = NULL;
	}

	return 0;
}

static int dma_prepare(struct snd_pcm_substream *substream)
{
	struct runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	pr_info("Entered %s\n", __func__);

	/* return if this is a bufferless transfer e.g.
	 * codec <--> BT codec or GSM modem -- lg FIXME */
	if (!prtd->params)
		return 0;

	/* flush the DMA channel */
	prtd->params->ops->flush(prtd->params->ch);
	prtd->dma_loaded = 0;
	prtd->dma_pos = prtd->dma_start;
	prtd->irq_pos = prtd->dma_start;
	prtd->irq_cnt = 0;

	/* enqueue dma buffers */
	dma_enqueue(substream);

	return ret;
}

static int dma_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct runtime_data *prtd = substream->runtime->private_data;
	int ret = 0;

	pr_info("[DP Audio] Entered %s ++\n", __func__);

	spin_lock(&prtd->lock);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		prtd->state |= ST_RUNNING;
		prtd->dp_config.audio_enable = true;
		displayport_audio_config(&prtd->dp_config);
		prtd->params->ops->trigger(prtd->params->ch);
		break;

	case SNDRV_PCM_TRIGGER_STOP:
		prtd->state &= ~ST_RUNNING;
		prtd->params->ops->stop(prtd->params->ch);
		prtd->dp_config.audio_enable = false;
		displayport_audio_config(&prtd->dp_config);
		break;

	default:
		ret = -EINVAL;
		break;
	}

	spin_unlock(&prtd->lock);

	pr_info("[DP Audio] Entered %s --\n", __func__);

	return ret;
}

static snd_pcm_uframes_t dma_pointer(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct runtime_data *prtd = runtime->private_data;
	unsigned long res;

	pr_debug("Entered %s\n", __func__);

	res = prtd->dma_pos - prtd->dma_start;

	pr_debug("Pointer offset: %lu\n", res);

	/* we seem to be getting the odd error from the pcm library due
	 * to out-of-bounds pointers. this is maybe due to the dma engine
	 * not having loaded the new values for the channel before being
	 * called... (todo - fix )
	 */

	if (res >= snd_pcm_lib_buffer_bytes(substream)) {
		if (res == snd_pcm_lib_buffer_bytes(substream))
			res = 0;
	}

	return bytes_to_frames(substream->runtime, res);
}

static int dma_open(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct runtime_data *prtd;

	pr_debug("Entered %s\n", __func__);

	prtd = kzalloc(sizeof(struct runtime_data), GFP_KERNEL);
	if (prtd == NULL)
		return -ENOMEM;

	spin_lock_init(&prtd->lock);

	memcpy(&prtd->hw, &dma_hardware, sizeof(struct snd_pcm_hardware));

	snd_pcm_hw_constraint_integer(runtime, SNDRV_PCM_HW_PARAM_PERIODS);
	runtime->private_data = prtd;
	snd_soc_set_runtime_hwparams(substream, &prtd->hw);

	pr_info("%s: prtd = %p\n", __func__, prtd);

	return 0;
}

static int dma_close(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct runtime_data *prtd = runtime->private_data;

	pr_debug("Entered %s\n", __func__);

	if (!prtd) {
		pr_debug("dma_close called with prtd == NULL\n");
		return 0;
	}

	pr_info("%s: prtd = %p, irq_cnt %u\n",
			__func__, prtd, prtd->irq_cnt);
	kfree(prtd);

	return 0;
}

static int dma_mmap(struct snd_pcm_substream *substream,
	struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	dma_addr_t dma_pa = runtime->dma_addr;
#ifdef CONFIG_SND_SAMSUNG_IOMMU
	struct dma_iova *di;
#endif

	pr_debug("Entered %s\n", __func__);

#ifdef CONFIG_SND_SAMSUNG_IOMMU
	list_for_each_entry(di, &iova_list, node) {
		if (di->iova == runtime->dma_addr)
			dma_pa = di->pa;
	}
#endif
	return dma_mmap_writecombine(substream->pcm->card->dev, vma,
				     runtime->dma_area, dma_pa,
				     runtime->dma_bytes);
}

static struct snd_pcm_ops pcm_dma_ops = {
	.open		= dma_open,
	.close		= dma_close,
	.ioctl		= snd_pcm_lib_ioctl,
	.hw_params	= dma_hw_params,
	.hw_free	= dma_hw_free,
	.prepare	= dma_prepare,
	.trigger	= dma_trigger,
	.pointer	= dma_pointer,
	.mmap		= dma_mmap,
};

static int preallocate_dma_buffer(struct snd_pcm *pcm, int stream)
{
	struct snd_pcm_substream *substream = pcm->streams[stream].substream;
	struct snd_dma_buffer *buf = &substream->dma_buffer;
	size_t size = dma_hardware.buffer_bytes_max;

	pr_debug("Entered %s\n", __func__);

	buf->dev.type = SNDRV_DMA_TYPE_DEV;
	buf->dev.dev = pcm->card->dev;
	buf->private_data = NULL;
	buf->area = dma_alloc_coherent(pcm->card->dev, size,
					&buf->addr, GFP_KERNEL);
	if (!buf->area)
		return -ENOMEM;
	buf->bytes = size;
	return 0;
}


static void dma_free_dma_buffers(struct snd_pcm *pcm)
{
	struct snd_pcm_substream *substream;
	struct snd_dma_buffer *buf;
	struct runtime_data *prtd;
	int stream;

	pr_debug("Entered %s\n", __func__);

	for (stream = 0; stream < 2; stream++) {
		substream = pcm->streams[stream].substream;
		if (!substream)
			continue;

		buf = &substream->dma_buffer;
		if (!buf->area)
			continue;

		prtd = substream->runtime->private_data;
		if (prtd->cap_dram_used) {
			dma_free_coherent(pcm->card->dev, buf->bytes,
						buf->area, buf->addr);
		} else {
			iounmap(buf->area);
		}

		buf->area = NULL;
	}
}

static u64 dma_mask = DMA_BIT_MASK(32);

static int dma_new(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm *pcm = rtd->pcm;
	int ret = 0;

	pr_debug("Entered %s\n", __func__);

	if (!card->dev->dma_mask)
		card->dev->dma_mask = &dma_mask;
	if (!card->dev->coherent_dma_mask)
		card->dev->coherent_dma_mask = DMA_BIT_MASK(32);

	if (pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream) {
		ret = preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_PLAYBACK);
		if (ret)
			goto out;
	}

	if (pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream) {
		ret = preallocate_dma_buffer(pcm,
			SNDRV_PCM_STREAM_CAPTURE);
		if (ret)
			goto out;
	}
out:
	return ret;
}

static struct snd_soc_platform_driver samsung_display_adma = {
	.ops		= &pcm_dma_ops,
	.pcm_new	= dma_new,
	.pcm_free	= dma_free_dma_buffers,
};

static int samsung_display_adma_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct runtime_data *data;

	g_debug_dev = dev;

	data = devm_kzalloc(dev, sizeof(struct runtime_data), GFP_KERNEL);

	if (!data) {
		dev_err(dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, data);

	spin_lock_init(&data->lock);

	return snd_soc_register_platform(&pdev->dev, &samsung_display_adma);
}

static int samsung_display_adma_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);
	return 0;
}

static const struct of_device_id samsung_display_adma_match[] = {
	{
		.compatible = "samsung,displayport-adma",
	},
	{},
};
MODULE_DEVICE_TABLE(of, samsung_display_adma_match);

static struct platform_driver samsung_display_adma_driver = {
	.probe	= samsung_display_adma_probe,
	.remove	= samsung_display_adma_remove,
	.driver = {
		.name = "samsung-displayport-adma",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(samsung_display_adma_match),
	},
};

module_platform_driver(samsung_display_adma_driver);

MODULE_AUTHOR("Ben Dooks, <ben@simtec.co.uk>");
MODULE_DESCRIPTION("Samsung Display Port Audio DMA Driver");
MODULE_ALIAS("platform:samsung-display-adma");
MODULE_LICENSE("GPL");
