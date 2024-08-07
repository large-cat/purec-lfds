/***** includes *****/
#include "lfds_btree_addonly_unbalanced_internal.h"

/***** private prototypes *****/
static void lfds_btree_au_internal_validate( struct lfds_btree_au_state *abs, struct lfds_misc_validation_info *vi, enum lfds_misc_validity *lfds_btree_au_validity );





/****************************************************************************/
void lfds_btree_au_query( struct lfds_btree_au_state *baus,
                             enum lfds_btree_au_query query_type,
                             void *query_input,
                             void *query_output )
{
  LFDS_PAL_ASSERT( baus != NULL );
  // TRD : query_type can be any value in its range

  LFDS_MISC_BARRIER_LOAD;

  switch( query_type )
  {
    case LFDS_BTREE_AU_QUERY_GET_POTENTIALLY_INACCURATE_COUNT:
    {
      struct lfds_btree_au_element
        *baue = NULL;

      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      *(lfds_pal_uint_t *) query_output = 0;

      while( btree_au_get_abs_rel_pos(baus, &baue, LFDS_BTREE_AU_ABSOLUTE_POSITION_SMALLEST_IN_TREE, LFDS_BTREE_AU_RELATIVE_POSITION_NEXT_LARGER_ELEMENT_IN_ENTIRE_TREE) )
        ( *(lfds_pal_uint_t *) query_output )++;
    }
    break;

    case LFDS_BTREE_AU_QUERY_SINGLETHREADED_VALIDATE:
      // TRD : query_input can be NULL
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_btree_au_internal_validate( baus, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output );
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_btree_au_internal_validate( struct lfds_btree_au_state *baus,
                                                struct lfds_misc_validation_info *vi,
                                                enum lfds_misc_validity *lfds_btree_au_validity )
{
  lfds_pal_uint_t
    number_elements_from_query_tree = 0,
    number_elements_from_walk = 0;

  struct lfds_btree_au_element
    *baue = NULL,
    *baue_prev = NULL;

  LFDS_PAL_ASSERT( baus!= NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_btree_au_validity != NULL );

  *lfds_btree_au_validity = LFDS_MISC_VALIDITY_VALID;

  /* TRD : validation is performed by;

           performing an in-order walk
           we should see every element is larger than the preceeding element
           we count elements as we go along (visited elements, that is)
           and check our tally equals the expected count
  */

  LFDS_MISC_BARRIER_LOAD;

  while( btree_au_get_abs_rel_pos(baus, &baue, LFDS_BTREE_AU_ABSOLUTE_POSITION_SMALLEST_IN_TREE, LFDS_BTREE_AU_RELATIVE_POSITION_NEXT_LARGER_ELEMENT_IN_ENTIRE_TREE) )
  {
    // TRD : baue_prev should always be smaller than or equal to baue
    if( baue_prev != NULL )
      if( baus->key_compare_function(baue_prev->key, baue->key) > 0 )
      {
        *lfds_btree_au_validity = LFDS_MISC_VALIDITY_INVALID_ORDER;
        return;
      }

    baue_prev = baue;
    number_elements_from_walk++;
  }

  if( *lfds_btree_au_validity == LFDS_MISC_VALIDITY_VALID )
  {
    lfds_btree_au_query( (struct lfds_btree_au_state *) baus, LFDS_BTREE_AU_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, &number_elements_from_query_tree );

    if( number_elements_from_walk > number_elements_from_query_tree )
      *lfds_btree_au_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;

    if( number_elements_from_walk < number_elements_from_query_tree )
      *lfds_btree_au_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;
  }

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds_btree_au_validity == LFDS_MISC_VALIDITY_VALID and vi != NULL )
  {
    lfds_btree_au_query( (struct lfds_btree_au_state *) baus, LFDS_BTREE_AU_QUERY_GET_POTENTIALLY_INACCURATE_COUNT, NULL, &number_elements_from_query_tree );

    if( number_elements_from_query_tree < vi->min_elements )
      *lfds_btree_au_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( number_elements_from_query_tree > vi->max_elements )
      *lfds_btree_au_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  return;
}

