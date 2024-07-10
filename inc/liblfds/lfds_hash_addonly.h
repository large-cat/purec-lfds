#ifndef _LFDS_HASH_ADDONLY_H_
#define _LFDS_HASH_ADDONLY_H_

/***** defines *****/
#define LFDS_HASH_A_GET_KEY_FROM_ELEMENT(hash_a_element) ((hash_a_element).key)
#define LFDS_HASH_A_SET_KEY_IN_ELEMENT(hash_a_element, new_key) ((hash_a_element).key = (void *)(lfds_pal_uint_t)(new_key))
#define LFDS_HASH_A_GET_VALUE_FROM_ELEMENT(hash_a_element) (LFDS_MISC_BARRIER_LOAD, (hash_a_element).value)
#define LFDS_HASH_A_SET_VALUE_IN_ELEMENT(hash_a_element, new_value) \
  {                                                                 \
    LFDS_PAL_ATOMIC_SET(&(hash_a_element).value, new_value);        \
  }
#define LFDS_HASH_A_GET_USER_STATE_FROM_STATE(hash_a_state) ((hash_a_state).user_state)

// TRD : a quality hash function, provided for user convenience - note hash must be initialized to 0 before the first call by the user

#if (LFDS_PAL_ALIGN_SINGLE_POINTER == 4)
// TRD : void *data, lfds_pal_uint_t data_length_in_bytes, lfds_pal_uint_t hash
#define LFDS_HASH_A_HASH_FUNCTION(data, data_length_in_bytes, hash) \
  {                                                                 \
    lfds_pal_uint_t                                                 \
        loop;                                                       \
                                                                    \
    for (loop = 0; loop < (data_length_in_bytes); loop++)           \
    {                                                               \
      (hash) += *((char unsigned *)(data) + loop);                  \
      (hash) = ((hash) ^ ((hash) >> 16)) * 0x85ebca6bUL;            \
      (hash) = ((hash) ^ ((hash) >> 13)) * 0xc2b2ae35UL;            \
      (hash) = (hash ^ (hash >> 16));                               \
    }                                                               \
  }
#endif

#if (LFDS_PAL_ALIGN_SINGLE_POINTER == 8)
// TRD : void *data, lfds_pal_uint_t data_length_in_bytes, lfds_pal_uint_t hash
#define LFDS_HASH_A_HASH_FUNCTION(data, data_length_in_bytes, hash) \
  {                                                                 \
    lfds_pal_uint_t                                                 \
        loop;                                                       \
                                                                    \
    for (loop = 0; loop < (data_length_in_bytes); loop++)           \
    {                                                               \
      (hash) += *((char unsigned *)(data) + loop);                  \
      (hash) = ((hash) ^ ((hash) >> 30)) * 0xBF58476D1CE4E5B9ULL;   \
      (hash) = ((hash) ^ ((hash) >> 27)) * 0x94D049BB133111EBULL;   \
      (hash) = (hash ^ (hash >> 31));                               \
    }                                                               \
  }
#endif

/***** enums *****/
enum lfds_hash_a_existing_key
{
  LFDS_HASH_A_EXISTING_KEY_OVERWRITE,
  LFDS_HASH_A_EXISTING_KEY_FAIL
};

enum lfds_hash_a_insert_result
{
  LFDS_HASH_A_PUT_RESULT_FAILURE_EXISTING_KEY,
  LFDS_HASH_A_PUT_RESULT_SUCCESS_OVERWRITE,
  LFDS_HASH_A_PUT_RESULT_SUCCESS
};

enum lfds_hash_a_query
{
  LFDS_HASH_A_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
  LFDS_HASH_A_QUERY_SINGLETHREADED_VALIDATE
};

/***** structs *****/
struct lfds_hash_a_element
{
  struct lfds_btree_au_element
    baue;

  void
    *key;

  void LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile value;
};

struct lfds_hash_a_iterate
{
  struct lfds_btree_au_element
    *baue;

  struct lfds_btree_au_state
    *baus,
    *baus_end;
};

struct lfds_hash_a_state
{
  enum lfds_hash_a_existing_key
    existing_key;

  int
    (*key_compare_function)( void const *new_key, void const *existing_key );

  lfds_pal_uint_t
    array_size;

  struct lfds_btree_au_state
    *baus_array;

  void
    (*element_cleanup_callback)( struct lfds_hash_a_state *has, struct lfds_hash_a_element *hae ),
    (*key_hash_function)( void const *key, lfds_pal_uint_t *hash ),
    *user_state;
};

/***** public prototypes *****/
void lfds_hash_a_init_core( struct lfds_hash_a_state *has,
                                                        struct lfds_btree_au_state *baus_array,
                                                        lfds_pal_uint_t array_size,
                                                        int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                        void (*key_hash_function)(void const *key, lfds_pal_uint_t *hash),
                                                        enum lfds_hash_a_existing_key existing_key,
                                                        void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_hash_a_cleanup( struct lfds_hash_a_state *has,
                             void (*element_cleanup_function)(struct lfds_hash_a_state *has, struct lfds_hash_a_element *hae) );

enum lfds_hash_a_insert_result lfds_hash_a_insert( struct lfds_hash_a_state *has,
                                                         struct lfds_hash_a_element *hae,
                                                         struct lfds_hash_a_element **existing_hae );
  // TRD : if existing_value is not NULL and the key exists, existing_hae is set to the hash element of the existing key

int lfds_hash_a_get_by_key( struct lfds_hash_a_state *has,
                               int (*key_compare_function)(void const *new_key, void const *existing_key),
                               void (*key_hash_function)(void const *key, lfds_pal_uint_t *hash),
                               void *key,
                               struct lfds_hash_a_element **hae );

void lfds_hash_a_iterate_init( struct lfds_hash_a_state *has, struct lfds_hash_a_iterate *hai );
int lfds_hash_a_iterate( struct lfds_hash_a_iterate *hai, struct lfds_hash_a_element **hae );

void lfds_hash_a_query( struct lfds_hash_a_state *has,
                           enum lfds_hash_a_query query_type,
                           void *query_input,
                           void *query_output );

#endif // _LFDS_HASH_ADDONLY_H_
