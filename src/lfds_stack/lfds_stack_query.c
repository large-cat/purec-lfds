/***** includes *****/
#include "lfds_stack_internal.h"

/***** private prototypes *****/
static void lfds_stack_internal_stack_validate( struct lfds_stack_state *ss,
                                                   struct lfds_misc_validation_info *vi,
                                                   enum lfds_misc_validity *lfds_stack_validity );





/****************************************************************************/
void lfds_stack_query( struct lfds_stack_state *ss,
                          enum lfds_stack_query query_type,
                          void *query_input,
                          void *query_output )
{
  struct lfds_stack_element
    *se;

  LFDS_MISC_BARRIER_LOAD;

  LFDS_PAL_ASSERT( ss != NULL );
  // TRD : query_type can be any value in its range

  switch( query_type )
  {
    case LFDS_STACK_QUERY_SINGLETHREADED_GET_COUNT:
      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      *(lfds_pal_uint_t *) query_output = 0;

      se = (struct lfds_stack_element *) ss->top[POINTER];

      while( se != NULL )
      {
        ( *(lfds_pal_uint_t *) query_output )++;
        se = (struct lfds_stack_element *) se->next;
      }
    break;

    case LFDS_STACK_QUERY_SINGLETHREADED_VALIDATE:
      // TRD : query_input can be NULL
      LFDS_PAL_ASSERT( query_output != NULL );

      lfds_stack_internal_stack_validate( ss, (struct lfds_misc_validation_info *) query_input, (enum lfds_misc_validity *) query_output );
    break;
  }

  return;
}





/****************************************************************************/
static void lfds_stack_internal_stack_validate( struct lfds_stack_state *ss,
                                                   struct lfds_misc_validation_info *vi,
                                                   enum lfds_misc_validity *lfds_stack_validity )
{
  lfds_pal_uint_t
    number_elements = 0;

  struct lfds_stack_element
    *se_fast,
    *se_slow;

  LFDS_PAL_ASSERT( ss != NULL );
  // TRD : vi can be NULL
  LFDS_PAL_ASSERT( lfds_stack_validity != NULL );

  *lfds_stack_validity = LFDS_MISC_VALIDITY_VALID;

  se_slow = se_fast = (struct lfds_stack_element *) ss->top[POINTER];

  /* TRD : first, check for a loop
           we have two pointers
           both of which start at the top of the stack
           we enter a loop
           and on each iteration
           we advance one pointer by one element
           and the other by two

           we exit the loop when both pointers are NULL
           (have reached the end of the stack)

           or

           if we fast pointer 'sees' the slow pointer
           which means we have a loop
  */

  if( se_slow != NULL )
    do
    {
      se_slow = se_slow->next;

      if( se_fast != NULL )
        se_fast = se_fast->next;

      if( se_fast != NULL )
        se_fast = se_fast->next;
    }
    while( se_slow != NULL and se_fast != se_slow );

  if( se_fast != NULL and se_slow != NULL and se_fast == se_slow )
    *lfds_stack_validity = LFDS_MISC_VALIDITY_INVALID_LOOP;

  /* TRD : now check for expected number of elements
           vi can be NULL, in which case we do not check
           we know we don't have a loop from our earlier check
  */

  if( *lfds_stack_validity == LFDS_MISC_VALIDITY_VALID and vi != NULL )
  {
    lfds_stack_query( ss, LFDS_STACK_QUERY_SINGLETHREADED_GET_COUNT, NULL, (void *) &number_elements );

    if( number_elements < vi->min_elements )
      *lfds_stack_validity = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( number_elements > vi->max_elements )
      *lfds_stack_validity = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  return;
}

