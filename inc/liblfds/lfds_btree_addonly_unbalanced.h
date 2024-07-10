/***** defines *****/
#define LFDS_BTREE_AU_GET_KEY_FROM_ELEMENT( btree_au_element )             ( (btree_au_element).key )
#define LFDS_BTREE_AU_SET_KEY_IN_ELEMENT( btree_au_element, new_key )      ( (btree_au_element).key = (void *) (lfds_pal_uint_t) (new_key) )
#define LFDS_BTREE_AU_GET_VALUE_FROM_ELEMENT( btree_au_element )           ( LFDS_MISC_BARRIER_LOAD, (btree_au_element).value )
#define LFDS_BTREE_AU_SET_VALUE_IN_ELEMENT( btree_au_element, new_value )  { LFDS_PAL_ATOMIC_SET( &(btree_au_element).value, new_value ); }
#define LFDS_BTREE_AU_GET_USER_STATE_FROM_STATE( btree_au_state )          ( (btree_au_state).user_state )

/***** enums *****/
enum lfds_btree_au_absolute_position
{
  LFDS_BTREE_AU_ABSOLUTE_POSITION_ROOT,
  LFDS_BTREE_AU_ABSOLUTE_POSITION_SMALLEST_IN_TREE,
  LFDS_BTREE_AU_ABSOLUTE_POSITION_LARGEST_IN_TREE
};

enum lfds_btree_au_existing_key
{
  LFDS_BTREE_AU_EXISTING_KEY_OVERWRITE,
  LFDS_BTREE_AU_EXISTING_KEY_FAIL
};

enum lfds_btree_au_insert_result
{
  LFDS_BTREE_AU_INSERT_RESULT_FAILURE_EXISTING_KEY,
  LFDS_BTREE_AU_INSERT_RESULT_SUCCESS_OVERWRITE,
  LFDS_BTREE_AU_INSERT_RESULT_SUCCESS
};

enum lfds_btree_au_query
{
  LFDS_BTREE_AU_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
  LFDS_BTREE_AU_QUERY_SINGLETHREADED_VALIDATE
};

enum lfds_btree_au_relative_position
{
  LFDS_BTREE_AU_RELATIVE_POSITION_UP,
  LFDS_BTREE_AU_RELATIVE_POSITION_LEFT,
  LFDS_BTREE_AU_RELATIVE_POSITION_RIGHT,
  LFDS_BTREE_AU_RELATIVE_POSITION_SMALLEST_ELEMENT_BELOW_CURRENT_ELEMENT,
  LFDS_BTREE_AU_RELATIVE_POSITION_LARGEST_ELEMENT_BELOW_CURRENT_ELEMENT,
  LFDS_BTREE_AU_RELATIVE_POSITION_NEXT_SMALLER_ELEMENT_IN_ENTIRE_TREE,
  LFDS_BTREE_AU_RELATIVE_POSITION_NEXT_LARGER_ELEMENT_IN_ENTIRE_TREE
};

/***** structs *****/
struct lfds_btree_au_element
{
  /* TRD : we are add-only, so these elements are only written once
           as such, the write is wholly negligible
           we are only concerned with getting as many structs in one cache line as we can
  */

  struct lfds_btree_au_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile left,
    *volatile right,
    *volatile up;

  void LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile value;

  void
    *key;
};

struct lfds_btree_au_state
{
  struct lfds_btree_au_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile root;

  int
    (*key_compare_function)( void const *new_key, void const *existing_key );

  enum lfds_btree_au_existing_key 
    existing_key;

  void
    *user_state;

  struct lfds_misc_backoff_state
    insert_backoff;
};

/***** public prototypes *****/
void btree_au_init_core( struct lfds_btree_au_state *baus,
                                                          int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                          enum lfds_btree_au_existing_key existing_key,
                                                          void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_btree_au_cleanup( struct lfds_btree_au_state *baus,
                               void (*element_cleanup_callback)(struct lfds_btree_au_state *baus, struct lfds_btree_au_element *baue) );

enum lfds_btree_au_insert_result lfds_btree_au_insert( struct lfds_btree_au_state *baus,
                                                             struct lfds_btree_au_element *baue,
                                                             struct lfds_btree_au_element **existing_baue );
  // TRD : if a link collides with an existing key and existing_baue is non-NULL, existing_baue is set to the existing element

int lfds_btree_au_get_by_key( struct lfds_btree_au_state *baus, 
                                 int (*key_compare_function)(void const *new_key, void const *existing_key),
                                 void *key,
                                 struct lfds_btree_au_element **baue );

int btree_au_get_abs_rel_pos( struct lfds_btree_au_state *baus,
                                                                             struct lfds_btree_au_element **baue,
                                                                             enum lfds_btree_au_absolute_position absolute_position,
                                                                             enum lfds_btree_au_relative_position relative_position );
  // TRD : if *baue is NULL, we get the element at position, otherwise we move from *baue according to direction

int btree_au_get_abs_pos( struct lfds_btree_au_state *baus,
                                               struct lfds_btree_au_element **baue,
                                               enum lfds_btree_au_absolute_position absolute_position );

int btree_au_get_rel_pos( struct lfds_btree_au_element **baue,
                                               enum lfds_btree_au_relative_position relative_position );

void lfds_btree_au_query( struct lfds_btree_au_state *baus,
                             enum lfds_btree_au_query query_type,
                             void *query_input,
                             void *query_output );

