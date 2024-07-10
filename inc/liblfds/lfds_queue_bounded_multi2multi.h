/***** defines *****/
#define LFDS_QUEUE_BMM_GET_USER_STATE_FROM_STATE( queue_bmm_state )  ( (queue_bmm_state).user_state )

/***** enums *****/
enum lfds_queue_bmm_query
{
  LFDS_QUEUE_BMM_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
  LFDS_QUEUE_BMM_QUERY_SINGLETHREADED_VALIDATE
};

/***** structures *****/
struct lfds_queue_bmm_element
{
  lfds_pal_uint_t volatile
    sequence_number;

  void
    *volatile key,
    *volatile value;
};

struct lfds_queue_bmm_state
{
  lfds_pal_uint_t
    number_elements,
    mask;

  lfds_pal_uint_t volatile LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    read_index,
    write_index;

  struct lfds_queue_bmm_element
    *element_array;

  void
    *user_state;

  struct lfds_misc_backoff_state
    dequeue_backoff,
    enqueue_backoff;
};

/***** public prototypes *****/
void lfds_queue_bmm_init_core( struct lfds_queue_bmm_state *qbmms,
                                                           struct lfds_queue_bmm_element *element_array,
                                                           lfds_pal_uint_t number_elements,
                                                           void *user_state );

void lfds_queue_bmm_cleanup( struct lfds_queue_bmm_state *qbmms,
                                void (*element_cleanup_callback)(struct lfds_queue_bmm_state *qbmms,
                                                                 void *key,
                                                                 void *value) );

int lfds_queue_bmm_enqueue( struct lfds_queue_bmm_state *qbmms,
                               void *key,
                               void *value );

int lfds_queue_bmm_dequeue( struct lfds_queue_bmm_state *qbmms,
                                      void **key,
                                      void **value );

void lfds_queue_bmm_query( struct lfds_queue_bmm_state *qbmms,
                              enum lfds_queue_bmm_query query_type,
                              void *query_input,
                              void *query_output );

