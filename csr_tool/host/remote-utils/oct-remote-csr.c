#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "cvmx-csr-db.h"
#include "cvmx-version.h"

#undef CVMX_ADD_IO_SEG
#define CVMX_ADD_IO_SEG(a) (a)	/* There aren't any Mips IO segments when using PCI */


#define CVMX_SRIO_CONFIG_TIMEOUT        10000	/* 10ms */

int node_num = -1;
int octeon_model = -1;
char * inputfile = NULL;

static uint64_t convert_number(const char *str) {
	unsigned long long result;
	if ((str[0] == '0') && (str[1] == 'x')) {
		if (sscanf(str + 2, "%llx", &result) != 1)
			result = 0;
	} else {
		if (sscanf(str, "%lli", &result) != 1)
			result = 0;
	}
	return result;
}

void usage(char * const *argv) {
	printf("\n" "Usage: %s -t cn78XX -i csrdump.txt\n"
	"    -v : display version\n\n",
	argv[0]);
}

#if 1
unsigned int cvmx_get_node_num(void)
{
	return 0;
}

uint32_t cvmx_read64_uint32(uint64_t csr_addr)
{
    return 0;
}

uint64_t cvmx_read_csr_node(uint64_t node __attribute__ ((unused)), uint64_t csr_addr)
{
    return 0;
}

uint32_t cvmx_get_proc_id(void)
{
    return octeon_model;
}
#endif

//0x000d9600:	/* CN70XX */
//0x000d9500:	/* CN78XX */
//0x000d9700:	/* CN73XX */
 int csr_get_model(char * p)
{
	if (strncasecmp(p, "CN70XX", 6) == 0) {
		return 0x000d9600;
	}
	if (strncasecmp(p, "CN71XX", 6) == 0) {
		return 0x000d9600;
	}	
	if (strncasecmp(p, "CN78XX", 6) == 0) {
		return 0x000d9500;
	}
	if (strncasecmp(p, "CN73XX", 6) == 0) {
		return 0x000d9700;
	}	
	return 0;
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
    	 {"target", 1, 0, 't'},
    	 {"input", 1, 0, 'i'},
        {"node", 1, 0, 'n'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "i:t:n:hv", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'n':
                node_num = convert_number(optarg);
                break;
            case 'i':
			inputfile = optarg;	
			break;
            case 't':
			octeon_model = csr_get_model(optarg);
			//printf("[%d]octeon_model: %s 0x%08x\n", __LINE__, optarg, octeon_model); 			
                break;			
	    /* fallthrough */
            case 'v':
                printf("SDK version: %s\n", OCTEON_SDK_VERSION_STRING); 
			exit(0);
            default:
                usage(argv);
                return -1;
        }
    }

    if (optind > argc)
    {
        usage(argv);
        return -1;
    }

	if(inputfile == NULL){
		printf("no input\n"); 
		usage(argv);
		return -1;
	}
	
    return 0;
}

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
		cvmx_csr_db_print_decode_by_prefix_node(0, octeon_model, reginfo->regname, 0);
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

int main(int argc, char * const *argv) 
{
	FILE *infile = NULL;
	char buf[MAX_LINE]; 
	int len; 
	
	/* Make sure we got the correct number of arguments */
	if (parse_options(argc, argv)){
		return -1;
	}
	
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
