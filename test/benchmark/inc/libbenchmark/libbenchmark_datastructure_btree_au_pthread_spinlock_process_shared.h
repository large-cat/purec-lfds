/***** defines *****/
#define LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_GET_KEY_FROM_ELEMENT( btree_au_state, btree_au_element )                    ( (btree_au_element).key )
#define LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_SET_KEY_IN_ELEMENT( btree_au_state, btree_au_element, new_key )             ( (btree_au_element).key = (void *) (lfds_pal_uint_t) (new_key) )
#define LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_GET_VALUE_FROM_ELEMENT( btree_au_state, btree_au_element, existing_value )  { LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_SHARED_GET((btree_au_state).lock); existing_value = (btree_au_element).value; LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_SHARED_RELEASE((btree_au_state).lock);                            }
#define LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_SET_VALUE_IN_ELEMENT( btree_au_state, btree_au_element, new_value )         { LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_SHARED_GET((btree_au_state).lock); (btree_au_element).value = (void *) (lfds_pal_uint_t) (new_value); LIBBENCHMARK_PAL_LOCK_PTHREAD_SPINLOCK_PROCESS_SHARED_RELEASE((btree_au_state).lock); }
#define LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_GET_USER_STATE_FROM_STATE( btree_au_state )                                 ( (btree_au_state).user_state )

/***** enums *****/
enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_absolute_position
{
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_ABSOLUTE_POSITION_ROOT,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_ABSOLUTE_POSITION_SMALLEST_IN_TREE,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_ABSOLUTE_POSITION_LARGEST_IN_TREE
};

enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_existing_key
{
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_EXISTING_KEY_OVERWRITE,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_EXISTING_KEY_FAIL
};

enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_insert_result
{
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_INSERT_RESULT_FAILURE_EXISTING_KEY,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_INSERT_RESULT_SUCCESS_OVERWRITE,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_INSERT_RESULT_SUCCESS
};

enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_relative_position
{
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_UP,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_LEFT,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_RIGHT,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_SMALLEST_ELEMENT_BELOW_CURRENT_ELEMENT,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_LARGEST_ELEMENT_BELOW_CURRENT_ELEMENT,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_NEXT_SMALLER_ELEMENT_IN_ENTIRE_TREE,
  LIBBENCHMARK_DATASTRUCTURE_BTREE_AU_PTHREAD_SPINLOCK_PROCESS_SHARED_RELATIVE_POSITION_NEXT_LARGER_ELEMENT_IN_ENTIRE_TREE
};

/***** structs *****/
struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element
{
  struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element
    *left,
    *right,
    *up;

  void
    *key,
    *value;
};

struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state
{
  struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element
    *root;

  pal_lock_pthread_spinlock_process_shared_state LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    lock;

  int
    (*key_compare_function)( void const *new_key, void const *existing_key );

  enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_existing_key 
    existing_key;

  void
    *user_state;
};

/***** public prototypes *****/
void libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_init( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                   int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                                   enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_existing_key existing_key,
                                                                   void *user_state );

void libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_cleanup( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                      void (*element_cleanup_callback)(struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus, struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element *baue) );

enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_insert_result libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_insert( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                                                                                           struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element *baue,
                                                                                                                                           struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element **existing_baue );

int libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_get_by_key( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus, 
                                                                        int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                                        void *key,
                                                                        struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element **baue );

int libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_get_by_absolute_position_and_then_by_relative_position( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                                                                    struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element **baue,
                                                                                                                    enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_absolute_position absolute_position,
                                                                                                                    enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_relative_position relative_position );

int libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_get_by_absolute_position( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                                      struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element **baue,
                                                                                      enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_absolute_position absolute_position );

int libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_get_by_relative_position( struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_state *baus,
                                                                                      struct libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_element **baue,
                                                                                      enum libbenchmark_datastructure_btree_au_pthread_spinlock_process_shared_relative_position relative_position );

