#ifndef _LFDS_FREELIST_H_
#define _LFDS_FREELIST_H_

/***** defines *****/
#define LFDS_FREELIST_GET_KEY_FROM_ELEMENT( freelist_element )             ( (freelist_element).key )
#define LFDS_FREELIST_SET_KEY_IN_ELEMENT( freelist_element, new_key )      ( (freelist_element).key = (void *) (lfds_pal_uint_t) (new_key) )
#define LFDS_FREELIST_GET_VALUE_FROM_ELEMENT( freelist_element )           ( (freelist_element).value )
#define LFDS_FREELIST_SET_VALUE_IN_ELEMENT( freelist_element, new_value )  ( (freelist_element).value = (void *) (lfds_pal_uint_t) (new_value) )
#define LFDS_FREELIST_GET_USER_STATE_FROM_STATE( freelist_state )          ( (freelist_state).user_state )

#define LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS  ( LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES / sizeof(struct lfds_freelist_element *) )

/***** enums *****/
enum lfds_freelist_query
{
  LFDS_FREELIST_QUERY_SINGLETHREADED_GET_COUNT,
  LFDS_FREELIST_QUERY_SINGLETHREADED_VALIDATE,
  LFDS_FREELIST_QUERY_GET_ELIMINATION_ARRAY_EXTRA_ELEMENTS_IN_FREELIST_ELEMENTS
};

/***** structures *****/
struct lfds_freelist_element
{
  struct lfds_freelist_element
    *next;

  void
    *key,
    *value;
};

struct lfds_freelist_state
{
  struct lfds_freelist_element LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    *volatile top[PAC_SIZE];

  lfds_pal_uint_t LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    elimination_array_size_in_elements;

  struct lfds_freelist_element * volatile
    (*elimination_array)[LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS];

  void
    *user_state;

  struct lfds_misc_backoff_state
    pop_backoff,
    push_backoff;
};

/***** public prototypes *****/
void lfds_freelist_init_core( struct lfds_freelist_state *fs,
                                                          struct lfds_freelist_element * volatile (*elimination_array)[LFDS_FREELIST_ELIMINATION_ARRAY_ELEMENT_SIZE_IN_FREELIST_ELEMENTS],
                                                          lfds_pal_uint_t elimination_array_size_in_elements,
                                                          void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_freelist_cleanup( struct lfds_freelist_state *fs,
                               void (*element_cleanup_callback)(struct lfds_freelist_state *fs, struct lfds_freelist_element *fe) );

void lfds_freelist_push( struct lfds_freelist_state *fs,
                                   struct lfds_freelist_element *fe,
                                   struct lfds_prng_st_state *psts );

int lfds_freelist_pop( struct lfds_freelist_state *fs,
                          struct lfds_freelist_element **fe,
                          struct lfds_prng_st_state *psts );

void lfds_freelist_query( struct lfds_freelist_state *fs,
                             enum lfds_freelist_query query_type,
                             void *query_input,
                             void *query_output );

#endif // _LFDS_FREELIST_H_
