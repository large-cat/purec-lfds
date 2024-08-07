/***** includes *****/
#include "libbenchmark_topology_internal.h"





/****************************************************************************/
void libbenchmark_topology_iterate_init( struct libbenchmark_topology_iterate_state *tis, enum libbenchmark_topology_node_type type )
{
  LFDS_PAL_ASSERT( tis != NULL );
  // TRD : type can be any value in its range

  tis->baue = NULL;
  tis->type = type;

  return;
}





/****************************************************************************/
int libbenchmark_topology_iterate( struct libbenchmark_topology_state *ts, struct libbenchmark_topology_iterate_state *tis, struct libbenchmark_topology_node_state **tns )
{
  int
    rv = 1;

  LFDS_PAL_ASSERT( ts != NULL );
  LFDS_PAL_ASSERT( tis != NULL );
  LFDS_PAL_ASSERT( tns != NULL );

  while( btree_au_get_abs_rel_pos(&ts->topology_tree, &tis->baue, LFDS_BTREE_AU_ABSOLUTE_POSITION_LARGEST_IN_TREE, LFDS_BTREE_AU_RELATIVE_POSITION_NEXT_SMALLER_ELEMENT_IN_ENTIRE_TREE) )
  {
    *tns = LFDS_BTREE_AU_GET_VALUE_FROM_ELEMENT( *tis->baue );

    if( (*tns)->type == tis->type )
      break;
  }

  if( tis->baue == NULL )
  {
    *tns = NULL;
    rv = 0;
  }

  return rv;
}

