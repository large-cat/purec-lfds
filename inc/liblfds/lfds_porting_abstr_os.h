#ifndef _LFDS_PORTING_ABSTR_OS_H_
#define _LFDS_PORTING_ABSTR_OS_H_

/****************************************************************************/
#if( defined _WIN32 && !defined KERNEL_MODE )

  #ifdef LFDS_PAL_OPERATING_SYSTEM
    #error More than one porting abstraction layer matches the current platform in "lfds_porting_abstr_os.h".
  #endif

  #define LFDS_PAL_OPERATING_SYSTEM

  #include <assert.h>

  #define LFDS_PAL_OS_STRING             "Windows"
  #define LFDS_PAL_ASSERT( expression )  if( !(expression) ) LFDS_MISC_DELIBERATELY_CRASH;

#endif





/****************************************************************************/
#if( defined _WIN32 && defined KERNEL_MODE )

  #ifdef LFDS_PAL_OPERATING_SYSTEM
    #error More than one porting abstraction layer matches the current platform in "lfds_porting_abstr_os.h".
  #endif

  #define LFDS_PAL_OPERATING_SYSTEM

  #include <assert.h>
  #include <wdm.h>

  #define LFDS_PAL_OS_STRING             "Windows"
  #define LFDS_PAL_ASSERT( expression )  if( !(expression) ) LFDS_MISC_DELIBERATELY_CRASH;

#endif





/****************************************************************************/
#if( defined __linux__ && !defined KERNEL_MODE )

  #ifdef LFDS_PAL_OPERATING_SYSTEM
    #error More than one porting abstraction layer matches the current platform in "lfds_porting_abstr_os.h".
  #endif

  #define LFDS_PAL_OPERATING_SYSTEM

  #define LFDS_PAL_OS_STRING             "Linux"
  #define LFDS_PAL_ASSERT( expression )  if( !(expression) ) LFDS_MISC_DELIBERATELY_CRASH;

#endif





/****************************************************************************/
#if( defined __linux__ && defined KERNEL_MODE )

  #ifdef LFDS_PAL_OPERATING_SYSTEM
    #error More than one porting abstraction layer matches the current platform in "lfds_porting_abstr_os.h".
  #endif

  #define LFDS_PAL_OPERATING_SYSTEM

  #include <linux/module.h>

  #define LFDS_PAL_OS_STRING             "Linux"
  #define LFDS_PAL_ASSERT( expression )  BUG_ON( expression )

#endif





/****************************************************************************/
#if( !defined LFDS_PAL_OPERATING_SYSTEM )

  #error No matching porting abstraction layer in lfds_porting_abstr_os.h

#endif

#endif // _LFDS_PORTING_ABSTR_OS_H_
