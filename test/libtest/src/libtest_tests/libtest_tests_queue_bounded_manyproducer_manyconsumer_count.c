/***** includes *****/
#include "libtest_tests_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void libtest_tests_queue_bmm_count( struct lfds_list_asu_state *list_of_logical_processors, struct libshared_memory_state *ms, enum lfds_misc_validity *dvs )
{
  int
    rv;

  lfds_pal_uint_t
    expected_count = 0,
    key,
    loop,
    number_elements,
    subloop,
    value;

  struct lfds_queue_bmm_element
    qbmme_array[8];

  struct lfds_queue_bmm_state
    qbmms;

  LFDS_PAL_ASSERT( list_of_logical_processors != NULL );
  LFDS_PAL_ASSERT( ms != NULL );
  LFDS_PAL_ASSERT( dvs != NULL );

  /* TRD : make a queue with 8 elements
           1. enqueue to full, count
           2. dequeue to empty, count
           3. enqueue to full, half dequeue, add two more, count
  */

  *dvs = LFDS_MISC_VALIDITY_VALID;

  lfds_queue_bmm_init_core( &qbmms, qbmme_array, 8, NULL );

  // TRD : loop a few times
  for( loop = 0 ; loop < 64 ; loop++ )
  {
    // TRD : enqueue two (but the queue may be full, so we might only queue one)
    for( subloop = 0 ; subloop < 2 ; subloop++ )
    {
      rv = lfds_queue_bmm_enqueue( &qbmms, NULL, NULL );

      if( rv == 1 and expected_count == 8 )
        *dvs = LFDS_MISC_VALIDITY_INVALID_TEST_DATA;

      if( rv == 1 and expected_count < 8 )
        expected_count++;
    }

    // TRD : dequeue one
    rv = lfds_queue_bmm_dequeue( &qbmms, (void **) &key, (void **) &value );

    if( rv == 1 )
      expected_count--;

    if( rv == 0 )
      *dvs = LFDS_MISC_VALIDITY_INVALID_TEST_DATA;

    lfds_queue_bmm_query( &qbmms, LFDS_QUEUE_BMM_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, (void *) &number_elements );

    if( number_elements != expected_count )
      *dvs = LFDS_MISC_VALIDITY_INVALID_TEST_DATA;
  }

  lfds_queue_bmm_cleanup( &qbmms, NULL );

  return;
}

#pragma warning( default : 4100 )

