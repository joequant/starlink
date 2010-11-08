      SUBROUTINE ARY_COPY( IARY1, PLACE, IARY2, STATUS )
*+
*  Name:
*     ARY_COPY

*  Purpose:
*     Copy an array to a new location.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL ARY_COPY( IARY1, PLACE, IARY2, STATUS )

*  Description:
*     The routine copies an array to a new location and returns an
*     identifier for the resulting new base array.

*  Arguments:
*     IARY1 = INTEGER (Given)
*        Identifier for the array (or array section) to be copied.
*     PLACE = INTEGER (Given and Returned)
*        An array placeholder (e.g. generated by the ARY_PLACE routine)
*        which indicates the position in the data system where the new
*        array will reside. The placeholder is annulled by this
*        routine, and a value of ARY__NOPL will be returned (as defined
*        in the include file ARY_PAR).
*     IARY2 = INTEGER (Returned)
*        Identifier for the new array.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Notes:
*     - The result of copying a scaled or delta array will be an
*     equivalent simple array.
*     - If this routine is called with STATUS set, then a value of
*     ARY__NOID will be returned for the IARY2 argument, although no
*     further processing will occur. The same value will also be
*     returned if the routine should fail for any reason. In either
*     event, the placeholder will still be annulled. The ARY__NOID
*     constant is defined in the include file ARY_PAR.

*  Algorithm:
*     -  Set an initial value for the IARY2 argument.
*     -  Save the error context.
*     -  Import the placeholder, converting it to an index into the
*     PCB.
*     -  If no error has occured, then import the input array
*     identifier.
*     -  Make a copy of the array.
*     -  Export an identifier for the new array.
*     -  Annul the placeholder, erasing the associated object if any
*     error has occurred.
*     -  Reset the PLACE argument.
*     -  Restore the error context, resetting the IARY2 argument and
*     reporting context information if appropriate.

*  Copyright:
*     Copyright (C) 1989 Science & Engineering Research Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     16-AUG-1989 (RFWS):
*        Original version.
*     9-OCT-1989 (RFWS):
*        Converted to use placeholder.
*     20-OCT-1989 (RFWS):
*        Added support for temporary placeholders.
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'DAT_PAR'          ! DAT_ public constants
      INCLUDE 'ARY_PAR'          ! ARY_ public constants
      INCLUDE 'ARY_CONST'        ! ARY_ private constants

*  Global Variables:
      INCLUDE 'ARY_PCB'          ! ARY_ Placeholder Control Block
*        PCB_LOC( ARY__MXPCB ) = CHARACTER * ( DAT__SZLOC ) (Read and
*        Write)
*           Placeholder locator.
*        PCB_TMP( ARY__MXPCB ) = LOGICAL (Read)
*           Whether the object which replaces the placeholder object
*           should be temporary.

*  Arguments Given:
      INTEGER IARY1
      INTEGER PLACE

*  Arguments Returned:
      INTEGER IARY2

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      INTEGER IACB1              ! Index to input array entry in ACB
      INTEGER IACB2              ! Index to output array entry in ACB
      INTEGER IPCB               ! Index to placeholder entry in the PCB
      INTEGER TSTAT              ! Temporary status variable
      LOGICAL ERASE              ! Whether to erase placeholder object

*.

*  Set an initial value for the IARY2 argument.
      IARY2 = ARY__NOID

*  Save the STATUS value and mark the error stack.
      TSTAT = STATUS
      CALL ERR_MARK

*  Import the array placeholder, converting it to a PCB index.
      STATUS = SAI__OK
      IPCB = 0
      CALL ARY1_IMPPL( PLACE, IPCB, STATUS )

*  If there has been no error at all so far, then import the input
*  array identifier.
      IF ( ( STATUS .EQ. SAI__OK ) .AND. ( TSTAT .EQ. SAI__OK ) ) THEN
         CALL ARY1_IMPID( IARY1, IACB1, STATUS )

*  Make a copy of the array.
         CALL ARY1_CPY( IACB1, PCB_TMP( IPCB ), PCB_LOC( IPCB ),
     :                  .TRUE., IACB2, STATUS )

*  Export an identifier for the new array.
         CALL ARY1_EXPID( IACB2, IARY2, STATUS )
      END IF

*  Annul the placeholder, erasing the associated object if any error has
*  occurred.
      IF ( IPCB .NE. 0 ) THEN
         ERASE = ( STATUS .NE. SAI__OK ) .OR. ( TSTAT .NE. SAI__OK )
         CALL ARY1_ANNPL( ERASE, IPCB, STATUS )
      END IF

*  Reset the PLACE argument.
      PLACE = ARY__NOPL

*  Annul any error if STATUS was previously bad, otherwise let the new
*  error report stand.
      IF ( STATUS .NE. SAI__OK ) THEN
         IF ( TSTAT .NE. SAI__OK ) THEN
            CALL ERR_ANNUL( STATUS )
            STATUS = TSTAT

*  If appropriate, reset the IARY2 argument, report the error context
*  and call the error tracing routine.
         ELSE
            IARY2 = ARY__NOID
            CALL ERR_REP( 'ARY_COPY_ERR',
     :      'ARY_COPY: Error making a copy of an array.', STATUS )
            CALL ARY1_TRACE( 'ARY_COPY', STATUS )
         END IF
      ELSE
         STATUS = TSTAT
      END IF

*  Release error stack.
      CALL ERR_RLSE

      END
