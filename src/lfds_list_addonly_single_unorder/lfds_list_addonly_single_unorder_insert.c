/***** includes *****/
#include "lfds_list_addonly_single_unorder_internal.h"





/****************************************************************************/
void lfds_list_asu_insert_at_position( struct lfds_list_asu_state *lasus,
                                          struct lfds_list_asu_element *lasue,
                                          struct lfds_list_asu_element *lasue_predecessor,
                                          enum lfds_list_asu_position position )
{
  LFDS_PAL_ASSERT( lasus != NULL );
  LFDS_PAL_ASSERT( lasue != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->next % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->value % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  // TRD : lasue_predecessor asserted in the switch
  // TRD : position can be any value in its range

  switch( position )
  {
    case LFDS_LIST_ASU_POSITION_START:
      lfds_list_asu_insert_at_start( lasus, lasue );
    break;

    case LFDS_LIST_ASU_POSITION_END:
      lfds_list_asu_insert_at_end( lasus, lasue );
    break;

    case LFDS_LIST_ASU_POSITION_AFTER:
      lfds_list_asu_insert_after_element( lasus, lasue, lasue_predecessor );
    break;
  }

  return;
}





/****************************************************************************/
void lfds_list_asu_insert_at_start( struct lfds_list_asu_state *lasus,
                                       struct lfds_list_asu_element *lasue )
{
  char unsigned 
    result;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  LFDS_PAL_ASSERT( lasus != NULL );
  LFDS_PAL_ASSERT( lasue != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->next % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->value % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );

  LFDS_MISC_BARRIER_LOAD;

  lasue->next = lasus->start->next;

  do
  {
    LFDS_MISC_BARRIER_STORE;
    LFDS_PAL_ATOMIC_CAS( &lasus->start->next, (struct lfds_list_asu_element **) &lasue->next, lasue, LFDS_MISC_CAS_STRENGTH_WEAK, result );
    if( result == 0 )
      LFDS_BACKOFF_EXPONENTIAL_BACKOFF( lasus->start_backoff, backoff_iteration );
  }
  while( result == 0 );

  LFDS_BACKOFF_AUTOTUNE( lasus->start_backoff, backoff_iteration );

  return;
}





/****************************************************************************/
void lfds_list_asu_insert_at_end( struct lfds_list_asu_state *lasus,
                                     struct lfds_list_asu_element *lasue )
{
  char unsigned 
    result;

  enum lfds_misc_flag
    finished_flag = LFDS_MISC_FLAG_LOWERED;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  struct lfds_list_asu_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *compare;

  struct lfds_list_asu_element
    *volatile lasue_next,
    *volatile lasue_end;

  LFDS_PAL_ASSERT( lasus != NULL );
  LFDS_PAL_ASSERT( lasue != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->next % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->value % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );

  /* TRD : begin by assuming end is correctly pointing to the final element
           try to link (comparing for next being NULL)
           if we fail, move down list till we find last element
           and retry
           when successful, update end to ourselves

           note there's a leading dummy element
           so lasus->end always points to an element
  */

  LFDS_MISC_BARRIER_LOAD;

  lasue->next = NULL;
  lasue_end = lasus->end;

  while( finished_flag == LFDS_MISC_FLAG_LOWERED )
  {
    compare = NULL;

    LFDS_MISC_BARRIER_STORE;
    LFDS_PAL_ATOMIC_CAS( &lasue_end->next, &compare, lasue, LFDS_MISC_CAS_STRENGTH_STRONG, result );

    if( result == 1 )
      finished_flag = LFDS_MISC_FLAG_RAISED;
    else
    {
      LFDS_BACKOFF_EXPONENTIAL_BACKOFF( lasus->end_backoff, backoff_iteration );

      lasue_end = compare;
      lasue_next = LFDS_LIST_ASU_GET_NEXT( *lasue_end );

      while( lasue_next != NULL )
      {
        lasue_end = lasue_next;
        lasue_next = LFDS_LIST_ASU_GET_NEXT( *lasue_end );
      }
    }
  }

  lasus->end = lasue;

  LFDS_BACKOFF_AUTOTUNE( lasus->end_backoff, backoff_iteration );

  return;
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void lfds_list_asu_insert_after_element( struct lfds_list_asu_state *lasus,
                                            struct lfds_list_asu_element *lasue,
                                            struct lfds_list_asu_element *lasue_predecessor )
{
  char unsigned 
    result;

  lfds_pal_uint_t
    backoff_iteration = LFDS_BACKOFF_INITIAL_VALUE;

  LFDS_PAL_ASSERT( lasus != NULL );
  LFDS_PAL_ASSERT( lasue != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->next % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &lasue->value % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  LFDS_PAL_ASSERT( lasue_predecessor != NULL );

  LFDS_MISC_BARRIER_LOAD;

  lasue->next = lasue_predecessor->next;

  do
  {
    LFDS_MISC_BARRIER_STORE;
    LFDS_PAL_ATOMIC_CAS( &lasue_predecessor->next, (struct lfds_list_asu_element **) &lasue->next, lasue, LFDS_MISC_CAS_STRENGTH_WEAK, result );
    if( result == 0 )
      LFDS_BACKOFF_EXPONENTIAL_BACKOFF( lasus->after_backoff, backoff_iteration );
  }
  while( result == 0 );

  LFDS_BACKOFF_AUTOTUNE( lasus->after_backoff, backoff_iteration );

  return;
}

#pragma warning( default : 4100 )

