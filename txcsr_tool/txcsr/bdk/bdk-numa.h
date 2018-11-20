/**
 * @file
 *
 * Functions for dealing with multiple chips organized into a
 * NUMA cluster.
 *
 * <hr>$Revision: 49448 $<hr>
 */

#define BDK_NUMA_MAX_NODES 4
typedef enum
{
    BDK_NODE_0 = 0,
    BDK_NODE_1 = 1,
    BDK_NODE_2 = 2,
    BDK_NODE_3 = 3,
} bdk_node_t;

/**
 * Return the local node number
 *
 * @return Node number
 */
static inline bdk_node_t bdk_numa_local(void) __attribute__ ((always_inline, pure));
static inline bdk_node_t bdk_numa_local(void)
{
    return BDK_NODE_0; /* FIXME: choose remote node */
}


