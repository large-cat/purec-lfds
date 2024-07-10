/***** defines *****/
#define LFDS_STACK_GET_KEY_FROM_ELEMENT( stack_element )             ( (stack_element).key )
#define LFDS_STACK_SET_KEY_IN_ELEMENT( stack_element, new_key )      ( (stack_element).key = (void *) (lfds_pal_uint_t) (new_key) )
#define LFDS_STACK_GET_VALUE_FROM_ELEMENT( stack_element )           ( (stack_element).value )
#define LFDS_STACK_SET_VALUE_IN_ELEMENT( stack_element, new_value )  ( (stack_element).value = (void *) (lfds_pal_uint_t) (new_value) )
#define LFDS_STACK_GET_USER_STATE_FROM_STATE( stack_state )          ( (stack_state).user_state )

/***** enums *****/
enum lfds_stack_query
{
  LFDS_STACK_QUERY_SINGLETHREADED_GET_COUNT,
  LFDS_STACK_QUERY_SINGLETHREADED_VALIDATE
};

/***** structures *****/
struct lfds_stack_element
{
  struct lfds_stack_element
    *next;

  void
    *key,
    *value;
};

struct lfds_stack_state
{
  struct lfds_stack_element LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    *volatile top[PAC_SIZE];

  void LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    *user_state;

  struct lfds_misc_backoff_state
    pop_backoff,
    push_backoff;
};

/***** public prototypes *****/
void lfds_stack_init_valid_on_current_logical_core( struct lfds_stack_state *ss,
                                                       void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_stack_cleanup( struct lfds_stack_state *ss,
                            void (*element_cleanup_callback)(struct lfds_stack_state *ss, struct lfds_stack_element *se) );

void lfds_stack_push( struct lfds_stack_state *ss,
                         struct lfds_stack_element *se );

int lfds_stack_pop( struct lfds_stack_state *ss,
                       struct lfds_stack_element **se );

void lfds_stack_query( struct lfds_stack_state *ss,
                          enum lfds_stack_query query_type,
                          void *query_input,
                          void *query_output );


