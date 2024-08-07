/***** includes *****/
#include "lfds_ringbuffer_internal.h"

/***** private prototypes *****/
static void lfds_ringbuffer_internal_queue_umm_element_cleanup_callback( struct lfds_queue_umm_state *qumms,
                                                                            struct lfds_queue_umm_element *qumme,
                                                                            enum lfds_misc_flag dummy_element_flag );
static void lfds_ringbuffer_internal_freelist_element_cleanup_callback( struct lfds_freelist_state *fs,
                                                                           struct lfds_freelist_element *fe );





/****************************************************************************/
void lfds_ringbuffer_cleanup( struct lfds_ringbuffer_state *rs,
                                 void (*element_cleanup_callback)(struct lfds_ringbuffer_state *rs, void *key, void *value, enum lfds_misc_flag unread_flag) )
{
  LFDS_PAL_ASSERT( rs != NULL );
  // TRD : element_cleanup_callback can be NULL

  if( element_cleanup_callback != NULL )
  {
    rs->element_cleanup_callback = element_cleanup_callback;
    lfds_queue_umm_cleanup( &rs->qumms, lfds_ringbuffer_internal_queue_umm_element_cleanup_callback );
    lfds_freelist_cleanup( &rs->fs, lfds_ringbuffer_internal_freelist_element_cleanup_callback );
  }

  return;
}





/****************************************************************************/
#pragma warning( disable : 4100 )

static void lfds_ringbuffer_internal_queue_umm_element_cleanup_callback( struct lfds_queue_umm_state *qumms,
                                                                            struct lfds_queue_umm_element *qumme,
                                                                            enum lfds_misc_flag dummy_element_flag )
{
  struct lfds_ringbuffer_element
    *re;

  struct lfds_ringbuffer_state
    *rs;

  LFDS_PAL_ASSERT( qumms != NULL );
  LFDS_PAL_ASSERT( qumme != NULL );
  // TRD : dummy_element can be any value in its range

  rs = (struct lfds_ringbuffer_state *) LFDS_QUEUE_UMM_GET_USER_STATE_FROM_STATE( *qumms );
  re = (struct lfds_ringbuffer_element *) LFDS_QUEUE_UMM_GET_VALUE_FROM_ELEMENT( *qumme );

  if( dummy_element_flag == LFDS_MISC_FLAG_LOWERED )
    rs->element_cleanup_callback( rs, re->key, re->value, LFDS_MISC_FLAG_RAISED );

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
#pragma warning( disable : 4100 )

static void lfds_ringbuffer_internal_freelist_element_cleanup_callback( struct lfds_freelist_state *fs,
                                                                           struct lfds_freelist_element *fe )
{
  struct lfds_ringbuffer_element
    *re;

  struct lfds_ringbuffer_state
    *rs;

  LFDS_PAL_ASSERT( fs != NULL );
  LFDS_PAL_ASSERT( fe != NULL );

  rs = (struct lfds_ringbuffer_state *) LFDS_FREELIST_GET_USER_STATE_FROM_STATE( *fs );
  re = (struct lfds_ringbuffer_element *) LFDS_FREELIST_GET_VALUE_FROM_ELEMENT( *fe );

  rs->element_cleanup_callback( rs, re->key, re->value, LFDS_MISC_FLAG_LOWERED );

  return;
}

#pragma warning( default : 4100 )

