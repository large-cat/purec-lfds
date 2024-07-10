/***** includes *****/
#include "lfds_list_addonly_singlylinked_unordered_internal.h"





/****************************************************************************/
void lfds_list_asu_cleanup( struct lfds_list_asu_state *lasus,
                               void (*element_cleanup_callback)(struct lfds_list_asu_state *lasus, struct lfds_list_asu_element *lasue) )
{
  struct lfds_list_asu_element
    *lasue,
    *temp;

  LFDS_PAL_ASSERT( lasus != NULL );
  // TRD : element_cleanup_callback can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( element_cleanup_callback == NULL )
    return;

  lasue = LFDS_LIST_ASU_GET_START( *lasus );

  while( lasue != NULL )
  {
    temp = lasue;

    lasue = LFDS_LIST_ASU_GET_NEXT( *lasue );

    element_cleanup_callback( lasus, temp );
  }

  return;
}

