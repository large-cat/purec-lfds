/***** includes *****/
#include "lfds_stack_internal.h"





/****************************************************************************/
void lfds_stack_init_valid_on_current_logical_core( struct lfds_stack_state *ss,
                                                       void *user_state )
{
  LFDS_PAL_ASSERT( ss != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) ss->top % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &ss->user_state % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  // TRD : user_state can be NULL

  ss->top[POINTER] = NULL;
  ss->top[COUNTER] = 0;

  ss->user_state = user_state;

  lfds_misc_internal_backoff_init( &ss->pop_backoff );
  lfds_misc_internal_backoff_init( &ss->push_backoff );

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}

