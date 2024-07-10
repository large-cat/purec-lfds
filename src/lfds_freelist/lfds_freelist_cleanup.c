/***** includes *****/
#include "lfds_freelist_internal.h"





/****************************************************************************/
void lfds_freelist_cleanup( struct lfds_freelist_state *fs,
                               void (*element_cleanup_callback)(struct lfds_freelist_state *fs, struct lfds_freelist_element *fe) )
{
  struct lfds_freelist_element
    *fe,
    *fe_temp;

  LFDS_PAL_ASSERT( fs != NULL );
  // TRD : element_cleanup_callback can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( element_cleanup_callback != NULL )
  {
    fe = fs->top[POINTER];

    while( fe != NULL )
    {
      fe_temp = fe;
      fe = fe->next;

      element_cleanup_callback( fs, fe_temp );
    }
  }

  return;
}

