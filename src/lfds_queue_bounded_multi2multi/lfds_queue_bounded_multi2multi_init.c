/***** includes *****/
#include "lfds_queue_bounded_multi2multi_internal.h"





/****************************************************************************/
void lfds_queue_bmm_init_core( struct lfds_queue_bmm_state *qbmms,
                                                           struct lfds_queue_bmm_element *element_array,
                                                           lfds_pal_uint_t number_elements,
                                                           void *user_state )
{
  lfds_pal_uint_t
    loop;

  LFDS_PAL_ASSERT( qbmms != NULL );
  LFDS_PAL_ASSERT( element_array != NULL );
  LFDS_PAL_ASSERT( number_elements >= 2 );
  LFDS_PAL_ASSERT( ( number_elements & (number_elements-1) ) == 0 ); // TRD : number_elements must be a positive integer power of 2
  // TRD : user_state can be NULL

  qbmms->number_elements = number_elements;
  qbmms->mask = qbmms->number_elements - 1;
  qbmms->read_index = 0;
  qbmms->write_index = 0;
  qbmms->element_array = element_array;
  qbmms->user_state = user_state;

  for( loop = 0 ; loop < qbmms->number_elements ; loop++ )
    qbmms->element_array[loop].sequence_number = loop;

  lfds_misc_internal_backoff_init( &qbmms->dequeue_backoff );
  lfds_misc_internal_backoff_init( &qbmms->enqueue_backoff );

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}

