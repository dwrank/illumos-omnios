/*
 * Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
 */

/*
 * This file contains code imported from the OFED rds source file ib_stats.c
 * Oracle elects to have and use the contents of ib_stats.c under and governed
 * by the OpenIB.org BSD license (see below for full license text). However,
 * the following notice accompanied the original version of this file:
 */

/*
 * Copyright (c) 2006 Oracle.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <sys/rds.h>

#include <sys/ib/clients/rdsv3/rdsv3.h>
#include <sys/ib/clients/rdsv3/ib.h>
#include <sys/ib/clients/rdsv3/rdsv3_debug.h>

struct rdsv3_ib_statistics	*rdsv3_ib_stats = NULL;

static char *rdsv3_ib_stat_names[] = {
	"ib_connect_raced",
	"ib_listen_closed_stale",
	"ib_evt_handler_call",
	"ib_tasklet_call",
	"ib_tx_cq_event",
	"ib_tx_ring_full",
	"ib_tx_throttle",
	"ib_tx_sg_mapping_failure",
	"ib_tx_stalled",
	"ib_tx_credit_updates",
	"ib_rx_cq_event",
	"ib_rx_ring_empty",
	"ib_rx_refill_from_cq",
	"ib_rx_refill_from_thread",
	"ib_rx_alloc_limit",
	"ib_rx_credit_updates",
	"ib_ack_sent",
	"ib_ack_send_failure",
	"ib_ack_send_delayed",
	"ib_ack_send_piggybacked",
	"ib_ack_received",
	"ib_rdma_mr_alloc",
	"ib_rdma_mr_free",
	"ib_rdma_mr_used",
	"ib_rdma_mr_pool_flush",
	"ib_rdma_mr_pool_wait",
	"ib_rdma_mr_pool_depleted",
};

unsigned int
rdsv3_ib_stats_info_copy(struct rdsv3_info_iterator *iter,
    unsigned int avail)
{
	struct rdsv3_ib_statistics stats = {0, };
	uint64_t *src;
	uint64_t *sum;
	size_t i;
	int cpu;

	RDSV3_DPRINTF4("rdsv3_ib_stats_info_copy", "iter: %p, avail: %d",
	    iter, avail);

	if (avail < ARRAY_SIZE(rdsv3_ib_stat_names))
		goto out;

	for (cpu = 0; cpu < nr_cpus; cpu++) {
		src = (uint64_t *)&(rdsv3_per_cpu(rdsv3_ib_stats, cpu));
		sum = (uint64_t *)&stats;
		for (i = 0; i < sizeof (stats) / sizeof (uint64_t); i++)
			*(sum++) += *(src++);
	}

	rdsv3_stats_info_copy(iter, (uint64_t *)&stats, rdsv3_ib_stat_names,
	    ARRAY_SIZE(rdsv3_ib_stat_names));

	RDSV3_DPRINTF4("rdsv3_ib_stats_info_copy",
	    "Return: iter: %p, avail: %d", iter, avail);
out:
	return (ARRAY_SIZE(rdsv3_ib_stat_names));
}
