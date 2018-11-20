
#ifndef __BDK_CSRS_RST_H__
#define __BDK_CSRS_RST_H__

struct l2c_cmb
{
        int cmbnum;
        int64_t add;
        int64_t store;
        int64_t commit;
        int64_t fill;
        int64_t inv;
        int64_t add_cent;
        int64_t store_cent;
        int64_t commit_cent;
        int64_t fill_cent;

};

struct l2c_cmb cmb[10];

struct l2c_io
{
        int ionum;
        int64_t ioc;
        int64_t ior;
        int64_t ioc_cent;
        int64_t ior_cent;

};

struct l2c_io io[4];

#define CORE_CLK_IN_100MHZ 19
struct l2c_tad
{
        int64_t tadnum;
        int64_t tag_hit;
        int64_t tag_miss;
        int64_t tag_noalloc;
        int64_t victim;
        int64_t sc_fail;
        int64_t sc_pass;
        int64_t lfb_occupancy;
        int64_t lfb_wait_lfb;
        int64_t lfb_wait_vab;
        int64_t quad0_index_bus;
        int64_t quad0_read_bus;
        int64_t quad0_banks_inuse;
        int64_t quad0_wdat_inuse;
                                        //(repeat quad1-qaud3)
        int64_t quad1_index_bus;
        int64_t quad1_read_bus;
        int64_t quad1_banks_inuse;
        int64_t quad1_wdat_inuse;

        int64_t quad2_index_bus;
        int64_t quad2_read_bus;
        int64_t quad2_banks_inuse;
        int64_t quad2_wdat_inuse;

        int64_t quad3_index_bus;
        int64_t quad3_read_bus;
        int64_t quad3_banks_inuse;
        int64_t quad3_wdat_inuse;

        int64_t lfb_occupancy_cent;          //          "lfb_occupancy %" = (percent utilization)
        int64_t lfb_wait_lfb_cent;           //                "lfb_wait_lfb %" = (percent utilization)
        int64_t lfb_wait_vab_cent;           //                "lfb_wait_vab %" = (percent utilization)
        int64_t quad0_index_bus_cent;        //                "quad0_index_bus %" = (percent utilization)
        int64_t quad0_read_bus_cent;         //                "quad0_read_bus %" = (percent utilization)
        int64_t quad0_banks_inuse_cent;      //                "quad0_banks_inuse %" = (percent utilization)
        int64_t quad0_wdat_inuse_cent;       //                "quad0_wdat_inuse %" = (percent utilization)
                                        //(repeat quad1-qaud3)
        int64_t quad1_index_bus_cent;        //                "quad1_index_bus %" = (percent utilization)
        int64_t quad1_read_bus_cent;         //                "quad1_read_bus %" = (percent utilization)
        int64_t quad1_banks_inuse_cent;      //                "quad1_banks_inuse %" = (percent utilization)
        int64_t quad1_wdat_inuse_cent;

        int64_t quad2_index_bus_cent;        //                "quad2_index_bus %" = (percent utilization)
        int64_t quad2_read_bus_cent;         //                "quad2_read_bus %" = (percent utilization)
        int64_t quad2_banks_inuse_cent;      //                "quad2_banks_inuse %" = (percent utilization)
        int64_t quad2_wdat_inuse_cent;
        int64_t quad3_index_bus_cent;        //                "quad3_index_bus %" = (percent utilization)
        int64_t quad3_read_bus_cent;         //                "quad3_read_bus %" = (percent utilization)
        int64_t quad3_banks_inuse_cent;      //                "quad3_banks_inuse %" = (percent utilization)
        int64_t quad3_wdat_inuse_cent;


};

struct l2c_tad tads[8];

union l2c_tadx_prf {
        uint64_t u;
        struct l2c_tadx_prf_s {
#ifdef __LITTLE_ENDIAN
                uint64_t cnt0sel                     : 8;
                uint64_t cnt1sel                     : 8;
                uint64_t cnt2sel                     : 8;
                uint64_t cnt3sel                     : 8;
                uint64_t reserved_32_63              : 32;
#else

                uint64_t reserved_32_63              : 32;
                uint64_t cnt3sel                     : 8;  /**< R/W - Selects event to count for L2C_TAD(0..7)_PFC(3). Enumerated by L2C_TAD_PRF_SEL_E. */
                uint64_t cnt2sel                     : 8;  /**< R/W - Selects event to count for L2C_TAD(0..7)_PFC(2). Enumerated by L2C_TAD_PRF_SEL_E. */
                uint64_t cnt1sel                     : 8;  /**< R/W - Selects event to count for L2C_TAD(0..7)_PFC(1). Enumerated by L2C_TAD_PRF_SEL_E. */
                uint64_t cnt0sel                     : 8;  /**< R/W - Selects event to count for L2C_TAD(0..7)_PFC(0). Enumerated by L2C_TAD_PRF_SEL_E. */

#endif
        } s;

};


union l2c_tadx_prf tadx_prf;


/**
 * Register (RSL) rst_boot
 *
 * RST Boot Register
 */
typedef union
{
    uint64_t u;
    struct bdk_rst_boot_s
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) Reserved. */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t reserved_47_54        : 8;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t lboot_pf_flr          : 4;  /**< [ 29: 26](R/W1C/H) Last boot cause was caused by a PF Function Level Reset event.
                                                                 <29> = Warm reset due to PF FLR on PEM3.
                                                                 <28> = Warm reset due to PF FLR on PEM2.
                                                                 <27> = Warm reset due to PF FLR on PEM1.
                                                                 <26> = Warm reset due to PF FLR on PEM0. */
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Last boot cause mask for PEM5 and PEM4; resets only with PLL_DC_OK.
                                                                 <23> = Warm reset due to Cntl5 link-down or hot-reset.
                                                                 <22> = Warm reset due to Cntl4 link-down or hot-reset.
                                                                 <21> = Cntl5 reset due to PERST5_L pin.
                                                                 <20> = Cntl4 reset due to PERST4_L pin.
                                                                 <19> = Warm reset due to PERST5_L pin.
                                                                 <18> = Warm reset due to PERST4_L pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
#else /* Word 0 - Little Endian */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Last boot cause mask for PEM5 and PEM4; resets only with PLL_DC_OK.
                                                                 <23> = Warm reset due to Cntl5 link-down or hot-reset.
                                                                 <22> = Warm reset due to Cntl4 link-down or hot-reset.
                                                                 <21> = Cntl5 reset due to PERST5_L pin.
                                                                 <20> = Cntl4 reset due to PERST4_L pin.
                                                                 <19> = Warm reset due to PERST5_L pin.
                                                                 <18> = Warm reset due to PERST4_L pin. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t lboot_pf_flr          : 4;  /**< [ 29: 26](R/W1C/H) Last boot cause was caused by a PF Function Level Reset event.
                                                                 <29> = Warm reset due to PF FLR on PEM3.
                                                                 <28> = Warm reset due to PF FLR on PEM2.
                                                                 <27> = Warm reset due to PF FLR on PEM1.
                                                                 <26> = Warm reset due to PF FLR on PEM0. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_47_54        : 8;
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) Reserved. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
#endif /* Word 0 - End */
    } s;
    struct bdk_rst_boot_cn81xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) Reserved. */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t reserved_47_54        : 8;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t reserved_26_29        : 4;
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Reserved. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Reserved
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Reserved
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Reserved
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
#else /* Word 0 - Little Endian */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Reserved
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Reserved
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Reserved
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Reserved. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t reserved_26_29        : 4;
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_47_54        : 8;
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) Reserved. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
#endif /* Word 0 - End */
    } cn81xx;
    struct bdk_rst_boot_cn88xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) VRM error. VRM did not complete operations within 5.25mS of PLL_DC_OK being
                                                                 asserted. PLLs were released automatically. */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t reserved_47_54        : 8;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t reserved_26_29        : 4;
        uint64_t reserved_24_25        : 2;
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Last boot cause mask for PEM5 and PEM4; resets only with PLL_DC_OK.
                                                                 <23> = Warm reset due to Cntl5 link-down or hot-reset.
                                                                 <22> = Warm reset due to Cntl4 link-down or hot-reset.
                                                                 <21> = Cntl5 reset due to PERST5_L pin.
                                                                 <20> = Cntl4 reset due to PERST4_L pin.
                                                                 <19> = Warm reset due to PERST5_L pin.
                                                                 <18> = Warm reset due to PERST4_L pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
#else /* Word 0 - Little Endian */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Last boot cause mask for PEM5 and PEM4; resets only with PLL_DC_OK.
                                                                 <23> = Warm reset due to Cntl5 link-down or hot-reset.
                                                                 <22> = Warm reset due to Cntl4 link-down or hot-reset.
                                                                 <21> = Cntl5 reset due to PERST5_L pin.
                                                                 <20> = Cntl4 reset due to PERST4_L pin.
                                                                 <19> = Warm reset due to PERST5_L pin.
                                                                 <18> = Warm reset due to PERST4_L pin. */
        uint64_t reserved_24_25        : 2;
        uint64_t reserved_26_29        : 4;
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_47_54        : 8;
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) VRM error. VRM did not complete operations within 5.25mS of PLL_DC_OK being
                                                                 asserted. PLLs were released automatically. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
#endif /* Word 0 - End */
    } cn88xx;
    struct bdk_rst_boot_cn83xx
    {
#if __BYTE_ORDER == __BIG_ENDIAN /* Word 0 - Big Endian */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) VRM error. VRM did not complete operations within 5.25mS of PLL_DC_OK being
                                                                 asserted. PLLs were released automatically. */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t reserved_47_54        : 8;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t lboot_pf_flr          : 4;  /**< [ 29: 26](R/W1C/H) Last boot cause was caused by a PF Function Level Reset event.
                                                                 <29> = Warm reset due to PF FLR on PEM3.
                                                                 <28> = Warm reset due to PF FLR on PEM2.
                                                                 <27> = Warm reset due to PF FLR on PEM1.
                                                                 <26> = Warm reset due to PF FLR on PEM0. */
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Reserved. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
#else /* Word 0 - Little Endian */
        uint64_t rboot_pin             : 1;  /**< [  0:  0](RO/H) Remote Boot Strap. Indicates the state of remote boot as initially determined by
                                                                 GPIO_STRAP<2:0> = RST_BOOT_METHOD_E::REMOTE. If set core 0 will remain in reset
                                                                 for the cold reset. */
        uint64_t rboot                 : 1;  /**< [  1:  1](R/W) Remote Boot. If set, indicates that core 0 will remain in reset after a
                                                                 chip warm/soft reset.  The initial value mimics the setting of the [RBOOT_PIN]. */
        uint64_t lboot                 : 10; /**< [ 11:  2](R/W1C/H) Last boot cause mask for PEM1 and PEM0; resets only with PLL_DC_OK.
                                                                 <11> = Soft reset due to watchdog.
                                                                 <10> = Soft reset due to RST_SOFT_RST write.
                                                                 <9> = Warm reset due to Cntl1 link-down or hot-reset.
                                                                 <8> = Warm reset due to Cntl0 link-down or hot-reset.
                                                                 <7> = Cntl1 reset due to PERST1_L pin.
                                                                 <6> = Cntl0 reset due to PERST0_L pin.
                                                                 <5> = Warm reset due to PERST1_L pin.
                                                                 <4> = Warm reset due to PERST0_L pin.
                                                                 <3> = Warm reset due to CHIP_RESET_L pin.
                                                                 <2> = Cold reset due to PLL_DC_OK pin. */
        uint64_t lboot_ext23           : 6;  /**< [ 17: 12](R/W1C/H) Last boot cause mask for PEM3 and PEM2; resets only with PLL_DC_OK.
                                                                 <17> = Warm reset due to Cntl3 link-down or hot-reset.
                                                                 <16> = Warm reset due to Cntl2 link-down or hot-reset.
                                                                 <15> = Cntl3 reset due to PERST3_L pin.
                                                                 <14> = Cntl2 reset due to PERST2_L pin.
                                                                 <13> = Warm reset due to PERST3_L pin.
                                                                 <12> = Warm reset due to PERST2_L pin. */
        uint64_t lboot_ext45           : 6;  /**< [ 23: 18](R/W1C/H) Reserved. */
        uint64_t lboot_jtg             : 1;  /**< [ 24: 24](R/W1C/H) Last boot cause was write to JTG reset. */
        uint64_t lboot_ckill           : 1;  /**< [ 25: 25](R/W1C/H) Last boot cause was chip kill timer expiring.  See RST_BOOT[CHIPKILL]. */
        uint64_t lboot_pf_flr          : 4;  /**< [ 29: 26](R/W1C/H) Last boot cause was caused by a PF Function Level Reset event.
                                                                 <29> = Warm reset due to PF FLR on PEM3.
                                                                 <28> = Warm reset due to PF FLR on PEM2.
                                                                 <27> = Warm reset due to PF FLR on PEM1.
                                                                 <26> = Warm reset due to PF FLR on PEM0. */
        uint64_t lboot_oci             : 3;  /**< [ 32: 30](R/W1C/H) Reserved.
                                                                 Internal:
                                                                 Last boot cause mask for CCPI; resets only with PLL_DC_OK.
                                                                 <32> = Warm reset due to CCPI link 2 going down.
                                                                 <31> = Warm reset due to CCPI link 1 going down.
                                                                 <30> = Warm reset due to CCPI link 0 going down. */
        uint64_t pnr_mul               : 6;  /**< [ 38: 33](RO/H) Coprocessor-clock multiplier. [PNR_MUL] = (coprocessor-clock speed) /(ref-clock speed).
                                                                 The value ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [PNR_MUL] is set from the pi_pnr_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[122:119].  If the fuse value is > 0, it is compared with the pi_pnr_pll_mul[4:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_39           : 1;
        uint64_t c_mul                 : 7;  /**< [ 46: 40](RO/H) Core-clock multiplier. [C_MUL] = (core-clock speed) / (ref-clock speed). The value
                                                                 ref-clock speed should always be 50 MHz.

                                                                 Internal:
                                                                 [C_MUL] is set from the pi_pll_mul pins plus 6 and is limited by a set of
                                                                 fuses[127:123].  If the fuse value is > 0, it is compared with the pi_pll_mul[5:1]
                                                                 pins and the smaller value is used. */
        uint64_t reserved_47_54        : 8;
        uint64_t dis_scan              : 1;  /**< [ 55: 55](R/W1S) Disable scan. When written to 1, and FUSF_CTL[ROT_LCK] = 1, reads as 1 and scan is not
                                                                 allowed in the part.
                                                                 This state persists across soft and warm resets.

                                                                 Internal:
                                                                 This state will persist across a simulation */
        uint64_t dis_huk               : 1;  /**< [ 56: 56](R/W1S) Disable HUK. Secure only and W1S set-only. When set FUSF_SSK(),
                                                                 FUSF_HUK(), FUSF_EK(), and FUSF_SW() cannot be read.
                                                                 Resets to (!trusted_mode && FUSF_CTL[FJ_DIS_HUK]). */
        uint64_t vrm_err               : 1;  /**< [ 57: 57](RO) VRM error. VRM did not complete operations within 5.25mS of PLL_DC_OK being
                                                                 asserted. PLLs were released automatically. */
        uint64_t jt_tstmode            : 1;  /**< [ 58: 58](RO) JTAG test mode. */
        uint64_t ckill_ppdis           : 1;  /**< [ 59: 59](R/W) Chipkill core disable. When set to 1, cores other than core 0 will immediately
                                                                 be disabled when RST_BOOT[CHIPKILL] is set. Writes have no effect when
                                                                 RST_BOOT[CHIPKILL]=1. */
        uint64_t trusted_mode          : 1;  /**< [ 60: 60](RO) When set, chip is operating as a trusted device. This bit is asserted when
                                                                 either MIO_FUS_DAT2[TRUSTZONE_EN], FUSF_CTL[TZ_FORCE2], or the trusted-mode
                                                                 strap GPIO_STRAP<10> are set. */
        uint64_t ejtagdis              : 1;  /**< [ 61: 61](R/W) Reserved. */
        uint64_t jtcsrdis              : 1;  /**< [ 62: 62](R/W) JTAG CSR disable. When set to 1, during the next warm or soft reset the JTAG TAP
                                                                 controller will be disabled, ie. DAP_IMP_DAR will be 0.  This field resets to 1
                                                                 in trusted-mode, else 0. */
        uint64_t chipkill              : 1;  /**< [ 63: 63](R/W1S) A 0-to-1 transition of CHIPKILL starts the CHIPKILL timer. When CHIPKILL=1 and the timer
                                                                 expires, chip reset is asserted internally. The CHIPKILL timer can be stopped only by
                                                                 a reset (cold, warm, soft). The length of the CHIPKILL timer is specified by
                                                                 RST_CKILL[TIMER].  This feature is effectively a delayed warm reset. */
#endif /* Word 0 - End */
    } cn83xx;
} bdk_rst_boot_t;


#endif /* __BDK_CSRS_RST_H__ */
