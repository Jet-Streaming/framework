/* Copyright (c) 2013, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include <odp/api/packet_io.h>
#include <odp_packet_io_internal.h>
#include <odp_packet_io_queue.h>
#include <odp/api/packet.h>
#include <odp_packet_internal.h>
#include <odp_internal.h>
#include <odp/api/spinlock.h>
#include <odp/api/ticketlock.h>
#include <odp/api/shared_memory.h>
#include <odp_packet_socket.h>
#include <odp/api/config.h>
#include <odp_queue_internal.h>
#include <odp_schedule_internal.h>
#include <odp_classification_internal.h>
#include <odp_debug_internal.h>

#include <string.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <errno.h>

pktio_table_t *pktio_tbl;

/* pktio pointer entries ( for inlines) */
void *pktio_entry_ptr[ODP_CONFIG_PKTIO_ENTRIES];

int odp_pktio_init_global(void)
{
	pktio_entry_t *pktio_entry;
	int id;
	odp_shm_t shm;
	int pktio_if;

	shm = odp_shm_reserve("odp_pktio_entries",
			      sizeof(pktio_table_t),
			      sizeof(pktio_entry_t), 0);
	pktio_tbl = odp_shm_addr(shm);

	if (pktio_tbl == NULL)
		return -1;

	memset(pktio_tbl, 0, sizeof(pktio_table_t));

	odp_spinlock_init(&pktio_tbl->lock);

	for (id = 1; id <= ODP_CONFIG_PKTIO_ENTRIES; ++id) {
		pktio_entry = &pktio_tbl->entries[id - 1];

		odp_ticketlock_init(&pktio_entry->s.rxl);
		odp_ticketlock_init(&pktio_entry->s.txl);
		odp_spinlock_init(&pktio_entry->s.cls.l2_cos_table.lock);
		odp_spinlock_init(&pktio_entry->s.cls.l3_cos_table.lock);

		pktio_entry_ptr[id - 1] = pktio_entry;
	}

	for (pktio_if = 0; pktio_if_ops[pktio_if]; ++pktio_if) {
		if (pktio_if_ops[pktio_if]->init_global)
			if (pktio_if_ops[pktio_if]->init_global()) {
				ODP_ERR("failed to initialized pktio type %d",
					pktio_if);
				return -1;
			}
	}

	return 0;
}

int odp_pktio_init_local(void)
{
	int pktio_if;

	for (pktio_if = 0; pktio_if_ops[pktio_if]; ++pktio_if) {
		if (pktio_if_ops[pktio_if]->init_local)
			if (pktio_if_ops[pktio_if]->init_local()) {
				ODP_ERR("failed to initialized pktio type %d",
					pktio_if);
				return -1;
			}
	}

	return 0;
}

int is_free(pktio_entry_t *entry)
{
	return (entry->s.taken == 0);
}

static void set_free(pktio_entry_t *entry)
{
	entry->s.taken = 0;
}

static void set_taken(pktio_entry_t *entry)
{
	entry->s.taken = 1;
}

static void lock_entry(pktio_entry_t *entry)
{
	odp_ticketlock_lock(&entry->s.rxl);
	odp_ticketlock_lock(&entry->s.txl);
}

static void unlock_entry(pktio_entry_t *entry)
{
	odp_ticketlock_unlock(&entry->s.txl);
	odp_ticketlock_unlock(&entry->s.rxl);
}

static void init_in_queues(pktio_entry_t *entry)
{
	int i;

	for (i = 0; i < PKTIO_MAX_QUEUES; i++) {
		entry->s.in_queue[i].queue = ODP_QUEUE_INVALID;
		entry->s.in_queue[i].pktin = PKTIN_INVALID;
	}
}

static void init_out_queues(pktio_entry_t *entry)
{
	int i;

	for (i = 0; i < PKTIO_MAX_QUEUES; i++) {
		entry->s.out_queue[i].queue  = ODP_QUEUE_INVALID;
		entry->s.out_queue[i].pktout = PKTOUT_INVALID;
	}
}

static void init_pktio_entry(pktio_entry_t *entry)
{
	set_taken(entry);
	pktio_cls_enabled_set(entry, 0);

	init_in_queues(entry);
	init_out_queues(entry);

	pktio_classifier_init(entry);
}

static odp_pktio_t alloc_lock_pktio_entry(void)
{
	odp_pktio_t id;
	pktio_entry_t *entry;
	int i;

	for (i = 0; i < ODP_CONFIG_PKTIO_ENTRIES; ++i) {
		entry = &pktio_tbl->entries[i];
		if (is_free(entry)) {
			lock_entry(entry);
			if (is_free(entry)) {
				init_pktio_entry(entry);
				id = _odp_cast_scalar(odp_pktio_t, i + 1);
				return id; /* return with entry locked! */
			}
			unlock_entry(entry);
		}
	}

	return ODP_PKTIO_INVALID;
}

static int free_pktio_entry(odp_pktio_t id)
{
	pktio_entry_t *entry = get_pktio_entry(id);

	if (entry == NULL)
		return -1;

	set_free(entry);

	return 0;
}

static odp_pktio_t setup_pktio_entry(const char *name, odp_pool_t pool,
				     const odp_pktio_param_t *param)
{
	odp_pktio_t id;
	pktio_entry_t *pktio_entry;
	int ret = -1;
	int pktio_if;

	if (strlen(name) >= PKTIO_NAME_LEN - 1) {
		/* ioctl names limitation */
		ODP_ERR("pktio name %s is too big, limit is %d bytes\n",
			name, PKTIO_NAME_LEN - 1);
		return ODP_PKTIO_INVALID;
	}

	id = alloc_lock_pktio_entry();
	if (id == ODP_PKTIO_INVALID) {
		ODP_ERR("No resources available.\n");
		return ODP_PKTIO_INVALID;
	}

	/* if successful, alloc_pktio_entry() returns with the entry locked */
	pktio_entry = get_pktio_entry(id);
	if (!pktio_entry)
		return ODP_PKTIO_INVALID;

	pktio_entry->s.pool = pool;
	memcpy(&pktio_entry->s.param, param, sizeof(odp_pktio_param_t));
	pktio_entry->s.handle = id;

	for (pktio_if = 0; pktio_if_ops[pktio_if]; ++pktio_if) {
		ret = pktio_if_ops[pktio_if]->open(id, pktio_entry, name, pool);

		if (!ret) {
			pktio_entry->s.ops = pktio_if_ops[pktio_if];
			ODP_DBG("%s uses %s\n",
				name, pktio_if_ops[pktio_if]->name);
			break;
		}
	}

	if (ret != 0) {
		free_pktio_entry(id);
		id = ODP_PKTIO_INVALID;
		ODP_ERR("Unable to init any I/O type.\n");
	} else {
		snprintf(pktio_entry->s.name,
			 sizeof(pktio_entry->s.name), "%s", name);
		pktio_entry->s.state = STATE_OPENED;
	}

	unlock_entry(pktio_entry);

	return id;
}

static int pool_type_is_packet(odp_pool_t pool)
{
	odp_pool_info_t pool_info;

	if (pool == ODP_POOL_INVALID)
		return 0;

	if (odp_pool_info(pool, &pool_info) != 0)
		return 0;

	return pool_info.params.type == ODP_POOL_PACKET;
}

odp_pktio_t odp_pktio_open(const char *name, odp_pool_t pool,
			   const odp_pktio_param_t *param)
{
	odp_pktio_t id;

	ODP_ASSERT(pool_type_is_packet(pool));

	id = odp_pktio_lookup(name);
	if (id != ODP_PKTIO_INVALID) {
		/* interface is already open */
		__odp_errno = EEXIST;
		return ODP_PKTIO_INVALID;
	}

	odp_spinlock_lock(&pktio_tbl->lock);
	id = setup_pktio_entry(name, pool, param);
	odp_spinlock_unlock(&pktio_tbl->lock);

	return id;
}

static int _pktio_close(pktio_entry_t *entry)
{
	int ret;

	ret = entry->s.ops->close(entry);
	if (ret)
		return -1;

	set_free(entry);
	return 0;
}

static void destroy_in_queues(pktio_entry_t *entry, int num)
{
	int i;

	for (i = 0; i < num; i++) {
		if (entry->s.in_queue[i].queue != ODP_QUEUE_INVALID) {
			odp_queue_destroy(entry->s.in_queue[i].queue);
			entry->s.in_queue[i].queue = ODP_QUEUE_INVALID;
		}
	}
}

static void destroy_out_queues(pktio_entry_t *entry, int num)
{
	int i, rc;

	for (i = 0; i < num; i++) {
		if (entry->s.out_queue[i].queue != ODP_QUEUE_INVALID) {
			rc = odp_queue_destroy(entry->s.out_queue[i].queue);
			ODP_ASSERT(rc == 0);
			entry->s.out_queue[i].queue = ODP_QUEUE_INVALID;
		}
	}
}

static void flush_in_queues(pktio_entry_t *entry)
{
	odp_pktin_mode_t mode;
	int num, i;
	int max_pkts = 16;
	odp_packet_t packets[max_pkts];

	mode = entry->s.param.in_mode;
	num  = entry->s.num_in_queue;

	if (mode == ODP_PKTIN_MODE_DIRECT) {
		for (i = 0; i < num; i++) {
			int ret;
			odp_pktin_queue_t pktin = entry->s.in_queue[i].pktin;

			while ((ret = odp_pktin_recv(pktin, packets,
						     max_pkts))) {
				if (ret < 0) {
					ODP_ERR("Queue flush failed\n");
					return;
				}

				odp_packet_free_multi(packets, ret);
			}
		}
	}
}

int odp_pktio_close(odp_pktio_t id)
{
	pktio_entry_t *entry;
	int res;

	entry = get_pktio_entry(id);
	if (entry == NULL)
		return -1;

	if (entry->s.state == STATE_STOPPED)
		flush_in_queues(entry);

	lock_entry(entry);

	destroy_in_queues(entry, entry->s.num_in_queue);
	destroy_out_queues(entry, entry->s.num_out_queue);

	entry->s.num_in_queue  = 0;
	entry->s.num_out_queue = 0;

	if (!is_free(entry)) {
		res = _pktio_close(entry);
		if (res)
			ODP_ABORT("unable to close pktio\n");
	}
	unlock_entry(entry);

	return 0;
}

int odp_pktio_start(odp_pktio_t id)
{
	pktio_entry_t *entry;
	odp_pktin_mode_t mode;
	int res = 0;

	entry = get_pktio_entry(id);
	if (!entry)
		return -1;

	lock_entry(entry);
	if (entry->s.state == STATE_STARTED) {
		unlock_entry(entry);
		return -1;
	}
	if (entry->s.ops->start)
		res = entry->s.ops->start(entry);
	if (!res)
		entry->s.state = STATE_STARTED;

	unlock_entry(entry);

	mode = entry->s.param.in_mode;

	if (mode == ODP_PKTIN_MODE_SCHED) {
		unsigned i;

		for (i = 0; i < entry->s.num_in_queue; i++) {
			int index = i;

			if (entry->s.in_queue[i].queue == ODP_QUEUE_INVALID) {
				ODP_ERR("No input queue\n");
				return -1;
			}

			schedule_pktio_start(id, 1, &index);
		}
	}

	return res;
}

static int _pktio_stop(pktio_entry_t *entry)
{
	int res = 0;

	if (entry->s.state != STATE_STARTED)
		return -1;

	if (entry->s.ops->stop)
		res = entry->s.ops->stop(entry);
	if (!res)
		entry->s.state = STATE_STOPPED;

	return res;
}

int odp_pktio_stop(odp_pktio_t id)
{
	pktio_entry_t *entry;
	int res;

	entry = get_pktio_entry(id);
	if (!entry)
		return -1;

	lock_entry(entry);
	res = _pktio_stop(entry);
	unlock_entry(entry);

	return res;
}

odp_pktio_t odp_pktio_lookup(const char *name)
{
	odp_pktio_t id = ODP_PKTIO_INVALID;
	pktio_entry_t *entry;
	int i;

	odp_spinlock_lock(&pktio_tbl->lock);

	for (i = 1; i <= ODP_CONFIG_PKTIO_ENTRIES; ++i) {
		entry = get_pktio_entry(_odp_cast_scalar(odp_pktio_t, i));
		if (!entry || is_free(entry))
			continue;

		lock_entry(entry);

		if (!is_free(entry) &&
		    strncmp(entry->s.name, name, sizeof(entry->s.name)) == 0)
			id = _odp_cast_scalar(odp_pktio_t, i);

		unlock_entry(entry);

		if (id != ODP_PKTIO_INVALID)
			break;
	}

	odp_spinlock_unlock(&pktio_tbl->lock);

	return id;
}

static int _odp_pktio_recv(odp_pktio_t id, odp_packet_t pkt_table[], int len)
{
	pktio_entry_t *pktio_entry = get_pktio_entry(id);
	int pkts;
	int i;

	if (pktio_entry == NULL)
		return -1;

	odp_ticketlock_lock(&pktio_entry->s.rxl);
	if (pktio_entry->s.param.in_mode == ODP_PKTIN_MODE_DISABLED) {
		odp_ticketlock_unlock(&pktio_entry->s.rxl);
		__odp_errno = EPERM;
		return -1;
	}
	pkts = pktio_entry->s.ops->recv(pktio_entry, pkt_table, len);
	odp_ticketlock_unlock(&pktio_entry->s.rxl);

	if (pkts < 0)
		return pkts;

	for (i = 0; i < pkts; ++i)
		odp_packet_hdr(pkt_table[i])->input = id;

	return pkts;
}

static int _odp_pktio_send(odp_pktio_t id, odp_packet_t pkt_table[], int len)
{
	pktio_entry_t *pktio_entry = get_pktio_entry(id);
	int pkts;

	if (pktio_entry == NULL)
		return -1;

	odp_ticketlock_lock(&pktio_entry->s.txl);
	if (pktio_entry->s.state != STATE_STARTED ||
	    pktio_entry->s.param.out_mode == ODP_PKTOUT_MODE_DISABLED) {
			odp_ticketlock_unlock(&pktio_entry->s.txl);
		__odp_errno = EPERM;
		return -1;
	}
	pkts = pktio_entry->s.ops->send(pktio_entry, pkt_table, len);
	odp_ticketlock_unlock(&pktio_entry->s.txl);

	return pkts;
}

int pktout_enqueue(queue_entry_t *qentry, odp_buffer_hdr_t *buf_hdr)
{
	odp_packet_t pkt = _odp_packet_from_buffer(buf_hdr->handle.handle);
	int len = 1;
	int nbr;

	nbr = odp_pktout_send(qentry->s.pktout, &pkt, len);
	return (nbr == len ? 0 : -1);
}

odp_buffer_hdr_t *pktout_dequeue(queue_entry_t *qentry ODP_UNUSED)
{
	ODP_ABORT("attempted dequeue from a pktout queue");
	return NULL;
}

int pktout_enq_multi(queue_entry_t *qentry, odp_buffer_hdr_t *buf_hdr[],
		     int num)
{
	odp_packet_t pkt_tbl[QUEUE_MULTI_MAX];
	int nbr;
	int i;

	for (i = 0; i < num; ++i)
		pkt_tbl[i] = _odp_packet_from_buffer(buf_hdr[i]->handle.handle);

	nbr = odp_pktout_send(qentry->s.pktout, pkt_tbl, num);
	return nbr;
}

int pktout_deq_multi(queue_entry_t *qentry ODP_UNUSED,
		     odp_buffer_hdr_t *buf_hdr[] ODP_UNUSED,
		     int num ODP_UNUSED)
{
	ODP_ABORT("attempted dequeue from a pktout queue");
	return 0;
}

int pktin_enqueue(queue_entry_t *qentry ODP_UNUSED,
		  odp_buffer_hdr_t *buf_hdr ODP_UNUSED, int sustain ODP_UNUSED)
{
	ODP_ABORT("attempted enqueue to a pktin queue");
	return -1;
}

odp_buffer_hdr_t *pktin_dequeue(queue_entry_t *qentry)
{
	odp_buffer_hdr_t *buf_hdr;
	odp_buffer_t buf;
	odp_packet_t pkt_tbl[QUEUE_MULTI_MAX];
	odp_buffer_hdr_t *hdr_tbl[QUEUE_MULTI_MAX];
	int pkts, i;

	buf_hdr = queue_deq(qentry);
	if (buf_hdr != NULL)
		return buf_hdr;

	pkts = odp_pktin_recv(qentry->s.pktin, pkt_tbl, QUEUE_MULTI_MAX);

	if (pkts <= 0)
		return NULL;

	for (i = 0; i < pkts; i++) {
		buf        = _odp_packet_to_buffer(pkt_tbl[i]);
		hdr_tbl[i] = odp_buf_to_hdr(buf);
	}

	if (pkts > 1)
		queue_enq_multi(qentry, &hdr_tbl[1], pkts - 1, 0);
	buf_hdr = hdr_tbl[0];
	return buf_hdr;
}

int pktin_enq_multi(queue_entry_t *qentry ODP_UNUSED,
		    odp_buffer_hdr_t *buf_hdr[] ODP_UNUSED,
		    int num ODP_UNUSED, int sustain ODP_UNUSED)
{
	ODP_ABORT("attempted enqueue to a pktin queue");
	return 0;
}

int pktin_deq_multi(queue_entry_t *qentry, odp_buffer_hdr_t *buf_hdr[], int num)
{
	int nbr;
	odp_packet_t pkt_tbl[QUEUE_MULTI_MAX];
	odp_buffer_hdr_t *hdr_tbl[QUEUE_MULTI_MAX];
	odp_buffer_t buf;
	int pkts, i, j;

	nbr = queue_deq_multi(qentry, buf_hdr, num);
	if (odp_unlikely(nbr > num))
		ODP_ABORT("queue_deq_multi req: %d, returned %d\n",
			num, nbr);

	/** queue already has number of requsted buffers,
	 *  do not do receive in that case.
	 */
	if (nbr == num)
		return nbr;

	pkts = odp_pktin_recv(qentry->s.pktin, pkt_tbl, QUEUE_MULTI_MAX);
	if (pkts <= 0)
		return nbr;

	/* Fill in buf_hdr first */
	for (i = 0; i < pkts && nbr < num; i++, nbr++) {
		buf        = _odp_packet_to_buffer(pkt_tbl[i]);
		buf_hdr[nbr] = odp_buf_to_hdr(buf);
	}
	/* Queue the rest for later */
	for (j = 0; i < pkts; i++, j++) {
		buf        = _odp_packet_to_buffer(pkt_tbl[i]);
		hdr_tbl[j] = odp_buf_to_hdr(buf);
	}

	if (j)
		queue_enq_multi(qentry, hdr_tbl, j, 0);
	return nbr;
}

int pktin_poll(pktio_entry_t *entry, int num_queue, int index[])
{
	odp_packet_t pkt_tbl[QUEUE_MULTI_MAX];
	odp_buffer_hdr_t *hdr_tbl[QUEUE_MULTI_MAX];
	int num, i, idx;
	odp_buffer_t buf;

	if (odp_unlikely(is_free(entry))) {
		ODP_ERR("Bad pktio entry\n");
		return -1;
	}

	/* Temporarely needed for odp_pktio_inq_remdef() */
	if (odp_unlikely(entry->s.num_in_queue == 0))
		return -1;

	if (entry->s.state != STATE_STARTED)
		return 0;

	for (idx = 0; idx < num_queue; idx++) {
		queue_entry_t *qentry;
		odp_queue_t queue;
		odp_pktin_queue_t pktin = entry->s.in_queue[index[idx]].pktin;

		num = odp_pktin_recv(pktin, pkt_tbl, QUEUE_MULTI_MAX);

		if (num == 0)
			continue;

		if (num < 0) {
			ODP_ERR("Packet recv error\n");
			return -1;
		}

		for (i = 0; i < num; i++) {
			buf        = _odp_packet_to_buffer(pkt_tbl[i]);
			hdr_tbl[i] = odp_buf_to_hdr(buf);
		}

		queue = entry->s.in_queue[index[idx]].queue;
		qentry = queue_to_qentry(queue);
		queue_enq_multi(qentry, hdr_tbl, num, 0);
	}

	return 0;
}

uint32_t odp_pktio_mtu(odp_pktio_t id)
{
	pktio_entry_t *entry;
	uint32_t ret = 0;

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return 0;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return 0;
	}

	if (entry->s.ops->mtu_get)
		ret = entry->s.ops->mtu_get(entry);

	unlock_entry(entry);
	return ret;
}

int odp_pktio_promisc_mode_set(odp_pktio_t id, odp_bool_t enable)
{
	pktio_entry_t *entry;
	int ret = -1;

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}
	if (entry->s.state == STATE_STARTED) {
		unlock_entry(entry);
		return -1;
	}

	if (entry->s.ops->promisc_mode_set)
		ret = entry->s.ops->promisc_mode_set(entry, enable);

	unlock_entry(entry);
	return ret;
}

int odp_pktio_promisc_mode(odp_pktio_t id)
{
	pktio_entry_t *entry;
	int ret = -1;

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}

	if (entry->s.ops->promisc_mode_get)
		ret = entry->s.ops->promisc_mode_get(entry);
	unlock_entry(entry);

	return ret;
}

int odp_pktio_mac_addr(odp_pktio_t id, void *mac_addr, int addr_size)
{
	pktio_entry_t *entry;
	int ret = ETH_ALEN;

	if (addr_size < ETH_ALEN) {
		/* Output buffer too small */
		return -1;
	}

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}

	if (entry->s.ops->mac_get) {
		ret = entry->s.ops->mac_get(entry, mac_addr);
	} else {
		ODP_DBG("pktio does not support mac addr get\n");
		ret = -1;
	}
	unlock_entry(entry);

	return ret;
}

int odp_pktio_link_status(odp_pktio_t id)
{
	pktio_entry_t *entry;
	int ret = -1;

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}

	if (entry->s.ops->link_status)
		ret = entry->s.ops->link_status(entry);
	unlock_entry(entry);

	return ret;
}

void odp_pktio_param_init(odp_pktio_param_t *params)
{
	memset(params, 0, sizeof(odp_pktio_param_t));
	params->in_mode  = ODP_PKTIN_MODE_DIRECT;
	params->out_mode = ODP_PKTOUT_MODE_DIRECT;
}

void odp_pktin_queue_param_init(odp_pktin_queue_param_t *param)
{
	memset(param, 0, sizeof(odp_pktin_queue_param_t));
	param->op_mode = ODP_PKTIO_OP_MT;
	param->num_queues = 1;
	/* no need to choose queue type since pktin mode defines it */
	odp_queue_param_init(&param->queue_param);
}

void odp_pktout_queue_param_init(odp_pktout_queue_param_t *param)
{
	memset(param, 0, sizeof(odp_pktout_queue_param_t));
	param->op_mode = ODP_PKTIO_OP_MT;
	param->num_queues = 1;
}

int odp_pktio_info(odp_pktio_t id, odp_pktio_info_t *info)
{
	pktio_entry_t *entry;

	entry = get_pktio_entry(id);

	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return -1;
	}

	memset(info, 0, sizeof(odp_pktio_info_t));
	info->name = entry->s.name;
	info->pool = entry->s.pool;
	memcpy(&info->param, &entry->s.param, sizeof(odp_pktio_param_t));

	return 0;
}

void odp_pktio_print(odp_pktio_t id)
{
	pktio_entry_t *entry;
	uint8_t addr[ETH_ALEN];
	int max_len = 512;
	char str[max_len];
	int len = 0;
	int n = max_len - 1;

	entry = get_pktio_entry(id);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", id);
		return;
	}

	len += snprintf(&str[len], n - len,
			"pktio\n");
	len += snprintf(&str[len], n - len,
			"  handle       %" PRIu64 "\n", odp_pktio_to_u64(id));
	len += snprintf(&str[len], n - len,
			"  name         %s\n", entry->s.name);
	len += snprintf(&str[len], n - len,
			"  type         %s\n", entry->s.ops->name);
	len += snprintf(&str[len], n - len,
			"  state        %s\n",
			entry->s.state ==  STATE_STARTED ? "start" :
		       (entry->s.state ==  STATE_STOPPED ? "stop" :
		       (entry->s.state ==  STATE_OPENED ? "opened" :
							  "unknown")));
	memset(addr, 0, sizeof(addr));
	odp_pktio_mac_addr(id, addr, ETH_ALEN);
	len += snprintf(&str[len], n - len,
			"  mac          %02x:%02x:%02x:%02x:%02x:%02x\n",
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	len += snprintf(&str[len], n - len,
			"  mtu          %" PRIu32 "\n", odp_pktio_mtu(id));
	len += snprintf(&str[len], n - len,
			"  promisc      %s\n",
			odp_pktio_promisc_mode(id) ? "yes" : "no");
	str[len] = '\0';

	ODP_PRINT("\n%s\n", str);
}

int odp_pktio_term_global(void)
{
	int ret;
	int id;
	int pktio_if;

	for (id = 0; id < ODP_CONFIG_PKTIO_ENTRIES; ++id) {
		pktio_entry_t *pktio_entry;

		pktio_entry = &pktio_tbl->entries[id];

		if (is_free(pktio_entry))
			continue;

		lock_entry(pktio_entry);
		if (pktio_entry->s.state == STATE_STARTED) {
			ret = _pktio_stop(pktio_entry);
			if (ret)
				ODP_ABORT("unable to stop pktio %s\n",
					  pktio_entry->s.name);
		}
		ret = _pktio_close(pktio_entry);
		if (ret)
			ODP_ABORT("unable to close pktio %s\n",
				  pktio_entry->s.name);
		unlock_entry(pktio_entry);
	}

	for (pktio_if = 0; pktio_if_ops[pktio_if]; ++pktio_if) {
		if (pktio_if_ops[pktio_if]->term)
			if (pktio_if_ops[pktio_if]->term())
				ODP_ABORT("failed to terminate pktio type %d",
					  pktio_if);
	}

	ret = odp_shm_free(odp_shm_lookup("odp_pktio_entries"));
	if (ret != 0)
		ODP_ERR("shm free failed for odp_pktio_entries");

	return ret;
}

int odp_pktio_capability(odp_pktio_t pktio, odp_pktio_capability_t *capa)
{
	pktio_entry_t *entry;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	if (entry->s.ops->capability)
		return entry->s.ops->capability(entry, capa);

	return single_capability(capa);
}

int odp_pktio_stats(odp_pktio_t pktio,
		    odp_pktio_stats_t *stats)
{
	pktio_entry_t *entry;
	int ret = -1;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}

	if (entry->s.ops->stats)
		ret = entry->s.ops->stats(entry, stats);
	unlock_entry(entry);

	return ret;
}

int odp_pktio_stats_reset(odp_pktio_t pktio)
{
	pktio_entry_t *entry;
	int ret = -1;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	lock_entry(entry);

	if (odp_unlikely(is_free(entry))) {
		unlock_entry(entry);
		ODP_DBG("already freed pktio\n");
		return -1;
	}

	if (entry->s.ops->stats)
		ret = entry->s.ops->stats_reset(entry);
	unlock_entry(entry);

	return ret;
}

int odp_pktin_queue_config(odp_pktio_t pktio,
			   const odp_pktin_queue_param_t *param)
{
	pktio_entry_t *entry;
	odp_pktin_mode_t mode;
	odp_pktio_capability_t capa;
	unsigned num_queues;
	unsigned i;
	int rc;
	odp_queue_t queue;
	odp_pktin_queue_param_t default_param;

	if (param == NULL) {
		odp_pktin_queue_param_init(&default_param);
		param = &default_param;
	}

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	if (entry->s.state == STATE_STARTED) {
		ODP_DBG("pktio %s: not stopped\n", entry->s.name);
		return -1;
	}

	mode = entry->s.param.in_mode;

	/* Ignore the call when packet input is disabled. */
	if (mode == ODP_PKTIN_MODE_DISABLED)
		return 0;

	num_queues = param->num_queues;

	if (num_queues == 0) {
		ODP_DBG("pktio %s: zero input queues\n", entry->s.name);
		return -1;
	}

	rc = odp_pktio_capability(pktio, &capa);
	if (rc) {
		ODP_DBG("pktio %s: unable to read capabilities\n",
			entry->s.name);
		return -1;
	}

	if (num_queues > capa.max_input_queues) {
		ODP_DBG("pktio %s: too many input queues\n", entry->s.name);
		return -1;
	}

	/* If re-configuring, destroy old queues */
	if (entry->s.num_in_queue)
		destroy_in_queues(entry, entry->s.num_in_queue);

	for (i = 0; i < num_queues; i++) {
		if (mode == ODP_PKTIN_MODE_QUEUE ||
		    mode == ODP_PKTIN_MODE_SCHED) {
			odp_queue_param_t queue_param;
			char name[ODP_QUEUE_NAME_LEN];
			int pktio_id = pktio_to_id(pktio);

			snprintf(name, sizeof(name), "odp-pktin-%i-%i",
				 pktio_id, i);

			memcpy(&queue_param, &param->queue_param,
			       sizeof(odp_queue_param_t));

			queue_param.type = ODP_QUEUE_TYPE_PLAIN;

			if (mode == ODP_PKTIN_MODE_SCHED)
				queue_param.type = ODP_QUEUE_TYPE_SCHED;

			queue = odp_queue_create(name, &queue_param);

			if (queue == ODP_QUEUE_INVALID) {
				ODP_DBG("pktio %s: event queue create failed\n",
					entry->s.name);
				destroy_in_queues(entry, i + 1);
				return -1;
			}

			if (mode == ODP_PKTIN_MODE_QUEUE) {
				queue_entry_t *qentry;

				qentry = queue_to_qentry(queue);
				qentry->s.pktin.index  = i;
				qentry->s.pktin.pktio  = pktio;

				qentry->s.enqueue = pktin_enqueue;
				qentry->s.dequeue = pktin_dequeue;
				qentry->s.enqueue_multi = pktin_enq_multi;
				qentry->s.dequeue_multi = pktin_deq_multi;
			}

			entry->s.in_queue[i].queue = queue;
		} else {
			entry->s.in_queue[i].queue = ODP_QUEUE_INVALID;
		}

		entry->s.in_queue[i].pktin.index = i;
		entry->s.in_queue[i].pktin.pktio = entry->s.handle;
	}

	entry->s.num_in_queue = num_queues;

	if (entry->s.ops->input_queues_config)
		return entry->s.ops->input_queues_config(entry, param);

	return 0;
}

int odp_pktout_queue_config(odp_pktio_t pktio,
			    const odp_pktout_queue_param_t *param)
{
	pktio_entry_t *entry;
	odp_pktout_mode_t mode;
	odp_pktio_capability_t capa;
	unsigned num_queues;
	unsigned i;
	int rc;
	odp_pktout_queue_param_t default_param;

	if (param == NULL) {
		odp_pktout_queue_param_init(&default_param);
		param = &default_param;
	}

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	if (entry->s.state == STATE_STARTED) {
		ODP_DBG("pktio %s: not stopped\n", entry->s.name);
		return -1;
	}

	mode = entry->s.param.out_mode;

	/* Ignore the call when packet output is disabled, or routed through
	 * traffic manager. */
	if (mode == ODP_PKTOUT_MODE_DISABLED || mode == ODP_PKTOUT_MODE_TM)
		return 0;

	if (mode != ODP_PKTOUT_MODE_DIRECT && mode != ODP_PKTOUT_MODE_QUEUE) {
		ODP_DBG("pktio %s: bad packet output mode\n", entry->s.name);
		return -1;
	}

	num_queues = param->num_queues;

	if (num_queues == 0) {
		ODP_DBG("pktio %s: zero output queues\n", entry->s.name);
		return -1;
	}

	rc = odp_pktio_capability(pktio, &capa);
	if (rc) {
		ODP_DBG("pktio %s: unable to read capabilities\n",
			entry->s.name);
		return -1;
	}

	if (num_queues > capa.max_output_queues) {
		ODP_DBG("pktio %s: too many output queues\n", entry->s.name);
		return -1;
	}

	/* If re-configuring, destroy old queues */
	if (entry->s.num_out_queue) {
		destroy_out_queues(entry, entry->s.num_out_queue);
		entry->s.num_out_queue = 0;
	}

	init_out_queues(entry);

	for (i = 0; i < num_queues; i++) {
		entry->s.out_queue[i].pktout.index = i;
		entry->s.out_queue[i].pktout.pktio = pktio;
	}

	entry->s.num_out_queue = num_queues;

	if (mode == ODP_PKTOUT_MODE_QUEUE) {
		for (i = 0; i < num_queues; i++) {
			odp_queue_t queue;
			odp_queue_param_t queue_param;
			queue_entry_t *qentry;
			char name[ODP_QUEUE_NAME_LEN];
			int pktio_id = pktio_to_id(pktio);

			snprintf(name, sizeof(name), "odp-pktout-%i-%i",
				 pktio_id, i);

			odp_queue_param_init(&queue_param);
			/* Application cannot dequeue from the queue */
			queue_param.deq_mode = ODP_QUEUE_OP_DISABLED;

			queue = odp_queue_create(name, &queue_param);

			if (queue == ODP_QUEUE_INVALID) {
				ODP_DBG("pktout %s: event queue create failed\n",
					entry->s.name);
				destroy_out_queues(entry, i + 1);
				return -1;
			}

			qentry = queue_to_qentry(queue);
			qentry->s.pktout.index  = i;
			qentry->s.pktout.pktio  = pktio;

			/* Override default enqueue / dequeue functions */
			qentry->s.enqueue       = queue_pktout_enq;
			qentry->s.dequeue       = pktout_dequeue;
			qentry->s.enqueue_multi = queue_pktout_enq_multi;
			qentry->s.dequeue_multi = pktout_deq_multi;

			entry->s.out_queue[i].queue = queue;
		}
	}

	if (entry->s.ops->output_queues_config)
		return entry->s.ops->output_queues_config(entry, param);

	return 0;
}

int odp_pktin_event_queue(odp_pktio_t pktio, odp_queue_t queues[], int num)
{
	pktio_entry_t *entry;
	odp_pktin_mode_t mode;
	int i;
	int num_queues;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	mode = entry->s.param.in_mode;

	if (mode == ODP_PKTIN_MODE_DISABLED)
		return 0;

	if (mode != ODP_PKTIN_MODE_QUEUE &&
	    mode != ODP_PKTIN_MODE_SCHED)
		return -1;

	num_queues = entry->s.num_in_queue;

	if (queues && num > 0) {
		for (i = 0; i < num && i < num_queues; i++)
			queues[i] = entry->s.in_queue[i].queue;
	}

	return num_queues;
}

int odp_pktin_queue(odp_pktio_t pktio, odp_pktin_queue_t queues[], int num)
{
	pktio_entry_t *entry;
	odp_pktin_mode_t mode;
	int i;
	int num_queues;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	mode = entry->s.param.in_mode;

	if (mode == ODP_PKTIN_MODE_DISABLED)
		return 0;

	if (mode != ODP_PKTIN_MODE_DIRECT)
		return -1;

	num_queues = entry->s.num_in_queue;

	if (queues && num > 0) {
		for (i = 0; i < num && i < num_queues; i++)
			queues[i] = entry->s.in_queue[i].pktin;
	}

	return num_queues;
}

int odp_pktout_event_queue(odp_pktio_t pktio, odp_queue_t queues[], int num)
{
	pktio_entry_t *entry;
	odp_pktout_mode_t mode;
	int i;
	int num_queues;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	mode = entry->s.param.out_mode;

	if (mode == ODP_PKTOUT_MODE_DISABLED)
		return 0;

	if (mode != ODP_PKTOUT_MODE_QUEUE)
		return -1;

	num_queues = entry->s.num_out_queue;

	if (queues && num > 0) {
		for (i = 0; i < num && i < num_queues; i++)
			queues[i] = entry->s.out_queue[i].queue;
	}

	return num_queues;
}

int odp_pktout_queue(odp_pktio_t pktio, odp_pktout_queue_t queues[], int num)
{
	pktio_entry_t *entry;
	odp_pktout_mode_t mode;
	int i;
	int num_queues;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	mode = entry->s.param.out_mode;

	if (mode == ODP_PKTOUT_MODE_DISABLED)
		return 0;

	if (mode != ODP_PKTOUT_MODE_DIRECT)
		return -1;

	num_queues = entry->s.num_out_queue;

	if (queues && num > 0) {
		for (i = 0; i < num && i < num_queues; i++)
			queues[i] = entry->s.out_queue[i].pktout;
	}

	return num_queues;
}

int odp_pktin_recv(odp_pktin_queue_t queue, odp_packet_t packets[], int num)
{
	pktio_entry_t *entry;
	odp_pktio_t pktio = queue.pktio;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	if (entry->s.ops->recv_queue)
		return entry->s.ops->recv_queue(entry, queue.index,
						packets, num);

	return single_recv_queue(entry, queue.index, packets, num);
}

int odp_pktout_send(odp_pktout_queue_t queue, odp_packet_t packets[], int num)
{
	pktio_entry_t *entry;
	odp_pktio_t pktio = queue.pktio;

	entry = get_pktio_entry(pktio);
	if (entry == NULL) {
		ODP_DBG("pktio entry %d does not exist\n", pktio);
		return -1;
	}

	if (entry->s.ops->send_queue)
		return entry->s.ops->send_queue(entry, queue.index,
						packets, num);

	return single_send_queue(entry, queue.index, packets, num);
}

int single_capability(odp_pktio_capability_t *capa)
{
	memset(capa, 0, sizeof(odp_pktio_capability_t));
	capa->max_input_queues  = 1;
	capa->max_output_queues = 1;
	capa->set_op.op.promisc_mode = 1;

	return 0;
}

int single_recv_queue(pktio_entry_t *entry, int index, odp_packet_t packets[],
		      int num)
{
	(void)index;
	return _odp_pktio_recv(entry->s.handle, packets, num);
}

int single_send_queue(pktio_entry_t *entry, int index, odp_packet_t packets[],
		      int num)
{
	(void)index;
	return _odp_pktio_send(entry->s.handle, packets, num);
}
