#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "cvmx-csr-db.h"

extern int octeon_model;

struct reg_info{
	char * regname;
	unsigned long long regaddr;
	unsigned long long regvalue;
};

void Parser_register(struct reg_info * reginfo)
{
	uint64_t csr_addr;
	int csr_type, csr_widthbits, ret;

	ret = cvmx_csr_db_get_params(octeon_model, 
			reginfo->regname, &csr_addr, &csr_type, NULL, &csr_widthbits);
	if (ret != 0) {
		/* This lists all csr names by partial or full basename */
		fprintf(stderr, "CSR not found in list. Looking for prefix \"%s\"\n", reginfo->regname);
		//cvmx_csr_db_print_decode_by_prefix_node(0, octeon_model, reginfo->regname, 0);
		ret = 0;
		return ;
	}

	if(csr_addr != (reginfo->regaddr & 0x00ffFFFFffffFFFF)){
        printf("warning addr is not match (0x%llx)(0x%llx)\n", reginfo->regaddr, (long long)csr_addr);
	}
	
	cvmx_csr_db_decode_by_name_node(0, octeon_model, reginfo->regname, reginfo->regvalue);
	printf("\n");
	return ;
}

//#define Parser_printf printf
#define Parser_printf(x,...) 

 void parser_line(char * s)
{
 	char *delim = " ";
	char *p;
	struct reg_info regtemp;

	p = strtok(s, delim);
	if(p == NULL)
		return ;
	regtemp.regname = p;
	Parser_printf("regname %s ", regtemp.regname);

	p = strtok(NULL, delim);
	if(p == NULL)
		return ;
	Parser_printf("regaddr %s ", p);
	regtemp.regaddr = strtoull(p, NULL, 0);
	Parser_printf("regaddr 0x%016llx ", regtemp.regaddr);

	p = strtok(NULL, delim);
	if(p == NULL)
		return ;
	regtemp.regvalue = strtoull(p, NULL, 0);
	Parser_printf("regvalue 0x%016llx ", regtemp.regvalue);

	Parser_printf("\n");

	Parser_register(&regtemp);
	return ;
}

#if 0
#define Readline_printf printf
#else
#define Readline_printf(x,...) 
#endif
 #define MAX_LINE 1024

int dumping_format(char * inputfile, int cpuid)
{
	FILE *infile = NULL;
	char buf[MAX_LINE]; 
	int len; 
	
	infile = fopen(inputfile, "r");
	if (!infile)
	{
	printf("input error\n"); 
	    return -1;
	}

	while(fgets(buf,MAX_LINE,infile) != NULL)
	{
	 len = strlen(buf);
	 buf[len-1] = '\0';  
	 
	 Readline_printf("%s %d \n",buf,len - 1);
	 parser_line(buf);
	 
	 memset(buf, 0, MAX_LINE);
	}

 
    fclose(infile);
	return 0;
}




