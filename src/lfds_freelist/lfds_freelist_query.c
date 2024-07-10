/***** includes *****/
#include "lfds_freelist_internal.h"

/***** private prototypes *****/
static void lfds_freelist_internal_freelist_validate( struct lfds_freelist_state *fs,
                                                         struct lfds_misc_validation_info *vi,
                                                         enum lfds_misc_validity *lfds_freelist_validity );





/****************************************************************************/
void lfds_freelist_query( struct lfds_freelist_state *fs,
                             enum lfds_freelist_query query_type,
                             void *query_input,
                             void *query_output )
{
  struct lfds_freelist_element
    *fe;

  LFDS_PAL_ASSERT( fs != NULL );
  // TRD : query_type can be any value in its range

  LFDS_MISC_BARRIER_LOAD;

  switch( query_type )
  {
    case LFDS_FREELIST_QUERY_SINGLETHREADED_GET_COUNT:
    {
      lfds_pal_uint_t
        loop,
        subloop;

      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      *(lfds_pal_uint_t *) query_output = 0;

      // TRD : count the elements in the elimination array
      for( loop = 0 ; loop < fs->elimination_array_size_in_elements ; loop++ )
        for( subloop = 0 ; subloop < LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS ; subloop++ )
          if( fs->elimination_array[loop][subloop] != NULL )
            ( *(lfds_pal_uint_t *) query_output )++;

      // TRD : count the elements on the freelist
      fe = (struct lfds_freelist_element *) fs->top[POINTER];

      while( fe != NULL )
      {
        ( *(lfds_pal_uint_t *) query_output )++;
        fe = (struct lfds_freelist_element *) fe->next;
      }
    }
    break;

    case LFDS_FREELIST_QUERY_SINGLETHREADED_VALIDATE:
      // TRD : query_input can be NULL
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_freelist_internal_freelist_validate( fs, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output );
    break;

    case LFDS_FREELIST_QUERY_GET_ELIMINATION_ARRAY_EXTRA_ELEMENTS_IN_FREELIST_ELEMENTS:
    {
      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      ( *(lfds_pal_uint_t *) query_output ) = (fs->elimination_array_size_in_elements-1) * LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS;
    }
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_freelist_internal_freelist_validate( struct lfds_freelist_state *fs,
                                                         struct lfds_misc_validation_info *vi,
                                                         enum lfds_misc_validity *lfds_freelist_validity )
{
  lfds_pal_uint_t
    number_elements = 0;

  struct lfds_freelist_element
    *fe_slow,
    *fe_fast;

  LFDS_PAL_ASSERT( fs != NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_freelist_validity != NULL );

  *lfds_freelist_validity = LFDS_MISC_VALIDITY_VALID;

  fe_slow = fe_fast = (struct lfds_freelist_element *) fs->top[POINTER];

  /* TRD : first, check for a loop
           we have two pointers
           both of which start at the top of the freelist
           we enter a loop
           and on each iteration
           we advance one pointer by one element
           and the other by two

           we exit the loop when both pointers are NULL
           (have reached the end of the freelist)

           or

           if we fast pointer 'sees' the slow pointer
           which means we have a loop
  */

  if( fe_slow != NULL )
    do
    {
      fe_slow = fe_slow->next;

      if( fe_fast != NULL )
        fe_fast = fe_fast->next;

      if( fe_fast != NULL )
        fe_fast = fe_fast->next;
    }
    while( fe_slow != NULL and fe_fast != fe_slow );

  if( fe_fast != NULL and fe_slow != NULL and fe_fast == fe_slow )
    *lfds_freelist_validity = LFDS_MISC_VALIDITY_INVALID_LOOP;

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds_freelist_validity == LFDS_MISC_VALIDITY_VALID and vi != NULL )
  {
    lfds_freelist_query( fs, LFDS_FREELIST_QUERY_SINGLETHREADED_GET_COUNT, NULL, (void *) &number_elements );

    if( number_elements < vi->min_elements )
      *lfds_freelist_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( number_elements > vi->max_elements )
      *lfds_freelist_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  return;
}

