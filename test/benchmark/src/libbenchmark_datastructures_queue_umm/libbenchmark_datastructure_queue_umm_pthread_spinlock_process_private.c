/***** includes *****/
#include "libbenchmark_datastructure_queue_umm_internal.h"





/****************************************************************************/
void libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_init( struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_state *qs, struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_element *qe, void *user_state )
{
  LFDS_PAL_ASSERT( qs != NULL );
  LFDS_PAL_ASSERT( qe != NULL );
  LFDS_PAL_ASSERT( user_state == NULL );

  qe->next = NULL;

  qs->enqueue_umm = qe;
  qs->dequeue_umm = qe;

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_CREATE( qs->lock_enqueue_umm );
  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_CREATE( qs->lock_dequeue_umm );

  qs->user_state = user_state;

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}





/****************************************************************************/
void libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_cleanup( struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_state *qs,
                                                                   void (*element_cleanup_callback)(struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_state *qs, struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_element *qe, enum flag dummy_element_flag) )
{
  struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_element
    *qe;

  LFDS_PAL_ASSERT( qs != NULL );
  // TRD : element_cleanup_callback can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( element_cleanup_callback != NULL )
    while( libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_dequeue_umm(qs, &qe) )
      element_cleanup_callback( qs, qe, LOWERED );

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_DESTROY( qs->lock_enqueue_umm );
  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_DESTROY( qs->lock_dequeue_umm );

  return;
}





/****************************************************************************/
void libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_enqueue_umm( struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_state *qs, struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_element *qe )
{
  LFDS_PAL_ASSERT( qs != NULL );
  LFDS_PAL_ASSERT( qe != NULL );

  qe->next = NULL;

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_GET( qs->lock_enqueue_umm );

  qs->enqueue_umm->next = qe;
  qs->enqueue_umm = qe;

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_RELEASE( qs->lock_enqueue_umm );

  return;
}





/****************************************************************************/
int libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_dequeue_umm( struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_state *qs, struct libbenchmark_datastructure_queue_umm_pthread_spinlock_process_private_element **qe )
{
  int
    rv = 1;

  LFDS_PAL_ASSERT( qs != NULL );
  LFDS_PAL_ASSERT( qe != NULL );

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_GET( qs->lock_dequeue_umm );

  if( qs->dequeue_umm == qs->enqueue_umm )
  {
    *qe = NULL;
    rv = 0;
  }
  else
  {
    *qe = qs->dequeue_umm;
    (*qe)->key = qs->dequeue_umm->next->key;
    (*qe)->key = qs->dequeue_umm->next->value;
    qs->dequeue_umm = qs->dequeue_umm->next;
  }

  LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_PRIVATE_RELEASE( qs->lock_dequeue_umm );

  return rv;
}

