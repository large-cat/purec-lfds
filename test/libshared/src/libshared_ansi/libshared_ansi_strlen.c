/***** includes *****/
#include "libshared_ansi_internal.h"





/****************************************************************************/
lfds_pal_uint_t libshared_ansi_strlen( char const * const string )
{
  char const
    *temp;

  LFDS_PAL_ASSERT( string != NULL );

  temp = (char const *) string;

  while( *temp++ != '\0' );

  return (lfds_pal_uint_t) (temp-1 - string);
}

