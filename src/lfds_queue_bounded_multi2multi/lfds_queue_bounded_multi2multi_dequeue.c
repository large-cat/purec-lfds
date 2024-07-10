/***** includes *****/
#include "lfds_queue_bounded_multi2multi_internal.h"





/****************************************************************************/
int lfds_queue_bmm_dequeue( struct lfds_queue_bmm_state *qbmms,
                               void **key,
                               void **value )
{
  char unsigned
    result;

  enum lfds_misc_flag
    finished_flag = LFDS_MISC_FLAG_LOWERED;

  int
    rv = 1;

  lfds_pal_uint_t
    read_index,
    sequence_number;

  lfds_pal_int_t
    difference;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  struct lfds_queue_bmm_element
    *qbmme = NULL;

  LFDS_PAL_ASSERT( qbmms != NULL );
  // TRD : key can be NULL
  // TRD : value can be NULL

  LFDS_MISC_BARRIER_LOAD;

  read_index = qbmms->read_index;

  while( finished_flag == LFDS_MISC_FLAG_LOWERED )
  {
    qbmme = &qbmms->element_array[ read_index & qbmms->mask ];
    LFDS_MISC_BARRIER_LOAD;
    sequence_number = qbmme->sequence_number;
    difference = (lfds_pal_int_t) sequence_number - (lfds_pal_int_t) (read_index + 1);

    if( difference == 0 )
    {
      LFDS_PAL_ATOMIC_CAS( &qbmms->read_index, &read_index, read_index + 1, LFDS_MISC_CAS_STRENGTH_WEAK, result );
      if( result == 0 )
        LFDS_BACKOFF_EXPONENTIAL_BACKOFF( qbmms->dequeue_backoff, backoff_iteration );
      if( result == 1 )
        finished_flag = LFDS_MISC_FLAG_RAISED;
    }

    if( difference < 0 )
    {
      rv = 0;
      finished_flag = LFDS_MISC_FLAG_RAISED;
    }

    if( difference > 0 )
    {
      LFDS_MISC_BARRIER_LOAD;
      read_index = qbmms->read_index;
    }
  }

  if( rv == 1 )
  {
    if( key != NULL )
      *key = qbmme->key;
    if( value != NULL )
      *value = qbmme->value;
    LFDS_MISC_BARRIER_STORE;
    qbmme->sequence_number = read_index + qbmms->mask + 1;
  }

  LFDS_BACKOFF_AUTOTUNE( qbmms->dequeue_backoff, backoff_iteration );

  return rv;
}

