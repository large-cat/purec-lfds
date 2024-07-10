/***** includes *****/
#include "lfds_freelist_internal.h"





/****************************************************************************/
void freelist_init_core( struct lfds_freelist_state *fs,
                                                          struct lfds_freelist_element * volatile (*elimination_array)[LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS],
                                                          lfds_pal_uint_t elimination_array_size_in_elements,
                                                          void *user_state )
{
  lfds_pal_uint_t
    loop,
    subloop;

  LFDS_PAL_ASSERT( fs != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) fs->top % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &fs->elimination_array_size_in_elements % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  // TRD : elimination_array can be NULL
  LFDS_PAL_ASSERT( (elimination_array == NULL) or 
                      ( (elimination_array != NULL) and (lfds_pal_uint_t) elimination_array % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 ) );
  LFDS_PAL_ASSERT( (elimination_array == NULL and elimination_array_size_in_elements == 0) or 
                      (elimination_array != NULL and elimination_array_size_in_elements >= 2 and (elimination_array_size_in_elements & (elimination_array_size_in_elements-1)) == 0) );
  // TRD : user_state can be NULL

  fs->top[POINTER] = NULL;
  fs->top[COUNTER] = 0;

  fs->elimination_array = elimination_array;
  fs->elimination_array_size_in_elements = elimination_array_size_in_elements;
  fs->user_state = user_state;

  for( loop = 0 ; loop < elimination_array_size_in_elements ; loop++ )
    for( subloop = 0 ; subloop < LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS ; subloop++ )
      fs->elimination_array[loop][subloop] = NULL;

  lfds_misc_internal_backoff_init( &fs->pop_backoff );
  lfds_misc_internal_backoff_init( &fs->push_backoff );

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}

