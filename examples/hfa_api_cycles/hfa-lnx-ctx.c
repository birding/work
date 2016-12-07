/***********************license start***************                              
* Copyright (c) 2008-2016 Cavium Inc. All rights reserved.
*                                                                                 
*                                                                                 
* Redistribution and use in source and binary forms, with or without              
* modification, are permitted provided that the following conditions are          
* met:                                                                            
                                                                                  
*   * Redistributions of source code must retain the above copyright              
*     notice, this list of conditions and the following disclaimer.               
*                                                                                 
*   * Redistributions in binary form must reproduce the above                     
*     copyright notice, this list of conditions and the following                 
*     disclaimer in the documentation and/or other materials provided             
*     with the distribution.                                                      
*                                                                                 
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
 * Linux Kernel mode:
 * Performance benchmarking application to illustrate HFA Engine performance.
 * It uses asynchronous mode of operation using poll mode. It does not report 
 * the pattern-matches found in the payload. Each core uses a single searchctx.
 * Once a packet is received, search parameter is created and submitted to HFA
 * engine on core's local searchctx. After search submission, parameter is 
 * added to the tail of searchctx param list. Each core then tries to poll 
 * search parameter at the head of searchctx param list. If the search 
 * corresponding to search paramater at head is completed, post processing is 
 * done and total matches are incremented. The application, therefore, does not
 * use any locks. This application supports searching of live network traffic.
 * This requires the Cavium OCTEON Ethernet driver, which should be configured
 * to use 60000 buffers(using num_packet_buffers module parameter). The ingress
 * ethernet interface which receives the network traffic may need to be set to
 * promiscuous mode to ensure that the packets are picked up by the OCTEON
 * Ethernet driver.
 *
 * The following lists the operational aspects of this application.
 * - Multicore - YES
 * - Type of API
 *       - Asynchronous OO API(search)
 *       - Synchronous OO API(memload and cacheload)
 * - Cluster resources - Managed by HFA API.
 * - Clusters shared among cores - YES
 * - Graph count - 1 (loaded by first core)
 * - Graph to cluster map - one graph on all clusters
 * - Nature of ctx - private to each core.
 * - Locks - NONE
 * - Clusters for load - all by default. Configurable using cmdline option
 * - Clusters for search - all by default. Performs search on cluster which is 
 *                         available(HW Load Balancing).
 * - Buffer Payload - Supported. File is converted to local packets(WQE) and 
 *                    submitted to OCTEON SSO hardware with WQE grp @b
 *                    hfa_pow_receive_group (@ref octeon_hfa_sdk_kernel_module_op)
 *                    These packets are picked by HFA SDK kernel module and
 *                    passed on to the application via process_pkt() thus
 *                    emulating the network traffic.
 * - Pcap Payload - Supported. Actual data from each packet in PCAP file is 
 *                  fetched and submitted locally as WQE to OCTEON SSO hardware 
 *                  with WQE grp @b hfa_pow_receive_group (@ref octeon_hfa_sdk_kernel_module_op)
 *                  These packets are picked by HFA SDK kernel module and passed
 *                  on to the application via process_pkt() thus emulating the
 *                  network traffic.
 * - Network traffic - Supported. Incoming packets are received from OCTEON Ethernet driver
 * - Cross Packet Search - Disabled
 * - FSAVEBUF - Not Supported
 */

#include <pcap.h>
#include <app-utils.h>

#define RBUFSIZE                2048
#define HFA_MAX_ALLOWED_PARAMS  25

extern int                      ibuf_aura;
extern int                      wqe_aura; 
extern int                      pko_aura; 
extern int                      ibuff_pool; 
extern int                      work_pool;                   
extern int                      pki_blk_sz;
extern int                      hfa_pow_rcv_grp[NR_CPUS];
/*Module parameters*/

static char     *graph = "hfa.out";
module_param (graph, charp, 0444);
MODULE_PARM_DESC (graph, "graph file");

static char     *payload = "payload";
module_param (payload, charp, 0444);
MODULE_PARM_DESC (payload, "payload file");

static unsigned int chunksize = 65535;
module_param (chunksize, uint, 0444);
MODULE_PARM_DESC (chunksize, "chunk size");

static unsigned int npkts= 200;
module_param (npkts, uint, 0444);
MODULE_PARM_DESC (npkts, "Number of pkts");

static int searchclmsk = 0x0;
module_param (searchclmsk, int, 0444);
MODULE_PARM_DESC (searchclmsk, "Cluster mask on which search will happen");

static unsigned int clmsk= 0;
module_param (clmsk, uint, 0444);
MODULE_PARM_DESC (clmsk, "Cluster Bitmask on which graph is loaded");

static int  matchacross=0;
module_param (matchacross, int, 0444);
MODULE_PARM_DESC (matchacross, "flags to enable or disable cross pkt search");

static int  singlematch=1;
module_param (singlematch, int, 0444);
MODULE_PARM_DESC (singlematch, "flags to enable or disable singlematch");

unsigned int network=1;
module_param (network, uint, 0444);
MODULE_PARM_DESC (network, "If network payload used");

static char    *port[20] = {"xaui0","xaui1","eth2","eth3","eth4","eth5"};
static int     count = 0;
module_param_array (port, charp, &count, 0444);
MODULE_PARM_DESC (port, "Packet incoming/ingress interfaces, On gen4 targets xaui* ports are known as eth*");

static int  pcap=0;
module_param (pcap, int, 0444);
MODULE_PARM_DESC (pcap, "payload file is pcap file");

unsigned int nctx=1;
module_param (nctx, uint, 0444);
MODULE_PARM_DESC (nctx, "number of searchctx");

int ttl = -1;
module_param (ttl, int, 0444);
MODULE_PARM_DESC (ttl, "Time to live (lifetime) in seconds, application will \
stop resubmission in network=0 case, \
module has to be removed manually.");

static int      verbose = 0;
module_param (verbose, int, 0444);
MODULE_PARM_DESC (verbose, "verbose option to print matches");

atomic64_t *buffers_needed = NULL;
/**
 * stop: "Option to stop kernel module effectively For network=0,"
 */
static atomic_t      stop = {0}, cleaned_skb_cnt={0};

#define HFA_SPARAM_FPA_POOL         -1
#define HFA_SPARAM_FPA_POOL_SIZE    paramsz
        
#define HFA_RPTR_FPA_POOL           -1
#define HFA_RPTR_FPA_POOL_SIZE      rptrsz
CVMX_SHARED int                         rbuf_pool = -1;                    
CVMX_SHARED int                         rbuf_aura = -1;                    
CVMX_SHARED int                         sparam_pool = -1;                    
CVMX_SHARED int                         sparam_aura = -1;
/**
 * OCTEON MBOX messaging variable
 */
static int                          ipi_handle_mesg;

/*Application related structures*/
typedef hfa_return_t (*matchcb_t)(hfa_searchctx_t *, uint64_t *);
typedef hfa_return_t (*freecb_t)(void *, int);

void  core_exit(void);
/*Ethernet driver IBUFPOOL configuration*/
extern int                          hfa_pow_receive_group;
extern int                          hfa_distribute_load;
extern int                          hfa_napi_perf;

typedef enum {
    WQE_DROP = 0,
    WQE_SUCCESS = 1,
    WQE_CLEANUP = 2
}wqe_status_t;

typedef enum {
    TRYAGAIN = 0,
    ADDED = 1,
    SUBMITTED =2,
    PROCESSED
}paramstatus_t;

/*Search Context*/
typedef struct {
    hfautils_lock_t         lock;
    hfautils_listhead_t     paramlist;
    hfa_searchctx_t         sctx;
    matchcb_t               matchcb;
    uint32_t                added;
    uint32_t                submitted;
}searchctx_t;

/*Search Parameters*/
typedef struct{
    hfautils_listhead_t     list;
    hfa_searchparams_t      sparam;
    paramstatus_t           status;
    cvmx_wqe_t              *pktwqe;
    void                    *rptr;
    hfa_iovec_t             input;
    freecb_t                cb;
    void                    *cba;
}searchparam_t;

/*Global and static variables*/
static struct net_device *input_device[20] = {NULL};

CVMX_SHARED hfa_dev_t                   dev;
CVMX_SHARED hfa_graph_t                 _graph;
CVMX_SHARED hfautils_fau_perfcntrs_t    stats;
CVMX_SHARED void                        *gbuf = NULL;
CVMX_SHARED hfa_size_t                  gsize=0, psize=0;
CVMX_SHARED options_t                   options;
CVMX_SHARED uint32_t                    paramsz=0, rptrsz =0;
CVMX_SHARED uint32_t                    ncores, bufcnt;
struct timer_list                       *poll_timer,timeout;

hfautils_payload_attr_t                 pattr;
char                                    *buf = NULL;
#ifdef USE_TIMER_FOR_STATS    
static struct timer_list                printresult_timer;
#endif
int                                     clno=0;
/**One context per core*/
CVMX_SHARED searchctx_t                *gctx[NR_CPUS];
atomic_t                                exitflag = ATOMIC_INIT(0);

cvm_oct_callback_result_t 
process_pkt(struct net_device *, void *, struct sk_buff *);
static inline hfa_return_t poll(searchctx_t *pctx);

static struct sk_buff *octeon3_eth_work_to_skb(void *w)
{
    struct sk_buff *skb;
    void **f = w;
    skb = f[-16];
    return skb;
}
static inline void 
freepktwqe(cvmx_wqe_t *wqe)
{
    if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE) && network) {
        struct sk_buff *skb;
        skb = octeon3_eth_work_to_skb(wqe);
        dev_kfree_skb_any(skb);
    }
    else {
        if(hfautils_likely(wqe)){
            if(hfa_is_gen4target()) {
                    atomic_inc(&cleaned_skb_cnt);
                    dev_kfree_skb_any(hfa_octeon3_eth_work_to_skb(wqe));
            }
            else {
                hfa_napi_free_work(wqe, wqe_aura, ibuf_aura);
            }
        }
    }
}

static inline void 
cleanup_pendwqe(void)
{
    cvmx_wqe_t                  *wqe = NULL;
    if(!network){
        while(HFAUTILS_FAU_FETCH_SHARED(stats, pend_wqe, 0ULL)){
            if(!cvmx_sso_get_total_wqe_count()){
                break;
            }
            wqe = cvmx_pow_work_request_sync(CVMX_POW_WAIT);
            if(wqe == NULL)
                continue;
            //cvmx_helper_free_pki_pkt_data(wqe);
            freepktwqe(wqe);
            HFAUTILS_FAU_DEC_SHARED(stats, pend_wqe);
        }
    }
}

static inline void 
send (cvmx_wqe_t *wqe)
{
    if(wqe){
#ifdef LOOPBACK
        hfautils_send_pkt(wqe);
#else                        
        freepktwqe(wqe);
#endif            
    }
}
static inline void 
resubmit(cvmx_wqe_t *wqe)
{
    if(hfautils_likely(wqe)){
        if(likely(!atomic_read(&stop))){
            if(likely(buffers_needed))
                atomic64_dec_unchecked(buffers_needed);
            HFAUTILS_FAU_INC_SHARED(stats, pend_wqe);
            hfa_pow_work_submit(wqe, cvmx_wqe_get_tag(wqe), cvmx_wqe_get_tt(wqe), 
                            cvmx_wqe_get_qos(wqe), hfa_wqe_get_grp(wqe));
        }else{
            freepktwqe(wqe);
        }
    }
}

static inline void 
sendpktwqe(cvmx_wqe_t *wqe, int wqe_status)
{
    if(network){
        switch(wqe_status) {
            case WQE_DROP:
                freepktwqe(wqe);
                HFAUTILS_FAU_INC(stats, dropped);
            break;
        
            case WQE_SUCCESS:
                HFAUTILS_FAU_INCBY(stats, tot_bytes, cvmx_wqe_get_len(wqe));
                send(wqe);
                HFAUTILS_FAU_INC(stats, out);
            break;
         
            case WQE_CLEANUP:
                send(wqe);
            break;
        
            default:
                /* Do nothing */
            break;
        }
    } else {
        switch(wqe_status) {
            case WQE_DROP:
                resubmit(wqe);
                HFAUTILS_FAU_INC(stats, dropped);
            break;
        
            case WQE_SUCCESS:
                HFAUTILS_FAU_INCBY(stats, tot_bytes, cvmx_wqe_get_len(wqe));
                HFAUTILS_FAU_INC(stats, out);
                resubmit(wqe);
            break;
        
            case WQE_CLEANUP:
                freepktwqe(wqe);
            break;

            default:
            break;
        }
    }
}
static inline hfa_return_t 
freeparam(void *cba, int wqe_status)
{
    searchparam_t   *param = (searchparam_t *)cba;

    if(hfautils_likely(param)){
        sendpktwqe((cvmx_wqe_t *)param->pktwqe, wqe_status);

        if(param->rptr){
            hfa_fpa_free(param->rptr, rbuf_aura, 0);
            param->rptr =0;
        }
        hfa_fpa_free(param, sparam_aura, 0);
        return HFA_SUCCESS;
    }
    return HFA_FAILURE;
}

/*static inline functions*/
static inline hfa_return_t
createctx (searchctx_t **ppctx, uint64_t searchflags, matchcb_t cb)
{
    searchctx_t     *pctx = NULL;

    if(hfautils_likely(ppctx)){
        if(hfautils_unlikely(NULL == (pctx = 
            hfautils_memoryalloc(sizeof(searchctx_t), 8)))){
            ERR("Memory allocation failed for pctx\n");
            return HFA_FAILURE;
        }
        memset(pctx, 0, sizeof(searchctx_t));
        hfautils_lockinit(&pctx->lock);
        HFA_OS_LISTHEAD_INIT(&pctx->paramlist);
        if(HFA_SUCCESS != hfa_dev_searchctx_init(&dev, &pctx->sctx)){
            ERR("error from searchctx_init\n");
            goto free_ctx;
        }
        if(HFA_SUCCESS != hfa_searchctx_setgraph(&pctx->sctx, &_graph)){
            ERR("setgraph failure\n");
            goto free_ctx;
        }
        hfa_searchctx_setflags(&pctx->sctx, searchflags);
        *ppctx = pctx;

        return HFA_SUCCESS;
free_ctx:
        hfautils_memoryfree(pctx, sizeof(searchctx_t));
    }
    return HFA_FAILURE;
}
static inline hfa_return_t
destroyctx(searchctx_t *pctx)
{
    hfautils_listhead_t *p1 = NULL, *p2 = NULL;
    searchparam_t       *param = NULL;
    uint32_t            searchstatus = 0;

    if(hfautils_likely(pctx)){
        hfautils_listforeachsafe(p1, p2, &pctx->paramlist){
            param = hfautils_listentry(p1, searchparam_t, list);
            
            /*remove from the list*/
            hfautils_listdel(&param->list);
            
            /*Free param list. If parameters are submitted 
             * poll for them first and then free*/
            switch(param->status){
                case ADDED:
                case TRYAGAIN:
                case PROCESSED:
                    freeparam(param, WQE_CLEANUP);
                break;

                case SUBMITTED:
                    searchstatus = HFA_SEARCH_SDONE;
                    do{
                        hfa_searchctx_get_searchstatus(&pctx->sctx, 
                                          &param->sparam, &searchstatus);
                    }while(HFA_SEARCH_SEAGAIN == searchstatus);
                    freeparam(param, WQE_CLEANUP);
                break;

                default:
                    /*Do Nothing*/
                break;
            }
        }
        hfa_dev_searchctx_cleanup(&dev, &pctx->sctx);
        hfautils_lockdestroy(&pctx->sctx);
        hfautils_memoryfree(pctx, sizeof(searchctx_t));
        return HFA_SUCCESS;
    }
    return HFA_FAILURE;
}
static inline hfa_return_t
addsearchparam(searchctx_t *pctx,searchparam_t *psparam,freecb_t cb,void *cba){

    if(hfautils_unlikely((NULL == psparam) || (NULL == pctx))){
        DBG("pctx: %p, pspram: %p\n", pctx, psparam);
        return HFA_FAILURE;
    }
    psparam->status = TRYAGAIN;
    
    if((pctx->added + pctx->submitted) >= HFA_MAX_ALLOWED_PARAMS){
        return HFA_FAILURE;
    }
    hfautils_listaddtail(&psparam->list, &pctx->paramlist);
    (pctx->added) += 1;
    psparam->status = ADDED;
    psparam->cb = cb;
    psparam->cba = cba;

    return HFA_SUCCESS;
}
static inline hfa_return_t
submit(searchctx_t *pctx, paramstatus_t *pstatus)
{
    hfautils_listhead_t     *p1 = NULL, *p2 = NULL;
    searchparam_t           *psparam = NULL;

    if(hfautils_likely(pctx && pstatus)){
        *pstatus = TRYAGAIN;
        if(pctx->added){
            hfautils_listforeachsafe(p1, p2, &pctx->paramlist){
                psparam = hfautils_listentry(p1, searchparam_t, list);
            
                switch(psparam->status){
                    case ADDED:
                        /*Submit the ctx*/
                        if(HFA_SUCCESS==hfa_searchctx_search_async(&pctx->sctx,
                                         &psparam->sparam)){
                            HFAUTILS_FAU_INC(stats, sdsuccess); 
                            psparam->status = SUBMITTED;
                            (pctx->added)--;
                            (pctx->submitted)++;
                            return HFA_SUCCESS;
                        } else {
                            HFAUTILS_FAU_INC(stats, sdfail);
                        }
                    break;

                    default:
                    break;
                }
            }
        }
        return HFA_SUCCESS;
    }
    return HFA_FAILURE;
}
static inline hfa_return_t
poll_searchctx(searchctx_t *pctx, paramstatus_t *pstatus)
{
    hfautils_listhead_t     *p1 = NULL, *p2 = NULL;
    uint64_t                *pmatch = NULL;           
    hfa_reason_t            reason=0;
    searchparam_t           *psparam = NULL;
    hfa_searchstatus_t      status;

    if(hfautils_likely(pctx && pstatus)){
        *pstatus = TRYAGAIN;

        hfautils_listforeachsafe(p1, p2, &pctx->paramlist){
            psparam = hfautils_listentry(p1, searchparam_t, list);

            switch(psparam->status){
                case SUBMITTED:
                    status = HFA_SEARCH_SDONE;
                    if(HFA_SUCCESS == hfa_searchctx_get_searchstatus(&pctx->sctx,
                                &psparam->sparam, &status)){
                        if(HFA_SEARCH_SEAGAIN == status){
                            continue;
                        }
                        *pstatus = PROCESSED;

                        /*Remove param from paramlist*/
                        hfautils_listdel(&psparam->list);

                        /*Update flow counters*/
                        (pctx->submitted)--;

                        /*Get HW reason and matches*/
                        hfa_searchparam_get_hwsearch_reason(&psparam->sparam,
                                &reason);
                        DBG("reason: %u\n", reason);
                        switch(reason) {
                            case HFA_REASON_ERR:
                                HFAUTILS_FAU_INC(stats, pdfail);
                                /*Call free cb*/
                                if(psparam->cb){
                                    psparam->cb(psparam->cba, WQE_DROP);
                                }
                                break;
                            case HFA_REASON_RFULL:
                                LOG("Warn: HFA search hits REASON RFULL," 
                                        "please increase the RBUFSIZE and try\n"); 
                                if(psparam->cb){
                                    psparam->cb(psparam->cba, WQE_DROP);
                                }
                                break;
                            case HFA_REASON_DDONE:
                            case HFA_REASON_TERM:
                                DBG("Getting matches\n");
                                pmatch = NULL;

                                hfa_searchctx_getmatches(&pctx->sctx, 
                                        &psparam->sparam, &pmatch);
                                /*Call matchcb*/
                                if(pctx->matchcb && pmatch){
                                    pctx->matchcb(&pctx->sctx, pmatch);
                                }
                                /*Call free cb*/
                                if(psparam->cb){
                                    psparam->cb(psparam->cba, WQE_SUCCESS);
                                }   
                                break;
                            default:
                                break;
                        }
                        return HFA_SUCCESS;
                    }
                    break;

                default:
                    /*Do Nothing*/
                    break;
            }
        }
        return HFA_SUCCESS;
    }
    return HFA_FAILURE;
}

static inline void 
timeout_action(unsigned long arg){

    atomic_set(&stop,1);
    LOG("\n\t\t\tPerformance capture accomplished. Module can be removed.\n");
    return ;
}

static inline void 
poll_timer_func(unsigned long arg)
{
    searchctx_t         *pctx = NULL;
    uint32_t corenum;
    corenum = cvmx_get_core_num();
    pctx = gctx[corenum];
    if(pctx){
        poll(pctx);
    }else{
        ERR("Not a valid pctx\n");
        return ;
    }
    mod_timer(&poll_timer[corenum],jiffies+25);
    return ;
}
#ifdef USE_TIMER_FOR_STATS    
static inline void 
printstats(unsigned long temp)
{
    del_timer(&printresult_timer);
    hfautils_printstats(&stats, NULL, verbose, NULL);    
    add_timer(&printresult_timer);
}
#endif    

static inline hfa_return_t
get_sparam(searchparam_t **pparam, cvmx_wqe_t *pktwqe)
{
    searchparam_t                *param = NULL;
    void                         *rptr = NULL;
    cvmx_buf_ptr_pki_t             packet_ptr78;

    if(hfautils_likely(pparam && pktwqe)){
        *pparam = NULL;
        param = hfa_fpa_alloc(sparam_aura);
        rptr = hfa_fpa_alloc(rbuf_aura);

        if(param && rptr){
            memset(param, 0, HFA_SPARAM_FPA_POOL_SIZE);

            HFA_OS_LISTHEAD_INIT(&param->list);
            param->pktwqe = pktwqe;
            param->cb = freeparam;
            param->cba = param;
            param->rptr = rptr;

            hfa_searchparam_set_matchcb(&param->sparam,hfautils_matchcb,&stats);
            /*Create Iovec*/
            if (octeon_has_feature(OCTEON_FEATURE_CN78XX_WQE) &&
                    network) {
                packet_ptr78 = cvmx_wqe_get_pki_pkt_ptr(pktwqe);
                (param->input).ptr = (uint8_t *)
                                    (cvmx_phys_to_ptr(packet_ptr78.addr));
            }else {
                (param->input).ptr = 
                                    cvmx_phys_to_ptr((pktwqe->packet_ptr).s.addr);
            }
            (param->input).len = cvmx_wqe_get_len(pktwqe);
            
            /*Initialize Sparam*/
            hfa_searchparam_set_inputiovec(&param->sparam, &param->input, 1);
            hfa_searchparam_set_output(&param->sparam, rptr, 
                                       HFA_RPTR_FPA_POOL_SIZE);
            /*Set on which HFA clusters, search can take place. Hardware will
             * automatically schedule search on a single cluster which is least
             * loaded. Hardware load balancing
             * Pre-requsite: graph should be loaded on all clusters where the bit
             * location in clmsk is 1*/
            hfa_searchparam_set_searchclmsk(&(param->sparam), searchclmsk);
            *pparam = param;
            return HFA_SUCCESS;
        } else {
            if(rptr){
                hfa_fpa_free(rptr, rbuf_aura,0);
            }
            if(param){
                hfa_fpa_free(param, sparam_aura,0);
            }
        }
    }
    return HFA_FAILURE;
}

#ifdef H3C_TEST
unsigned long cycle_poll_start[48];
unsigned long cycle_poll_end[48];
unsigned long cycle_poll_count[48];
unsigned long cycle_submit_start[48];
unsigned long cycle_submit_end[48];
unsigned long cycle_submit_count[48];
unsigned long cycle_submit_start2[48];
unsigned long cycle_submit_end2[48];
unsigned long cycle_submit_count2[48];
unsigned long cycle_submit_start3[48];
unsigned long cycle_submit_end3[48];
unsigned long cycle_submit_count3[48];
void cycle_stat(void)
{
	int i=0;
	unsigned long tol=0;
	unsigned long tol_cnt=0;

	for(i=0; i<48; i++){
		tol += cycle_submit_end[i];
		tol_cnt += cycle_submit_count[i];
	}
	printf("submit %lu %lu:%lu\n",  tol, tol_cnt, (unsigned long)(tol/tol_cnt));

	i=0; tol=0; tol_cnt=0;
	for(i=0; i<48; i++){
		tol += cycle_submit_end2[i];
		tol_cnt += cycle_submit_count2[i];
	}
	printf("submit2 %lu %lu:%lu\n",  tol, tol_cnt, (unsigned long)(tol/tol_cnt));	
	
	i=0; tol=0; tol_cnt=0;
	for(i=0; i<48; i++){
		tol += cycle_submit_end3[i];
		tol_cnt += cycle_submit_count3[i];
	}
	printf("submit3 %lu %lu:%lu\n",  tol, tol_cnt, (unsigned long)(tol/tol_cnt));	
	
	i=0; tol=0; tol_cnt=0;
	for(i=0; i<48; i++){
		tol += cycle_poll_end[i];
		tol_cnt += cycle_poll_count[i];
	}
	printf("poll %lu %lu:%lu\n",  tol, tol_cnt, (unsigned long)(tol/tol_cnt));
}
#endif

static inline hfa_return_t
poll(searchctx_t *pctx)
{
	int  corenum = cvmx_get_core_num();
    paramstatus_t   status;
    
    if(hfautils_likely(pctx)){
        if(pctx->submitted){
		cycle_poll_start[corenum]= cvmx_get_cycle();
		
            /*Try to poll local ctx*/
            if(HFA_SUCCESS != poll_searchctx(pctx, &status)){
                HFAUTILS_FAU_INC(stats, pdfail);
            } else {
                switch(status){
                    case PROCESSED:
                        HFAUTILS_FAU_INC(stats, pdsuccess);
                    break;

                    default:
                        HFAUTILS_FAU_INC(stats, pdretry);
                    break;
                }
            }
		cycle_poll_end[corenum] += (cvmx_get_cycle()-cycle_poll_start[corenum]);
		cycle_poll_count[corenum]++;
		cycle_poll_start[corenum]=0;
        }
        return HFA_SUCCESS;
    }
    return HFA_FAILURE;
}
cvm_oct_callback_result_t 
process_pkt(struct net_device *dev, void *work, struct sk_buff *skb)
{
    
    searchparam_t       *param = NULL;
    cvmx_wqe_t          *wqe = (cvmx_wqe_t *) work;
    searchctx_t         *pctx = NULL;
    paramstatus_t       status= TRYAGAIN;
    int                 addflag;
    int                 corenum = cvmx_get_core_num();
#ifdef GEN4_HFA
    if(!network && HFA_LOCALPKT_WQE != hfa_get_wqe_type(wqe))
        return CVM_OCT_PASS;
#endif
    HFAUTILS_FAU_INC(stats, in);
    if(network){
        if(likely(!poll_timer[corenum].function)){
            poll_timer[corenum].function = poll_timer_func;
            poll_timer[corenum].expires = jiffies+50;
            add_timer(&poll_timer[corenum]);
        }
    }else{
            if(unlikely(atomic_read(&stop))){
                freepktwqe(wqe);
                goto done;
            }
    }
    pctx = gctx[corenum];
#ifdef H3C_TEST
	cycle_submit_start[corenum] = cvmx_get_cycle();
#endif
    addflag = HFA_FALSE;
    /*Get search parameters*/
    if(HFA_SUCCESS != get_sparam(&param, wqe)){
        sendpktwqe(wqe, WQE_DROP);
    } else { 
        if(HFA_SUCCESS != addsearchparam(pctx, param, freeparam, param)){
            HFAUTILS_FAU_INC(stats, adfail);
            freeparam((void *)param, WQE_DROP);
        } else {
            if(ADDED != param->status){
                HFAUTILS_FAU_INC(stats, adretry);
                freeparam((void *)param, WQE_DROP);
            } else {
                HFAUTILS_FAU_INC(stats, adsuccess);
                addflag = HFA_TRUE;
            }
        }
    }
#ifdef H3C_TEST	
	cycle_submit_end[corenum] += (cvmx_get_cycle()-cycle_submit_start[corenum]);
	cycle_submit_count[corenum]++;
	cycle_submit_start[corenum]=0;
#endif

    if(addflag){
#ifdef H3C_TEST		
	cycle_submit_start2[corenum] = cvmx_get_cycle();
#endif	
        /*Submit the ctx*/
        if(HFA_SUCCESS == hfa_searchctx_search_async(&pctx->sctx,
                    &param->sparam)){
#ifdef H3C_TEST	
	cycle_submit_end2[corenum] += (cvmx_get_cycle()-cycle_submit_start2[corenum]);
	cycle_submit_count2[corenum]++;
	cycle_submit_start2[corenum]=0;
#endif
            HFAUTILS_FAU_INC(stats, sdsuccess);
            param->status = SUBMITTED;
            (pctx->added)--;
            (pctx->submitted)++;
        } else {
            HFAUTILS_FAU_INC(stats, sdfail);
        }
        addflag = HFA_FALSE;
    }else {
#ifdef H3C_TEST		
	cycle_submit_start3[corenum] = cvmx_get_cycle();
#endif    
        if(pctx->added){
            submit(pctx, &status);
        }
#ifdef H3C_TEST	
	cycle_submit_end3[corenum] += (cvmx_get_cycle()-cycle_submit_start3[corenum]);
	cycle_submit_count3[corenum]++;
	cycle_submit_start3[corenum]=0;
#endif			
    }

done:
    poll(pctx);
    HFAUTILS_FAU_DEC_SHARED(stats, pend_wqe);
    if(!cvmx_get_core_num()){		
#ifndef H3C_TEST			
        hfautils_printstats(&stats, NULL,verbose, NULL);
#else
        hfautils_printstats(&stats, NULL,verbose, cycle_stat);
#endif
    }
    if(hfa_is_gen4target())
        return CVM_OCT_TAKE_OWNERSHIP_SKB;
    else
        return CVM_OCT_TAKE_OWNERSHIP_WORK;
}

/** 
 * Process command line options,
 * Read graph and payload 
 */
static inline hfa_return_t 
process_options (void) 
{
    hfa_size_t              off=0;
    char                    *cwd = NULL;
    struct path             pwd, root;
    
    if(!(clmsk) || clmsk > hfa_get_max_clmsk()){
        clmsk = hfa_get_max_clmsk();
    }
    if(!(searchclmsk) || searchclmsk > hfa_get_max_clmsk()) {
        searchclmsk = hfa_get_max_clmsk();
    }
    if(!hfa_isethernetdrv_present()){
        ERR("Try after inserting OCTEON Ethernet driver\n");
        return HFA_FAILURE;
    }
    if(!network){
        if(!hfa_is_gen4target())
            if(!hfa_napi_perf || hfa_distribute_load){
                ERR("Reinsert HFA_LIB_MODULE with hfa_napi_perf=1 and "\
                    "hfa_distribute_load=0\n" \
                "\t eg. insmod cvm-hfa-lib.ko hfa_distribute_load=0 hfa_napi_perf=1\n");
                return HFA_FAILURE;
            }
    }
    /*Request IPI handler for exiting of all cores */
    ipi_handle_mesg = octeon_request_ipi_handler(core_exit);
    if(ipi_handle_mesg < 0){
        panic("No IPI handler available\n");
    }
    hfautils_file_size(graph, &gsize);
    if(!network){
        hfautils_file_size(payload, &psize);
        if(HFA_SUCCESS != hfautils_validate_chunksize(&chunksize, psize)){
            return HFA_FAILURE;
        }
        options.payloadsize = psize;
        options.chunksize = chunksize;
        options.pcap = pcap;
        options.payload = payload;
        /* Get current working directory */
        if(current && current->fs) {
            pwd = current->fs->pwd;
            path_get(&pwd);
            root= current->fs->root;
            path_get(&root);
            buf = (char *)hfautils_memoryalloc(100*sizeof(char), 8); 
            if(buf == NULL) {
                ERR ("Memory allocation failed for path \n");   
                return HFA_FAILURE;
            } 
            cwd = d_path(&pwd, buf, 100 * sizeof(char));
            memset(&pattr, 0, sizeof(hfautils_payload_attr_t));
            
            pattr.path = cwd;
            /* Initialize attributes for parsing the payload file */
            if(HFA_SUCCESS != 
                hfautils_init_payload_attributes (&pattr, &options)){
                ERR ("Failure in hfautils_init_payload_attributes\n");
                hfautils_memoryfree(buf, 100 * sizeof(char)); 
                return HFA_FAILURE;
            }
        }
    }
    /*Read Graph file*/
    if(HFA_SUCCESS != hfautils_read_file(graph, &gbuf, gsize, &off, HFA_TRUE)){
        ERR("failure in reading graph: %s\n", graph);
        goto pattr_cleanup;
    }
    ncores = hfautils_get_number_of_cores();
    
    paramsz = sizeof(searchparam_t);
    rptrsz = RBUFSIZE;

    hfautils_fpa_aligned(&paramsz);
    hfautils_fpa_aligned(&rptrsz);

    return HFA_SUCCESS;
pattr_cleanup:
    if(!network) {
        hfautils_cleanup_payload_attributes(&pattr, &options);
        hfautils_memoryfree(buf, 100 * sizeof(char));
    }
    return HFA_FAILURE;
}

int 
entry(void)
{
    int                     i, msk;
    long unsigned int       sflags = HFA_SEARCHCTX_FNOCROSS | 
                                     HFA_SEARCHCTX_FSINGLEMATCH;
    /* Process command line options, read graph and payload */ 
    if(HFA_SUCCESS != process_options()) {
        ERR("failure in process_options\n");
        return HFA_FAILURE;
    }

	memset(cycle_poll_start, 0, 48*sizeof(unsigned long));
	memset(cycle_poll_end, 0, 48*sizeof(unsigned long));
	memset(cycle_poll_count, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_start, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_end, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_count, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_start2, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_end2, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_count2, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_start3, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_end3, 0, 48*sizeof(unsigned long));
	memset(cycle_submit_count3, 0, 48*sizeof(unsigned long));
	
    for_each_online_cpu(i){
        atomic_inc(&exitflag);
        hfa_fau_init();
    }
    /*Initialize HFA device*/
    if(HFA_SUCCESS != hfa_dev_init((hfa_dev_t *)&dev)){
        ERR("Dev Init Failed\n");
        goto m_graph_free; 
    }
    /*Initialize graph*/
    hfa_dev_graph_init((hfa_dev_t *)&dev, &_graph);

    /*Set cluster mask*/
    if(HFA_SUCCESS != hfa_graph_setcluster(&_graph, clmsk)){
        ERR("Failure returned from hfa_graph_setcluster\n");
        goto m_dev_cleanup;
    }
    /*Download Graph*/
    if(HFA_SUCCESS != hfautils_download_graph(&_graph, gbuf, gsize,
                                            GRAPHCHUNK, 0)){
        ERR("Failure in downloading the graph\n");
        goto m_dev_cleanup;
    }
    /*Cacheload Graph*/
    if(!HFA_GET_GRAPHATTR(&_graph, memonly)){
        if(HFA_SUCCESS != hfa_graph_cacheload(&_graph)){
            ERR("Failure in Graph Cache Load\n");
            goto m_graph_cleanup;
        }
    }
    /*Parameter and Result buffer cnt should be equal to number of WQE and 
     * Packet data pool setup by ethernet driver*/
    if (hfa_create_fpa_pool(&sparam_pool, &sparam_aura, "Search Param Nodes", 
               HFA_SPARAM_FPA_POOL,HFA_SPARAM_FPA_POOL_SIZE, 60000, &msk)){ 
        ERR("Unable to create search Param FPA\n");
        goto m_graph_cacheunload;
    }
    if (hfa_create_fpa_pool(&rbuf_pool, &rbuf_aura, "Result buffers", 
                   HFA_RPTR_FPA_POOL,HFA_RPTR_FPA_POOL_SIZE, 60000, &msk)){ 
        ERR("Unable to create Result buffers FPA\n");
        goto m_graph_cacheunload;
    }

    if (!octeon_has_feature(OCTEON_FEATURE_FAU)) {
        /* Initialize the software FAU system */
        hfa_fau_init();
    }
    if(HFA_FAILURE == hfautils_init_perf_cntrs(&stats)){
        ERR("Unable to init perf counters\n");
        goto m_graph_cacheunload;
    }
    
    /*Initialize per core searchctx*/

    if(matchacross){
         HFA_BITMSKCLR(sflags, HFA_SEARCHCTX_FNOCROSS);
    } else {
         HFA_BITMSKSET(sflags, HFA_SEARCHCTX_FNOCROSS);
    }
    if(singlematch){
       HFA_BITMSKSET(sflags, HFA_SEARCHCTX_FSINGLEMATCH);
    } else {
       HFA_BITMSKCLR(sflags, HFA_SEARCHCTX_FSINGLEMATCH);
    }
    LOG("Searchctx flags: 0x%lx\n", sflags);
    /*Initialize per core context*/
    for_each_online_cpu(i){
        if(HFA_SUCCESS != createctx(&(gctx[i]), sflags, NULL)){
            ERR("FlowCtx creation error : %d\n", i);
            goto m_destroy_ctx; 
        }
    }
    if(network){

        poll_timer = hfautils_memoryalloc(hfautils_get_number_of_cores() * sizeof(struct timer_list),8);
        if(!poll_timer){
            ERR("Unable to allocate mem for timer_list\n");
            return HFA_FAILURE;
        }
        memset(poll_timer,0,hfautils_get_number_of_cores() * sizeof(struct timer_list));
        for(i=0;i<hfautils_get_number_of_cores();i++){
            init_timer(&poll_timer[i]);
        }
        for(i=0; i<count; i++){
#ifdef GEN4_HFA
            input_device[i] = 
                octeon3_register_callback(port[i], process_pkt);
#else
            input_device[i] = 
                cvm_oct_register_callback(port[i], process_pkt);
            work_pool = wqe_aura = OCTEON_WQE_POOL;
#endif
            if(NULL == input_device[i]){
                ERR("process pkt registration failed for port: %s\n", port[i]);
                goto m_deregister_reschedulecb;
            }
        }
    }
    else { 
        /* LOCALPKT snippet*/
        /* timeout in sec to stop local pkt processing.*/
        if(ttl > 0){
            init_timer(&timeout);
            timeout.function = timeout_action;
            timeout.expires = jiffies + ttl*HZ;
            add_timer(&timeout);
        }
#ifdef GEN4_HFA
	/* If given iface physically exits, 
	 * it will register irrespective of iface UP.
	 */
        if(!count || (NULL == (input_device[0] = cvm_octeon3_register_callback(port[0], process_pkt, 
                    &pattr.ipdport, &wqe_aura, &wqe_pool, &pattr.grp, &pki_blk_sz, &buffers_needed)))){
            ERR("%s\n",count ? "Error in registering cb for Pkt WQE." :
			    "Port/Iface is required.");
            goto m_destroy_ctx;
        }
        /* Ensuring the iface is UP/ACTIVE. */
        if(!IS_IFACE_UP(input_device[0])){
            ERR("%s Port/Interface is not up.\n",port[0]);
            goto m_deregister_pktcb;
        }
        hfa_dbg("ipdport: %d, aura: %d, pool: %d, pki_blk_sz: %d, grp: %lu\n",
                pattr.ipdport,wqe_aura,wqe_pool,pki_blk_sz,pattr.grp);
        ibuf_aura = wqe_aura;
#else   
        work_pool = wqe_aura = OCTEON_WQE_POOL;
        ibuff_pool = ibuf_aura = OCTEON_IBUFPOOL;
        pattr.grp = hfa_pow_rcv_grp[cvmx_get_core_num()];
        if(HFA_SUCCESS != hfa_register_packet_interceptcb(process_pkt)){
            ERR("Error in registering cb for Pkt WQE\n");
            goto m_destroy_ctx;
        }
#endif
        /*Create local packets if network payload is disabled*/
        pattr.npkts = npkts * ncores;
        pattr.tt = CVMX_POW_TAG_TYPE_ORDERED;
        pattr.ibuf_pool = ibuf_aura; 
        pattr.wqe_pool = wqe_aura;
	/* N/W pkt size <= 1500 and local packet max is 64K but for kernel there
	 * a limit with fpa pool allocations as wqe pool is being used for 
	 * OIII targets.
	 */
	if(hfa_is_gen4target())
		options.chunksize = HFA_OCT3_ETH_CHUNK_SIZE;
        if(HFA_SUCCESS != hfautils_create_localpkts (&pattr, &options)){ 
            ERR("Failure in creating localpkts\n");
            goto m_deregister_pktcb;
        }
        HFAUTILS_FAU_INCBY_SHARED(stats, pend_wqe, pattr.npkts);
    } /*End of LOCALPKT snippet*/
#ifdef USE_TIMER_FOR_STATS    
    /*Insert timer*/
    init_timer(&printresult_timer);
    printresult_timer.function = printstats;
    printresult_timer.expires = jiffies;
    add_timer(&printresult_timer);
#endif    
    LOG("%s is inserted successfully\n", THIS_MODULE->name);
    if(network)
        LOG("Stats will be print once ethernet driver receives the packets\n");
    
    return 0;

m_deregister_pktcb:
#ifdef GEN4_HFA
        cvm_octeon3_register_callback(port[0],NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#else    
        hfa_register_packet_interceptcb(NULL);
#endif
m_deregister_reschedulecb:
    if(network){
        for(i=0; i< count; i++){
            if(input_device[i]){
#ifdef GEN4_HFA
                octeon3_register_callback(port[i], NULL);
#else
                cvm_oct_register_callback(port[i], NULL);
#endif
            }
        }
    }
m_destroy_ctx:
    /* Cleanup fau counters */
    hfautils_cleanup_perf_cntrs(&stats);
    for_each_online_cpu(i){
        destroyctx(gctx[i]);
        gctx[i]=0;
    }
    hfa_shutdown_fpa_pool(rbuf_aura);
    hfa_shutdown_fpa_pool(sparam_aura);
m_graph_cacheunload:        
    if(!HFA_GET_GRAPHATTR(&_graph, memonly))
        hfa_graph_cacheunload(&_graph);
m_graph_cleanup:
    hfa_dev_graph_cleanup((hfa_dev_t *)&dev, &_graph);
m_dev_cleanup:
    hfa_dev_cleanup ((hfa_dev_t *)&dev);        
m_graph_free:
    hfautils_vmmemoryfree(gbuf, gsize);
    if(!network) {
        hfautils_cleanup_payload_attributes(&pattr, &options);
        hfautils_memoryfree(buf, 100 * sizeof(char));
    }
    octeon_release_ipi_handler(ipi_handle_mesg);
    return(-1);
}
static void core_exit_wrapper(void *info)
{
    searchctx_t     *pctx = info;
    destroyctx(pctx);
    
    atomic_dec(&exitflag);
    if(!atomic_read(&exitflag)){
        octeon_release_ipi_handler(ipi_handle_mesg);
    }
}
void exit(void)
{
    int i;
    if(network){
        for(i=0; i< count; i++){
            if(input_device[i]){
#ifdef GEN4_HFA
                octeon3_register_callback(port[i], NULL);
#else
                cvm_oct_register_callback(port[i], NULL);
#endif
            }
        }
        for(i=0;i<hfautils_get_number_of_cores();i++){
            if(poll_timer[i].function)
                del_timer(&poll_timer[i]);
        }
        if(poll_timer)
            hfautils_memoryfree(poll_timer,hfautils_get_number_of_cores()*sizeof(struct timer_list));
    } else {
        atomic_set(&stop,1);
        while(HFAUTILS_FAU_FETCH_SHARED(stats, pend_wqe, 0ULL));
#ifdef GEN4_HFA
        cvm_octeon3_register_callback(port[0],NULL,NULL,NULL,NULL,NULL,NULL,NULL);
#else
        hfa_register_packet_interceptcb(NULL);
#endif
        hfautils_cleanup_payload_attributes(&pattr, &options);
        hfautils_memoryfree(buf, 100 * sizeof(char));
        if(timeout.function)
            del_timer(&timeout);
    }


#ifdef USE_TIMER_FOR_STATS    
    del_timer(&printresult_timer);
#endif
    for_each_online_cpu(i){
        if(i == smp_processor_id()){
            core_exit();
        }else {
            if(hfa_is_gen4target()) {
                smp_call_function_single(i, core_exit_wrapper,
                          gctx[i],0);
            }
            else {
                octeon_send_ipi_single(i, ipi_handle_mesg);
            }
        }
    }
    while(atomic_read(&exitflag));
    
    if(!HFA_GET_GRAPHATTR(&_graph, memonly)){
        hfa_graph_cacheunload (&_graph);
    }
    /* Cleanup fau counters */
    hfautils_cleanup_perf_cntrs(&stats);
    hfa_shutdown_fpa_pool(rbuf_aura);
    hfa_shutdown_fpa_pool(sparam_aura);
    hfa_dev_graph_cleanup(&dev, &_graph);
    hfa_dev_cleanup (&dev);       
    hfautils_vmmemoryfree(gbuf, gsize);
    hfa_dbg("Cleanedskbcnt: %d\n",atomic_read(&cleaned_skb_cnt));
    LOG("%s is exited successfully\n", THIS_MODULE->name);
}
void core_exit(void)
{
    searchctx_t     *pctx = NULL;
   
    pctx = gctx[smp_processor_id()];
    destroyctx(pctx);
    
    atomic_dec(&exitflag);

    if(!atomic_read(&exitflag)){
        octeon_release_ipi_handler(ipi_handle_mesg);
    }
}

/* @cond APPINTERNAL */
module_init (entry);
module_exit (exit);
MODULE_LICENSE ("Cavium");
MODULE_AUTHOR("Cavium Networks <support@caviumnetworks.com>");
/* @endcond APPINTERNAL */
