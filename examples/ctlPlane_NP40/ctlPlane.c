/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Inc. (support@cavium.com). All rights 
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


/*
 * File version info: $Id: hello.c 104184 2014-09-10 16:54:34Z cchavva $
 */
 
#include <stdio.h>
#include <string.h>

#include "cvmx.h"
#include "cvmx-spinlock.h"

#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"
#include "cvmx-helper-util.h"
#include "cvmx-bootmem.h"

#include "ctlPlane.h"

int main(void)
{
	cvmx_sysinfo_t *sysinfo;
	struct cvmx_coremask coremask;
	unsigned core = cvmx_get_local_core_num();
	
    printf("Hello world on core %d!\n", core);

	cvmx_user_app_init();
	sysinfo = cvmx_sysinfo_get();
	cvmx_coremask_copy(&coremask, &sysinfo->core_mask);

#if 0
	 cvmx_coremask_barrier_sync(&coremask);
	if (cvmx_is_init_core()) {
		
		printf("Version: %s\n", cvmx_helper_get_version());

		printf("\n");
		printf("------------------\n");
		cvmx_bootmem_print_named();
		printf("------------------\n");
		cvmx_bootmem_phy_list_print();
		printf("------------------\n");
		cvmx_bootmem_phy_named_block_print();
		printf("\n");

	}
#endif

#if 0
	cvmx_coremask_barrier_sync(&coremask);
	if (cvmx_is_init_core()) {
		char *alloc_name = "session_counter\0\0";
		const cvmx_bootmem_named_block_desc_t *block_desc;
		unsigned long * pCouBase=NULL;
		int i=0,j=0;

		printf("\n" );
		block_desc = cvmx_bootmem_find_named_block(alloc_name);
		if (block_desc != NULL) {
			pCouBase = (unsigned long *)((1ull << 63) |block_desc->base_addr);
		}
		np_printf("[%s][%d][%d]pCouBase %p \n", __func__, __LINE__, core, pCouBase);
		if(pCouBase != NULL)
		{
			for(i=0; i< TAG_MAXNUM; i++){
				unsigned long counter;
				counter=0;
				for(j=0; j<HASH_LEN; j++){
					counter += pCouBase[i+j*TAG_MAXNUM];
				}
				if(counter)
					printf("hash counter 0x%x: %ld\n", i, counter);
			}
		}
		
	}
#endif
	cvmx_coremask_barrier_sync(&coremask);
	if (cvmx_is_init_core()) {
		char *failure_name = "hash_failure\0\0";
		const cvmx_bootmem_named_block_desc_t *block_desc;
		unsigned long * pFailureBase=NULL;
		int i=0;

		printf("\n" );
		block_desc = cvmx_bootmem_find_named_block(failure_name);
		if (block_desc != NULL) {
			pFailureBase = (unsigned long *)((1ull << 63) |block_desc->base_addr);
		}
		np_printf("[%s][%d][%d]pCouBase %p \n", __func__, __LINE__, core, pFailureBase);
		if(pFailureBase != NULL)
		{
			unsigned long counter;
			counter=0;
			for(i=0; i< 48; i++){				
				counter += pFailureBase[i];
			}
			if(counter)
				printf("hash failure counter %ld\n", counter);
		}
		
	}
	cvmx_coremask_barrier_sync(&coremask);

	if (cvmx_is_init_core()) {
		const cvmx_bootmem_named_block_desc_t *block_desc;
		stable_t * pTableBase=NULL;
		int i=0;
		int active_count, aged_count;

		char *stable_name = "session_table\0\0";
		printf("\n" );
		block_desc = cvmx_bootmem_find_named_block(stable_name);
		if (block_desc == NULL) {
			printf("%s is not alloced\n", stable_name);
			return -1;
		}
		pTableBase = (stable_t *)((1ull << 63) |block_desc->base_addr);
		if(pTableBase == NULL){
			printf("%s memory has errors\n", stable_name);
		}
		np_printf("[%s][%d][%d]pTableBase %p \n", __func__, __LINE__, core, pTableBase);

		active_count=0;
		aged_count=0;
		for(i=0; i<TAG_MAXNUM*HASH_LEN; i++){
			stable_t * pStbl = (stable_t *)pTableBase +i;
			if(0 != pStbl->age_count){
				nppkt_printf("[%s][%d]found pTbl %p age_count %d, valid %d\n", __func__, __LINE__, 
						pStbl, pStbl->age_count, pStbl->valid);
				dump_session_table(pStbl);
				active_count++;
				pStbl->age_count=0;				
			}else{
				if(0 != pStbl->valid){
					dump_session_table(pStbl);
					pStbl->valid = 0;
					aged_count++;
				}
			}
		}
		CVMX_SYNCS;
		printf("totol of active session is %d\n", active_count);
		printf("%d session tables were deleted \n", aged_count);
	}
	cvmx_coremask_barrier_sync(&coremask);
    printf(" example exit \n");
    return 0;
}
