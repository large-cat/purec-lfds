/***** the library wide include file *****/
#include "../liblfds_internal.h"

/***** private prototypes *****/
void lfds_freelist_internal_push_without_ea( struct lfds_freelist_state *fs,
                                                struct lfds_freelist_element *fe );

