/***** includes *****/
#include "lfds_ringbuffer_internal.h"





/****************************************************************************/
void lfds_ringbuffer_init_core( struct lfds_ringbuffer_state *rs,
                                                            struct lfds_ringbuffer_element *re_array_inc_dummy,
                                                            lfds_pal_uint_t number_elements_inc_dummy,
                                                            void *user_state )
{
  lfds_pal_uint_t
    loop;

  LFDS_PAL_ASSERT( rs != NULL );
  LFDS_PAL_ASSERT( re_array_inc_dummy != NULL );
  LFDS_PAL_ASSERT( number_elements_inc_dummy >= 2 );
  // TRD : user_state can be NULL

  rs->user_state = user_state;

  re_array_inc_dummy[0].qumme_use = &re_array_inc_dummy[0].qumme;

  freelist_init_core( &rs->fs, NULL, 0, rs );
  lfds_queue_umm_init_core( &rs->qumms, &re_array_inc_dummy[0].qumme, rs );

  for( loop = 1 ; loop < number_elements_inc_dummy ; loop++ )
  {
    re_array_inc_dummy[loop].qumme_use = &re_array_inc_dummy[loop].qumme;
    LFDS_FREELIST_SET_VALUE_IN_ELEMENT( re_array_inc_dummy[loop].fe, &re_array_inc_dummy[loop] );
    lfds_freelist_push( &rs->fs, &re_array_inc_dummy[loop].fe, NULL );
  }

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}

