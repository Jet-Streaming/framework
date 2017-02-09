/* Copyright (c) 2013, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include <odp/api/packet.h>
#include <odp_packet_internal.h>
#include <odp_debug_internal.h>
#include <odp/api/hints.h>
#include <odp/api/byteorder.h>

#include <odp/helper/eth.h>
#include <odp/helper/ip.h>
#include <odp/helper/tcp.h>
#include <odp/helper/udp.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>

/*
 *
 * Alloc and free
 * ********************************************************
 *
 */

static inline void packet_parse_disable(odp_packet_hdr_t *pkt_hdr)
{
	pkt_hdr->input_flags.parsed_l2  = 1;
	pkt_hdr->input_flags.parsed_all = 1;
}

void packet_parse_reset(odp_packet_hdr_t *pkt_hdr)
{
	/* Reset parser metadata before new parse */
	pkt_hdr->error_flags.all  = 0;
	pkt_hdr->input_flags.all  = 0;
	pkt_hdr->output_flags.all = 0;
	pkt_hdr->l2_offset        = 0;
	pkt_hdr->l3_offset        = ODP_PACKET_OFFSET_INVALID;
	pkt_hdr->l4_offset        = ODP_PACKET_OFFSET_INVALID;
	pkt_hdr->payload_offset   = ODP_PACKET_OFFSET_INVALID;
	pkt_hdr->vlan_s_tag       = 0;
	pkt_hdr->vlan_c_tag       = 0;
	pkt_hdr->l3_protocol      = 0;
	pkt_hdr->l4_protocol      = 0;
}

/**
 * Initialize packet
 */
static void packet_init(pool_entry_t *pool, odp_packet_hdr_t *pkt_hdr,
			size_t size, int parse)
{
       /*
	* Reset parser metadata.  Note that we clear via memset to make
	* this routine indepenent of any additional adds to packet metadata.
	*/
	const size_t start_offset = ODP_FIELD_SIZEOF(odp_packet_hdr_t, buf_hdr);
	uint8_t *start;
	size_t len;

	start = (uint8_t *)pkt_hdr + start_offset;
	len = sizeof(odp_packet_hdr_t) - start_offset;
	memset(start, 0, len);

	/* Set metadata items that initialize to non-zero values */
	pkt_hdr->l3_offset = ODP_PACKET_OFFSET_INVALID;
	pkt_hdr->l4_offset = ODP_PACKET_OFFSET_INVALID;
	pkt_hdr->payload_offset = ODP_PACKET_OFFSET_INVALID;

	/* Disable lazy parsing on user allocated packets */
	if (!parse)
		packet_parse_disable(pkt_hdr);

       /*
	* Packet headroom is set from the pool's headroom
	* Packet tailroom is rounded up to fill the last
	* segment occupied by the allocated length.
	*/
	pkt_hdr->frame_len = size;
	pkt_hdr->headroom  = pool->s.headroom;
	pkt_hdr->tailroom  =
		(pool->s.seg_size * pkt_hdr->buf_hdr.segcount) -
		(pool->s.headroom + size);
}

odp_packet_t packet_alloc(odp_pool_t pool_hdl, uint32_t len, int parse)
{
	odp_packet_t pkt;
	odp_packet_hdr_t *pkt_hdr;
	pool_entry_t *pool = odp_pool_to_entry(pool_hdl);

	if (pool->s.params.type != ODP_POOL_PACKET)
		return ODP_PACKET_INVALID;

	/* Handle special case for zero-length packets */
	if (len == 0) {
		len = pool->s.params.buf.size;

		pkt = (odp_packet_t)buffer_alloc(pool_hdl, len);

		if (pkt == ODP_PACKET_INVALID)
			return ODP_PACKET_INVALID;

		pull_tail(odp_packet_hdr(pkt), len);

	} else {
		pkt = (odp_packet_t)buffer_alloc(pool_hdl, len);

		if (pkt == ODP_PACKET_INVALID)
			return ODP_PACKET_INVALID;
	}

	pkt_hdr = odp_packet_hdr(pkt);

	packet_init(pool, pkt_hdr, len, parse);

	return pkt;
}

odp_packet_t odp_packet_alloc(odp_pool_t pool_hdl, uint32_t len)
{
	return packet_alloc(pool_hdl, len, 0);
}

int odp_packet_alloc_multi(odp_pool_t pool_hdl, uint32_t len,
			   odp_packet_t pkt[], int num)
{
	pool_entry_t *pool = odp_pool_to_entry(pool_hdl);
	size_t pkt_size = len ? len : pool->s.params.buf.size;
	int count, i;

	if (pool->s.params.type != ODP_POOL_PACKET) {
		__odp_errno = EINVAL;
		return -1;
	}

	count = buffer_alloc_multi(pool_hdl, pkt_size,
				   (odp_buffer_t *)pkt, num);

	for (i = 0; i < count; ++i) {
		odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt[i]);

		packet_init(pool, pkt_hdr, pkt_size, 0);
		if (len == 0)
			pull_tail(pkt_hdr, pkt_size);
	}

	return count;
}

void odp_packet_free(odp_packet_t pkt)
{
	odp_buffer_free((odp_buffer_t)pkt);
}

void odp_packet_free_multi(const odp_packet_t pkt[], int num)
{
	odp_buffer_free_multi((const odp_buffer_t *)pkt, num);
}

int odp_packet_reset(odp_packet_t pkt, uint32_t len)
{
	odp_packet_hdr_t *const pkt_hdr = odp_packet_hdr(pkt);
	pool_entry_t *pool = odp_buf_to_pool(&pkt_hdr->buf_hdr);
	uint32_t totsize = pool->s.headroom + len + pool->s.tailroom;

	if (totsize > pkt_hdr->buf_hdr.size)
		return -1;

	packet_init(pool, pkt_hdr, len, 0);

	return 0;
}

odp_packet_t _odp_packet_from_buffer(odp_buffer_t buf)
{
	return (odp_packet_t)buf;
}

odp_buffer_t _odp_packet_to_buffer(odp_packet_t pkt)
{
	return (odp_buffer_t)pkt;
}

odp_packet_t odp_packet_from_event(odp_event_t ev)
{
	return (odp_packet_t)ev;
}

odp_event_t odp_packet_to_event(odp_packet_t pkt)
{
	return (odp_event_t)pkt;
}

/*
 *
 * Pointers and lengths
 * ********************************************************
 *
 */

void *odp_packet_head(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	return buffer_map(&pkt_hdr->buf_hdr, 0, NULL, 0);
}

uint32_t odp_packet_buf_len(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.size;
}

void *odp_packet_data(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	return packet_map(pkt_hdr, 0, NULL);
}

uint32_t odp_packet_seg_len(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	uint32_t seglen;

	/* Call returns length of 1st data segment */
	packet_map(pkt_hdr, 0, &seglen);
	return seglen;
}

uint32_t odp_packet_len(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->frame_len;
}

uint32_t odp_packet_headroom(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->headroom;
}

uint32_t odp_packet_tailroom(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->tailroom;
}

void *odp_packet_tail(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	return packet_map(pkt_hdr, pkt_hdr->frame_len, NULL);
}

void *odp_packet_push_head(odp_packet_t pkt, uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (len > pkt_hdr->headroom)
		return NULL;

	push_head(pkt_hdr, len);
	return packet_map(pkt_hdr, 0, NULL);
}

void *odp_packet_pull_head(odp_packet_t pkt, uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (len > pkt_hdr->frame_len)
		return NULL;

	pull_head(pkt_hdr, len);
	return packet_map(pkt_hdr, 0, NULL);
}

void *odp_packet_push_tail(odp_packet_t pkt, uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	uint32_t origin = pkt_hdr->frame_len;

	if (len > pkt_hdr->tailroom)
		return NULL;

	push_tail(pkt_hdr, len);
	return packet_map(pkt_hdr, origin, NULL);
}

void *odp_packet_pull_tail(odp_packet_t pkt, uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (len > pkt_hdr->frame_len)
		return NULL;

	pull_tail(pkt_hdr, len);
	return packet_map(pkt_hdr, pkt_hdr->frame_len, NULL);
}

void *odp_packet_offset(odp_packet_t pkt, uint32_t offset, uint32_t *len,
			odp_packet_seg_t *seg)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	void *addr = packet_map(pkt_hdr, offset, len);

	if (addr != NULL && seg != NULL) {
		odp_buffer_bits_t seghandle;
		seghandle.handle = (odp_buffer_t)pkt;
		seghandle.seg = (pkt_hdr->headroom + offset) /
			pkt_hdr->buf_hdr.segsize;
		*seg = (odp_packet_seg_t)seghandle.handle;
	}

	return addr;
}

/*
 *
 * Meta-data
 * ********************************************************
 *
 */

odp_pool_t odp_packet_pool(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.pool_hdl;
}

odp_pktio_t odp_packet_input(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->input;
}

void *odp_packet_user_ptr(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.buf_ctx;
}

void odp_packet_user_ptr_set(odp_packet_t pkt, const void *ctx)
{
	odp_packet_hdr(pkt)->buf_hdr.buf_cctx = ctx;
}

void *odp_packet_user_area(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.uarea_addr;
}

uint32_t odp_packet_user_area_size(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.uarea_size;
}

void *odp_packet_l2_ptr(odp_packet_t pkt, uint32_t *len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (!packet_hdr_has_l2(pkt_hdr))
		return NULL;
	return packet_map(pkt_hdr, pkt_hdr->l2_offset, len);
}

uint32_t odp_packet_l2_offset(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (!packet_hdr_has_l2(pkt_hdr))
		return ODP_PACKET_OFFSET_INVALID;
	return pkt_hdr->l2_offset;
}

int odp_packet_l2_offset_set(odp_packet_t pkt, uint32_t offset)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (offset >= pkt_hdr->frame_len)
		return -1;

	packet_hdr_has_l2_set(pkt_hdr, 1);
	pkt_hdr->l2_offset = offset;
	return 0;
}

void *odp_packet_l3_ptr(odp_packet_t pkt, uint32_t *len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	return packet_map(pkt_hdr, pkt_hdr->l3_offset, len);
}

uint32_t odp_packet_l3_offset(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	return pkt_hdr->l3_offset;
}

int odp_packet_l3_offset_set(odp_packet_t pkt, uint32_t offset)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (offset >= pkt_hdr->frame_len)
		return -1;

	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	pkt_hdr->l3_offset = offset;
	return 0;
}

void *odp_packet_l4_ptr(odp_packet_t pkt, uint32_t *len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	return packet_map(pkt_hdr, pkt_hdr->l4_offset, len);
}

uint32_t odp_packet_l4_offset(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	return pkt_hdr->l4_offset;
}

int odp_packet_l4_offset_set(odp_packet_t pkt, uint32_t offset)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (offset >= pkt_hdr->frame_len)
		return -1;

	if (packet_parse_not_complete(pkt_hdr))
		packet_parse_full(pkt_hdr);
	pkt_hdr->l4_offset = offset;
	return 0;
}

uint32_t odp_packet_flow_hash(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	return pkt_hdr->flow_hash;
}

void odp_packet_flow_hash_set(odp_packet_t pkt, uint32_t flow_hash)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	pkt_hdr->flow_hash = flow_hash;
	pkt_hdr->has_hash = 1;
}

int odp_packet_is_segmented(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.segcount > 1;
}

int odp_packet_num_segs(odp_packet_t pkt)
{
	return odp_packet_hdr(pkt)->buf_hdr.segcount;
}

odp_packet_seg_t odp_packet_first_seg(odp_packet_t pkt)
{
	return (odp_packet_seg_t)pkt;
}

odp_packet_seg_t odp_packet_last_seg(odp_packet_t pkt)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	odp_buffer_bits_t seghandle;

	seghandle.handle = (odp_buffer_t)pkt;
	seghandle.seg = pkt_hdr->buf_hdr.segcount - 1;
	return (odp_packet_seg_t)seghandle.handle;
}

odp_packet_seg_t odp_packet_next_seg(odp_packet_t pkt, odp_packet_seg_t seg)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	return (odp_packet_seg_t)segment_next(&pkt_hdr->buf_hdr,
					      (odp_buffer_seg_t)seg);
}

/*
 *
 * Segment level
 * ********************************************************
 *
 */

void *odp_packet_seg_buf_addr(odp_packet_t pkt, odp_packet_seg_t seg)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	return segment_map(&pkt_hdr->buf_hdr, (odp_buffer_seg_t)seg, NULL,
			   pkt_hdr->headroom + pkt_hdr->frame_len, 0);
}

uint32_t odp_packet_seg_buf_len(odp_packet_t pkt,
				odp_packet_seg_t seg ODP_UNUSED)
{
	return odp_packet_hdr(pkt)->buf_hdr.segsize;
}

void *odp_packet_seg_data(odp_packet_t pkt, odp_packet_seg_t seg)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	return segment_map(&pkt_hdr->buf_hdr, (odp_buffer_seg_t)seg, NULL,
			   pkt_hdr->frame_len, pkt_hdr->headroom);
}

uint32_t odp_packet_seg_data_len(odp_packet_t pkt, odp_packet_seg_t seg)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	uint32_t seglen = 0;

	segment_map(&pkt_hdr->buf_hdr, (odp_buffer_seg_t)seg, &seglen,
		    pkt_hdr->frame_len, pkt_hdr->headroom);

	return seglen;
}

/*
 *
 * Manipulation
 * ********************************************************
 *
 */

odp_packet_t odp_packet_add_data(odp_packet_t pkt, uint32_t offset,
				 uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	uint32_t pktlen = pkt_hdr->frame_len;
	odp_packet_t newpkt;

	if (offset > pktlen)
		return ODP_PACKET_INVALID;

	newpkt = odp_packet_alloc(pkt_hdr->buf_hdr.pool_hdl, pktlen + len);

	if (newpkt != ODP_PACKET_INVALID) {
		if (_odp_packet_copy_to_packet(pkt, 0,
					       newpkt, 0, offset) != 0 ||
		    _odp_packet_copy_to_packet(pkt, offset, newpkt,
					       offset + len,
					       pktlen - offset) != 0) {
			odp_packet_free(newpkt);
			newpkt = ODP_PACKET_INVALID;
		} else {
			_odp_packet_copy_md_to_packet(pkt, newpkt);
			odp_packet_free(pkt);
		}
	}

	return newpkt;
}

odp_packet_t odp_packet_rem_data(odp_packet_t pkt, uint32_t offset,
				 uint32_t len)
{
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);
	uint32_t pktlen = pkt_hdr->frame_len;
	odp_packet_t newpkt;

	if (offset > pktlen || offset + len > pktlen)
		return ODP_PACKET_INVALID;

	newpkt = odp_packet_alloc(pkt_hdr->buf_hdr.pool_hdl, pktlen - len);

	if (newpkt != ODP_PACKET_INVALID) {
		if (_odp_packet_copy_to_packet(pkt, 0,
					       newpkt, 0, offset) != 0 ||
		    _odp_packet_copy_to_packet(pkt, offset + len,
					       newpkt, offset,
					       pktlen - offset - len) != 0) {
			odp_packet_free(newpkt);
			newpkt = ODP_PACKET_INVALID;
		} else {
			_odp_packet_copy_md_to_packet(pkt, newpkt);
			odp_packet_free(pkt);
		}
	}

	return newpkt;
}

/*
 *
 * Copy
 * ********************************************************
 *
 */

odp_packet_t odp_packet_copy(odp_packet_t pkt, odp_pool_t pool)
{
	odp_packet_hdr_t *srchdr = odp_packet_hdr(pkt);
	uint32_t pktlen = srchdr->frame_len;
	uint32_t meta_offset = ODP_FIELD_SIZEOF(odp_packet_hdr_t, buf_hdr);
	odp_packet_t newpkt = odp_packet_alloc(pool, pktlen);

	if (newpkt != ODP_PACKET_INVALID) {
		odp_packet_hdr_t *newhdr = odp_packet_hdr(newpkt);
		uint8_t *newstart, *srcstart;

		/* Must copy metadata first, followed by packet data */
		newstart = (uint8_t *)newhdr + meta_offset;
		srcstart = (uint8_t *)srchdr + meta_offset;

		memcpy(newstart, srcstart,
		       sizeof(odp_packet_hdr_t) - meta_offset);

		if (_odp_packet_copy_to_packet(pkt, 0,
					       newpkt, 0, pktlen) != 0) {
			odp_packet_free(newpkt);
			newpkt = ODP_PACKET_INVALID;
		}
	}

	return newpkt;
}

int odp_packet_copydata_out(odp_packet_t pkt, uint32_t offset,
			    uint32_t len, void *dst)
{
	void *mapaddr;
	uint32_t seglen = 0; /* GCC */
	uint32_t cpylen;
	uint8_t *dstaddr = (uint8_t *)dst;
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (offset + len > pkt_hdr->frame_len)
		return -1;

	while (len > 0) {
		mapaddr = packet_map(pkt_hdr, offset, &seglen);
		cpylen = len > seglen ? seglen : len;
		memcpy(dstaddr, mapaddr, cpylen);
		offset  += cpylen;
		dstaddr += cpylen;
		len     -= cpylen;
	}

	return 0;
}

int odp_packet_copydata_in(odp_packet_t pkt, uint32_t offset,
			   uint32_t len, const void *src)
{
	void *mapaddr;
	uint32_t seglen = 0; /* GCC */
	uint32_t cpylen;
	const uint8_t *srcaddr = (const uint8_t *)src;
	odp_packet_hdr_t *pkt_hdr = odp_packet_hdr(pkt);

	if (offset + len > pkt_hdr->frame_len)
		return -1;

	while (len > 0) {
		mapaddr = packet_map(pkt_hdr, offset, &seglen);
		cpylen = len > seglen ? seglen : len;
		memcpy(mapaddr, srcaddr, cpylen);
		offset  += cpylen;
		srcaddr += cpylen;
		len     -= cpylen;
	}

	return 0;
}

/*
 *
 * Debugging
 * ********************************************************
 *
 */

void odp_packet_print(odp_packet_t pkt)
{
	int max_len = 512;
	char str[max_len];
	int len = 0;
	int n = max_len-1;
	odp_packet_hdr_t *hdr = odp_packet_hdr(pkt);

	len += snprintf(&str[len], n-len, "Packet ");
	len += odp_buffer_snprint(&str[len], n-len, (odp_buffer_t) pkt);
	len += snprintf(&str[len], n-len,
			"  input_flags  0x%" PRIx32 "\n", hdr->input_flags.all);
	len += snprintf(&str[len], n-len,
			"  error_flags  0x%" PRIx32 "\n", hdr->error_flags.all);
	len += snprintf(&str[len], n-len,
			"  output_flags 0x%" PRIx32 "\n", hdr->output_flags.all);
	len += snprintf(&str[len], n-len,
			"  l2_offset    %" PRIu32 "\n", hdr->l2_offset);
	len += snprintf(&str[len], n-len,
			"  l3_offset    %" PRIu32 "\n", hdr->l3_offset);
	len += snprintf(&str[len], n-len,
			"  l4_offset    %" PRIu32 "\n", hdr->l4_offset);
	len += snprintf(&str[len], n-len,
			"  frame_len    %" PRIu32 "\n", hdr->frame_len);
	len += snprintf(&str[len], n-len,
			"  input        %" PRIu64 "\n",
			odp_pktio_to_u64(hdr->input));
	str[len] = '\0';

	ODP_PRINT("\n%s\n", str);
}

int odp_packet_is_valid(odp_packet_t pkt)
{
	odp_buffer_hdr_t *buf = validate_buf((odp_buffer_t)pkt);

	return (buf != NULL && buf->type == ODP_EVENT_PACKET);
}

/*
 *
 * Internal Use Routines
 * ********************************************************
 *
 */

void _odp_packet_copy_md_to_packet(odp_packet_t srcpkt, odp_packet_t dstpkt)
{
	odp_packet_hdr_t *srchdr = odp_packet_hdr(srcpkt);
	odp_packet_hdr_t *dsthdr = odp_packet_hdr(dstpkt);

	dsthdr->input = srchdr->input;
	dsthdr->buf_hdr.buf_u64 = srchdr->buf_hdr.buf_u64;
	if (dsthdr->buf_hdr.uarea_addr != NULL &&
	    srchdr->buf_hdr.uarea_addr != NULL)
		memcpy(dsthdr->buf_hdr.uarea_addr,
		       srchdr->buf_hdr.uarea_addr,
		       dsthdr->buf_hdr.uarea_size <=
		       srchdr->buf_hdr.uarea_size ?
		       dsthdr->buf_hdr.uarea_size :
		       srchdr->buf_hdr.uarea_size);
	odp_atomic_store_u32(
		&dsthdr->buf_hdr.ref_count,
		odp_atomic_load_u32(
			&srchdr->buf_hdr.ref_count));
	copy_packet_parser_metadata(srchdr, dsthdr);
}

int _odp_packet_copy_to_packet(odp_packet_t srcpkt, uint32_t srcoffset,
			       odp_packet_t dstpkt, uint32_t dstoffset,
			       uint32_t len)
{
	odp_packet_hdr_t *srchdr = odp_packet_hdr(srcpkt);
	odp_packet_hdr_t *dsthdr = odp_packet_hdr(dstpkt);
	void *srcmap;
	void *dstmap;
	uint32_t cpylen, minseg;
	uint32_t srcseglen = 0; /* GCC */
	uint32_t dstseglen = 0; /* GCC */

	if (srcoffset + len > srchdr->frame_len ||
	    dstoffset + len > dsthdr->frame_len)
		return -1;

	while (len > 0) {
		srcmap = packet_map(srchdr, srcoffset, &srcseglen);
		dstmap = packet_map(dsthdr, dstoffset, &dstseglen);

		minseg = dstseglen > srcseglen ? srcseglen : dstseglen;
		cpylen = len > minseg ? minseg : len;
		memcpy(dstmap, srcmap, cpylen);

		srcoffset += cpylen;
		dstoffset += cpylen;
		len       -= cpylen;
	}

	return 0;
}

/**
 * Parser helper function for IPv4
 */
static inline uint8_t parse_ipv4(odp_packet_hdr_t *pkt_hdr,
				 const uint8_t **parseptr, uint32_t *offset)
{
	const odph_ipv4hdr_t *ipv4 = (const odph_ipv4hdr_t *)*parseptr;
	uint8_t ver = ODPH_IPV4HDR_VER(ipv4->ver_ihl);
	uint8_t ihl = ODPH_IPV4HDR_IHL(ipv4->ver_ihl);
	uint16_t frag_offset;
	uint32_t dstaddr = odp_be_to_cpu_32(ipv4->dst_addr);

	pkt_hdr->l3_len = odp_be_to_cpu_16(ipv4->tot_len);

	if (odp_unlikely(ihl < ODPH_IPV4HDR_IHL_MIN) ||
	    odp_unlikely(ver != 4) ||
	    (pkt_hdr->l3_len > pkt_hdr->frame_len - *offset)) {
		pkt_hdr->error_flags.ip_err = 1;
		return 0;
	}

	*offset   += ihl * 4;
	*parseptr += ihl * 4;

	if (odp_unlikely(ihl > ODPH_IPV4HDR_IHL_MIN))
		pkt_hdr->input_flags.ipopt = 1;

	/* A packet is a fragment if:
	*  "more fragments" flag is set (all fragments except the last)
	*     OR
	*  "fragment offset" field is nonzero (all fragments except the first)
	*/
	frag_offset = odp_be_to_cpu_16(ipv4->frag_offset);
	if (odp_unlikely(ODPH_IPV4HDR_IS_FRAGMENT(frag_offset)))
		pkt_hdr->input_flags.ipfrag = 1;

	/* Handle IPv4 broadcast / multicast */
	pkt_hdr->input_flags.ip_bcast = (dstaddr == 0xffffffff);
	pkt_hdr->input_flags.ip_mcast = (dstaddr >> 28) == 0xd;

	return ipv4->proto;
}

/**
 * Parser helper function for IPv6
 */
static inline uint8_t parse_ipv6(odp_packet_hdr_t *pkt_hdr,
				 const uint8_t **parseptr, uint32_t *offset)
{
	const odph_ipv6hdr_t *ipv6 = (const odph_ipv6hdr_t *)*parseptr;
	const odph_ipv6hdr_ext_t *ipv6ext;
	uint32_t dstaddr0 = odp_be_to_cpu_32(ipv6->dst_addr[0]);

	pkt_hdr->l3_len = odp_be_to_cpu_16(ipv6->payload_len);

	/* Basic sanity checks on IPv6 header */
	if ((odp_be_to_cpu_32(ipv6->ver_tc_flow) >> 28) != 6 ||
	    pkt_hdr->l3_len > pkt_hdr->frame_len - *offset) {
		pkt_hdr->error_flags.ip_err = 1;
		return 0;
	}

	/* IPv6 broadcast / multicast flags */
	pkt_hdr->input_flags.ip_mcast = (dstaddr0 & 0xff000000) == 0xff000000;
	pkt_hdr->input_flags.ip_bcast = 0;

	/* Skip past IPv6 header */
	*offset   += sizeof(odph_ipv6hdr_t);
	*parseptr += sizeof(odph_ipv6hdr_t);

	/* Skip past any IPv6 extension headers */
	if (ipv6->next_hdr == ODPH_IPPROTO_HOPOPTS ||
	    ipv6->next_hdr == ODPH_IPPROTO_ROUTE) {
		pkt_hdr->input_flags.ipopt = 1;

		do  {
			ipv6ext    = (const odph_ipv6hdr_ext_t *)*parseptr;
			uint16_t extlen = 8 + ipv6ext->ext_len * 8;

			*offset   += extlen;
			*parseptr += extlen;
		} while ((ipv6ext->next_hdr == ODPH_IPPROTO_HOPOPTS ||
			  ipv6ext->next_hdr == ODPH_IPPROTO_ROUTE) &&
			*offset < pkt_hdr->frame_len);

		if (*offset >= pkt_hdr->l3_offset +
		    odp_be_to_cpu_16(ipv6->payload_len)) {
			pkt_hdr->error_flags.ip_err = 1;
			return 0;
		}

		if (ipv6ext->next_hdr == ODPH_IPPROTO_FRAG)
			pkt_hdr->input_flags.ipfrag = 1;

		return ipv6ext->next_hdr;
	}

	if (odp_unlikely(ipv6->next_hdr == ODPH_IPPROTO_FRAG)) {
		pkt_hdr->input_flags.ipopt = 1;
		pkt_hdr->input_flags.ipfrag = 1;
	}

	return ipv6->next_hdr;
}

/**
 * Parser helper function for TCP
 */
static inline void parse_tcp(odp_packet_hdr_t *pkt_hdr,
			     const uint8_t **parseptr, uint32_t *offset)
{
	const odph_tcphdr_t *tcp = (const odph_tcphdr_t *)*parseptr;

	if (tcp->hl < sizeof(odph_tcphdr_t)/sizeof(uint32_t))
		pkt_hdr->error_flags.tcp_err = 1;
	else if ((uint32_t)tcp->hl * 4 > sizeof(odph_tcphdr_t))
		pkt_hdr->input_flags.tcpopt = 1;

	pkt_hdr->l4_len = pkt_hdr->l3_len +
		pkt_hdr->l3_offset - pkt_hdr->l4_offset;

	*offset   += (uint32_t)tcp->hl * 4;
	*parseptr += (uint32_t)tcp->hl * 4;
}

/**
 * Parser helper function for UDP
 */
static inline void parse_udp(odp_packet_hdr_t *pkt_hdr,
			     const uint8_t **parseptr, uint32_t *offset)
{
	const odph_udphdr_t *udp = (const odph_udphdr_t *)*parseptr;
	uint32_t udplen = odp_be_to_cpu_16(udp->length);

	if (udplen < sizeof(odph_udphdr_t) ||
	    udplen > (pkt_hdr->l3_len +
		      pkt_hdr->l3_offset - pkt_hdr->l4_offset)) {
		pkt_hdr->error_flags.udp_err = 1;
	}

	pkt_hdr->l4_len = udplen;

	*offset   += sizeof(odph_udphdr_t);
	*parseptr += sizeof(odph_udphdr_t);
}

/**
 * Initialize L2 related parser flags and metadata
 */
void packet_parse_l2(odp_packet_hdr_t *pkt_hdr)
{
	/* Packet alloc or reset have already init other offsets and flags */

	/* We only support Ethernet for now */
	pkt_hdr->input_flags.eth = 1;

	/* Detect jumbo frames */
	if (pkt_hdr->frame_len > ODPH_ETH_LEN_MAX)
		pkt_hdr->input_flags.jumbo = 1;

	/* Assume valid L2 header, no CRC/FCS check in SW */
	pkt_hdr->input_flags.l2 = 1;

	pkt_hdr->input_flags.parsed_l2 = 1;
}

int _odp_parse_common(odp_packet_hdr_t *pkt_hdr, const uint8_t *ptr)
{
	const odph_ethhdr_t *eth;
	const odph_vlanhdr_t *vlan;
	uint16_t ethtype;
	uint32_t offset, seglen;
	uint8_t ip_proto = 0;
	const uint8_t *parseptr;
	uint16_t macaddr0, macaddr2, macaddr4;

	offset = sizeof(odph_ethhdr_t);
	if (packet_parse_l2_not_done(pkt_hdr))
		packet_parse_l2(pkt_hdr);

	eth = ptr ? (const odph_ethhdr_t *)ptr :
		(odph_ethhdr_t *)packet_map(pkt_hdr, 0, &seglen);

	/* Handle Ethernet broadcast/multicast addresses */
	macaddr0 = odp_be_to_cpu_16(*((const uint16_t *)(const void *)eth));
	pkt_hdr->input_flags.eth_mcast = (macaddr0 & 0x0100) == 0x0100;

	if (macaddr0 == 0xffff) {
		macaddr2 =
			odp_be_to_cpu_16(*((const uint16_t *)
					   (const void *)eth + 1));
		macaddr4 =
			odp_be_to_cpu_16(*((const uint16_t *)
					   (const void *)eth + 2));
		pkt_hdr->input_flags.eth_bcast =
			(macaddr2 == 0xffff) && (macaddr4 == 0xffff);
	} else {
		pkt_hdr->input_flags.eth_bcast = 0;
	}

	/* Get Ethertype */
	parseptr = (const uint8_t *)&eth->type;
	ethtype = odp_be_to_cpu_16(*((const uint16_t *)
				     (const void *)parseptr));

	/* Parse the VLAN header(s), if present */
	if (ethtype == ODPH_ETHTYPE_VLAN_OUTER) {
		pkt_hdr->input_flags.vlan_qinq = 1;
		pkt_hdr->input_flags.vlan = 1;

		vlan = (const odph_vlanhdr_t *)(const void *)parseptr;
		pkt_hdr->vlan_s_tag = ((ethtype << 16) |
				       odp_be_to_cpu_16(vlan->tci));
		offset += sizeof(odph_vlanhdr_t);
		parseptr += sizeof(odph_vlanhdr_t);
		ethtype = odp_be_to_cpu_16(*((const uint16_t *)
					   (const void *)parseptr));
	}

	if (ethtype == ODPH_ETHTYPE_VLAN) {
		pkt_hdr->input_flags.vlan = 1;
		vlan = (const odph_vlanhdr_t *)(const void *)parseptr;
		pkt_hdr->vlan_c_tag = ((ethtype << 16) |
				       odp_be_to_cpu_16(vlan->tci));
		offset += sizeof(odph_vlanhdr_t);
		parseptr += sizeof(odph_vlanhdr_t);
		ethtype = odp_be_to_cpu_16(*((const uint16_t *)
					   (const void *)parseptr));
	}

	/* Check for SNAP vs. DIX */
	if (ethtype < ODPH_ETH_LEN_MAX) {
		pkt_hdr->input_flags.snap = 1;
		if (ethtype > pkt_hdr->frame_len - offset) {
			pkt_hdr->error_flags.snap_len = 1;
			goto parse_exit;
		}
		offset   += 8;
		parseptr += 8;
		ethtype = odp_be_to_cpu_16(*((const uint16_t *)
					   (const void *)parseptr));
	}

	/* Consume Ethertype for Layer 3 parse */
	parseptr += 2;

	/* Set l3_offset+flag only for known ethtypes */
	pkt_hdr->input_flags.l3 = 1;
	pkt_hdr->l3_offset = offset;
	pkt_hdr->l3_protocol = ethtype;

	/* Parse Layer 3 headers */
	switch (ethtype) {
	case ODPH_ETHTYPE_IPV4:
		pkt_hdr->input_flags.ipv4 = 1;
		ip_proto = parse_ipv4(pkt_hdr, &parseptr, &offset);
		break;

	case ODPH_ETHTYPE_IPV6:
		pkt_hdr->input_flags.ipv6 = 1;
		ip_proto = parse_ipv6(pkt_hdr, &parseptr, &offset);
		break;

	case ODPH_ETHTYPE_ARP:
		pkt_hdr->input_flags.arp = 1;
		ip_proto = 255;  /* Reserved invalid by IANA */
		break;

	default:
		pkt_hdr->input_flags.l3 = 0;
		pkt_hdr->l3_offset = ODP_PACKET_OFFSET_INVALID;
		ip_proto = 255;  /* Reserved invalid by IANA */
	}

	/* Set l4_offset+flag only for known ip_proto */
	pkt_hdr->input_flags.l4 = 1;
	pkt_hdr->l4_offset = offset;
	pkt_hdr->l4_protocol = ip_proto;

	/* Parse Layer 4 headers */
	switch (ip_proto) {
	case ODPH_IPPROTO_ICMP:
		pkt_hdr->input_flags.icmp = 1;
		break;

	case ODPH_IPPROTO_TCP:
		pkt_hdr->input_flags.tcp = 1;
		parse_tcp(pkt_hdr, &parseptr, &offset);
		break;

	case ODPH_IPPROTO_UDP:
		pkt_hdr->input_flags.udp = 1;
		parse_udp(pkt_hdr, &parseptr, &offset);
		break;

	case ODPH_IPPROTO_AH:
	case ODPH_IPPROTO_ESP:
		pkt_hdr->input_flags.ipsec = 1;
		break;

	default:
		pkt_hdr->input_flags.l4 = 0;
		pkt_hdr->l4_offset = ODP_PACKET_OFFSET_INVALID;
		break;
	}

       /*
	* Anything beyond what we parse here is considered payload.
	* Note: Payload is really only relevant for TCP and UDP.  For
	* all other protocols, the payload offset will point to the
	* final header (ARP, ICMP, AH, ESP, or IP Fragment).
	*/
	pkt_hdr->payload_offset = offset;

parse_exit:
	pkt_hdr->input_flags.parsed_all = 1;
	return pkt_hdr->error_flags.all != 0;
}

int _odp_cls_parse(odp_packet_hdr_t *pkt_hdr, const uint8_t *parseptr)
{
	return _odp_parse_common(pkt_hdr, parseptr);
}

/**
 * Simple packet parser
 */
int packet_parse_full(odp_packet_hdr_t *pkt_hdr)
{
	return _odp_parse_common(pkt_hdr, NULL);
}
