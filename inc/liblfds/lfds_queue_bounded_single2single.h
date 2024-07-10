/***** defines *****/
#define LFDS_QUEUE_BSS_GET_USER_STATE_FROM_STATE( queue_bss_state )  ( (queue_bss_state).user_state )

/***** enums *****/
enum lfds_queue_bss_query
{
  LFDS_QUEUE_BSS_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
  LFDS_QUEUE_BSS_QUERY_VALIDATE
};

/***** structures *****/
struct lfds_queue_bss_element
{
  void
    *volatile key,
    *volatile value;
};

struct lfds_queue_bss_state
{
  lfds_pal_uint_t
    number_elements,
    mask;

  lfds_pal_uint_t volatile
    read_index,
    write_index;

  struct lfds_queue_bss_element
    *element_array;

  void
    *user_state;
};

/***** public prototypes *****/
void lfds_queue_bss_init_valid_on_current_logical_core( struct lfds_queue_bss_state *qbsss, 
                                                           struct lfds_queue_bss_element *element_array,
                                                           lfds_pal_uint_t number_elements,
                                                           void *user_state );
  // TRD : number_elements must be a positive integer power of 2
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_queue_bss_cleanup( struct lfds_queue_bss_state *qbsss,
                                void (*element_cleanup_callback)(struct lfds_queue_bss_state *qbsss, void *key, void *value) );

int lfds_queue_bss_enqueue( struct lfds_queue_bss_state *qbsss,
                               void *key,
                               void *value );

int lfds_queue_bss_dequeue( struct lfds_queue_bss_state *qbsss,
                               void **key,
                               void **value );

void lfds_queue_bss_query( struct lfds_queue_bss_state *qbsss,
                              enum lfds_queue_bss_query query_type,
                              void *query_input,
                              void *query_output );

