      SUBROUTINE ARY_TEMP( PLACE, STATUS )
*+
*  Name:
*     ARY_TEMP

*  Purpose:
*     Obtain a placeholder for a temporary array.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL ARY_TEMP( PLACE, STATUS )

*  Description:
*     The routine returns an array placeholder which may be used to
*     create a temporary array (i.e. one which will be deleted
*     automatically once the last identifier associated with it is
*     annulled). The placeholder returned by this routine may be passed
*     to other routines (e.g. ARY_NEW or ARY_COPY) to produce a
*     temporary array in the same way as a new permanent array would be
*     created.

*  Arguments:
*     PLACE = INTEGER (Returned)
*        Placeholder for a temporary array.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Notes:
*     -  Placeholders are intended only for local use within an
*     application and only a limited number of them are available
*     simultaneously. They are always annulled as soon as they are
*     passed to another routine to create a new array, where they are
*     effectively exchanged for an array identifier.
*     -  If this routine is called with STATUS set, then a value of
*     ARY__NOPL will be returned for the PLACE argument, although no
*     further processing will occur. The same value will also be
*     returned if the routine should fail for any reason. The ARY__NOPL
*     constant is defined in the include file ARY_PAR.

*  Algorithm:
*     -  Set an initial value for the PLACE argument before checking the
*     inherited global status.
*     -  Obtain a free slot in the Placeholder Control Block.
*     -  Create a temporary array placeholder object and store a locator
*     to it in the PCB.
*     -  Link the locator into a private group to prevent external
*     events from annulling it.
*     -  Increment the count of placeholders issued so far and encode
*     the PCB index into a new placeholder value.
*     -  Store the placeholder value for later validation.
*     -  Note that the placeholder is to be replaced with a temporary
*     object.
*     -  If an error occurred, then release the PCB slot.

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     20-OCT-1989 (RFWS):
*        Original, derived from the NDF_TEMP routine.
*     12-MAR-1990 (RFWS):
*        Changed placeholder type to ARRAY.
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
      INCLUDE 'PRM_PAR'          ! PRIMDAT primitive data constants

*  Global Variables:
      INCLUDE 'ARY_PCB'          ! ARY_ Placeholder Control Block
*        PCB_CHK( ARY__MXPCB ) = INTEGER (Write)
*           Placeholder check value for future validation.
*        PCB_LOC( ARY__MXPCB ) = CHARACTER * ( DAT__SZLOC ) (Write)
*           Locator to placeholder data object.
*        PCB_PLCNT = INTEGER (Read and Write)
*           Count of the number of placeholders issued so far.
*        PCB_TMP( ARY__MXPCB ) = LOGICAL (Write)
*           Whether the object which replaces the placeholder should be
*           temporary.

*  Arguments Returned:
      INTEGER PLACE

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      INTEGER DUMMY( 1 )         ! Dummy dimension array
      INTEGER IPCB               ! Index to placeholder entry in the PCB
      INTEGER PLCODE             ! Value for encoding placeholders

*.

*  Set an initial value for the PLACE argument.
      PLACE = ARY__NOPL

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Obtain a free slot in the PCB.
      CALL ARY1_FFS( ARY__PCB, IPCB, STATUS )
      IF ( STATUS .EQ. SAI__OK ) THEN

*  Create a temporary array placeholder object, storing a locator to it
*  in the PCB.
         DUMMY( 1 ) = 0
         CALL ARY1_TEMP( 'ARRAY', 0, DUMMY, PCB_LOC( IPCB ), STATUS )

*  Link the locator into a private group to prevent external events
*  from annulling it.
         CALL HDS_LINK( PCB_LOC( IPCB ), 'ARY_PCB', STATUS )
         IF ( STATUS .EQ. SAI__OK ) THEN

*  Increment the count of placeholders issued so far.
            PCB_PLCNT = PCB_PLCNT + 1

*  Encode the PCB index into a placeholder value, making use of the
*  placeholder count and protecting against numerical overflow.
            PLCODE = MOD( PCB_PLCNT, ( NUM__MAXI / ARY__MXPCB ) )
            PLACE = IPCB + ARY__MXPCB * PLCODE

*  Store the placeholder value for later verification.
            PCB_CHK( IPCB ) = PLACE

*  Note the object to replace the placeholder is to be temporary.
            PCB_TMP( IPCB ) = .TRUE.

*  If an error occurred, then release the PCB slot.
         ELSE
            CALL ARY1_RLS( ARY__PCB, IPCB, STATUS )
         END IF
      END IF
       
*  If an error occurred, then report context information and call the
*  error tracing routine.
      IF ( STATUS .NE. SAI__OK ) THEN
         CALL ERR_REP( 'ARY_TEMP_ERR',
     :   'ARY_TEMP: Error obtaining placeholder for temporary array.',
     :   STATUS )
         CALL ARY1_TRACE( 'ARY_TEMP', STATUS )
      END IF

      END
