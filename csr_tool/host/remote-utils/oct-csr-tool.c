#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include "cvmx-csr-db.h"
#include "cvmx-version.h"

//version 1.1.8 fixed bug in format function
#define OCT_CSR_TOOL_VERSION_STRING "1.1.8"

int octeon_model = -1;
char * octeon_modelStr = NULL;
char * inputfile = NULL;

#define FUNC_NULL 0
#define FUNC_REG_SEARCH 1
#define FUNC_GEN_CODE 2
#define FUNC_DUMP_FORMAT 3
int FuncMode = FUNC_NULL;

const char * get_csr_tool_version(void)
{
   return OCT_CSR_TOOL_VERSION_STRING;
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
	printf("\n%s is not implemented\n\n", __func__);
    return octeon_model;
}
#endif

//0x000d9600:	/* CN70XX */
//0x000d9500:	/* CN78XX */
//0x000d9500:	/* CN78XXp2 */
//0x000d9700:	/* CN73XX */
 int csr_get_model(char * p)
{
	if( (strncasecmp(p, "CN70XX", 6) == 0) ||(strncasecmp(p, "CN71XX", 6) == 0) ){
		return 0x000d9600;
	}
	if( (strncasecmp(p, "CN70XXp1", 6) == 0) ||(strncasecmp(p, "CN71XXp1", 6) == 0) ){
		return 0x000d9601;
	}	
	if (strncasecmp(p, "CN78XXp2", 8) == 0) {
		return 0x000d9508;
	}	
	if (strncasecmp(p, "CN78XX", 6) == 0) {
		return 0x000d9500;
	}
	if( (strncasecmp(p, "CN73XX", 6) == 0) || ((strncasecmp(p, "CN72XX", 6) == 0) )){
		return 0x000d9700;
	}	
	if (strncasecmp(p, "CN68XX", 6) == 0) {
		return 0x000d9100;
	}	
	return 0;
}

void usage(char * const *argv) {
	printf("\n" "Usage: %s \n"
		"    -h : help\n"
		"    -v : display version\n"
		"    -t : target in (CN70XX,CN70XXp1,CN78XX,CN78XXp2,CN73XX,CN68XX)\n"
		"    -s : search the registers\n"
		"    -g : generate the dumping code\n"
		"    -f : format the dumping results\n"
		"examples:\n"
		"    -t cn78XXp2 -s -i GSER1_PLL > GSER1_PLL : search the registers like Gser1_pll* to reg.txt\n"
		"    -t cn78XXp2 -g -i GSER1_PLL : generate the dumping code.\n"
		"    -t cn78XXp2 -f -i csrdump.txt : translate the csr dumping to readable format\n"
		"    \n",
		argv[0]);
}

int parse_options(int argc, char * const *argv)
{
    const struct option long_options[] =
    {
    	 {"target", 1, 0, 't'},
	 {"input", 1, 0, 'i'},
	 {"search", 0, 0, 's'},
    	 {"generate", 0, 0, 'g'},
        {"format", 0, 0, 'f'},
        {"version", 0, 0, 'v'},
        {"help", 0, 0, 'h'},
        {NULL, 0, 0, 0}
    };

    while (1)
    {
        char option = getopt_long(argc, argv, "t:i:sgfvh", long_options, NULL);
        if (option <= 0)
            break;

        switch (option)
        {
            case 't':
			octeon_modelStr = optarg;	
			octeon_model = csr_get_model(optarg);
			//printf("[%d]octeon_model: %s 0x%08x\n", __LINE__, octeon_modelStr, octeon_model); 
			break;
            case 's':
			FuncMode = FUNC_REG_SEARCH;
			break;
            case 'g':
			FuncMode = FUNC_GEN_CODE;
			break;
            case 'f':
			FuncMode = FUNC_DUMP_FORMAT;
			break;			
	      case 'i':
			inputfile = optarg;	
			break;
	     case 'h':
			usage(argv);	
			exit(0);
            case 'v':
			printf("\n%s version: %s\n", argv[0], OCT_CSR_TOOL_VERSION_STRING); 
			printf("SDK version: %s\n\n", OCTEON_SDK_VERSION_STRING); 			
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

int __dumping_format(char * inputfile, int cpuid)
{
	printf("\n%s is not supported\n\n", __func__);
	return -1;
}
int dumping_format(char * inputfile, int cpuid) 
	__attribute__((weak, alias("__dumping_format")));

int register_serach_code(char * regStr, int octeon_model)
{
	printf("\n%s is not supported\n\n", __func__);
	return -1;
}
int register_serach_code(char * regStr, int octeon_model) __attribute__((weak));

int __generate_code(char * inputfile, char * outputfile, int cpuid, char *pattern)
{
	printf("\n%s is not supported\n\n", __func__);
	return -1;
}
int generate_code(char * inputfile, char * outputfile, int cpuid, char *pattern) 
	__attribute__((weak, alias("__generate_code")));


char strInput[256]={0};
char strOutput[256]={0};
char strModel[256]={0};
char cRedirect=0;

int console_prompt(int argc, char * const *argv)
{
	int iCpu;
    int iFuncMode;
	
	printf("\nVersion: %s\n", OCT_CSR_TOOL_VERSION_STRING); 
	printf("SDK version: %s\n\n", OCTEON_SDK_VERSION_STRING); 	
	printf("Select CPU model:\n\t[1]CN78XXP2\n\t[2]CN78XX\n"
			"\t[3]CN73XX\n\t[4]CN70XXP1\n\t[5]CN70XX\n\t[6]CN68XX\n>> ");
	scanf("%d",&iCpu);
	switch(iCpu)
	{
		case 6:
			strcpy(strModel, "CN68XX");
			break;
		case 5:
			strcpy(strModel, "CN70XX");
			break;	
		case 4:
			strcpy(strModel, "CN70XXP1");
			break;	
		case 3:
			strcpy(strModel, "CN73XX");
			break;	
		case 2:
			strcpy(strModel, "CN78XX");
			break;				
		case 1:
			strcpy(strModel, "CN78XXP2");
			break;
		default:
			return -3;
	}
	octeon_modelStr = strModel;
	octeon_model = csr_get_model(octeon_modelStr);
	
	printf("Select function:\n\t[1]searching the partial register name in the database\n"
			"\t[2]generate printf code with the input TXT file\n"
			"\t[3]translate register dumping into a readable format\n>> ");
	scanf("%d",&iFuncMode);
	switch(iFuncMode)
	{
		case 1:
			FuncMode = FUNC_REG_SEARCH;
			printf("Register Name:\n>> ");
			break;	
		case 2:
			FuncMode = FUNC_GEN_CODE;
			printf("Registers file:\n>> ");
			break;				
		case 3:
			FuncMode =  FUNC_DUMP_FORMAT;
			printf("csr dumping file:\n>> ");
			break;
		default:
			return -2;
	}

	scanf("%s", strInput);
	inputfile = strInput;
	fflush(stdin);
#ifndef QT_PROJECT
    getchar();
#endif
	//printf("\n#%s#\n", strInput);

	//get the output file name
    cRedirect = '0';
	memset(strOutput, 0, 256);
	if((FUNC_REG_SEARCH ==  FuncMode)||(FUNC_DUMP_FORMAT ==  FuncMode)){
		printf("output all the result to a TXT file(Y/N):\n>> ");
        cRedirect = getchar();
		fflush(stdin);
	}	
    if(('y' == cRedirect)||('Y' == cRedirect)){
		if(FUNC_REG_SEARCH == FuncMode)
			sprintf(strOutput, "%s.%s", strInput, "reg");

		if(FUNC_DUMP_FORMAT == FuncMode)
			sprintf(strOutput, "%s.%s", strInput, "readable");
	}

	//output the command
	printf("Command is running: %s -t %s ", argv[0], strModel);
	switch(FuncMode)
	{
		case FUNC_REG_SEARCH:
			printf("-s -i %s ", strInput);
			break;
		case FUNC_GEN_CODE:
			printf("-g -i %s ", strInput);
			break;
		case FUNC_DUMP_FORMAT:
			printf("-f -i %s ", strInput);
			break;
		default :
			return -5;
	}
    if(('y' == cRedirect)||('Y' == cRedirect))
		printf(" > %s", strOutput);
	printf("\n");
		
	return 0;
} 
#ifdef QT_PROJECT
int QT_main(int argc, char * const *argv)
#else
int main(int argc, char * const *argv) 
#endif
{
	int ret = 0;
	if(1 == argc){
		ret = console_prompt(argc, argv);
		if(0 != ret){
			printf("\n invalid input %d\n", ret);
			return -1;
		}
	}else{
		if (parse_options(argc, argv)){
			return -1;
		}
	}
		
	//printf("[%d]octeon_model: %s 0x%08x\n", __LINE__, octeon_modelStr, octeon_model); 
	//printf("[%d]FuncMode: 0x%x\n", __LINE__, FuncMode); 
	if(inputfile == NULL)
	{
		printf("[%d]no input\n", __LINE__); 
		usage(argv);
		return -1;
	}

    if(('y' == cRedirect)||('Y' == cRedirect)){
#ifndef QT_PROJECT
        stdout = freopen(strOutput,"w",stdout);
#else
        freopen(strOutput,"w",stdout);
#endif
    }

	if(FUNC_REG_SEARCH == FuncMode)
	{
		register_serach_code(inputfile, octeon_model);
	}
	if(FUNC_GEN_CODE == FuncMode)
	{
		sprintf(strOutput, "%s.%s", inputfile, "code");
		generate_code(inputfile, strOutput, octeon_model, octeon_modelStr);
	}
	if(FUNC_DUMP_FORMAT == FuncMode)
	{
		dumping_format(inputfile, octeon_model);
	}	

    if(('y' == cRedirect)||('Y' == cRedirect)){
#ifdef QT_PROJECT
        freopen("CON","w",stdout);
#endif
    }
	return 0;
}

