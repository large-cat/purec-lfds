/***** includes *****/
#include "lfds_queue_unbounded_multi2multi_internal.h"





/****************************************************************************/
void lfds_queue_umm_enqueue( struct lfds_queue_umm_state *qumms,
                                struct lfds_queue_umm_element *qumme )
{
  char unsigned
    result = 0;

  enum lfds_misc_flag
    finished_flag = LFDS_MISC_FLAG_LOWERED;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  struct lfds_queue_umm_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_DOUBLE_POINTER)
    *volatile enqueue[PAC_SIZE],
    *new_enqueue[PAC_SIZE],
    *volatile next[PAC_SIZE];

  LFDS_PAL_ASSERT( qumms != NULL );
  LFDS_PAL_ASSERT( qumme != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) qumme->next % LFDS_PAL_ALIGN_DOUBLE_POINTER == 0 );

  qumme->next[POINTER] = NULL;
  LFDS_PAL_ATOMIC_ADD( &qumms->aba_counter, 1, qumme->next[COUNTER], struct lfds_queue_umm_element * );
  LFDS_MISC_BARRIER_STORE;

  new_enqueue[POINTER] = qumme;

  LFDS_MISC_BARRIER_LOAD;

  do
  {
    /* TRD : note here the deviation from the white paper
             in the white paper, next is loaded from enqueue, not from qumms->enqueue
             what concerns me is that between the load of enqueue and the load of
             enqueue->next, the element can be dequeued by another thread *and freed*

             by ordering the loads (load barriers), and loading both from qumms,
             the following if(), which checks enqueue is still the same as qumms->enqueue
             still continues to ensure next belongs to enqueue, while avoiding the
             problem with free
    */

    enqueue[COUNTER] = qumms->enqueue[COUNTER];
    enqueue[POINTER] = qumms->enqueue[POINTER];

    LFDS_MISC_BARRIER_LOAD;

    next[COUNTER] = qumms->enqueue[POINTER]->next[COUNTER];
    next[POINTER] = qumms->enqueue[POINTER]->next[POINTER];

    LFDS_MISC_BARRIER_LOAD;

    if( qumms->enqueue[COUNTER] == enqueue[COUNTER] and qumms->enqueue[POINTER] == enqueue[POINTER] )
    {
      if( next[POINTER] == NULL )
      {
        new_enqueue[COUNTER] = next[COUNTER] + 1;
        LFDS_PAL_ATOMIC_DWCAS( enqueue[POINTER]->next, next, new_enqueue, LFDS_MISC_CAS_STRENGTH_WEAK, result );
        if( result == 1 )
          finished_flag = LFDS_MISC_FLAG_RAISED;
      }
      else
      {
        next[COUNTER] = enqueue[COUNTER] + 1;
        // TRD : strictly, this is a weak CAS, but we do an extra iteration of the main loop on a fake failure, so we set it to be strong
        LFDS_PAL_ATOMIC_DWCAS( qumms->enqueue, enqueue, next, LFDS_MISC_CAS_STRENGTH_STRONG, result );
      }
    }
    else
      result = 0;

    if( result == 0 )
      LFDS_BACKOFF_EXPONENTIAL_BACKOFF( qumms->enqueue_backoff, backoff_iteration );
  }
  while( finished_flag == LFDS_MISC_FLAG_LOWERED );

  // TRD : move enqueue along; only a weak CAS as the dequeue will solve this if it's out of place
  new_enqueue[COUNTER] = enqueue[COUNTER] + 1;
  LFDS_PAL_ATOMIC_DWCAS( qumms->enqueue, enqueue, new_enqueue, LFDS_MISC_CAS_STRENGTH_WEAK, result );

  if( result == 0 )
    LFDS_BACKOFF_EXPONENTIAL_BACKOFF( qumms->enqueue_backoff, backoff_iteration );

  LFDS_BACKOFF_AUTOTUNE( qumms->enqueue_backoff, backoff_iteration );

  return;
}

