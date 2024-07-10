/***** includes *****/
#include "libtest_tests_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void libtest_tests_queue_bss_dequeuing( struct lfds_list_asu_state *list_of_logical_processors, struct libshared_memory_state *ms, enum lfds_misc_validity *dvs )
{
  lfds_pal_uint_t
    loop;

  struct lfds_queue_bss_element
    element_array[128];

  struct lfds_queue_bss_state
    qs;

  struct lfds_misc_validation_info
    vi;

  void
    *value;

  LFDS_PAL_ASSERT( list_of_logical_processors != NULL );
  LFDS_PAL_ASSERT( ms != NULL );
  LFDS_PAL_ASSERT( dvs != NULL );

  /* TRD : create an empty queue
           enqueue 128 elements
           then dequeue the elements, in the same thread - we're API testing
           it's a single producer queue, so we just do this in our current thread
           since we're enqueuing and dequeuing in the same thread
  */

  *dvs = LFDS_MISC_VALIDITY_VALID;

  lfds_queue_bss_init_valid_on_current_logical_core( &qs, element_array, 128, NULL );

  for( loop = 0 ; loop < 127 ; loop++ )
    lfds_queue_bss_enqueue( &qs, NULL, (void *) loop );

  for( loop = 0 ; loop < 127 ; loop++ )
  {
    lfds_queue_bss_dequeue( &qs, NULL, &value );
    if( (lfds_pal_uint_t) value != 127 - loop )
      *dvs = LFDS_MISC_VALIDITY_INVALID_TEST_DATA;
  }

  vi.min_elements = vi.max_elements = 0;

  lfds_queue_bss_query( &qs, LFDS_QUEUE_BSS_QUERY_VALIDATE, &vi, dvs );

  lfds_queue_bss_cleanup( &qs, NULL );

  return;
}

#pragma warning( default : 4100 )

