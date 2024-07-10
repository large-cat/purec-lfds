/***** includes *****/
#include "lfds_queue_unbounded_manyproducer_manyconsumer_internal.h"





/****************************************************************************/
void lfds_queue_umm_cleanup( struct lfds_queue_umm_state *qumms,
                                void (*element_cleanup_callback)(struct lfds_queue_umm_state *qumms, struct lfds_queue_umm_element *qumme, enum lfds_misc_flag dummy_element_flag) )
{
  struct lfds_queue_umm_element
    *qumme;

  void
    *value;

  LFDS_PAL_ASSERT( qumms != NULL );
  // TRD : element_cleanup_callback can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( element_cleanup_callback != NULL )
  {
    while( qumms->dequeue[POINTER] != qumms->enqueue[POINTER] )
    {
      // TRD : trailing dummy element, so the first real value is in the next element
      value = qumms->dequeue[POINTER]->next[POINTER]->value;

      // TRD : user is given back *an* element, but not the one his user data was in
      qumme = qumms->dequeue[POINTER];

      // TRD : remove the element from queue
      qumms->dequeue[POINTER] = qumms->dequeue[POINTER]->next[POINTER];

      // TRD : write value into the qumme we're going to give the user
      qumme->value = value;

      element_cleanup_callback( qumms, qumme, LFDS_MISC_FLAG_LOWERED );
    }

    // TRD : and now the final element
    element_cleanup_callback( qumms, (struct lfds_queue_umm_element *) qumms->dequeue[POINTER], LFDS_MISC_FLAG_RAISED );
  }

  return;
}

