/***** includes *****/
#include "lfds_ringbuffer_internal.h"

/***** private prototypes *****/
static void lfds_ringbuffer_internal_validate( struct lfds_ringbuffer_state *rs,
                                                  struct lfds_misc_validation_info *vi,
                                                  enum lfds_misc_validity *lfds_queue_umm_validity,
                                                  enum lfds_misc_validity *lfds_freelist_validity );



/****************************************************************************/
void lfds_ringbuffer_query( struct lfds_ringbuffer_state *rs,
                               enum lfds_ringbuffer_query query_type,
                               void *query_input,
                               void *query_output )
{
  LFDS_PAL_ASSERT( rs != NULL );
  // TRD : query_type can be any value in its range

  LFDS_MISC_BARRIER_LOAD;

  switch( query_type )
  {
    case LFDS_RINGBUFFER_QUERY_SINGLETHREADED_GET_COUNT:
      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_queue_umm_query( &rs->qumms, LFDS_QUEUE_UMM_QUERY_SINGLETHREADED_GET_COUNT, NULL, query_output );
    break;

    case LFDS_RINGBUFFER_QUERY_SINGLETHREADED_VALIDATE:
      // TRD : query_input can be NULL
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_ringbuffer_internal_validate( rs, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output, ((enum lfds_misc_validity *) query_output)+1 );
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_ringbuffer_internal_validate( struct lfds_ringbuffer_state *rs,
                                                  struct lfds_misc_validation_info *vi,
                                                  enum lfds_misc_validity *lfds_queue_umm_validity,
                                                  enum lfds_misc_validity *lfds_freelist_validity )
{
  LFDS_PAL_ASSERT( rs != NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_queue_umm_validity != NULL );
  LFDS_PAL_ASSERT( lfds_freelist_validity != NULL );

  if( vi == NULL )
  {
    lfds_queue_umm_query( &rs->qumms, LFDS_QUEUE_UMM_QUERY_SINGLETHREADED_VALIDATE, NULL, lfds_queue_umm_validity );
    lfds_freelist_query( &rs->fs, LFDS_FREELIST_QUERY_SINGLETHREADED_VALIDATE, NULL, lfds_freelist_validity );
  }

  if( vi != NULL )
  {
    struct lfds_misc_validation_info
      freelist_vi,
      queue_vi;

    queue_vi.min_elements = 0;
    freelist_vi.min_elements = 0;
    queue_vi.max_elements = vi->max_elements;
    freelist_vi.max_elements = vi->max_elements;

    lfds_queue_umm_query( &rs->qumms, LFDS_QUEUE_UMM_QUERY_SINGLETHREADED_VALIDATE, &queue_vi, lfds_queue_umm_validity );
    lfds_freelist_query( &rs->fs, LFDS_FREELIST_QUERY_SINGLETHREADED_VALIDATE, &freelist_vi, lfds_freelist_validity );
  }

  return;
}

