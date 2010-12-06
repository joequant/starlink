/*
*+
*  Name:
*     ffitstable.c

*  Purpose:
*     Define a FORTRAN 77 interface to the AST FitsTable class.

*  Type of Module:
*     C source file.

*  Description:
*     This file defines FORTRAN 77-callable C functions which provide
*     a public FORTRAN 77 interface to the FitsTable class.

*  Routines Defined:
*     AST_ISAFITSTABLE
*     AST_FITSTABLE

*  Copyright:
*     Copyright (C) 2010 Science & Technology Facilities Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public Licence as
*     published by the Free Software Foundation; either version 2 of
*     the Licence, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public Licence for more details.
*
*     You should have received a copy of the GNU General Public Licence
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*  Authors:
*     DSB: David S.Berry (Starlink)

*  History:
*     25-NOV-2010 (DSB):
*        Original version.
*/

/* Define the astFORTRAN77 macro which prevents error messages from
   AST C functions from reporting the file and line number where the
   error occurred (since these would refer to this file, they would
   not be useful). */
#define astFORTRAN77

/* Header files. */
/* ============= */
#include "f77.h"                 /* FORTRAN <-> C interface macros (SUN/209) */
#include "c2f77.h"               /* F77 <-> C support functions/macros */
#include "error.h"               /* Error reporting facilities */
#include "memory.h"              /* Memory handling facilities */
#include "fitstable.h"           /* C interface to the FitsTable class */

F77_LOGICAL_FUNCTION(ast_isafitstable)( INTEGER(THIS),
                                        INTEGER(STATUS) ) {
   GENPTR_INTEGER(THIS)
   F77_LOGICAL_TYPE(RESULT);

   astWatchSTATUS(
   astAt( "AST_ISAFITSTABLE", NULL, 0 );
      RESULT = astIsAFitsTable( astI2P( *THIS ) ) ? F77_TRUE : F77_FALSE;
   )
   return RESULT;
}

F77_INTEGER_FUNCTION(ast_fitstable)( CHARACTER(OPTIONS),
                                     INTEGER(STATUS)
                                     TRAIL(OPTIONS) ) {
   GENPTR_CHARACTER(OPTIONS)
   F77_INTEGER_TYPE(RESULT);
   int i;
   char *options;

   astAt( "AST_FITSTABLE", NULL, 0 );
   astWatchSTATUS(
      options = astString( OPTIONS, OPTIONS_length );

/* Change ',' to '\n' (see AST_SET in fobject.c for why). */
      if ( astOK ) {
         for ( i = 0; options[ i ]; i++ ) {
            if ( options[ i ] == ',' ) options[ i ] = '\n';
         }
      }
      RESULT = astP2I( astFitsTable( "%s", options ) );
      astFree( options );
   )
   return RESULT;
}

F77_INTEGER_FUNCTION(ast_gettableheader)( INTEGER(THIS),
                                          INTEGER(STATUS) ) {
   GENPTR_INTEGER(THIS)
   F77_INTEGER_TYPE(RESULT);

   astAt( "AST_GETTABLEHEADER", NULL, 0 );
   astWatchSTATUS(
      RESULT = astP2I( astGetTableHeader( astI2P( *THIS ) ) );
   )
   return RESULT;
}


F77_SUBROUTINE(ast_puttableheader)( INTEGER(THIS),
                                    INTEGER(HEADER),
                                    INTEGER(STATUS) ) {
   GENPTR_INTEGER(THIS)
   GENPTR_INTEGER(HEADER)

   astAt( "AST_PUTTABLEHEADER", NULL, 0 );
   astWatchSTATUS(
      astPutTableHeader( astI2P( *THIS ), astI2P( *HEADER ) );
   )
}

F77_INTEGER_FUNCTION(ast_columnnull)( INTEGER(THIS),
                                      CHARACTER(COLUMN),
                                      LOGICAL(SET),
                                      INTEGER(NEWVAL),
                                      LOGICAL(WASSET),
                                      LOGICAL(HASNULL),
                                      INTEGER(STATUS)
                                      TRAIL(COLUMN) ) {
   GENPTR_INTEGER(THIS)
   GENPTR_CHARACTER(COLUMN)
   GENPTR_LOGICAL(SET)
   GENPTR_INTEGER(NEWVAL)
   GENPTR_LOGICAL(WASSET)
   GENPTR_LOGICAL(HASNULL)
   F77_INTEGER_TYPE(RESULT);
   int wasset, hasnull;
   char *column;

   astAt( "AST_COLUMNNULL", NULL, 0 );
   astWatchSTATUS(
      column = astString( COLUMN, COLUMN_length );
      RESULT = astColumnNull( astI2P( *THIS ), column,
                              F77_ISTRUE( *SET ) ? 1 : 0, *NEWVAL,
                              &wasset, &hasnull );
      *WASSET = wasset ? F77_TRUE : F77_FALSE;
      *HASNULL = hasnull ? F77_TRUE : F77_FALSE;
      astFree( column );
   )
   return RESULT;
}
