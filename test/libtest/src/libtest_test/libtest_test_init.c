/***** includes *****/
#include "libtest_test_internal.h"





/****************************************************************************/
void libtest_test_init( struct libtest_test_state *ts,
                        char *name,
                        enum libtest_test_id test_id,
                        void (*test_function)(struct lfds_list_asu_state *list_of_logical_processors, struct libshared_memory_state *ms, enum lfds_misc_validity *dvs) )
{
  LFDS_PAL_ASSERT( ts != NULL );
  LFDS_PAL_ASSERT( name != NULL );
  // TRD : test_id can be any value in its range
  LFDS_PAL_ASSERT( test_function != NULL );

  ts->name = name;
  ts->test_id = test_id;
  ts->test_function = test_function;

  return;
}

