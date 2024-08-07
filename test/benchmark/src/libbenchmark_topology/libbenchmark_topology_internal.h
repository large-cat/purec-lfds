/***** the library wide include file *****/
#include "../libbenchmark_internal.h"

/***** structs *****/
struct libbenchmark_topology_lp_printing_offset
{
  int
    offset;

  struct lfds_btree_au_element
    baue;

  struct libbenchmark_topology_node_state
    tns;
};

/***** private prototypes *****/
lfds_pal_uint_t count_of_logical_processors_below_node( struct lfds_btree_au_element *baue );

