/***** includes *****/
#include "libbenchmark_topology_node_internal.h"





/****************************************************************************/
void libbenchmark_topology_node_init( struct libbenchmark_topology_node_state *tns )
{
  LFDS_PAL_ASSERT( tns != NULL );

  // TRD : we only ever add logical processor nodes to the logical_processor_children list
  lfds_list_aso_init_core( &tns->logical_processor_children, libbenchmark_topology_node_compare_nodes_function, LFDS_LIST_ASO_EXISTING_KEY_FAIL, NULL );

  return;
}

