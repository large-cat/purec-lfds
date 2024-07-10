/***** includes *****/
#include "lfds_queue_bounded_singleproducer_singleconsumer_internal.h"





/****************************************************************************/
void lfds_queue_bss_cleanup( struct lfds_queue_bss_state *qbsss,
                                void (*element_cleanup_callback)(struct lfds_queue_bss_state *qbsss, void *key, void *value) )
{
  lfds_pal_uint_t
    loop;

  struct lfds_queue_bss_element
    *qbsse;

  LFDS_PAL_ASSERT( qbsss != NULL );
  // TRD : element_cleanup_callback can be NULL

  if( element_cleanup_callback != NULL )
    for( loop = qbsss->read_index ; loop < qbsss->read_index + qbsss->number_elements ; loop++ )
    {
      qbsse = qbsss->element_array + (loop % qbsss->number_elements);
      element_cleanup_callback( qbsss, qbsse->key, qbsse->value );
    }

  return;
}

