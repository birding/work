/**
 * @file
 *
 * Functions for determining which Cavium chip you are running
 * on.
 *
 * <hr>$Revision: 49448 $<hr>
 * @addtogroup chips
 * @{
 */


/* Flag bits in top byte. The top byte of MIDR_EL1 is defined
   as ox43, the Cavium implementer code. In this number, bit 7
   and bit 5 are defiend as zero. We use these bits to signal
   that revision numbers should be ignored. It isn't ideal
   that these are in the middle of an already defined field,
   but this keeps the model numbers as 32 bits */

#include <helper.h>

#define __OM_IGNORE_REVISION        0x80000000
#define __OM_IGNORE_MINOR_REVISION  0x20000000
#define __OM_IGNORE_MODEL           0x10000000

#define CAVIUM_CN88XX_PASS1_0   0x430f0a10
#define CAVIUM_CN88XX_PASS1_1   0x430f0a11
#define CAVIUM_CN88XX_PASS2_0   0x431f0a10
#define CAVIUM_CN88XX_PASS2_1   0x431f0a11
#define CAVIUM_CN88XX_PASS2_2   0x431f0a12
#define CAVIUM_CN88XX           (CAVIUM_CN88XX_PASS2_0 | __OM_IGNORE_REVISION)
#define CAVIUM_CN88XX_PASS1_X   (CAVIUM_CN88XX_PASS1_0 | __OM_IGNORE_MINOR_REVISION)
#define CAVIUM_CN88XX_PASS2_X   (CAVIUM_CN88XX_PASS2_0 | __OM_IGNORE_MINOR_REVISION)

#define CAVIUM_CN83XX_PASS1_0   0x430f0a30
#define CAVIUM_CN83XX           (CAVIUM_CN83XX_PASS1_0 | __OM_IGNORE_REVISION)
#define CAVIUM_CN83XX_PASS1_X   (CAVIUM_CN83XX_PASS1_0 | __OM_IGNORE_MINOR_REVISION)

#define CAVIUM_CN81XX_PASS1_0   0x430f0a20
#define CAVIUM_CN81XX           (CAVIUM_CN81XX_PASS1_0 | __OM_IGNORE_REVISION)
#define CAVIUM_CN81XX_PASS1_X   (CAVIUM_CN81XX_PASS1_0 | __OM_IGNORE_MINOR_REVISION)

#define CAVIUM_CN98XX_PASS1_0   0x430f0b10
#define CAVIUM_CN98XX           (CAVIUM_CN98XX_PASS1_0 | __OM_IGNORE_REVISION)
#define CAVIUM_CN98XX_PASS1_X   (CAVIUM_CN98XX_PASS1_0 | __OM_IGNORE_MINOR_REVISION)

/* These match entire families of chips */
#define CAVIUM_CN8XXX           (CAVIUM_CN88XX_PASS1_0 | __OM_IGNORE_MODEL)
#define CAVIUM_CN9XXX           (CAVIUM_CN98XX_PASS1_0 | __OM_IGNORE_MODEL)

/**
 * Return non-zero if the chip matech the passed model.
 *
 * @param arg_model One of the CAVIUM_* constants for chip models and passes
 *
 * @return Non-zero if match
 */
static inline int CAVIUM_IS_MODEL(uint32_t arg_model) __attribute__ ((pure, always_inline));
static inline int CAVIUM_IS_MODEL(uint32_t arg_model)
{
    const uint32_t part_mask = 0x0000fff0;
    const uint32_t variant_mask = 0x00f00000;

    uint32_t arg_part = (arg_model & part_mask);
    uint32_t arg_variant = (arg_model & variant_mask);

    uint32_t board_part = get_board_part();
    uint32_t board_variant = get_board_variant();

    return ((board_part == arg_part) && (board_variant == arg_variant));
}

/**
 * Return non-zero if the die is in an alternate package. The
 * normal is_model() checks will treat alternate package parts
 * as all the same, where this function can be used to detect
 * them.
 *
 * @param arg_model One of the CAVIUM_* constants for chip models and passes
 *
 * @return Non-zero if match
 */
extern int cavium_is_altpkg(uint32_t arg_model);

/** @} */
