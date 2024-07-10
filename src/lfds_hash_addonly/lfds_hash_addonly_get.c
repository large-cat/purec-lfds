/***** includes *****/
#include "lfds_hash_addonly_internal.h"





/****************************************************************************/
int lfds_hash_a_get_by_key( struct lfds_hash_a_state *has,
                               int (*key_compare_function)(void const *new_key, void const *existing_key),
                               void (*key_hash_function)(void const *key, lfds_pal_uint_t *hash),
                               void *key,
                               struct lfds_hash_a_element **hae )
{
  int
    rv;

  lfds_pal_uint_t
    hash = 0;

  struct lfds_btree_au_element
    *baue;

  LFDS_PAL_ASSERT( has != NULL );
  // TRD : key_compare_function can be NULL
  // TRD : key_hash_function can be NULL
  // TRD : key can be NULL
  LFDS_PAL_ASSERT( hae != NULL );

  if( key_compare_function == NULL )
    key_compare_function = has->key_compare_function;

  if( key_hash_function == NULL )
    key_hash_function = has->key_hash_function;

  key_hash_function( key, &hash );

  rv = lfds_btree_au_get_by_key( has->baus_array + (hash % has->array_size), key_compare_function, key, &baue );

  if( rv == 1 )
    *hae = LFDS_BTREE_AU_GET_VALUE_FROM_ELEMENT( *baue );
  else
    *hae = NULL;

  return rv;
}

