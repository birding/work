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
extern const char * get_csr_tool_version(void);
int generate_code(char * inputfile, int cpuid, char *pattern)
{
	FILE *infile = NULL;
	FILE *outfile = NULL;
	char buf[MAX_LINE]; 
	int len; 
	uint64_t csr_addr;
	int csr_type, csr_widthbits;	
	char outbuf[MAX_LINE];
	
	memset(buf, 0, MAX_LINE);
	sprintf(buf, "%s_%s.c", pattern, inputfile);
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
		printf("input open failed\n"); 
		return -1;
	}
	
	memset(outbuf, 0, MAX_LINE);
	sprintf(outbuf, "void %s_registers_dump(void)\n{\n", pattern);
	fwrite (outbuf , strlen(outbuf), 1 , outfile);

	memset(outbuf, 0, MAX_LINE);
	sprintf(outbuf, "\tcvmx_dprintf(\"###CSRTOOL%s\\n\");\n", get_csr_tool_version());
	fwrite (outbuf , strlen(outbuf), 1 , outfile);

	memset(outbuf, 0, MAX_LINE);
	sprintf(outbuf, "\tcvmx_dprintf(\"###check#cpuid:0x%x==0x%%x\\n\",cvmx_get_proc_id());\n", cpuid);
	fwrite (outbuf , strlen(outbuf), 1 , outfile);
	
	memset(outbuf, 0, MAX_LINE);
	memset(buf, 0, MAX_LINE);
	while(fgets(buf,MAX_LINE,infile) != NULL)
	{		
		len = strlen(buf);
		buf[len-1] = '\0';  
		//printf("buf %s %d \n",buf,len - 1);
		
		cvmx_csr_db_get_params(cpuid, 
				buf, &csr_addr, &csr_type, NULL, &csr_widthbits);
		//printf("0x%016llx \n", (unsigned long long)csr_addr);
		//sprintf(outbuf, "%s 0x%016llx\n", buf, (unsigned long long)csr_addr);
		
		if ((csr_type == CVMX_CSR_DB_TYPE_PCICONFIGEP) || (csr_type == CVMX_CSR_DB_TYPE_PCICONFIGRC)) {
			/* Names are of the format "PCIE??#_CFG???". The # is the pcie port number */
			int pcie_port = (buf[6] - '0');
		
			sprintf(outbuf, "\t{\n\t"
						"cvmx_pemx_cfg_rd_t pemx_cfg_rd; pemx_cfg_rd.u64 = 0;pemx_cfg_rd.s.addr = 0x%x;\n\t"
						"cvmx_write_csr(CVMX_PEMX_CFG_RD(%d), pemx_cfg_rd.u64);"
						"pemx_cfg_rd.u64 = cvmx_read_csr(CVMX_PEMX_CFG_RD(%d));\n\t"
						"cvmx_dprintf(\"%s 0x%016llx 0x%%016llx\\n\","
						"(long long)(0xffffffffull & pemx_cfg_rd.s.data));\n\t"
						"}\n", 
						(unsigned int)(csr_addr&0xffffFFFF),//?????? 
						pcie_port,
						pcie_port,
						buf, (unsigned long long)csr_addr);
		}else{
			csr_addr |= (1ull <<63);
			sprintf(outbuf, "\tcvmx_dprintf(\"%s 0x%016llx 0x%%016llx\\n\","
						"(long long)cvmx_read_csr(0x%016llx));\n", 
						buf, (unsigned long long)csr_addr, (unsigned long long)csr_addr);
		}
		
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


