/***** includes *****/
#include "libbenchmark_benchmarksuite_internal.h"





/****************************************************************************/
void libbenchmark_benchmarksuite_get_list_of_gnuplot_strings( struct libbenchmark_benchmarksuite_state *bss,
                                                              struct libbenchmark_results_state *rs,
                                                              char *gnuplot_system_string,
                                                              struct libbenchmark_gnuplot_options *gpo,
                                                              struct lfds_list_asu_state *list_of_gnuplot_strings )
{
  struct libbenchmark_benchmarkset_state
    *bsets;

  struct libbenchmark_benchmarkset_gnuplot
    *bg;

  struct lfds_list_asu_element
    *lasue = NULL,
    *lasue_numa = NULL;

  struct libbenchmark_topology_numa_node
    *numa_mode;

  LFDS_PAL_ASSERT( bss != NULL );
  LFDS_PAL_ASSERT( rs != NULL );
  LFDS_PAL_ASSERT( gnuplot_system_string != NULL );
  LFDS_PAL_ASSERT( gpo != NULL );
  LFDS_PAL_ASSERT( list_of_gnuplot_strings != NULL );

  lfds_list_asu_init_core( list_of_gnuplot_strings, NULL );

  // TRD : iterate over all benchmarksets
  while( LFDS_LIST_ASU_GET_START_AND_THEN_NEXT(bss->benchmarksets,lasue) )
  {
    bsets = LFDS_LIST_ASU_GET_VALUE_FROM_ELEMENT( *lasue );

    // TRD : iterate over NUMA nodes - separate gnuplot for each
    while( LFDS_LIST_ASU_GET_START_AND_THEN_NEXT(*bsets->numa_modes_list,lasue_numa) )
    {
      numa_mode = LFDS_LIST_ASU_GET_VALUE_FROM_ELEMENT( *lasue_numa );

      bg = libshared_memory_alloc_from_most_free_space_node( bss->ms, sizeof(struct libbenchmark_benchmarkset_gnuplot), LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES );

      libbenchmark_benchmarkset_gnuplot_emit( bsets, rs, gnuplot_system_string, numa_mode->mode, gpo, bg );

      LFDS_LIST_ASU_SET_VALUE_IN_ELEMENT( bg->lasue, bg );
      lfds_list_asu_insert_at_end( list_of_gnuplot_strings, &bg->lasue );
    }
  }

  return;
}

