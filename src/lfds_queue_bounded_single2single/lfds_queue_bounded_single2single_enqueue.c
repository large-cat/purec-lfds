/***** includes *****/
#include "lfds_queue_bounded_single2single_internal.h"





/****************************************************************************/
int lfds_queue_bss_enqueue( struct lfds_queue_bss_state *qbsss,
                               void *key,
                               void *value )
{
  struct lfds_queue_bss_element
    *qbsse;

  LFDS_PAL_ASSERT( qbsss != NULL );
  // TRD : key can be NULL
  // TRD : value can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( ( (qbsss->write_index+1) & qbsss->mask ) != qbsss->read_index )
  {
    qbsse = qbsss->element_array + qbsss->write_index;

    qbsse->key = key;
    qbsse->value = value;

    LFDS_MISC_BARRIER_STORE;

    qbsss->write_index = (qbsss->write_index + 1) & qbsss->mask;

    return 1;
  }

  return 0;
}

