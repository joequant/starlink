************************************************************************
*+  AGI_1WPIC - Fill a picture

      SUBROUTINE AGI_1WPIC ( WKNAME, PNAME, COMENT, DEVICE, NDC, WORLD,
     :                       MEMID, PICNUM, STATUS )

*    Description :
*     Write the picture descriptors into the database and make the given
*     picture the current picture.
*
*    Invocation :
*     CALL AGI_1WPIC( WKNAME, PNAME, COMENT, DEVICE, NDC, WORLD,
*    :                MEMID, PICNUM, STATUS )
*
*    Method :
*     Check status on entry.
*     Ensure a picture structure exists.
*     Put the parameters into the picture structure.
*
*    Authors :
*     Nick Eaton  ( DUVAD::NE )
*
*    History :
*     July 1988
*     June 1990  Added MEMID parameter
*     September 1990  Changed PICNUM to an export argument
*    endhistory
*
*    Type Definitions :
      IMPLICIT NONE

*    Global constants :
      INCLUDE 'SAE_PAR'
      INCLUDE 'DAT_PAR'

*    Import :
*     Name of workstation
      CHARACTER * ( * ) WKNAME

*     Picture name
      CHARACTER * ( * ) PNAME

*     Description of picture
      CHARACTER * ( * ) COMENT

*     Device coordinates of picture
      REAL DEVICE( 4 )

*     Normalised device coordinates of picture
      REAL NDC( 4 )

*     World coordinates of picture
      REAL WORLD( 4 )

*     Memory identifier
      INTEGER MEMID

*    Export :
*     Number of picture in array of pictures
      INTEGER PICNUM

*    Status :
      INTEGER STATUS

*    Local variables :
      CHARACTER * ( DAT__SZLOC ) PICLOC, WKSLOC
*-

      IF ( STATUS .EQ. SAI__OK ) THEN

*   Check picture structure is present
         CALL AGI_1ODB( STATUS )
         WKSLOC = ' '
         CALL AGI_1OWORK( WKNAME, WKSLOC, STATUS )
         PICLOC = ' '
         CALL AGI_1OPIC( WKSLOC, PICNUM, PICLOC, STATUS )

*   Fill elements with passed parameters
         CALL AGI_1WPARS( PICLOC, PNAME, COMENT, DEVICE, NDC, WORLD,
     :                    MEMID, STATUS )

         CALL DAT_ANNUL( PICLOC, STATUS )
         PICLOC = ' '
         CALL DAT_ANNUL( WKSLOC, STATUS )
         WKSLOC = ' '

      ENDIF

*      print*, '+++++ AGI_1WPIC +++++'
*      call HDS_SHOW( 'FILES', status )
*      call HDS_SHOW( 'LOCATORS', status )

      END

