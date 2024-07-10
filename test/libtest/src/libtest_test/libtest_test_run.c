/***** includes *****/
#include "libtest_test_internal.h"





/****************************************************************************/
void libtest_test_run( struct libtest_test_state *ts,
                       struct lfds_list_asu_state *list_of_logical_processors,
                       struct libshared_memory_state *ms,
                       enum lfds_misc_validity *dvs )
{
  LFDS_PAL_ASSERT( ts != NULL );
  LFDS_PAL_ASSERT( list_of_logical_processors != NULL );
  LFDS_PAL_ASSERT( ms != NULL );
  LFDS_PAL_ASSERT( dvs != NULL );

  ts->test_function( list_of_logical_processors, ms, dvs );

  return;
}

