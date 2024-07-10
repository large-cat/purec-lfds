/***** includes *****/
#include "libshared_memory_internal.h"





/****************************************************************************/
void libshared_memory_init( struct libshared_memory_state *ms )
{
  LFDS_PAL_ASSERT( ms != NULL );

  lfds_list_asu_init_core( &ms->list_of_allocations, NULL );

  return;
}

