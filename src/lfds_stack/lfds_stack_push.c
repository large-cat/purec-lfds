/***** includes *****/
#include "lfds_stack_internal.h"





/****************************************************************************/
void lfds_stack_push( struct lfds_stack_state *ss,
                         struct lfds_stack_element *se )
{
  char unsigned
    result;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  struct lfds_stack_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_DOUBLE_POINTER)
    *new_top[PAC_SIZE],
    *volatile original_top[PAC_SIZE];

  LFDS_PAL_ASSERT( ss != NULL );
  LFDS_PAL_ASSERT( se != NULL );

  new_top[POINTER] = se;

  original_top[COUNTER] = ss->top[COUNTER];
  original_top[POINTER] = ss->top[POINTER];

  do
  {
    se->next = original_top[POINTER];
    LFDS_MISC_BARRIER_STORE;

    new_top[COUNTER] = original_top[COUNTER] + 1;
    LFDS_PAL_ATOMIC_DWCAS( ss->top, original_top, new_top, LFDS_MISC_CAS_STRENGTH_WEAK, result );

    if( result == 0 )
      LFDS_BACKOFF_EXPONENTIAL_BACKOFF( ss->push_backoff, backoff_iteration );
  }
  while( result == 0 );

  LFDS_BACKOFF_AUTOTUNE( ss->push_backoff, backoff_iteration );

  return;
}

