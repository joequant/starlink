      SUBROUTINE NDF1_AWCRE( IAX, IDCB, STATUS )
*+
*  Name:
*     NDF1_AWCRE

*  Purpose:
*     Ensure that an NDF axis width array exists, creating one if
*     necessary.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL NDF1_AWCRE( IAX, IDCB, STATUS )

*  Description:
*     The routine ensures that an axis width array exists for an NDF
*     with an entry in the DCB. If the data object does not currently
*     have an axis width array, then one is created.  An NDF axis
*     coordinate system is first created by this routine if necessary.

*  Arguments:
*     IAX = INTEGER (Given)
*        Axis number.
*     IDCB = INTEGER (Given)
*        Index to the data object entry in the DCB.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Notes:
*     -  Any width array created is left in an undefined state by this
*     routine.

*  Algorithm:
*     -  Ensure that an NDF axis structure exists, creating one if
*     necessary.
*     -  Ensure that axis width array information is available.
*     -  See if the DCB ARY_ system identifier for the axis width array
*     is valid. If so, then an axis width array exists, so there is
*     nothing to do.
*     -  Obtain the NDF bounds and number of dimensions from the ARY_
*     system identifier for the data array held in the DCB.
*     -  Obtain an ARY_ system placeholder for the width array in the
*     appropriate cell of the NDF's axis structure.
*     -  Create the width array with the required storage form, numeric
*     type and shape.
*     -  If the axis width array form stored in the DCB was not
*     recognised, then report an error.
*     -  If an error occurred, then erase any axis width array which
*     may have been created.
*     -  Note if the axis width array information in the DCB is valid.

*  Copyright:
*     Copyright (C) 1990 Science & Engineering Research Council.
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
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     15-OCT-1990 (RFWS):
*        Original version, derived from the NDF1_AVCRE routine.
*     16-OCT-1990 (RFWS):
*        Corrected error in test for array existence.
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'DAT_PAR'          ! DAT_ public constants
      INCLUDE 'NDF_CONST'        ! NDF_ private constants
      INCLUDE 'NDF_PAR'          ! NDF_ public constants
      INCLUDE 'NDF_ERR'          ! NDF_ error codes
      INCLUDE 'ARY_PAR'          ! ARY_ public constants

*  Global Variables:
      INCLUDE 'NDF_DCB'          ! NDF_ Data Control Block
*        DCB_ALOC( NDF__MXDIM, NDF__MXDCB ) = CHARACTER * ( DAT__SZLOC )
*        (Read)
*           Locators to axis structure elements.
*        DCB_AWFRM( NDF__MXDIM, NDF__MXDCB ) = CHARACTER * ( NDF__SZFRM
*        ) (Read)
*           Storage form of axis width arrays.
*        DCB_AWID( NDF__MXDIM, NDF__MXDCB ) = INTEGER (Read and Write)
*           ARY_ system identifiers for axis width arrays.
*        DCB_AWTYP( NDF__MXDIM, NDF__MXDCB ) = CHARACTER * ( NDF__SZTYP
*        ) (Read)
*           Numeric data type of axis width arrays.
*        DCB_DID( NDF__MXDCB ) = INTEGER (Read)
*           ARY_ system identifier for the NDF's data array.
*        DCB_KAW( NDF__MXDIM, NDF__MXDCB ) = LOGICAL (Write)
*           Whether information about axis width arrays is available.

*  Arguments Given:
      INTEGER IAX
      INTEGER IDCB

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      INTEGER LBND( NDF__MXDIM ) ! NDF lower bounds
      INTEGER NDIM               ! Number of NDF dimensions
      INTEGER PLACE              ! ARY_ system placeholder
      INTEGER UBND( NDF__MXDIM ) ! NDF upper bounds

*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Ensure that an NDF axis structure exists, creating one if necessary.
      CALL NDF1_ACRE( IDCB, STATUS )

*  Ensure that axis width array information is available.
      CALL NDF1_DAW( IAX, IDCB, STATUS )
      IF ( STATUS .EQ. SAI__OK ) THEN

*  See if the DCB ARY_ system identifier for the axis width array is
*  valid. If so, then an axis width array exists, so there is nothing
*  to do.
         IF ( DCB_AWID( IAX, IDCB ) .EQ. ARY__NOID ) THEN

*  Obtain the NDF bounds and number of dimensions from the ARY_ system
*  identifier for the data array held in the DCB.
            CALL ARY_BOUND( DCB_DID( IDCB ), NDF__MXDIM, LBND, UBND,
     :                      NDIM, STATUS )

*  Obtain an ARY_ system placeholder for the width array in the
*  appropriate cell of the NDF's axis structure.
            CALL ARY_PLACE( DCB_ALOC( IAX, IDCB ), 'WIDTH', PLACE,
     :                      STATUS )

*  Create the width array with the required storage form, numeric type
*  and shape.

*  ...primitive arrays.
            IF ( DCB_AWFRM( IAX, IDCB ) .EQ. 'PRIMITIVE' ) THEN
               CALL ARY_NEWP( DCB_AWTYP( IAX, IDCB ), 1, UBND( IAX ),
     :                        PLACE, DCB_AWID( IAX, IDCB ), STATUS )

*  ...simple arrays.
            ELSE IF ( DCB_AWFRM( IAX, IDCB ) .EQ. 'SIMPLE' ) THEN
               CALL ARY_NEW( DCB_AWTYP( IAX, IDCB ), 1, LBND( IAX ),
     :                       UBND( IAX ), PLACE, DCB_AWID( IAX, IDCB ),
     :                       STATUS )

*  If the axis width array form stored in the DCB was not recognised,
*  then report an error.
            ELSE
               STATUS = NDF__FATIN
               CALL MSG_SETC( 'BADFORM', DCB_AWFRM( IAX, IDCB ) )
               CALL ERR_REP( 'NDF1_AWCRE_FRM',
     :                       'Invalid axis array storage form ' //
     :                       '''^BADFORM'' encountered in the NDF_ ' //
     :                       'system Data Control Block (internal ' //
     :                       'programming error).', STATUS )
            END IF

*  If an error occurred, then erase any axis width array which may have
*  been created.
            IF ( STATUS .NE. SAI__OK ) THEN
               CALL ARY_DELET( DCB_AWID( IAX, IDCB ), STATUS )
            END IF

*  Note if the axis width array information in the DCB is valid.
            DCB_KAW( IAX, IDCB ) = STATUS .EQ. SAI__OK
         END IF
      END IF

*  Call error tracing routine and exit.
      IF ( STATUS .NE. SAI__OK ) CALL NDF1_TRACE( 'NDF1_AWCRE', STATUS )

      END
