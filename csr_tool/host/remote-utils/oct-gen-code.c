#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "cvmx-csr-db.h"
#include "cvmx-version.h"


#if 1
#define Debug_printf printf
#else
#define Debug_printf(x,...) 
#endif

int octeon_model = 0;
char * regStr = NULL;
char * inputfile = NULL;

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



void usage(char * const *argv) {
	printf("\n" "Usage: %s \n"
	" -t cn78XX -g Gser1_pll > reg.txt\n"
	" -t cn78XX -i reg.txt\n"
	" -v : display version\n\n",
	argv[0]);
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
    	 {"target", 1, 0, 't'},
        {"generate", 1, 0, 'g'},
        {"input", 1, 0, 'i'},
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'v'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "i:t:g:hv", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 'g':
                	regStr = optarg;
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
	
    return 0;
}

 #define MAX_LINE 1024

int string_to_upper(char * p)
{
	int len,i; 

	len = strlen(p);

	if(len > MAX_LINE){
		return -1;
	}
	for(i=0;i<=len;i++)
	 {
	 	if((p[i] >= 'a')&&(p[i] <= 'z'))
	 		p[i] = p[i]-32;
	 }
	return len;
}

int generate_code(void)
{
	FILE *infile = NULL;
	FILE *outfile = NULL;
	char buf[MAX_LINE]; 
	int len; 
	uint64_t csr_addr;
	int csr_type, csr_widthbits;	
	char outbuf[MAX_LINE];
	
	memset(buf, 0, MAX_LINE);
	sprintf(buf, "%s_%x.c", inputfile, octeon_model);
	//printf("\n %s \n", buf);
	
	outfile = fopen(buf, "wt+");
	if (!outfile)
	{
		printf("outfile error\n"); 
		return -1;
	}
	
	infile = fopen(inputfile, "r");
	if (!infile)
	{
		printf("input error\n"); 
		return -1;
	}
	
	memset(outbuf, 0, MAX_LINE);
	sprintf(outbuf, "void registers_dump_%x(void)\n{\n", octeon_model);
	fwrite (outbuf , strlen(outbuf), 1 , outfile);

	memset(outbuf, 0, MAX_LINE);
	memset(buf, 0, MAX_LINE);
	while(fgets(buf,MAX_LINE,infile) != NULL)
	{		
		len = strlen(buf);
		buf[len-1] = '\0';  
		//printf("buf %s %d \n",buf,len - 1);
		
		cvmx_csr_db_get_params(octeon_model, 
				buf, &csr_addr, &csr_type, NULL, &csr_widthbits);
		//printf("0x%016llx \n", (unsigned long long)csr_addr);
		csr_addr |= (1ull <<63);
		
#if 0		
		sprintf(outbuf, "%s 0x%016llx\n", buf, (unsigned long long)csr_addr);
#else
		sprintf(outbuf, "\tcvmx_dprintf(\"%s 0x%016llx 0x%%016llx\\n\","
					"(long long)cvmx_read_csr_node(0,0x%016llx));\n", 
					buf, (unsigned long long)csr_addr, (unsigned long long)csr_addr);
#endif
		fwrite (outbuf , strlen(outbuf), 1 , outfile);
		
		memset(buf, 0, MAX_LINE);
		memset(outbuf, 0, MAX_LINE);
	}

	memset(outbuf, 0, MAX_LINE);
	sprintf(outbuf, "}\n");
	fwrite (outbuf , strlen(outbuf), 1 , outfile);
	
	fclose(outfile);
	fclose(infile);
	return 0;
}

int main(int argc, char * const *argv) 
{

	
	/* Make sure we got the correct number of arguments */
	if (parse_options(argc, argv)){
		return -1;
	}

	if(0 == octeon_model){
	        usage(argv);
	        return -1;
	}
	
	if(NULL != regStr)
	{
		cvmx_csr_db_print_decode_by_prefix_node(0, octeon_model, regStr, 0);
		return 0;
	}

	if(NULL != inputfile){
		generate_code();
		return 0;
	}

	usage(argv);
	return 0;
}
