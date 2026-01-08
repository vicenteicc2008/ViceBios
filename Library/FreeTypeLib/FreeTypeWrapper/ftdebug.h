/*
  Here we enable the debug function since it is not possible to output debug function via LibC in EDK2.
  SPDX-License-Identifier: WTFPL
 */
#ifndef FTDEBUG_H_
#define FTDEBUG_H_


#include <ft2build.h>
#include FT_CONFIG_CONFIG_H
#include <freetype/freetype.h>

#include <freetype/internal/compiler-macros.h>

#include <stdarg.h>

FT_BEGIN_HEADER

FT_Error EFIAPI FtLogEdk2   (FT_Error Level, char *Format,...);
FT_Error EFIAPI FtErrEdk2   (char *Format,...);
FT_Error EFIAPI FtPanicEdk2 (char *Format,...);

#ifndef MDEPKG_NDEBUG
#define FT_LOG(level,...)   FtLogEdk2(level,__VA_ARGS__)
#define FT_TRACE(level,...) FtLogEdk2(level,__VA_ARGS__)
#define FT_ERROR(varformat )          FtErrEdk2 varformat

#define FT_DEBUG_LEVEL_TRACE
#define FT_ASSERT( condition )                                         \
          do                                                           \
          {                                                            \
            if ( !( condition ) )                                      \
              FtPanicEdk2( "assertion failed on line %d of file %a\n", \
                        __LINE__, __FILE__ );                          \
          } while ( 0 )
#define FT_THROW( e )                                   \
          (FtPanicEdk2( "%a, Line %d:%a\n",__FILE__,__LINE__,#e),FT_ERR_CAT( FT_ERR_PREFIX, e ))




#else
#define FT_LOG( level, varformat )   do { } while ( 0 )      /* nothing */
#define FT_TRACE( level, varformat )  do { } while ( 0 )      /* nothing */
#define FT_ERROR( varformat )  do { } while ( 0 )      /* nothing */
#define FT_ASSERT( condition )  do { } while ( 0 )
#define FT_THROW( e )  FT_ERR_CAT( FT_ERR_PREFIX, e )

#endif

#define FT_Trace_Disable()
#define FT_Trace_Enable()

#define _FT_TRACE0(...)         FT_TRACE(0,__VA_ARGS__)
#define _FT_TRACE1(...)         FT_TRACE(1,__VA_ARGS__)
#define _FT_TRACE2(...)         FT_TRACE(2,__VA_ARGS__)
#define _FT_TRACE3(...)         FT_TRACE(3,__VA_ARGS__)
#define _FT_TRACE4(...)         FT_TRACE(4,__VA_ARGS__)
#define _FT_TRACE5(...)         FT_TRACE(5,__VA_ARGS__)
#define _FT_TRACE6(...)         FT_TRACE(6,__VA_ARGS__)
#define _FT_TRACE7(...)         FT_TRACE(7,__VA_ARGS__)
#define FT_TRACE0( varformat )  _FT_TRACE0 varformat
#define FT_TRACE1( varformat )  _FT_TRACE1 varformat
#define FT_TRACE2( varformat )  _FT_TRACE2 varformat
#define FT_TRACE3( varformat )  _FT_TRACE3 varformat
#define FT_TRACE4( varformat )  _FT_TRACE4 varformat
#define FT_TRACE5( varformat )  _FT_TRACE5 varformat
#define FT_TRACE6( varformat )  _FT_TRACE6 varformat
#define FT_TRACE7( varformat )  _FT_TRACE7 varformat

FT_BASE( void )
ft_debug_init( void );


FT_END_HEADER

#endif /* FTDEBUG_H_ */


/* END */
