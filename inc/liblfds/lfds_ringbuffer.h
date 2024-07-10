/***** enums *****/
#define LFDS_RINGBUFFER_GET_USER_STATE_FROM_STATE( ringbuffer_state )  ( (ringbuffer_state).user_state )

/***** enums *****/
enum lfds_ringbuffer_query
{
  LFDS_RINGBUFFER_QUERY_SINGLETHREADED_GET_COUNT,
  LFDS_RINGBUFFER_QUERY_SINGLETHREADED_VALIDATE
};

/***** structures *****/
struct lfds_ringbuffer_element
{
  struct lfds_freelist_element
    fe;

  struct lfds_queue_umm_element
    qumme;

  struct lfds_queue_umm_element
    *qumme_use; // TRD : hack; we need a new queue with no dummy element

  void
    *key,
    *value;
};

struct lfds_ringbuffer_state
{
  struct lfds_freelist_state
    fs;

  struct lfds_queue_umm_state
    qumms;

  void
    (*element_cleanup_callback)( struct lfds_ringbuffer_state *rs, void *key, void *value, enum lfds_misc_flag unread_flag ),
    *user_state;
};

/***** public prototypes *****/
void lfds_ringbuffer_init_valid_on_current_logical_core( struct lfds_ringbuffer_state *rs,
                                                            struct lfds_ringbuffer_element *re_array_inc_dummy,
                                                            lfds_pal_uint_t number_elements_inc_dummy,
                                                            void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_ringbuffer_cleanup( struct lfds_ringbuffer_state *rs,
                                 void (*element_cleanup_callback)(struct lfds_ringbuffer_state *rs, void *key, void *value, enum lfds_misc_flag unread_flag) );

int lfds_ringbuffer_read( struct lfds_ringbuffer_state *rs,
                             void **key,
                             void **value );

void lfds_ringbuffer_write( struct lfds_ringbuffer_state *rs,
                               void *key,
                               void *value,
                               enum lfds_misc_flag *overwrite_occurred_flag,
                               void **overwritten_key,
                               void **overwritten_value );

void lfds_ringbuffer_query( struct lfds_ringbuffer_state *rs,
                               enum lfds_ringbuffer_query query_type,
                               void *query_input,
                               void *query_output );

