/***** includes *****/
#include "libtest_tests_internal.h"

/***** structs *****/
struct test_element
{
  struct lfds_btree_au_element
    baue;

  lfds_pal_uint_t
    datum;
};

/***** private prototypes *****/
static int key_compare_function( void const *new_key, void const *existing_key );
static void key_hash_function( void const *key, lfds_pal_uint_t *hash );





/****************************************************************************/
#pragma warning( disable : 4100 )

void libtest_tests_hash_a_iterate( struct lfds_list_asu_state *list_of_logical_processors, struct libshared_memory_state *ms, enum lfds_misc_validity *dvs )
{
  lfds_pal_uint_t
    *counter_array,
    loop;

  struct lfds_hash_a_element
    *hae;

  struct lfds_hash_a_iterate
    hai;

  struct lfds_hash_a_state
    has;

  struct lfds_hash_a_element
    *element_array;

  struct lfds_btree_au_state
    *baus,
    *baus_thousand;

  void
    *value;

  LFDS_PAL_ASSERT( list_of_logical_processors != NULL );
  LFDS_PAL_ASSERT( ms != NULL );
  LFDS_PAL_ASSERT( dvs != NULL );

  /* TRD : single-threaded test
           we create a single hash_a
           we populate with 1000 elements
           where key and value is the number of the element (e.g. 0 to 999)
           we then allocate 1000 counters, init to 0
           we then iterate
           we increment each element as we see it in the iterate
           if any are missing or seen more than once, problemo!

           we do this once with a table of 10, to ensure each table has (or almost certainly has) something in
           and then a second tiem with a table of 10000, to ensure some empty tables exist
  */

  *dvs = LFDS_MISC_VALIDITY_VALID;

  counter_array = libshared_memory_alloc_from_unknown_node( ms, sizeof(lfds_pal_uint_t) * 1000, sizeof(lfds_pal_uint_t) );
  element_array = libshared_memory_alloc_from_unknown_node( ms, sizeof(struct lfds_hash_a_element) * 1000, LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES );
  baus = libshared_memory_alloc_from_unknown_node( ms, sizeof(struct lfds_btree_au_state) * 10, LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES );
  baus_thousand = libshared_memory_alloc_from_unknown_node( ms, sizeof(struct lfds_btree_au_state) * 1000, LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES );

  // TRD : first time around
  lfds_hash_a_init_core( &has, baus, 10, key_compare_function, key_hash_function, LFDS_HASH_A_EXISTING_KEY_FAIL, NULL );

  for( loop = 0 ; loop < 1000 ; loop++ )
  {
    LFDS_HASH_A_SET_KEY_IN_ELEMENT( *(element_array+loop), loop );
    LFDS_HASH_A_SET_VALUE_IN_ELEMENT( *(element_array+loop), loop );
    lfds_hash_a_insert( &has, element_array+loop, NULL );
  }

  for( loop = 0 ; loop < 1000 ; loop++ )
    *(counter_array+loop) = 0;

  lfds_hash_a_iterate_init( &has, &hai );

  while( *dvs == LFDS_MISC_VALIDITY_VALID and lfds_hash_a_iterate(&hai, &hae) )
  {
    value = LFDS_HASH_A_GET_VALUE_FROM_ELEMENT( *hae );
    ( *(counter_array + (lfds_pal_uint_t) value) )++;
  }

  if( *dvs == LFDS_MISC_VALIDITY_VALID )
    for( loop = 0 ; loop < 1000 ; loop++ )
    {
      if( *(counter_array+loop) > 1 )
        *dvs = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;

      if( *(counter_array+loop) == 0 )
        *dvs = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;
    }

  lfds_hash_a_cleanup( &has, NULL );

  // TRD : second time around
  if( *dvs == LFDS_MISC_VALIDITY_VALID )
  {
    lfds_hash_a_init_core( &has, baus_thousand, 10000, key_compare_function, key_hash_function, LFDS_HASH_A_EXISTING_KEY_FAIL, NULL );

    for( loop = 0 ; loop < 1000 ; loop++ )
    {
      LFDS_HASH_A_SET_KEY_IN_ELEMENT( *(element_array+loop), loop );
      LFDS_HASH_A_SET_VALUE_IN_ELEMENT( *(element_array+loop), loop );
      lfds_hash_a_insert( &has, element_array+loop, NULL );
    }

    for( loop = 0 ; loop < 1000 ; loop++ )
      *(counter_array+loop) = 0;

    lfds_hash_a_iterate_init( &has, &hai );

    while( *dvs == LFDS_MISC_VALIDITY_VALID and lfds_hash_a_iterate(&hai, &hae) )
    {
      value = LFDS_HASH_A_GET_VALUE_FROM_ELEMENT( *hae );
      ( *(counter_array + (lfds_pal_uint_t) value ) )++;
    }

    if( *dvs == LFDS_MISC_VALIDITY_VALID )
      for( loop = 0 ; loop < 1000 ; loop++ )
      {
        if( *(counter_array+loop) > 1 )
          *dvs = LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;

        if( *(counter_array+loop) == 0 )
          *dvs = LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS;
      }

    lfds_hash_a_cleanup( &has, NULL );
  }

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
static int key_compare_function( void const *new_key, void const *existing_key )
{
  int
    cr = 0;

  // TRD : new_key can be NULL (i.e. 0)
  // TRD : existing_key can be NULL (i.e. 0)

  if( (lfds_pal_uint_t) new_key < (lfds_pal_uint_t) existing_key )
    cr = -1;

  if( (lfds_pal_uint_t) new_key > (lfds_pal_uint_t) existing_key )
    cr = 1;

  return cr;
}





/****************************************************************************/
static void key_hash_function( void const *key, lfds_pal_uint_t *hash )
{
  // TRD : key can be NULL
  LFDS_PAL_ASSERT( hash != NULL );

  *hash = 0;

  /* TRD : this function iterates over the user data
           and we are using the void pointer AS user data
           so here we need to pass in the addy of value
  */

  LFDS_HASH_A_HASH_FUNCTION( (void *) &key, sizeof(lfds_pal_uint_t), *hash );

  return;
}

