/*
 * Copyright (c) 2005-2011 Atheros Communications Inc.
 * Copyright (c) 2011-2013 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if !defined(_TRACE_H_) || defined(TRACE_HEADER_MULTI_READ)

#include <linux/tracepoint.h>
#include "core.h"

#define _TRACE_H_

/* create empty functions when tracing is disabled */
#if !defined(CONFIG_ATH10K_TRACING)
#undef TRACE_EVENT
#define TRACE_EVENT(name, proto, ...) \
static inline void trace_ ## name(proto) {}
#undef DECLARE_EVENT_CLASS
#define DECLARE_EVENT_CLASS(...)
#undef DEFINE_EVENT
#define DEFINE_EVENT(evt_class, name, proto, ...) \
static inline void trace_ ## name(proto) {}
#endif /* !CONFIG_ATH10K_TRACING || __CHECKER__ */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM ath10k

#define ATH10K_MSG_MAX 200

DECLARE_EVENT_CLASS(ath10k_log_event,
	TP_PROTO(struct ath10k *ar, struct va_format *vaf),
	TP_ARGS(ar, vaf),
	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__dynamic_array(char, msg, ATH10K_MSG_MAX)
	),
	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		WARN_ON_ONCE(vsnprintf(__get_dynamic_array(msg),
				       ATH10K_MSG_MAX,
				       vaf->fmt,
				       *vaf->va) >= ATH10K_MSG_MAX);
	),
	TP_printk(
		"%s %s %s",
		__get_str(driver),
		__get_str(device),
		__get_str(msg)
	)
);

DEFINE_EVENT(ath10k_log_event, ath10k_log_err,
	     TP_PROTO(struct ath10k *ar, struct va_format *vaf),
	     TP_ARGS(ar, vaf)
);

DEFINE_EVENT(ath10k_log_event, ath10k_log_warn,
	     TP_PROTO(struct ath10k *ar, struct va_format *vaf),
	     TP_ARGS(ar, vaf)
);

DEFINE_EVENT(ath10k_log_event, ath10k_log_info,
	     TP_PROTO(struct ath10k *ar, struct va_format *vaf),
	     TP_ARGS(ar, vaf)
);

TRACE_EVENT(ath10k_log_dbg,
	TP_PROTO(struct ath10k *ar, unsigned int level, struct va_format *vaf),
	TP_ARGS(ar, level, vaf),
	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(unsigned int, level)
		__dynamic_array(char, msg, ATH10K_MSG_MAX)
	),
	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->level = level;
		WARN_ON_ONCE(vsnprintf(__get_dynamic_array(msg),
				       ATH10K_MSG_MAX,
				       vaf->fmt,
				       *vaf->va) >= ATH10K_MSG_MAX);
	),
	TP_printk(
		"%s %s %s",
		__get_str(driver),
		__get_str(device),
		__get_str(msg)
	)
);

TRACE_EVENT(ath10k_log_dbg_dump,
	TP_PROTO(struct ath10k *ar, const char *msg, const char *prefix,
		 const void *buf, size_t buf_len),

	TP_ARGS(ar, msg, prefix, buf, buf_len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__string(msg, msg)
		__string(prefix, prefix)
		__field(size_t, buf_len)
		__dynamic_array(u8, buf, buf_len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__assign_str(msg, msg);
		__assign_str(prefix, prefix);
		__entry->buf_len = buf_len;
		memcpy(__get_dynamic_array(buf), buf, buf_len);
	),

	TP_printk(
		"%s %s %s/%s\n",
		__get_str(driver),
		__get_str(device),
		__get_str(prefix),
		__get_str(msg)
	)
);

TRACE_EVENT(ath10k_wmi_cmd,
	TP_PROTO(struct ath10k *ar, int id, void *buf, size_t buf_len, int ret),

	TP_ARGS(ar, id, buf, buf_len, ret),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(unsigned int, id)
		__field(size_t, buf_len)
		__dynamic_array(u8, buf, buf_len)
		__field(int, ret)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->id = id;
		__entry->buf_len = buf_len;
		__entry->ret = ret;
		memcpy(__get_dynamic_array(buf), buf, buf_len);
	),

	TP_printk(
		"%s %s id %d len %zu ret %d",
		__get_str(driver),
		__get_str(device),
		__entry->id,
		__entry->buf_len,
		__entry->ret
	)
);

TRACE_EVENT(ath10k_wmi_event,
	TP_PROTO(struct ath10k *ar, int id, void *buf, size_t buf_len),

	TP_ARGS(ar, id, buf, buf_len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(unsigned int, id)
		__field(size_t, buf_len)
		__dynamic_array(u8, buf, buf_len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->id = id;
		__entry->buf_len = buf_len;
		memcpy(__get_dynamic_array(buf), buf, buf_len);
	),

	TP_printk(
		"%s %s id %d len %zu",
		__get_str(driver),
		__get_str(device),
		__entry->id,
		__entry->buf_len
	)
);

TRACE_EVENT(ath10k_htt_stats,
	TP_PROTO(struct ath10k *ar, void *buf, size_t buf_len),

	TP_ARGS(ar, buf, buf_len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(size_t, buf_len)
		__dynamic_array(u8, buf, buf_len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->buf_len = buf_len;
		memcpy(__get_dynamic_array(buf), buf, buf_len);
	),

	TP_printk(
		"%s %s len %zu",
		__get_str(driver),
		__get_str(device),
		__entry->buf_len
	)
);

TRACE_EVENT(ath10k_wmi_dbglog,
	TP_PROTO(struct ath10k *ar, void *buf, size_t buf_len),

	TP_ARGS(ar, buf, buf_len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(size_t, buf_len)
		__dynamic_array(u8, buf, buf_len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->buf_len = buf_len;
		memcpy(__get_dynamic_array(buf), buf, buf_len);
	),

	TP_printk(
		"%s %s len %zu",
		__get_str(driver),
		__get_str(device),
		__entry->buf_len
	)
);

TRACE_EVENT(ath10k_htt_pktlog,
	    TP_PROTO(struct ath10k *ar, void *buf, u16 buf_len),

	TP_ARGS(ar, buf, buf_len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(u16, buf_len)
		__dynamic_array(u8, pktlog, buf_len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->buf_len = buf_len;
		memcpy(__get_dynamic_array(pktlog), buf, buf_len);
	),

	TP_printk(
		"%s %s size %hu",
		__get_str(driver),
		__get_str(device),
		__entry->buf_len
	 )
);

TRACE_EVENT(ath10k_htt_rx_desc,
	    TP_PROTO(struct ath10k *ar, u32 tsf, void *rxdesc, u16 len),

	TP_ARGS(ar, tsf, rxdesc, len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(u32, tsf)
		__field(u16, len)
		__dynamic_array(u8, rxdesc, len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->tsf = tsf;
		__entry->len = len;
		memcpy(__get_dynamic_array(rxdesc), rxdesc, len);
	),

	TP_printk(
		"%s %s %u len %hu",
		__get_str(driver),
		__get_str(device),
		__entry->tsf,
		__entry->len
	 )
);

TRACE_EVENT(ath10k_htt_tx,
	    TP_PROTO(struct ath10k *ar, u16 msdu_id, u16 msdu_len,
		     u8 vdev_id, u8 tid),

	TP_ARGS(ar, msdu_id, msdu_len, vdev_id, tid),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(u16, msdu_id)
		__field(u16, msdu_len)
		__field(u8, vdev_id)
		__field(u8, tid)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->msdu_id = msdu_id;
		__entry->msdu_len = msdu_len;
		__entry->vdev_id = vdev_id;
		__entry->tid = tid;
	),

	TP_printk(
		"%s %s msdu_id %d msdu_len %d vdev_id %d tid %d",
		__get_str(driver),
		__get_str(device),
		__entry->msdu_id,
		__entry->msdu_len,
		__entry->vdev_id,
		__entry->tid
	 )
);

TRACE_EVENT(ath10k_txrx_tx_unref,
	    TP_PROTO(struct ath10k *ar, u16 msdu_id),

	TP_ARGS(ar, msdu_id),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(u16, msdu_id)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->msdu_id = msdu_id;
	),

	TP_printk(
		"%s %s msdu_id %d",
		__get_str(driver),
		__get_str(device),
		__entry->msdu_id
	 )
);

DECLARE_EVENT_CLASS(ath10k_data_event,
		    TP_PROTO(struct ath10k *ar, void *data, size_t len),

	TP_ARGS(ar, data, len),

	TP_STRUCT__entry(
		__string(device, dev_name(ar->dev))
		__string(driver, dev_driver_string(ar->dev))
		__field(size_t, len)
		__dynamic_array(u8, data, len)
	),

	TP_fast_assign(
		__assign_str(device, dev_name(ar->dev));
		__assign_str(driver, dev_driver_string(ar->dev));
		__entry->len = len;
		memcpy(__get_dynamic_array(data), data, len);
	),

	TP_printk(
		"%s %s len %zu\n",
		__get_str(driver),
		__get_str(device),
		__entry->len
	)
);

DEFINE_EVENT(ath10k_data_event, ath10k_htt_tx_msdu,
	     TP_PROTO(struct ath10k *ar, void *data, size_t len),
	     TP_ARGS(ar, data, len)
);

DEFINE_EVENT(ath10k_data_event, ath10k_htt_rx_pop_msdu,
	     TP_PROTO(struct ath10k *ar, void *data, size_t len),
	     TP_ARGS(ar, data, len)
);

DEFINE_EVENT(ath10k_data_event, ath10k_wmi_mgmt_tx,
	     TP_PROTO(struct ath10k *ar, void *data, size_t len),
	     TP_ARGS(ar, data, len)
);

DEFINE_EVENT(ath10k_data_event, ath10k_wmi_bcn_tx,
	     TP_PROTO(struct ath10k *ar, void *data, size_t len),
	     TP_ARGS(ar, data, len)
);
#endif /* _TRACE_H_ || TRACE_HEADER_MULTI_READ*/

/* we don't want to use include/trace/events */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE trace

/* This part must be outside protection */
#include <trace/define_trace.h>
