
#include <bdk.h>
#include <error.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include <bdk.h>
#include <bdk-csrs-rst.h>

#define TOOL_VERSION "3.1"

#define debug_print(args ...) if (verbose) printf(args)

/* RCLK = PLL_REF_CLK (= 50 MHz) × [C_MUL]
 *  */

#define PLL_REF_CLK 50  //Reference clk value 50 MHz

#define info(args ...)    printf(args)
#define warning(args ...) printf(args)
#define error(args ...)   fprintf(stderr, args)

int bdk_csr_decode(const char *name, uint64_t value, int inf_id[]);
int reg_addr_to_name(uint64_t reg_address, char *reg_name);
void name_suggestion(char *name);
void name_suggestion_for_details(char * app, char *name, int mode);
const __bdk_csr_db_type_t *__bdk_csr_lookup(const char *name, int inf_id[]);

static int verbose, processor_num = 0, g_mul = 0;

static void sig_handler(int signum) {
	if (signum == SIGBUS) {
		printf("\n A bus error signal has been captured. Reason be :\
			\n 1. Register access permissions from user-space(EL0).\
			\n 2. Set-Reset controller register.\n\
			\n Note: Please read ThunderX/ARMv8 HRM for details.\n\n");
		exit(1);
	}
	if (signum == SIGSEGV) {
		printf("\n A bus error signal has been captured. Reason be :\
			\n 1. Register access permissions from user-space(EL0).\
			\n 2. Set-Reset controller register.\n\
			\n Note: Please read ThunderX/ARMv8 HRM for details.\n\n");
		exit(1);
	}
}

/*
 * Function prints usage options
*/

static void usage()
{
	info("\nUSAGE:\n");
        info("\nRead operation :\n\
                sudo ./txcsr <register-name> <options>\n\nWrite operation :\n\
                sudo ./txcsr <register-name> <options> <value>\n\nOptions :\n");
	info("\t\t--help | -h          Print this message\n\
		-v	             Debug mode\n\
		-n <0/1>             socket number(for 2S-CRB),default value 0\n\
		-d	             Decode register value (only for read operation)\n\
		-p	             Processor/core number value (used to access system register)\n\
		-s <4/8>             Access size (4 or 8 bytes) when address is specified\n\
		\n============================================================================================\n\
		\n\t\tFor registers like NIC_QS(0..127)_RQ(0..7)_STAT(0..1) \n\
		give name : NIC_QSX_RQX_STATX with foll interfaces\n \n\
		-a <offset>          Alpha value\n\
		-b <offset>          Beta value\n\
		-g <offset>          Gamma value\n\
		-c <offset>          Chi offset\n\
		\n============================================================================================\n\
		\n\t\tSpecial functions use\n\n\
		--cpufreq              Prints CPU Frequency\n\
		--listreg <block-name> List available registers for a block eg: SATAX block\n\
		--listregd0 <block-name> List details of available registers for a block eg: SATAX block\n\
		--listregd1 <block-name> List details of available registers for a block eg: SATAX block\n\
		--listregd2 <block-name> List details of available registers for a block eg: SATAX block\n\
		--l2stats	       Displays L2C TAD register contents for performance monitoring\n\n\
		\n============================================================================================\n\
		\nNOTE: \n1. If register address address is known read/write can be done as :\n\
		      sudo ./txcsr <reg-address>\n\
		      sudo ./txcsr <reg-address> <value>\n\n2. To print contents of Physical memory :\n\
		      sudo ./txcsr <physical-memory-address> <number-of-bytes-to-read>\n\
		      example : sudo ./txcsr 0x1400000 1024\n\n3. To access system registers, DAP_IMP_DAR[CABEN] must be set.\n\
		      To set : Default value of DAP_IMP_DAR is 0x23F, after setting CABEN bit the value becomes 0x63F\n\
		      sudo ./txcsr DAP_IMP_DAR 0x63F\n\n");
	exit(0);
}
/*
  * Special operations like CPU frequency,listing registers of a block

  * INPUT : command line arguments
*/
/*
 * Function to map device memory to user space
*/
static int open_devmem () {
	int fd;

	/*opening a file*/
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		error("ERROR: Faied to open /dev/mem\n");
		return -1;
	}
	return fd;
}

static int close_devmem (int fd)
{
	close (fd);
	return 0;
}

/*
 * Maps register hardware address to virtual user space accessible address.

 * Register address is mapped to user space from where register read/write
   operation is performed.

 * INPUT : Register h/w address, file discriptor, pointer variable to map
	   address
 * OUTPUT: Virtual,user space, address to access register.
*/
static void *map_reg_pa_to_va (uint64_t reg_pa,  int fd){

	uint64_t reg_offset;
	uint8_t *hw_addr;
	off64_t  reg_base;
	size_t length;

	reg_offset = reg_pa % getpagesize();

	reg_base = reg_pa - reg_offset;
	length = getpagesize();

	hw_addr = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, reg_base);
	if (hw_addr == MAP_FAILED) {
		error("Mmap failed");
		exit(1);
	}

	return (hw_addr + reg_offset);
}


/*
 * Calculates CPU Frequency

 * Method :
	1. From RST_BOOT register read core-clock multiplier (C_MULL)
	2. Multiply c_mull by refernce clock frequency (50 MHz, fixed,refer to HRM for details)
	3. Multiplied value is CPU clock frequency
*/
static int print_cpufreq (void) {

	int fd;
	uint64_t *reg_va;
	uint64_t reg_pa;
	bdk_rst_boot_t rst_boot;
	int inf_id[BDK_CSR_DB_MAX_PARAM]={-1, -1, -1, -1};

	debug_print("\nIn print_cpufreq function ...\n");
	fd = open_devmem();
	reg_pa = bdk_csr_read_by_name("RST_BOOT",inf_id);
	debug_print("\nAccessing register \"RST_BOOT\" : address : 0x%016llx\n", (unsigned long long)reg_pa);
	reg_va = map_reg_pa_to_va(reg_pa, fd);
	rst_boot.u = *reg_va;
	debug_print("\nc_mul value from RST_BOOT : %d\n",rst_boot.cn88xx.c_mul);
	info("\nCPU FREQUENCY (RCLK): %d MHz\n", PLL_REF_CLK * rst_boot.cn88xx.c_mul);
	info("CPU FREQUENCY (SCLK): %d MHz\n", PLL_REF_CLK * rst_boot.cn88xx.pnr_mul);
	close_devmem(fd);
	return 0;
}

static inline uint64_t
bdk_clock_get_count(void)
{
        uint64_t tsc;
#ifdef _X86_HOST_
       tsc=0;
	printf("[%s][%d]not for x86\n", __func__, __LINE__);
#else
	asm volatile("mrs %0, cntvct_el0" : "=r" (tsc));
#endif
        return tsc * g_mul;
}


static void l2_perf_cmb( bdk_node_t node)
{

    /* These define the array bounds for L2C_CBCX_*X_PFC */
    const int  L2_BUS_ROWS = CAVIUM_IS_MODEL(CAVIUM_CN88XX) ? 4 : CAVIUM_IS_MODEL(CAVIUM_CN83XX) ? 2 : 1;
    const int L2_BUS_COLS = CAVIUM_IS_MODEL(CAVIUM_CN81XX) ? 2 : 3;
    //const int L2_NUM_IOB = CAVIUM_IS_MODEL(CAVIUM_CN81XX) ? 1 : 2;
    int row, col, fd;
    int64_t value;
    int inf_id[BDK_CSR_DB_MAX_PARAM]={-1, -1, -1, -1};
    uint64_t *reg_va;
    uint64_t reg_pa;

	//printf("\nInside l2_perf_cmb L2_BUS_ROWS: %d\n", L2_BUS_ROWS);

    fd = open_devmem();
    /* This is the time of the last update for each counter */
    static uint64_t last_start;

    uint64_t current_rclk = bdk_clock_get_count();
    uint64_t delta_rclk = current_rclk - last_start;
    uint64_t delta_rclk_percent = delta_rclk / 100;


    if (delta_rclk_percent == 0)
        delta_rclk_percent = 1;
    last_start = current_rclk;
    for (row = 0; row < L2_BUS_ROWS; row++)
    {
        for (col = 0; col < L2_BUS_COLS; col++)
        {
            int cmb_num = col * L2_BUS_ROWS + row;
            if (cmb_num > 9)
                continue;
            char bus_name[8];
            if (cmb_num < 8)
            {
                cmb[cmb_num].cmbnum = cmb_num;
                snprintf(bus_name, sizeof(bus_name), "cmb%d", cmb_num);
            }
            else
            {

                cmb[cmb_num].cmbnum = cmb_num;    // cmb_num will be either 8 or 9. 8 should map to iob0(cmb_num-8) and 9 should map to iob1(cmb_num-8)
            }

            /* Counts the number of cycles the XMC(Add bus) was busy. So the
                percentage used is simply 100 * count / RCLK */
	    inf_id[0] = row;
	    inf_id[1] = col;
            reg_pa = bdk_csr_read_by_name("L2C_CBCX_XMCX_PFC", inf_id);
	    reg_va = map_reg_pa_to_va(reg_pa, fd);	
	    value  = (int64_t)*reg_va;
            cmb[cmb_num].add = value;
            cmb[cmb_num].add_cent = value/delta_rclk_percent;
	    *reg_va = 0;
            //value = bdk_csr_write_by_name(BDK_NODE_0, "L2C_CBCX_XMCX_PFC", 0, row, col);


            /* Counts the number of cycles the XMD(Store bus) was busy. So the
                percentage used is simply 100 * count / RCLK */

            inf_id[0] = row;
            inf_id[1] = col;
            reg_pa = bdk_csr_read_by_name("L2C_CBCX_XMDX_PFC", inf_id);
            reg_va = map_reg_pa_to_va(reg_pa, fd);
            value  = (int64_t)*reg_va;
            cmb[cmb_num].store = value;
            cmb[cmb_num].store_cent = value/delta_rclk_percent;
	    *reg_va = 0;	


            /* Counts the number of cycles the RSC(Commit bus) was busy. So the
                percentage used is simply 100 * count / RCLK */

            inf_id[0] = row;
            inf_id[1] = col;
            reg_pa = bdk_csr_read_by_name("L2C_CBCX_RSCX_PFC", inf_id);
            reg_va = map_reg_pa_to_va(reg_pa, fd);
            value  = (int64_t)*reg_va;
            cmb[cmb_num].commit = value;
            cmb[cmb_num].commit_cent = value/delta_rclk_percent;
            *reg_va = 0;



            /* Counts the number of cycles the RSD(Fill bus) was busy. So the
                percentage used is simply 100 * count / RCLK */

            inf_id[0] = row;
            inf_id[1] = col;
            reg_pa = bdk_csr_read_by_name("L2C_CBCX_RSDX_PFC", inf_id);
            reg_va = map_reg_pa_to_va(reg_pa, fd);
            value  = (int64_t)*reg_va;
            cmb[cmb_num].fill = value;
            cmb[cmb_num].fill_cent = value/delta_rclk_percent;
            *reg_va = 0;


            /* It's not obvious, but column 2 (CMB 8-9) corespond to IOB, which
               doesn't have invalidates */
            if (col < 2)
            {
                /* Number of invalidates */
            	inf_id[0] = row;
            	inf_id[1] = col;
                reg_pa = bdk_csr_read_by_name("L2C_CBCX_INVX_PFC", inf_id);
	        reg_va = map_reg_pa_to_va(reg_pa, fd);
        	value  = (int64_t)*reg_va;
                cmb[cmb_num].inv = value;
            	*reg_va = 0;

            }
        }
    }
    close_devmem(fd);
}

static void l2_perf_io(bdk_node_t node)
{
    const int L2_NUM_IO = CAVIUM_IS_MODEL(CAVIUM_CN88XX) ? 4 : CAVIUM_IS_MODEL(CAVIUM_CN83XX) ? 2 : 1;
    int fd;
    int inf_id[BDK_CSR_DB_MAX_PARAM]={-1, -1, -1, -1};
    uint64_t *reg_va;
    uint64_t reg_pa;

    /* This is the time of the last update for each counter */
    static uint64_t last_start;
    int64_t value;
    uint64_t current_rclk = bdk_clock_get_count();
    uint64_t delta_rclk = current_rclk - last_start;
    uint64_t delta_rclk_percent = delta_rclk / 100;


    fd = open_devmem();

    if (delta_rclk_percent == 0)
        delta_rclk_percent = 1;
    last_start = current_rclk;
    int io_num;
    for ( io_num = 0; io_num < L2_NUM_IO; io_num++)
    {
        char io_name[8];
        io[io_num].ionum = io_num;
        snprintf(io_name, sizeof(io_name), "io%d", io_num);

       /* Counts the number of cycles the IOC(IO request bus) was busy.
            So the percentage used is simply 100 * count / RCLK */
	inf_id[0] = io_num;
	inf_id[1] = 0;
        reg_pa = bdk_csr_read_by_name("L2C_CBCX_IOCX_PFC", inf_id);
	reg_va = map_reg_pa_to_va(reg_pa, fd);
	value  = (int64_t)*reg_va;
        io[io_num].ioc=value;
        io[io_num].ioc_cent=value/delta_rclk_percent;
	*reg_va = 0;

        /* Counts the number of cycles the IOR(IO response bus) was busy.
            So the percentage used is simply 100 * count / RCLK */
	
        reg_pa = bdk_csr_read_by_name("L2C_CBCX_IORX_PFC", inf_id);
	reg_va = map_reg_pa_to_va(reg_pa, fd);
	value  = (int64_t)*reg_va;
        io[io_num].ior=value;
        io[io_num].ior=value/delta_rclk_percent;
	*reg_va = 0;

    }
    close_devmem(fd);	
}

static void l2_perf_tad( bdk_node_t node)
{

    int64_t value,value1,value2;
    int tad;
    int fd;
    int inf_id[BDK_CSR_DB_MAX_PARAM]={-1, -1, -1, -1};
    uint64_t *reg_va;
    uint64_t reg_pa;

    /* These define TAD counter ranges */
    const int MAX_TADS = CAVIUM_IS_MODEL(CAVIUM_CN88XX) ? 8 : CAVIUM_IS_MODEL(CAVIUM_CN83XX) ? 4 : 1;
    const int NUM_TAD_COUNTERS = 4;



    /* We cycle through 6 counter sets. 0-3 are each quad, and 4,5 are
        handpicked values */
    static int tad_count_set = 0;
    /* This is the start time of the setup for current count_set */
    static uint64_t last_start[BDK_NUMA_MAX_NODES][8][4];

    uint64_t rclk_rate = g_mul*100000000;
    uint64_t current_rclk = bdk_clock_get_count();

    fd = open_devmem();

    for ( tad = 0; tad < MAX_TADS; tad++)
    {
        char tad_name[8];
        tads[tad].tadnum = tad;
        snprintf(tad_name, sizeof(tad_name), "tad%d", tad);



        /* Read the current performace counter settings */
	inf_id[0] = tad;
	inf_id[1] = -1;
        reg_pa = bdk_csr_read_by_name("L2C_TADX_PRF", inf_id);
	reg_va = map_reg_pa_to_va(reg_pa, fd);
	tadx_prf.u = (uint64_t)*reg_va;
	//*reg_va = 0;

        int counter;
        /* Cycle through counters adding them to our L2's table */
        for (counter=0; counter<NUM_TAD_COUNTERS; counter++)
        {
            /* Calculate time since last update */
            uint64_t delta_rclk = current_rclk - last_start[node][tad][counter];
            if (delta_rclk == 0)
                delta_rclk = 1;
            uint64_t delta_rclk_percent = delta_rclk / 100;
            if (delta_rclk_percent == 0)
                delta_rclk_percent = 1;
            last_start[node][tad][counter] = current_rclk;


            /* Read the appropriate counter */
            inf_id[0] = tad;
            inf_id[1] = counter;
            reg_pa = bdk_csr_read_by_name("L2C_TADX_PFCX", inf_id);
	    reg_va = map_reg_pa_to_va(reg_pa, fd);
	    value  = (uint64_t)*reg_va;
	    *reg_va = 0;	
	    	
            /* Figure out what to do based on the counter type */
	
            int count_type = (tadx_prf.u >> (counter*8)) & 0xff;
            switch (count_type)
            {
                case 0x00:  /* Nothing (disabled) */
                    break;
                case 0x01:  /* L2 Tag Hit */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].tag_hit = value;
                    break;
                case 0x02:  /* L2 Tag Miss */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].tag_miss = value;
                    break;
                case 0x03: /* L2 Tag NoAlloc (forced no-allocate) */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].tag_noalloc = value;
                    break;
                case 0x04: /* L2 Victim */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].victim = value;
                    break;
                case 0x05: /* SC Fail */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].sc_fail = value;
                    break;
                case 0x06: /* SC Pass */
                    /* Value is a simple count, not a utilization. Scale to
                        be cout/sec */
                    value = value * rclk_rate / delta_rclk;
                    tads[tad].sc_pass = value;
                    break;
                case 0x07: /* LFB Occupancy (each cycle adds \# of LFBs valid) */
                    /* There are 16 LFBs, so utilization percentage is
                        100 * count / (RCLK*16) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*16;
                    tads[tad].lfb_occupancy = value1;
                    tads[tad].lfb_occupancy_cent = value2;
                    break;
                case 0x08: /* LFB Wait LFB (each cycle adds \# LFBs waiting for other LFBs) */
                    /* There are 16 LFBs, so utilization percentage is
                        100 * count / (RCLK*16) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*16;

                    tads[tad].lfb_wait_lfb = value1;
                    tads[tad].lfb_wait_lfb_cent = value2;
                    break;
                case 0x09: /* LFB Wait VAB (each cycle adds \# LFBs waiting for VAB) */
                    /* There are 16 LFBs, so utilization percentage is
                        100 * count / (RCLK*16) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*16;

                    tads[tad].lfb_wait_vab = value1;
                    tads[tad].lfb_wait_vab_cent = value2;
                    break;
                case 0x80: /* Quad 0 index bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;


                    tads[tad].quad0_index_bus = value1;
                    tads[tad].quad0_index_bus_cent = value2;
                    break;
                case 0x81: /* Quad 0 read data bus inuse */
                   /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;

                    tads[tad].quad0_read_bus = value1;
                    tads[tad].quad0_read_bus_cent = value2;
                    break;
                case 0x82: /* Quad 0 \# banks inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad0_banks_inuse = value1;
                    tads[tad].quad0_banks_inuse_cent = value2;
                    break;
                case 0x83: /* Quad 0 wdat flops inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad0_wdat_inuse = value1;
                    tads[tad].quad0_wdat_inuse_cent = value2;
                    break;
                case 0x90: /* Quad 1 index bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                   value1 = value;
                   value2 = value;
                   value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                   value2 /= delta_rclk_percent;

                   tads[tad].quad1_index_bus = value1;
                   tads[tad].quad1_index_bus_cent = value2;
                    break;
                case 0x91: /* Quad 1 read data bus inuse */
                   /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;

                    tads[tad].quad1_read_bus = value1;
                    tads[tad].quad1_read_bus_cent = value2;
                    break;
                case 0x92: /* Quad 1 \# banks inuse (0-4/cycle) */
                   /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad1_banks_inuse = value1;
                    tads[tad].quad1_banks_inuse_cent = value2;
                    break;
                case 0x93: /* Quad 1 wdat flops inuse (0-4/cycle) */
                   /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;


                    tads[tad].quad1_wdat_inuse = value1;
                    tads[tad].quad1_wdat_inuse_cent = value2;
                    break;
                case 0xA0: /* Quad 2 index bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                     value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;

                    tads[tad].quad2_index_bus = value1;
                    tads[tad].quad2_index_bus_cent = value2;

                    break;
                case 0xA1: /* Quad 2 read data bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;


                    tads[tad].quad2_read_bus = value1;
                    tads[tad].quad2_read_bus_cent = value2;
                    break;
                case 0xA2: /* Quad 2 \# banks inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad2_banks_inuse = value1;
                    tads[tad].quad2_banks_inuse_cent = value2;
                    break;
                case 0xA3: /* Quad 2 wdat flops inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad2_wdat_inuse = value1;
                    tads[tad].quad2_wdat_inuse_cent = value2;
                    break;
                case 0xB0: /* Quad 3 index bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;

                    tads[tad].quad3_index_bus = value1;
                    tads[tad].quad3_index_bus_cent = value2;
                    break;
                case 0xB1: /* Quad 3 read data bus inuse */
                    /* Count is number of cycle bus in use, so utilization
                        percentage is 100 * count / RCLK */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent;

                    tads[tad].quad3_read_bus = value1;
                    tads[tad].quad3_read_bus_cent = value2;
                    break;
                case 0xB2: /* Quad 3 \# banks inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad3_banks_inuse = value1;
                    tads[tad].quad3_banks_inuse_cent = value2;
                    break;
                case 0xB3: /* Quad 3 wdat flops inuse (0-4/cycle) */
                    /* Count is number of banks bus in use per cycle (there are
                        4 banks), so utilization percentage is 100 * count /
                        (RCLK*4) */
                    value1 = value;
                    value2 = value;
                    value1 = value1 * rclk_rate / (delta_rclk); // not divided by (delta_rclk * 16) because it is not calculating % utilisation 
                    value2 /= delta_rclk_percent*4;

                    tads[tad].quad3_wdat_inuse = value1;
                    tads[tad].quad3_wdat_inuse_cent = value2;
                    break;
            }
        }

        switch (tad_count_set)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                tadx_prf.s.cnt0sel = 0x80 + tad_count_set * 0x10;   /* Quad X index bus inuse */
                tadx_prf.s.cnt1sel = 0x81 + tad_count_set * 0x10;  /* Quad X read data bus inuse */
                tadx_prf.s.cnt2sel = 0x82 + tad_count_set * 0x10;    /* Quad X # banks inuse (0-4/cycle) */
                tadx_prf.s.cnt3sel = 0x83 + tad_count_set * 0x10;   /* Quad X wdat flops inuse (0-4/cycle) */
                break;
            case 4:
                tadx_prf.s.cnt0sel = 0x01; /* L2 Tag Hit */
                tadx_prf.s.cnt1sel = 0x02; /* L2 Tag Miss */
                tadx_prf.s.cnt2sel = 0x03; /* L2 Tag NoAlloc (forced no-allocate) */
                tadx_prf.s.cnt3sel = 0x04; /* L2 Victim */
                break;
            case 5:
                tadx_prf.s.cnt0sel = 0x05; /* SC Fail */
                tadx_prf.s.cnt1sel = 0x06; /* SC Pass */
                tadx_prf.s.cnt2sel = 0x07; /* LFB Occupancy (each cycle adds \# of LFBs valid) */
                tadx_prf.s.cnt3sel = 0x08; /* LFB Wait LFB (each cycle adds \# LFBs waiting for other LFBs) */
                //  0x09; /* LFB Wait VAB (each cycle adds \# LFBs waiting for VAB) */
                break;
         }

        inf_id[0] = tad;
        inf_id[1] = -1;
        reg_pa = bdk_csr_read_by_name("L2C_TADX_PRF", inf_id);
        reg_va = map_reg_pa_to_va(reg_pa, fd);
        *reg_va = tadx_prf.u;
    }

    /* Next update will use a different set of counters */
    tad_count_set++;
    if (tad_count_set > 5)
        tad_count_set = 0;

    close_devmem(fd);
}

void print_l2stats()
{
        int i;
        printf("Printing l2c statistics.......\n");
        printf("\n*********************************************************************************************************************************************\n\t");
         printf(" |%-21s  |%-21s  |%-21s  |%-21s  |%-17s \n","add     (%) ","str     (%) ","commit  (%) ","fill     (%) ","inv ");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        for(i=0;i<10;i++)
        {
                if(i<8)
                {
                        printf("cmb%d\t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t |%-10ld",cmb[i].cmbnum, cmb[i].add, cmb[i].add_cent, cmb[i].store, cmb[i].store_cent, cmb[i].commit, cmb[i].commit_cent, cmb[i].fill, cmb[i].fill_cent, cmb[i].inv);
                        printf("\n");
                }
                else
                {
                        printf("iobn%d\t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t |%-7ld (%-3ld) \t",cmb[i].cmbnum-8, cmb[i].add, cmb[i].add_cent, cmb[i].store, cmb[i].store_cent, cmb[i].commit, cmb[i].commit_cent, cmb[i].fill, cmb[i].fill_cent);
                        printf("\n");
                }


        }
        printf("\n");

        printf("**************************************************************************************************************************************************** \n");
        printf("\t |%-21s  |%-17s \n","ioc     (%) ","ior     (%)");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
        for(i=0;i<4;i++)
        {
                 printf("io%d\t |%-7ld (%-3ld) \t |%-7ld (%-3ld)",io[i].ionum, io[i].ioc, io[i].ioc_cent, io[i].ior, io[i].ior_cent);
                 printf("\n");
        }


        printf("************************************************************************************************************************************************ \n");
        printf("%-20s"," ");
        for(i=0;i<8;i++)
        {
                printf("|tad%-12ld",tads[i].tadnum);
        }
        printf("\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%-20s","tag_hit (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].tag_hit);
        }
        printf("\n");

        printf("%-20s","tag_miss (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].tag_miss);
        }
        printf("\n");

        printf("%-20s","tag_noalloc (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].tag_noalloc);
        }
        printf("\n");

        printf("%-20s","victim (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].victim);
        }
        printf("\n");

        printf("%-20s","sc_fail (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].sc_fail);
        }
        printf("\n");

        printf("%-20s","sc_pass (num/s)");
        for(i=0;i<8;i++)
        {
                printf("|%-15ld",tads[i].sc_pass);
        }
        printf("\n");

        printf("%-20s","lfb_occ (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].lfb_occupancy, tads[i].lfb_occupancy_cent);
        }
        printf("\n");

        printf("%-20s","lfb_wait_lfb (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].lfb_wait_lfb, tads[i].lfb_wait_lfb_cent);
        }
        printf("\n");

        printf("%-20s","lfb_wait_vab (%)");

        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].lfb_wait_vab, tads[i].lfb_wait_vab_cent);
        }
        printf("\n");

        printf("%-20s","q0_index (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad0_index_bus, tads[i].quad0_index_bus_cent);
        }
        printf("\n");

        printf("%-20s","q0_read (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad0_read_bus, tads[i].quad0_read_bus_cent);
        }
        printf("\n");

        printf("%-20s","q0_banks_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad0_banks_inuse, tads[i].quad0_banks_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q0_wdat_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad0_wdat_inuse, tads[i].quad0_wdat_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q1_index (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad1_index_bus, tads[i].quad1_index_bus_cent);
        }
        printf("\n");

        printf("%-20s","q1_read (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad1_read_bus, tads[i].quad1_read_bus_cent);
        }
        printf("\n");

        printf("%-20s","q1_banks_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad1_banks_inuse, tads[i].quad1_banks_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q1_wdat_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad1_wdat_inuse, tads[i].quad1_wdat_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q2_index (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad2_index_bus, tads[i].quad2_index_bus_cent);
        }
        printf("\n");

        printf("%-20s","q2_read (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad2_read_bus,tads[i].quad2_read_bus_cent);
        }
        printf("\n");

        printf("%-20s","q2_banks_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad2_banks_inuse, tads[i].quad2_banks_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q2_wdat_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad2_wdat_inuse, tads[i].quad2_wdat_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q3_index (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad3_index_bus, tads[i].quad3_index_bus_cent);
        }
        printf("\n");

        printf("%-20s","q3_read (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad3_read_bus, tads[i].quad3_read_bus_cent);
        }
        printf("\n");

        printf("%-20s","q3_banks_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad3_banks_inuse, tads[i].quad3_banks_inuse_cent);
        }
        printf("\n");

        printf("%-20s","q3_wdat_inuse (%)");
        for(i=0;i<8;i++)
        {
                printf("|%-10ld (%-2ld)",tads[i].quad3_wdat_inuse, tads[i].quad3_wdat_inuse_cent);
        }
        printf("\n");


}


static inline void
update_multiplier(void)
{
        int fd;
        uint64_t *reg_va;
        uint64_t reg_pa;
        bdk_rst_boot_t rst_boot;
        int inf_id[BDK_CSR_DB_MAX_PARAM]={-1, -1, -1, -1};	
	
	fd = open_devmem();
	reg_pa =bdk_csr_read_by_name("RST_BOOT",inf_id);
	reg_va = map_reg_pa_to_va(reg_pa, fd);
	rst_boot.u = *reg_va;
        g_mul = rst_boot.s.c_mul/2;
	close_devmem(fd);
}

void sigint(int a)
{
        //csr_rw_deinit();
        exit(1);
}


/*
 * Function to display l2stats 
 */
static int l2stats() {
        bdk_node_t node = BDK_NODE_0;
        update_multiplier();
        signal(SIGINT, sigint);	

        while(1)
        {
                system("clear");


                l2_perf_cmb(node);
                l2_perf_io(node);
                l2_perf_tad(node);

                print_l2stats();
                sleep(1);

        }
        return 0;
}
/*
 * Function to dump physical memory content

 * INPUT : Physical memory address from where to dump the memory content
*/
static int do_dram_read (uint64_t reg_pa, uint64_t length){

        uint64_t i, fd;
	int c;
	uint64_t *reg_va;
	debug_print("\nControl entered into dram read function\n");
        while (length > 0) {
		fd = open_devmem();
		reg_va = map_reg_pa_to_va(reg_pa, fd);
		printf("%llx\t", (unsigned long long)reg_pa);
		for (i = 0;  i < 16;  i++) {
			if (i < length) {
				printf("%02X ", (unsigned)(reg_va[i] & 0xFF));
			}
			else {
				printf("   ");
			}
			if (i == 7) printf(" ");
		}
		printf(" |");
		for (i = 0;  i < 16;  i++) {
			if (i < length) {
				c = (int)(reg_va[i] & 0xFF);
				if ((c < 0x20) || (c >= 0x7F)) c = '.';
			}
			else {
				c = ' ';
			}
			printf("%c", c);
		}
		printf("|\n");
		length -= 16;
		reg_pa += 16;
		close_devmem(fd);
        }
	return 0;
}

/*
 * Reads register value if user enters register hardware address.

 * INPUT :  reg_pa   : Register hardware address
	    node_id  : In case of dual socket board, specifies socket number
	    inf_id[] : Offset to be used.(Alpha, beta, Gamma,Chi value)
	    decode   : If set,register output is decoded into its components and their values

 * OUTPUT : Prints register value with/without decode
*/
static int do_read_reg_hex (uint64_t reg_pa, int node_id, int inf_id[], int decode, int size) {

	int fd;
	volatile uint64_t *reg_va;
	uint64_t reg_val;
	char reg_name[64];

	fd = open_devmem();

	debug_print("\nInside do_read_reg_hex function.\n");

	reg_va = map_reg_pa_to_va(reg_pa, fd);

	debug_print("\nAfter map_reg_pa_to_va function pa address : 0x%llx va address : 0x%llx next address is : 0x%llx\n",
		    (unsigned long long)reg_pa, (unsigned long long)reg_va, (unsigned long long)(reg_va + 1));

	if (size == 8) {
		reg_val = *reg_va;
		info("\nValue of [0x%016llx] :[0x%016llx]\n", (unsigned long long)reg_pa, (unsigned long long)reg_val);
		debug_print("Value of [0x%016llx] = 0x%016llx\n", (unsigned long long)reg_pa, (unsigned long long)reg_val);
	} else if (size == 4) {
		volatile uint32_t *va_32bit = (volatile uint32_t *)reg_va;
		reg_val = *va_32bit;
		info("\nValue of [0x%016llx] :[0x%08x]\n", (unsigned long long)reg_pa, (unsigned)reg_val);
		debug_print("Value of [0x%016llx] = 0x%08x\n", (unsigned long long)reg_pa, (unsigned)reg_val);
	}

	if(decode)
		 bdk_csr_decode(reg_name, reg_val, inf_id);

	close_devmem(fd);
	return 0;
}


/*
 * Reads register value if user enters register name.

 * INPUT :  reg_name : Register name
	    node_id  : In case of dual socket board, specifies socket number
	    inf_id[] : Offset to be used.(Alpha, beta, Gamma,Chi value)
	    decode   : If set,register output is decoded into its components and their values

 * OUTPUT : Prints register value with/without decode
*/
static int do_read_reg_name (char *reg_name, int node_id, int inf_id[], int decode) {

	int fd;
	void *reg_va;
	uint64_t reg_value;
	uint64_t reg_pa;
	const __bdk_csr_db_type_t *db;

	fd = open_devmem();

	debug_print("\nInside function do_read_reg_name\n");
	if((reg_name[0] == 'A' && reg_name[1] == 'P' && reg_name[2] == '_') ||
		(reg_name[0] == 'a' && reg_name[1] == 'p' && reg_name[2] == '_')) {
		if (processor_num >=0 && processor_num <=47)	
			reg_pa = bdk_sysreg_read(reg_name, inf_id, processor_num);
		else {
			error("Incorrect core number entered. Please enter core number in range (0..47)\n\n");
			exit(1);
		}
	}
	else
		reg_pa = bdk_csr_read_by_name(reg_name, inf_id);

	db = __bdk_csr_lookup(reg_name, inf_id);
	if (!db) {
		error("ERROR: __bdk_csr_lookup failed\n");
		exit(1);
	}

	debug_print("\nPhysical address of %s : 0x%016llx\n", reg_name, (unsigned long long)reg_pa);

	if (node_id) {
		reg_pa = bdk_numa_get_address(node_id, reg_pa);
	}

	reg_va = map_reg_pa_to_va(reg_pa, fd);
	if (db->width == 8) {
		reg_value = *(volatile uint64_t *)reg_va;
		info("\nValue of %s [0x%016llx] :[0x%016llx]\n", reg_name,
		     (unsigned long long)reg_pa, (unsigned long long)reg_value);
	} else if (db->width == 4) {
		reg_value = *(volatile uint32_t*)reg_va;
		info("\nValue of %s [0x%016llx] :[0x%08llx]\n", reg_name,
		     (unsigned long long)reg_pa, (unsigned long long)reg_value);
	} else {
		error("ERROR: Unsupported width: %d\n", (int)db->width);
		exit(1);
	}
	
	debug_print("Value of [0x%016llx] = 0x%016llx\n",
		    (unsigned long long)reg_pa, (unsigned long long)reg_value);

	if (decode)
		 bdk_csr_decode(reg_name, reg_value, inf_id);

	close_devmem(fd);
	return 0;
}


/*
 * Writes register value if user enters register hardware address.

 * INPUT :  reg_pa   : Register hardware address
	    node_id  : In case of dual socket board, specifies socket number
	    reg_value: Value to be written to register

 * OUTPUT : Writes to register value given by user
*/
static int do_write_reg_hex (uint64_t reg_pa, int node_id, uint64_t reg_value, int size){

        int fd;
        volatile uint64_t *reg_va;

        fd = open_devmem();

        reg_va = map_reg_pa_to_va(reg_pa, fd);

	if (size == 8) {
		*reg_va = reg_value;
	} else if (size == 4) {
		volatile uint32_t *va_32bit = (volatile uint32_t *)reg_va;
		*va_32bit = (uint32_t)reg_value;
	}
		

        close_devmem(fd);
        return 0;
}

/*
 * Writes register value if user enters register name.

 * INPUT :  reg_pa   : Register hardware address
	    node_id  : In case of dual socket board, specifies socket number
	    inf_id[] : Offset to be used.(Alpha, beta, Gamma,Chi value)
	    reg_value: Value to be written to register

 * OUTPUT : Writes to register value given by user
*/
static int do_write_reg_name (char *reg_name, int node_id, int inf_id[], uint64_t reg_value ) {

        int fd;
        void *reg_va;
        uint64_t reg_pa;
	const __bdk_csr_db_type_t *db;

        fd =open_devmem();

        if((reg_name[0] == 'A' && reg_name[1] == 'P' && reg_name[2] == '_') ||
			(reg_name[0] == 'a' && reg_name[1] == 'p' && reg_name[2] == '_')) {
                if (processor_num >=0 && processor_num <=47)    
                        reg_pa = bdk_sysreg_read(reg_name, inf_id, processor_num);
                else {
                        error("Incorrect core number entered. Please enter core number in range (0..47)\n\n");
                        exit(1);
                }       
        }
        else
                reg_pa = bdk_csr_read_by_name(reg_name,inf_id);

	db = __bdk_csr_lookup(reg_name, inf_id);
	if (!db) {
		error("ERROR: __bdk_csr_lookup failed\n");
		exit(1);
	}

        if (node_id) {
                reg_pa = bdk_numa_get_address(node_id, reg_pa);
        }

        reg_va = map_reg_pa_to_va(reg_pa, fd);
        reg_addr_to_name(reg_pa, reg_name);

	if (db->width == 8) {
		*(volatile uint64_t *)reg_va = reg_value;
	} else if (db->width == 4) {
		*(volatile uint32_t*)reg_va = (uint32_t)reg_value;
	} else {
		error("ERROR: Unsupported width: %d\n", (int)db->width);
		exit(1);
	}

        close_devmem(fd);
        return 0;
}
int do_spl_functions (int argc, char **argv)
{
	if (argc < 2){
		usage();
		return 0;
	}

	if (!strcmp(argv[1], "--version")) {
		if (argc != 2)
			usage();
		info("Version: %s\n", TOOL_VERSION);
		exit(0);
	}

	if (!strcmp(argv[1], "--cpufreq")) {
		if (argc != 2)
			usage();
		debug_print("\nSelected option : CPU frequency\n");
		print_cpufreq();
		exit(0);
	}

	if (!strcmp(argv[1], "--listreg")) {
		if (argc != 3)
			usage();
		debug_print("Selected option : to list registers of block-name: %s\n", argv[2]);
		name_suggestion(argv[2]);
		exit(0);
	}

	if (!strcmp(argv[1], "--listregd0")) {
		if (argc != 3)
			usage();
		debug_print("Selected option : to list details of registers of block-name: %s\n", argv[2]);
		name_suggestion_for_details(argv[0], argv[2], 0);
		exit(0);
	}
	if (!strcmp(argv[1], "--listregd1")) {
		if (argc != 3)
			usage();
		debug_print("Selected option : to list details of registers of block-name: %s\n", argv[2]);
		name_suggestion_for_details(argv[0], argv[2], 1);
		exit(0);
	}
	if (!strcmp(argv[1], "--listregd2")) {
		if (argc != 3)
			usage();
		debug_print("Selected option : to list details of registers of block-name: %s\n", argv[2]);
		name_suggestion_for_details(argv[0], argv[2], 2);
		exit(0);
	}	
	
	if (!strcmp(argv[1], "--help")) {
		usage();
		exit(0);
	}

        if (!strcmp(argv[1], "--l2stats")) {
                if (argc != 2)
                        usage();
                debug_print("\nSelected option : l2stats\n");
                l2stats();
                exit(0);
        }
	
	return 0;
}

int main (int argc, char **argv) {

	int i;
	int decode = 0;
	int node_id = 0;
	int inf_id[BDK_CSR_DB_MAX_PARAM] = {-1, -1, -1, -1};
	int access_size = 8;
	uint64_t reg_pa;
	uint64_t reg_value;
	uint64_t arg_value;
	char *reg_name;
	bool do_write_reg, use_name, use_addr;


	signal(SIGBUS, sig_handler);
	signal(SIGSEGV, sig_handler);
	do_spl_functions(argc, argv);

	while ((i = getopt(argc, argv, "a:b:g:c:n:p:s:hvd")) != -1) {

		switch (i) {
                case 'a':
                        inf_id[0] = (int)strtol(optarg, NULL, 0);
                        debug_print("Alpha value entered : %d\n", inf_id[0]);
                        break;

                case 'b':
                        inf_id[1] = (int)strtol(optarg, NULL, 0);
                        debug_print("Beta value entered :  %d\n", inf_id[1]);
                        break;

                case 'g':
                        inf_id[2] = (int)strtol(optarg, NULL, 0);
                        debug_print("Gamma value entered : %d\n", inf_id[2]);
                        break;

                case 'c':
                        inf_id[3] = (int)strtol(optarg, NULL, 0);
                        debug_print("Chi value entered : %d\n", inf_id[3]);
                        break;

		case 'n':
			node_id = (int)strtol(optarg, NULL, 0);
			debug_print("Socket number selected : %d\n", node_id);
			break;

		case 'p':
			debug_print("\nProcessor Number for system registers\n");
			processor_num = strtol(optarg, NULL, 10);
			break;

		case 's':
			access_size = (int)strtol(optarg, NULL, 0);
			if (access_size != 8 && access_size != 4) {
				error("Invalid access size: %d\n", access_size);
				exit(1);
			}
			debug_print("\nAccess size for non-symbolic registers: %d-bytes\n", access_size);
			break;

		case 'h':
			debug_print("Option selected : help \n");
			usage();
			break;

		case 'v':
			verbose = 1;
			debug_print("Debug operation selected \n");
			break;

		case 'd':
			debug_print("Decode operation selected \n");
			decode = 1;
			break;

		default:
			error("\nInavalid option, refer to usage mentioned below :\n");
			debug_print("Defult option %s\n", optarg);
			usage();
			break;
		}
	}

	do_write_reg = false;
	use_name = false;
	use_addr = false;

	for (i = optind; i < argc; i++) {
		arg_value = (uint64_t)strtoul(argv[i], NULL, 0);
		if (!arg_value && !(use_name || use_addr)) {
			debug_print("\nRegister name entered is :  %s\n", argv[i]);
			use_name = true;
			reg_name = strdup(argv[i]);
		}
		else {
			 debug_print("\nHex value entered is : 0x%lx\n", arg_value);
			 if (use_name || use_addr) {
				 reg_value = arg_value;
				 do_write_reg = true;
				 debug_print("\nRegister value entered ! 0x%lx\n", reg_value);
			 }
			else {
				use_addr = true;
				reg_pa = arg_value;
				debug_print("\nRegister address entered ! 0x%lx\n", reg_pa);
				if (!((reg_pa >> 47) & 0x1)) {
					long l;
					debug_print("\nPhysical memory dump option selected !\n");
					l = strtoul(argv[++i], NULL, 0);
					debug_print("\nLength of bytes to read : %ld\n", l);
					do_dram_read(reg_pa, l);
					exit(1);
				}
			}
		}
	}

	if (do_write_reg) {
		if (use_name) {
			debug_print("cmd READ_REG_HEX\n");
			do_write_reg_name(reg_name, node_id, inf_id, reg_value);
		} else {
			debug_print("cmd READ_REG_STRING\n");
			do_write_reg_hex(reg_pa, node_id, reg_value, access_size);
		}
	} else {
		if (use_name) {
			debug_print("cmd READ_REG_name \n");
			do_read_reg_name(reg_name, node_id, inf_id, decode);
		} else {
			debug_print("cmd READ_REG_HEX\n");
			do_read_reg_hex(reg_pa, node_id, inf_id, decode, access_size);
		}
	}

	exit(0);
}
