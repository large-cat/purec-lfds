/***** includes *****/
#include "lfds_hash_addonly_internal.h"





/****************************************************************************/
enum lfds_hash_a_insert_result lfds_hash_a_insert( struct lfds_hash_a_state *has,
                                                         struct lfds_hash_a_element *hae,
                                                         struct lfds_hash_a_element **existing_hae )
{
  enum lfds_hash_a_insert_result
    apr = LFDS_HASH_A_PUT_RESULT_SUCCESS;

  enum lfds_btree_au_insert_result
    alr;

  lfds_pal_uint_t
    hash = 0;

  struct lfds_btree_au_element
    *existing_baue;

  LFDS_PAL_ASSERT( has != NULL );
  LFDS_PAL_ASSERT( hae != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &hae->value % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );
  // TRD : existing_hae can be NULL

  // TRD : alignment checks
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &hae->baue % LFDS_PAL_ALIGN_SINGLE_POINTER == 0 );

  has->key_hash_function( hae->key, &hash );

  LFDS_BTREE_AU_SET_KEY_IN_ELEMENT( hae->baue, hae->key );
  LFDS_BTREE_AU_SET_VALUE_IN_ELEMENT( hae->baue, hae );

  alr = lfds_btree_au_insert( has->baus_array + (hash % has->array_size), &hae->baue, &existing_baue );

  switch( alr )
  {
    case LFDS_BTREE_AU_INSERT_RESULT_FAILURE_EXISTING_KEY:
      if( existing_hae != NULL )
        *existing_hae = LFDS_BTREE_AU_GET_VALUE_FROM_ELEMENT( *existing_baue );

      apr = LFDS_HASH_A_PUT_RESULT_FAILURE_EXISTING_KEY;
    break;

    case LFDS_BTREE_AU_INSERT_RESULT_SUCCESS_OVERWRITE:
      apr = LFDS_HASH_A_PUT_RESULT_SUCCESS_OVERWRITE;
    break;

    case LFDS_BTREE_AU_INSERT_RESULT_SUCCESS:
      apr = LFDS_HASH_A_PUT_RESULT_SUCCESS;
    break;
  }

  return apr;
}

