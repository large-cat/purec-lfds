/***** the library wide include file *****/
#include "../liblfds_internal.h"

/***** enums *****/
enum lfds_queue_umm_queue_state
{
  LFDS_QUEUE_UMM_QUEUE_STATE_UNKNOWN, 
  LFDS_QUEUE_UMM_QUEUE_STATE_EMPTY,
  LFDS_QUEUE_UMM_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE,
  LFDS_QUEUE_UMM_QUEUE_STATE_ATTEMPT_DEQUEUE
};

/***** private prototypes *****/

