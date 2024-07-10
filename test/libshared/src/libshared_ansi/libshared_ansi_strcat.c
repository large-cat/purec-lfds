/***** includes *****/
#include "libshared_ansi_internal.h"





/****************************************************************************/
void libshared_ansi_strcat( char *destination, char const * const source )
{
  LFDS_PAL_ASSERT( destination != NULL );
  LFDS_PAL_ASSERT( source != NULL );

  while( *destination++ != '\0' );

  libshared_ansi_strcpy( destination-1, source );

  return;
}

