/**
 * @file
 *
 * Functions and macros for accessing Cavium CSRs.
 *
 * <hr>$Revision: 49448 $<hr>
 *
 * @defgroup csr CSR support
 * @{
 */

/**
 * Possible CSR bus types
 */
typedef enum {
   BDK_CSR_TYPE_DAB,            /**< External debug 64bit CSR */
   BDK_CSR_TYPE_DAB32b,         /**< External debug 32bit CSR */
   BDK_CSR_TYPE_NCB,            /**< Fast 64bit CSR */
   BDK_CSR_TYPE_NCB32b,         /**< Fast 32bit CSR */
   BDK_CSR_TYPE_PCCBR,
   BDK_CSR_TYPE_PCCPF,
   BDK_CSR_TYPE_PCCVF,
   BDK_CSR_TYPE_PCICONFIGRC,    /**< PCIe config address (RC mode) */
   BDK_CSR_TYPE_PCICONFIGEP,    /**< PCIe config address (EP mode) */
   BDK_CSR_TYPE_PEXP,           /**< PCIe BAR 0 address only */
   BDK_CSR_TYPE_PEXP_NCB,       /**< NCB-direct and PCIe BAR0 address */
   BDK_CSR_TYPE_RSL,            /**< Slow 64bit CSR */
   BDK_CSR_TYPE_RSL32b,         /**< Slow 32bit CSR */
   BDK_CSR_TYPE_SYSREG,         /**< Core system register */
} bdk_csr_type_t;

#define BDK_CSR_DB_MAX_PARAM 4
typedef struct __attribute__ ((packed)) {
    uint32_t        name_index : 20;/**< Index into __bdk_csr_db_string where the name is */
    uint32_t        base_index : 13;/**< Index into __bdk_csr_db_number where the base address is */
    uint8_t         unused : 7;
    bdk_csr_type_t  type : 4;       /**< Enum type from above */
    uint8_t         width : 4;      /**< CSR width in bytes */
    uint16_t        field_index;    /**< Index into __bdk_csr_db_fieldList where the fields start */
    uint8_t         range[BDK_CSR_DB_MAX_PARAM]; /**< Index into __bdk_csr_db_range where the range is */
    uint16_t        param_inc[BDK_CSR_DB_MAX_PARAM]; /**< Index into __bdk_csr_db_number where the param multiplier is */
} __bdk_csr_db_type_t;

typedef struct __attribute__ ((packed)) {
    uint32_t        name_index : 20;/**< Index into __bdk_csr_db_string where the name is */
    uint32_t        start_bit : 6;  /**< LSB of the field */
    uint32_t        stop_bit : 6;   /**< MSB of the field */
} __bdk_csr_db_field_t;

typedef struct {
    uint32_t model;
    const int16_t *data;            /**< Array of integers indexing __bdk_csr_db_csr */
} __bdk_csr_db_map_t;


