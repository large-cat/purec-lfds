/***** includes *****/
#include "libtest_tests_internal.h"





/****************************************************************************/
#pragma warning( disable : 4100 4127 ) // TRD : disables MSVC warning for condition expressions being const

void libtest_tests_hash_a_alignment( struct lfds_list_asu_state *list_of_logical_processors, struct libshared_memory_state *ms, enum lfds_misc_validity *dvs )
{
  /* TRD : these are compile time checks
           but we do them here because this is a test programme
           and it should indicate issues to users when it is *run*,
           not when it is compiled, because a compile error normally
           indicates a problem with the code itself and so is misleading
  */

  LFDS_PAL_ASSERT( list_of_logical_processors != NULL );
  LFDS_PAL_ASSERT( ms != NULL );
  LFDS_PAL_ASSERT( dvs != NULL );

  *dvs = LFDS_MISC_VALIDITY_VALID;

  // TRD : struct lfds_hash_a_element
  if( LIBTEST_MISC_OFFSETOF(struct lfds_hash_a_element,value) % LFDS_PAL_ALIGN_SINGLE_POINTER != 0 )
    *dvs = LFDS_MISC_VALIDITY_INVALID_TEST_DATA;

  return;
}

#pragma warning( default : 4100 4127 )

