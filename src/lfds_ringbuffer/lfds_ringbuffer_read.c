/***** includes *****/
#include "lfds_ringbuffer_internal.h"





/****************************************************************************/
int lfds_ringbuffer_read( struct lfds_ringbuffer_state *rs,
                             void **key,
                             void **value )
{
  int
    rv;

  struct lfds_queue_umm_element
    *qumme;

  struct lfds_ringbuffer_element
    *re;

  LFDS_PAL_ASSERT( rs != NULL );
  // TRD : key can be NULL
  // TRD : value can be NULL
  // TRD : psts can be NULL

  rv = lfds_queue_umm_dequeue( &rs->qumms, &qumme );

  if( rv == 1 )
  {
    re = LFDS_QUEUE_UMM_GET_VALUE_FROM_ELEMENT( *qumme );
    re->qumme_use = (struct lfds_queue_umm_element *) qumme;
    if( key != NULL )
      *key = re->key;
    if( value != NULL )
      *value = re->value;
    LFDS_FREELIST_SET_VALUE_IN_ELEMENT( re->fe, re );
    lfds_freelist_push( &rs->fs, &re->fe, NULL );
  }

  return rv;
}

