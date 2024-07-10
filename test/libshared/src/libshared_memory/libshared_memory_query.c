/***** includes *****/
#include "libshared_memory_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 )

void libshared_memory_query( struct libshared_memory_state *ms, enum libshared_memory_query query_type, void *query_input, void *query_output )
{
  LFDS_PAL_ASSERT( ms != NULL );
  // TRD : query_type can be any value in its range

  LFDS_MISC_BARRIER_LOAD;

  switch( query_type )
  {
    case LIBSHARED_MEMORY_QUERY_GET_AVAILABLE:
    {
      lfds_pal_uint_t
        available_bytes = 0;

      struct lfds_list_asu_element
        *lasue = NULL;

      struct libshared_memory_element
        *me;

      LFDS_PAL_ASSERT( query_input == NULL );
      LFDS_PAL_ASSERT( query_output != NULL );

      while( LFDS_LIST_ASU_GET_START_AND_THEN_NEXT(ms->list_of_allocations,lasue) )
      {
        me = LFDS_LIST_ASU_GET_VALUE_FROM_ELEMENT( *lasue );
        available_bytes += me->current_memory_size_in_bytes;
      }

      *(lfds_pal_uint_t *) query_output = available_bytes;
    }
    break;
  }

  return;
}

#pragma warning( default : 4100 )

