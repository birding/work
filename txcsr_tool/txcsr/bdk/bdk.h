#ifndef __BDK_H__
#define __BDK_H__

/**
 * @file
 *
 * Master include file for all BDK function.
 *
 * <hr>$Revision: 49448 $<hr>
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <bdk-numa.h>
#include <bdk-model.h>
#include <bdk-csr.h>
#include <helper.h>

#define bdk_error printf

uint64_t bdk_csr_read_by_name(const char *name,int inf_id[]);
uint64_t bdk_numa_get_address(bdk_node_t node, uint64_t pa);
uint64_t bdk_sysreg_read(const char *name, int inf_id[], int core);

#endif
