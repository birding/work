/***********************license start***************
 * Copyright (c) 2014  Cavium Inc. (support@cavium.com). All rights
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
 * Interface to the Octeon 78xx extended error status.
 *
 * <hr>$Revision$<hr>
 */
#ifdef CVMX_BUILD_FOR_LINUX_KERNEL
#include <asm/octeon/cvmx.h>
#include <asm/octeon/cvmx-error.h>
#include <asm/octeon/cvmx-error-custom.h>
#include <asm/octeon/cvmx-pcie.h>
#include <asm/octeon/cvmx-srio.h>
#include <asm/octeon/cvmx-ciu2-defs.h>
#include <asm/octeon/cvmx-dfm-defs.h>
#include <asm/octeon/cvmx-lmcx-defs.h>
#include <asm/octeon/cvmx-pexp-defs.h>
#include <asm/octeon/cvmx-helper-cfg.h>
#else
#include "cvmx.h"
#include "cvmx-error.h"
#include "cvmx-error-arrays-cn78xx.h"
#include "cvmx-ciu3-defs.h"
#include "cvmx-interrupt.h"
#include "cvmx-helper-cfg.h"
#endif

static CVMX_SHARED struct cvmx_error_78xx *octeon_error_array_current = NULL;
static CVMX_SHARED int octeon_error_array_current_size = 0;

/**
 * @INTERNAL
 *
 * @param intsn interrupt source number to display
 *
 * @return Zero on success, -1 on error
 */
int cvmx_error_intsn_display_v3(int node, uint32_t intsn) {
	struct cvmx_error_78xx *entry;
	int offs, idx, prev_low, prev_high;
	char msg[128];

	prev_low = 0;
	prev_high = octeon_error_array_current_size - 1;

	idx = prev_high / 2;

	/* Try to do a binary search */
	while (prev_low < prev_high && octeon_error_array_current[idx].intsn != intsn) {
		if (octeon_error_array_current[idx].intsn < intsn) {
			prev_low = idx + 1;
			idx += (prev_high - idx) / 2;
			if (idx < prev_low)
				idx = prev_low;
		} else {
			prev_high = idx - 1;
			idx -= (idx - prev_low) / 2;
			if (idx > prev_high)
				idx = prev_high;
		}
	}
	if (intsn != octeon_error_array_current[idx].intsn) {
		fprintf(stderr, "ERROR: Unknown intsn 0x%x\n", (unsigned int)intsn);
		cvmx_error_intsn_disable_v3(node, intsn);
		return -1;
	}
	entry  = &octeon_error_array_current[idx];
	offs = sprintf(msg, "%d:", node);
	snprintf(msg + offs, sizeof(msg), entry->err_mesg,
		entry->block_csr | ((uint64_t)node << CVMX_NODE_IO_SHIFT), entry->block_csr_bitpos);
	cvmx_safe_printf("%s\n", msg);
	if (entry->block_csr) {
		uint64_t clear_addr;
		clear_addr = 0x8000000000000000ull | entry->block_csr;
		cvmx_write_csr_node(node, clear_addr, 1ull << entry->block_csr_bitpos);
	}
	return 0;
}

/**
 * Initalize the error status system for cn78xx. This should be called once
 * before any other functions are called. This function enables the interrupts
 * described in the array.
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_initialize_cn78xx(int node) {
	int i;
	int arrsize;
	int procid = cvmx_get_proc_id();
	struct cvmx_error_array *array = octeon_error_arrays;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;
	for (i = 0; array->array != NULL; i++) {
		if ((procid & array->prid_mask) == array->prid_val) {
			arrsize = cvmx_error_78xx_array_sizes[i];
			break;
		}
		array++;
	}

	if (!array->array) {
		fprintf(stderr, "ERROR: No array found\n");
		return -1;
	}

	octeon_error_array_current = array->array;
	octeon_error_array_current_size = arrsize;

	/* Enable all interrupts described in the array */
	for (i = 0; octeon_error_array_current[i].block_csr != 0; i++)
		if (octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ETHERNET
		    && octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ILA
		    && octeon_error_array_current[i].error_group != CVMX_ERROR_GROUP_ILK)
			cvmx_error_intsn_enable_v3(node, octeon_error_array_current[i].intsn);

	return 0;
}

/**
 * Enable interrupt for a specific INTSN.
 *
 * @param intsn Interrupt source number
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_intsn_enable_v3(int node, uint32_t intsn) {
	uint64_t addr;
	uint8_t idt = cvmx_get_local_core_num() * 4 + 3;
	cvmx_ciu3_iscx_ctl_t isc_ctl;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	addr = CVMX_CIU3_ISCX_CTL(intsn);
	isc_ctl.u64 = cvmx_read_csr_node(node, addr);
	if (!isc_ctl.s.imp) {
		return -1;
	}
	//if (isc_ctl.s.en)
		/* First disable the entry, in order to be able to write IDT */
	cvmx_error_intsn_disable_v3(node, intsn);
	isc_ctl.u64 = 0;
	isc_ctl.s.idt = idt;
	isc_ctl.s.en = 1;
	cvmx_write_csr_node(node, addr, isc_ctl.u64);
	/* Read back to ensure it was written */
	cvmx_read_csr_node(node, addr);
	
	return 0;
}

/**
 * Disable interrupt for a specific INTSN.
 *
 * @param intsn Interrupt source number
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_intsn_disable_v3(int node, uint32_t intsn) {
	cvmx_ciu3_iscx_w1c_t w1c;
	uint64_t isc_w1c_addr = CVMX_CIU3_ISCX_W1C(intsn);
	
	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	w1c.u64 = 0;
	w1c.s.en = 1;
	cvmx_write_csr_node(node, isc_w1c_addr, w1c.u64);
	cvmx_read_csr_node(node, isc_w1c_addr);
	return 0;
}

/**
 * Enable interrupts for a specific CSR(all the bits/intsn in the csr).
 *
 * @param csr_address CSR address
 *
 * @return Zero on success, negative on failure.
 */
int cvmx_error_csr_enable_v3(int node, uint64_t csr_address)
{
	int i, ret, flag = 0;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (csr_address != octeon_error_array_current[i].block_csr) {
			if (flag) /* If we pass that CSR address - break */
				break;
			continue;
		}

		flag = 1;
		ret = cvmx_error_intsn_enable_v3(node, octeon_error_array_current[i].intsn);
		if (0 != ret)
			return ret;
	}

	return 0;
}

/**
 * Disable interrupts for a specific CSR (all the bits/intsn in the csr).
 *
 * @param csr_address CSR address
 *
 * @return Zero
 */
int cvmx_error_csr_disable_v3(int node, uint64_t csr_address)
{
	int i, flag = 0;

	if (cvmx_sysinfo_get()->board_type == CVMX_BOARD_TYPE_SIM)
		return 0;
	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (csr_address != octeon_error_array_current[i].block_csr) {
			if (flag) /* If we pass that CSR address - break */
				break;
			continue;
		}

		flag = 1;
		cvmx_error_intsn_disable_v3(node, octeon_error_array_current[i].intsn);
	}

	return 0;
}

/**
 * Enable all error registers for a logical group. This should be
 * called whenever a logical group is brought online.
 *
 * @param group  Logical group to enable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_enable_group_v3(cvmx_error_group_t group, int xipd_port)
{
	int i;
	int xiface = cvmx_helper_get_interface_num(xipd_port);
	int index = cvmx_helper_get_interface_index_num(xipd_port);
	struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);
	int interface = xi.interface;
	unsigned long long int match_ifindex = interface*0x1000000 + index*0x100000;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].error_group == group) {
			if (octeon_error_array_current[i].error_group == CVMX_ERROR_GROUP_ETHERNET) {
				/* Skip the loopback interface */
				if (octeon_error_array_current[i].block_csr == (CVMX_LBK_INT & ~(1ull << 63)))
					continue;
				if ((octeon_error_array_current[i].block_csr & 0xff00000ull) != match_ifindex)
					continue;
			}
			cvmx_error_intsn_enable_v3(xi.node, octeon_error_array_current[i].intsn);
		}
	}

	return 0;
}

/**
 * Disable all error registers for a logical group.
 *
 * @param group  Logical group to disable
 * @param xipd_port  The IPD port value
 *
 * @return Zero.
 */
int cvmx_error_disable_group_v3(cvmx_error_group_t group, int xipd_port)
{
	int i;
	int xiface = cvmx_helper_get_interface_num(xipd_port);
	int index = cvmx_helper_get_interface_index_num(xipd_port);
	struct cvmx_xiface xi = cvmx_helper_xiface_to_node_interface(xiface);
	int interface = xi.interface;
	unsigned long long int match_ifindex = interface*0x1000000 + index*0x100000;

	if (!octeon_has_feature(OCTEON_FEATURE_CIU3))
		return 0;

	for (i = 0; i < octeon_error_array_current_size; i++) {
		if (octeon_error_array_current[i].error_group == group) {
			if (octeon_error_array_current[i].error_group == CVMX_ERROR_GROUP_ETHERNET) {
				/* Skip the loopback interface */
				if (octeon_error_array_current[i].block_csr == (CVMX_LBK_INT & ~(1ull << 63)))
					continue;
				if ((octeon_error_array_current[i].block_csr & 0xff00000ull) != match_ifindex)
					continue;
			}
			cvmx_error_intsn_disable_v3(xi.node, octeon_error_array_current[i].intsn);
		}
	}

	return 0;
}
