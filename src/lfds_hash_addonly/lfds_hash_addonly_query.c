/***** includes *****/
#include "lfds_hash_addonly_internal.h"

/***** private prototypes *****/
static void lfds_hash_a_internal_validate( struct lfds_hash_a_state *has,
                                              struct lfds_misc_validation_info *vi,
                                              enum lfds_misc_validity *lfds_hash_a_validity );





/****************************************************************************/
void lfds_hash_a_query( struct lfds_hash_a_state *has,
                           enum lfds_hash_a_query query_type,
                           void *query_input,
                           void *query_output )
{
  LFDS_PAL_ASSERT( has != NULL );
  // TRD : query_type can be any value in its range

  LFDS_MISC_BARRIER_LOAD;

  switch( query_type )
  {
    case LFDS_HASH_A_QUERY_GET_POTENTIALLY_INACCURATE_COUNT:
    {
      struct lfds_hash_a_iterate
        ai;

      struct lfds_hash_a_element
        *hae;

      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      *(lfds_pal_uint_t *) query_output = 0;

      lfds_hash_a_iterate_init( has, &ai );

      while( lfds_hash_a_iterate(&ai, &hae) )
        ( *(lfds_pal_uint_t *) query_output )++;
    }
    break;

    case LFDS_HASH_A_QUERY_SINGLETHREADED_VALIDATE:
      // TRD: query_input can be any value in its range
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_hash_a_internal_validate( has, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output );
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_hash_a_internal_validate( struct lfds_hash_a_state *has,
                                              struct lfds_misc_validation_info *vi,
                                              enum lfds_misc_validity *lfds_hash_a_validity )
{
  lfds_pal_uint_t
    lfds_hash_a_total_number_elements = 0,
    lfds_btree_au_total_number_elements = 0,
    number_elements;

  lfds_pal_uint_t
    loop;

  LFDS_PAL_ASSERT( has!= NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_hash_a_validity != NULL );

  /* TRD : validate every btree_addonly_unbalanced in the addonly_hash
           sum elements in each btree_addonly_unbalanced
           check matches expected element counts (if vi is provided)
  */

  *lfds_hash_a_validity = LFDS_MISC_VALIDITY_VALID;

  for( loop = 0 ; *lfds_hash_a_validity == LFDS_MISC_VALIDITY_VALID and loop < has->array_size ; loop++ )
    lfds_btree_au_query( has->baus_array+loop, LFDS_BTREE_AU_QUERY_SINGLETHREADED_VALIDATE, NULL, (void *) lfds_hash_a_validity );

  if( *lfds_hash_a_validity == LFDS_MISC_VALIDITY_VALID )
  {
    for( loop = 0 ; loop < has->array_size ; loop++ )
    {
      lfds_btree_au_query( has->baus_array+loop, LFDS_BTREE_AU_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, (void *) &number_elements );
      lfds_btree_au_total_number_elements += number_elements;
    }

    // TRD : first, check btree_addonly_unbalanced total vs the addonly_hash total
    lfds_hash_a_query( has, LFDS_HASH_A_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, &lfds_hash_a_total_number_elements );

    // TRD : the btree_addonly_unbalanceds are assumed to speak the truth
    if( lfds_hash_a_total_number_elements < lfds_btree_au_total_number_elements )
      *lfds_hash_a_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;

    if( lfds_hash_a_total_number_elements > lfds_btree_au_total_number_elements )
      *lfds_hash_a_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

    // TRD : second, if we're still valid and vi is provided, check the btree_addonly_unbalanced total against vi
    if( *lfds_hash_a_validity == LFDS_MISC_VALIDITY_VALID and vi != NULL )
    {
      if( lfds_btree_au_total_number_elements < vi->min_elements )
        *lfds_hash_a_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

      if( lfds_btree_au_total_number_elements > vi->max_elements )
        *lfds_hash_a_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
    }
  }

  return;
}

