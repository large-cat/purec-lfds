/***** includes *****/
#include "lfds_list_addonly_single_unorder_internal.h"





/****************************************************************************/
void lfds_list_asu_init_valid_on_current_logical_core( struct lfds_list_asu_state *lasus,
                                                          void *user_state )
{
  LFDS_PAL_ASSERT( lasus != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasus->dummy_element % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasus->end % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasus->start % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  // TRD : user_state can be NULL

  // TRD : dummy start element - makes code easier when you can always use ->next
  lasus->start = lasus->end = &lasus->dummy_element;

  lasus->start->next = NULL;
  lasus->start->value = NULL;
  lasus->user_state = user_state;

  lfds_misc_internal_backoff_init( &lasus->after_backoff );
  lfds_misc_internal_backoff_init( &lasus->start_backoff );
  lfds_misc_internal_backoff_init( &lasus->end_backoff );

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}

