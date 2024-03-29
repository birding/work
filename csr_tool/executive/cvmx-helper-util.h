/***********************license start***************
 * Copyright (c) 2003-2014  Cavium Inc. (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Inc. nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM INC. MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * Small helper utilities.
 *
 * <hr>$Revision: 153454 $<hr>
 */

#ifndef __CVMX_HELPER_UTIL_H__
#define __CVMX_HELPER_UTIL_H__

#include "cvmx.h"
#include "cvmx-mio-defs.h"
#include "cvmx-helper.h"
#include "cvmx-fpa.h"

typedef char cvmx_pknd_t;
typedef char cvmx_bpid_t;

#define CVMX_INVALID_PKND	((cvmx_pknd_t) -1)
#define CVMX_INVALID_BPID	((cvmx_bpid_t) -1)
#define CVMX_MAX_PKND		((cvmx_pknd_t) 64)
#define CVMX_MAX_BPID		((cvmx_bpid_t) 64)

#define CVMX_HELPER_MAX_IFACE		11
#define CVMX_HELPER_MAX_PORTS		16


/* Maximum range for normalized (a.k.a. IPD) port numbers (12-bit field) */
#define	CVMX_PKO3_IPD_NUM_MAX	0x1000	//FIXME- take it from someplace else ?

#define CVMX_PKO3_IPD_PORT_NULL (CVMX_PKO3_IPD_NUM_MAX-1)
#define CVMX_PKO3_IPD_PORT_LOOP 0

struct cvmx_xport {
	int node;
	int port;
};
typedef struct cvmx_xport cvmx_xport_t;

static inline struct cvmx_xport cvmx_helper_ipd_port_to_xport(int ipd_port)
{
	struct cvmx_xport r;
	r.port = ipd_port & (CVMX_PKO3_IPD_NUM_MAX - 1);
	r.node = (ipd_port >> 12) & CVMX_NODE_MASK;
	return r;
}

static inline int cvmx_helper_node_to_ipd_port(int node, int index)
{
	return (node << 12) + index;
}

struct cvmx_xiface {
	int node;
	int interface;
};
typedef struct cvmx_xiface cvmx_xiface_t;

/**
 * Return node and interface number from XIFACE.
 *
 * @param xiface interface with node information
 *
 * @return struct that contains node and interface number.
 */
static inline struct cvmx_xiface cvmx_helper_xiface_to_node_interface(int xiface)
{
	cvmx_xiface_t interface_node;
	/*
	 * If the majic number 0xde0000 is not present in the
	 * interface, then assume it is node 0.
	 */

	if (((xiface >> 0x8) & 0xff) == 0xde) {
		interface_node.node = (xiface >> 16) & CVMX_NODE_MASK;
		interface_node.interface = xiface & 0xff;
	} else {
		interface_node.node = cvmx_get_node_num();
		interface_node.interface = xiface & 0xff;
	}
	return interface_node;
}

/* Used internally only*/
static inline bool __cvmx_helper_xiface_is_null(int xiface)
{
	return (xiface & 0xff) == 0xff;
}

#define __CVMX_XIFACE_NULL 0xff

/**
 * Return interface with majic number and node information (XIFACE)
 *
 * @param node       node of the interface referred to
 * @param interface  interface to use.
 *
 * @return
 */
static inline int cvmx_helper_node_interface_to_xiface(int node, int interface)
{
	return ((node & CVMX_NODE_MASK) << 16) | (0xde << 8) | (interface & 0xff);
}

/**
 * Free the pip packet buffers contained in a work queue entry.
 * The work queue entry is not freed.
 *
 * @param work   Work queue entry with packet to free
 */
static inline void cvmx_helper_free_pip_pkt_data(cvmx_wqe_t *work)
{
	uint64_t        number_buffers;
	cvmx_buf_ptr_t  buffer_ptr;
	cvmx_buf_ptr_t  next_buffer_ptr;
	uint64_t        start_of_buffer;

	number_buffers = work->word2.s.bufs;
	if (number_buffers == 0)
		return;
	buffer_ptr = work->packet_ptr;

    /* Since the number of buffers is not zero, we know this is not a dynamic
	short packet. We need to check if it is a packet received with
	IPD_CTL_STATUS[NO_WPTR]. If this is true, we need to free all buffers
	except for the first one. The caller doesn't expect their WQE pointer
	to be freed */
	start_of_buffer = ((buffer_ptr.s.addr >> 7) - buffer_ptr.s.back) << 7;
	if (cvmx_ptr_to_phys(work) == start_of_buffer)
	{
		next_buffer_ptr = *(cvmx_buf_ptr_t*)cvmx_phys_to_ptr(buffer_ptr.s.addr - 8);
		buffer_ptr = next_buffer_ptr;
		number_buffers--;
	}

	while (number_buffers--)
	{
		/* Remember the back pointer is in cache lines, not 64bit words */
		start_of_buffer = ((buffer_ptr.s.addr >> 7) - buffer_ptr.s.back) << 7;
		/* Read pointer to next buffer before we free the current buffer. */
		next_buffer_ptr = *(cvmx_buf_ptr_t*)cvmx_phys_to_ptr(buffer_ptr.s.addr - 8);
		cvmx_fpa_free(cvmx_phys_to_ptr(start_of_buffer), buffer_ptr.s.pool, 0);
		buffer_ptr = next_buffer_ptr;
	}
}

/**
 * Free the pki packet buffers contained in a work queue entry.
 * If first packet buffer contains wqe, wqe gets freed too so do not access
 * wqe after calling this function.
 * This function asssumes that buffers to be freed are from
 * Naturally aligned pool/aura.
 * It does not use don't write back.
 * @param work   Work queue entry with packet to free
 */
static inline void cvmx_helper_free_pki_pkt_data(cvmx_wqe_t *work)
{
	uint64_t        number_buffers;
	uint64_t        start_of_buffer;
	cvmx_buf_ptr_pki_t  next_buffer_ptr;
	cvmx_buf_ptr_pki_t  buffer_ptr;
	cvmx_wqe_78xx_t *wqe = (cvmx_wqe_78xx_t*)work;

	if (!octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return;
	}
	/* Make sure errata pki-20776 has been applied*/
	cvmx_wqe_pki_errata_20776(work);
	buffer_ptr = wqe->packet_ptr;
	number_buffers = cvmx_wqe_get_bufs(work);

	while (number_buffers--) {
		/* FIXME: change WQE function prototype */
		unsigned x = cvmx_wqe_get_aura(work);
		cvmx_fpa3_gaura_t aura = __cvmx_fpa3_gaura(x >> 10, x & 0x3ff);
		/* XXX- assumes the buffer is cache-line aligned and naturally aligned mode*/
		start_of_buffer = (buffer_ptr.addr >> 7) << 7;
		/* Read pointer to next buffer before we free the current buffer. */
		next_buffer_ptr = *(cvmx_buf_ptr_pki_t *)
			cvmx_phys_to_ptr(buffer_ptr.addr - 8);
		/* FPA AURA comes from WQE, includes node */
		cvmx_fpa3_free(cvmx_phys_to_ptr(start_of_buffer), aura, 0);
		buffer_ptr = next_buffer_ptr;
	}
}

/**
 * Free the pki wqe entry buffer.
 * If wqe buffers contains first packet buffer, wqe does not get freed here.
 * This function asssumes that buffers to be freed are from
 * Naturally aligned pool/aura.
 * It does not use don't write back.
 * @param work   Work queue entry to free
 */
static inline void cvmx_wqe_pki_free(cvmx_wqe_t *work)
{
	cvmx_wqe_78xx_t *wqe = (cvmx_wqe_78xx_t *)work;
	unsigned x;
	cvmx_fpa3_gaura_t aura;

	if (!octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE)) {
		return;
	}
	/* Do nothing if the first packet buffer shares WQE buffer */
	if (!wqe->packet_ptr.packet_outside_wqe)
		return;

	/* FIXME change WQE function prototype */
	x = cvmx_wqe_get_aura(work);
	aura = __cvmx_fpa3_gaura(x >> 10, x & 0x3ff);

	cvmx_fpa3_free(work, aura, 0);
}

/**
 * Convert a interface mode into a human readable string
 *
 * @param mode   Mode to convert
 *
 * @return String
 */
extern const char *cvmx_helper_interface_mode_to_string(cvmx_helper_interface_mode_t mode);

/**
 * Debug routine to dump the packet structure to the console
 *
 * @param work   Work queue entry containing the packet to dump
 * @return
 */
extern int cvmx_helper_dump_packet(cvmx_wqe_t *work);

/**
 * Get the version of the CVMX libraries.
 *
 * @return Version string. Note this buffer is allocated statically
 *         and will be shared by all callers.
 */
extern const char *cvmx_helper_get_version(void);

/**
 * @INTERNAL
 * Setup the common GMX settings that determine the number of
 * ports. These setting apply to almost all configurations of all
 * chips.
 *
 * @param xiface Interface to configure
 * @param num_ports Number of ports on the interface
 *
 * @return Zero on success, negative on failure
 */
extern int __cvmx_helper_setup_gmx(int xiface, int num_ports);

/**
 * @INTERNAL
 * Get the number of pko_ports on an interface.
 *
 * @param interface
 *
 * @return the number of pko_ports on the interface.
 */
extern int __cvmx_helper_get_num_pko_ports(int interface);


/**
 * Returns the IPD port number for a port on the given
 * interface.
 *
 * @param interface Interface to use
 * @param port      Port on the interface
 *
 * @return IPD port number
 */
extern int cvmx_helper_get_ipd_port(int interface, int port);

/**
 * Returns the PKO port number for a port on the given interface,
 * This is the base pko_port for o68 and ipd_port for older models.
 *
 * @param interface Interface to use
 * @param port      Port on the interface
 *
 * @return PKO port number and -1 on error.
 */
extern int cvmx_helper_get_pko_port(int interface, int port);

/**
 * Returns the IPD/PKO port number for the first port on the given
 * interface.
 *
 * @param interface Interface to use
 *
 * @return IPD/PKO port number
 */
static inline int cvmx_helper_get_first_ipd_port(int interface)
{
	return cvmx_helper_get_ipd_port(interface, 0);
}

/**
 * Returns the IPD/PKO port number for the last port on the given
 * interface.
 *
 * @param interface Interface to use
 *
 * @return IPD/PKO port number
 *
 * Note: for o68, the last ipd port on an interface does not always equal to
 * the first plus the number of ports as the ipd ports are not contiguous in
 * some cases, e.g., SGMII.
 *
 * Note: code that makes the assumption of contiguous ipd port numbers needs to
 * be aware of this.
 */
static inline int cvmx_helper_get_last_ipd_port(int interface)
{
	extern int cvmx_helper_ports_on_interface(int interface);

	return cvmx_helper_get_ipd_port(interface,
	    cvmx_helper_ports_on_interface(interface) - 1);
}

/**
 * Free the packet buffers contained in a work queue entry.
 * The work queue entry is not freed.
 * Note that this function will not free the work queue entry
 * even if it contains a non-redundant data packet, and hence
 * it is not really comparable to how the PKO would free a packet
 * buffers if requested.
 *
 * @param work   Work queue entry with packet to free
 */
void cvmx_helper_free_packet_data(cvmx_wqe_t *work);

/**
 * Returns the interface number for an IPD/PKO port number.
 *
 * @param ipd_port IPD/PKO port number
 *
 * @return Interface number
 */
extern int cvmx_helper_get_interface_num(int ipd_port);

/**
 * Returns the interface index number for an IPD/PKO port
 * number.
 *
 * @param ipd_port IPD/PKO port number
 *
 * @return Interface index number
 */
extern int cvmx_helper_get_interface_index_num(int ipd_port);

/**
 * Get port kind for a given port in an interface.
 *
 * @param xiface  Interface
 * @param index   index of the port in the interface
 *
 * @return port kind on sucicess  and -1 on failure
 */
extern int cvmx_helper_get_pknd(int xiface, int index);

/**
 * Get bpid for a given port in an interface.
 *
 * @param interface  Interface
 * @param port       index of the port in the interface
 *
 * @return port kind on sucicess  and -1 on failure
 */
extern int cvmx_helper_get_bpid(int interface, int port);

/**
 * Internal functions.
 */
extern int __cvmx_helper_post_init_interfaces(void);
extern int cvmx_helper_setup_red(int pass_thresh, int drop_thresh);
extern void cvmx_helper_show_stats(int port);

/*
 * Return number of array alements
 */
#define	NUM_ELEMENTS(arr) (sizeof(arr)/sizeof((arr)[0]))

#ifndef CVMX_BUILD_FOR_LINUX_KERNEL
/**
 * Prints out a buffer with the address, hex bytes, and ASCII
 *
 * @param	addr	Start address to print on the left
 * @param[in]	buffer	array of bytes to print
 * @param	count	Number of bytes to print
 */
void cvmx_print_buffer_u8(unsigned addr, const uint8_t *buffer, size_t count);
#endif
#endif /* __CVMX_HELPER_H__ */
