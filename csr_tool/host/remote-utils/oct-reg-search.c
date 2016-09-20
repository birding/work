#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "cvmx-csr-db.h"

int register_serach_code(char * regStr, int octeon_model)
{
	if(NULL != regStr)
	{
		cvmx_csr_db_print_decode_by_prefix_node(0, octeon_model, regStr, 0);
		return 0;
	}
	return -1;
}
	