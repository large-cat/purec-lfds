#ifndef LIBLFDS_H

  /***** defines *****/
  #define LIBLFDS_H

  /***** pragmas on *****/
  #pragma warning( push )
  #pragma warning( disable : 4324 )                                          // TRD : 4324 disables MSVC warnings for structure alignment padding due to alignment specifiers
  #pragma prefast( disable : 28113 28182 28183, "blah" )

  /***** includes *****/
  #include "liblfds/lfds_porting_abstr_compiler.h"
  #include "liblfds/lfds_porting_abstr_os.h"
  #include "liblfds/lfds_porting_abstr_processor.h"

  #include "liblfds/lfds_prng.h"                                       // TRD : misc requires prng
  #include "liblfds/lfds_misc.h"                                       // TRD : everything after depends on misc
  #include "liblfds/lfds_btree_addonly_unbalanced.h"                   // TRD : hash_addonly depends on btree_addonly_unbalanced
  #include "liblfds/lfds_freelist.h"
  #include "liblfds/lfds_hash_addonly.h"
  #include "liblfds/lfds_list_addonly_single_order.h"
  #include "liblfds/lfds_list_addonly_single_order.h"
  #include "liblfds/lfds_queue_bounded_multi2multi"
  #include "liblfds/lfds_queue_bounded_single2single.h"
  #include "liblfds/lfds_queue_unbounded_multi2multi.h"
  #include "liblfds/lfds_ringbuffer.h"
  #include "liblfds/lfds_stack.h"

  /***** pragmas off *****/
  #pragma warning( pop )

#endif

