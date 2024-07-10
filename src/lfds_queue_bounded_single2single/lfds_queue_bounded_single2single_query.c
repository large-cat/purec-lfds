/***** includes *****/
#include "lfds_queue_bounded_single2single_internal.h"

/***** private prototypes *****/
static void lfds_queue_bss_internal_validate( struct lfds_queue_bss_state *qbsss,
                                                 struct lfds_misc_validation_info *vi,
                                                 enum lfds_misc_validity *lfds_validity );





/****************************************************************************/
void lfds_queue_bss_query( struct lfds_queue_bss_state *qbsss,
                              enum lfds_queue_bss_query query_type,
                              void *query_input,
                              void *query_output )
{
  LFDS_PAL_ASSERT( qbsss != NULL );
  // TRD : query_type can be any value in its range

  switch( query_type )
  {
    case LFDS_QUEUE_BSS_QUERY_GET_POTENTIALLY_INACCURATE_COUNT:
    {
      lfds_pal_uint_t
        local_read_index,
        local_write_index;

      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      LFDS_MISC_BARRIER_LOAD;

      local_read_index = qbsss->read_index;
      local_write_index = qbsss->write_index;

      *(lfds_pal_uint_t *) query_output = +( local_write_index - local_read_index );

      if( local_read_index > local_write_index )
        *(lfds_pal_uint_t *) query_output = qbsss->number_elements - *(lfds_pal_uint_t *) query_output;
    }
    break;

    case LFDS_QUEUE_BSS_QUERY_VALIDATE:
      // TRD : query_input can be NULL
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_queue_bss_internal_validate( qbsss, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output );
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_queue_bss_internal_validate( struct lfds_queue_bss_state *qbsss,
                                                 struct lfds_misc_validation_info *vi,
                                                 enum lfds_misc_validity *lfds_validity )
{
  LFDS_PAL_ASSERT( qbsss != NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_validity != NULL );

  *lfds_validity = LFDS_MISC_VALIDITY_VALID;

  if( vi != NULL )
  {
    lfds_pal_uint_t
      number_elements;

    lfds_queue_bss_query( qbsss, LFDS_QUEUE_BSS_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, (void *) &number_elements );

    if( number_elements < vi->min_elements )
      *lfds_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( number_elements > vi->max_elements )
      *lfds_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  return;
}

