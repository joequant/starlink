************************************************************************

      SUBROUTINE AGI_RCS ( PNAME, PSTART, PICID, STATUS )

*+
*  Name :
*     AGI_RCS
*
*  Purpose :
*     Recall succeeding picture of specified name
*
*  Invocation :
*     CALL AGI_RCS( PNAME, PSTART, PICID, STATUS )
*
*  Description :
*     Recall the picture succeeding the given one on the current device
*     that has the specified name and lies within the bounds of the
*     current picture. The search is started at the picture identified
*     by the PSTART argument. The name string has leading blanks removed
*     and is converted to upper case before being compared. An empty
*     name string (just spaces) results in a search for a picture of any
*     name. This picture becomes the current picture. If no picture
*     fulfills the conditions an error is returned.
*
*  Arguments :
*     PNAME = CHARACTER*(*) (Given)
*        Name of picture
*     PSTART = INTEGER (Given)
*        Identifier of picture starting the search
*     PICID = INTEGER (Returned)
*        Picture identifier
*     STATUS = INTEGER (Given and Returned)
*        The global status
*
*  Algorithm :
*     Check status on entry.
*     Get details of the current picture.
*     Read the contents of the current picture.
*     Get details of the picture from which the search is started.
*     Check this is on the same device as the current picture.
*     Find a picture of the given name searching forwards.
*     If the current picture is not the root picture then
*        Repeat while the recovered picture is not inside the current one
*           Look for another picture of the same name.
*        Endwhile
*     Endif
*     Get a new picture identifier and store the details in the
*     common block.
*
*  Authors :
*     NE: Nick Eaton (Durham University)
*
*  History :
*     Aug 1988 (NE):
*        Original version
*     Dec 1989 (NE):
*        Added CIDIID
*     Jun 1990 (NE):
*        Added MEMID parameter
*     Jul 1990 (NE):
*        Added root picture
*     Sep 1990 (NE):
*        Added fuzzy test
*-
*  Type Definitions :
      IMPLICIT NONE

*  Global constants :
      INCLUDE 'SAE_PAR'
      INCLUDE 'DAT_PAR'
      INCLUDE 'AGI_PAR'
      INCLUDE 'AGI_ERR'

*  Global variables :
      INCLUDE 'agi_locs'
      INCLUDE 'agi_nest'
      INCLUDE 'agi_pfree'

*  Arguments Given :
      CHARACTER * ( * ) PNAME
      INTEGER PSTART

*  Arguments Returned :
      INTEGER PICID

*  Status :
      INTEGER STATUS

*  Local variables :
      LOGICAL FOUND

      INTEGER CURMID, ISTAT, MEMID, PICNUM, SPICNM

      REAL CURDEV( 4 ), CURNDC( 4 ), CURWOR( 4 ), SFA
      REAL DEVICE( 4 ), NDC( 4 ), WORLD( 4 )

      CHARACTER * ( AGI__CMAX ) CURCOM, COMENT
      CHARACTER * ( AGI__SZNAM ) CURNAM, OPNAME
      CHARACTER * ( DAT__SZNAM ) SWKNAM, WKNAME
*.

*   Check status on entry
      IF ( STATUS .EQ. SAI__OK ) THEN

*   Get the details of the current picture
         IF ( CURPID .GT. 0 ) THEN
            WKNAME = CAGIWK( CURPID )
            PICNUM = CPICNM( CURPID )
         ELSE
            STATUS = AGI__NOCUP
            CALL ERR_REP( 'AGI_RCS_NOCUP',
     :                    'No current picture', STATUS )
            GOTO 99
         ENDIF

*   Read the contents of the current picture
         CALL AGI_1RPIC( WKNAME, PICNUM, CURNAM, CURCOM, CURDEV,
     :                   CURNDC, CURWOR, CURMID, FOUND, STATUS )

*   If picture is not there then indicate an error
         IF ( .NOT. FOUND ) THEN
            STATUS = AGI__PICNF
            CALL ERR_REP( 'AGI_RCS_PICNF', 'Picture not found', STATUS )
            GOTO 99
         ENDIF

*   Have to allow for fuzzy edges for the search because IDI only works
*   with whole pixels. However the device may be a metafile so if all
*   the device coordinates are less than one then assume it is a metafile.
         IF ( ( CURDEV( 1 ) .LT. 1.0 ) .AND.
     :        ( CURDEV( 2 ) .LT. 1.0 ) .AND.
     :        ( CURDEV( 3 ) .LT. 1.0 ) .AND.
     :        ( CURDEV( 4 ) .LT. 1.0 ) ) THEN
            SFA = 0.001
         ELSE
            SFA = 0.999
         ENDIF

*   Get the details of the picture starting the search
         IF ( PSTART .GT. 0 ) THEN
            SWKNAM = CAGIWK( PSTART )
            SPICNM = CPICNM( PSTART )

*   If the starting picture number is invalid then start from 1
            IF ( SPICNM .LT. 1 ) THEN
               SPICNM = 1
            ENDIF

*   Flag an error if PSTART is improper
         ELSE
            STATUS = AGI__IMPID
            CALL ERR_REP( 'AGI_RCS_IMPID',
     :                    'Picture identifier improper', STATUS )
            GOTO 99
         ENDIF

*   Check that the starting picture and the current picture are on the
*   same device
         IF ( WKNAME .NE. SWKNAM ) THEN
            STATUS = AGI__DIFDV
            CALL ERR_REP( 'AGI_RCS_DIFDV', 'Different devices', STATUS )
            GOTO 99
         ENDIF

*   Find a picture of the given name searching forwards
*   from the current one. Have to increment SPICNM before starting.
         SPICNM = SPICNM + 1
         CALL AGI_1SEARP( WKNAME, PNAME, 'P', 'F', SPICNM, OPNAME,
     :                    COMENT, DEVICE, NDC, WORLD, MEMID, STATUS )

*   If the root flag is set then ignore the current picture limits
         IF ( CROOT .NE. 1 ) THEN

*   Check that the picture is inside the current one, otherwise look
*   for another one.
            DO WHILE( ( ( DEVICE( 1 ) .LT. CURDEV( 1 ) - SFA ) .OR.
     :                  ( DEVICE( 2 ) .GT. CURDEV( 2 ) + SFA ) .OR.
     :                  ( DEVICE( 3 ) .LT. CURDEV( 3 ) - SFA ) .OR.
     :                  ( DEVICE( 4 ) .GT. CURDEV( 4 ) + SFA ) ) .AND.
     :                ( STATUS .EQ. SAI__OK ) ) 
               SPICNM = SPICNM + 1
               CALL AGI_1SEARP( WKNAME, PNAME, 'P', 'F', SPICNM, OPNAME,
     :                          COMENT, DEVICE, NDC, WORLD, MEMID,
     :                          STATUS )
            ENDDO
         ENDIF
         IF ( STATUS .NE. SAI__OK ) GOTO 99

*   Get a picture identifier
*   Use a local status for the free list access
         ISTAT = SAI__OK

*   Get a picture identifier from the free list
         CALL AGI_1FNEXT( FRELEN, FRELIS, NEXFRE, PICID, ISTAT )

*   If the exit status is not OK then the free list is full
         IF ( ISTAT .NE. SAI__OK ) THEN
            STATUS = AGI__NOPID
            CALL ERR_REP( 'AGI_RCS_NOPID',
     :                    'No more picture identifiers', STATUS )
            GOTO 99
         ELSE
            CNUMID = CNUMID + 1
         ENDIF

*   Store the picture details in the common block.
*   Copy some of the details from the current picture.
         CAGIWK( PICID ) = WKNAME
         CPICNM( PICID ) = SPICNM
         CGRAWK( PICID ) = CGRAWK( CURPID )
         PICACS( PICID ) = PICACS( CURPID )
         CIDIID( PICID ) = CIDIID( CURPID )
         CLEVEL( PICID ) = CNEST

*   Make this the current picture
         CURPID = PICID

*   Remember this in the database
         CALL AGI_1WPACT( WKNAME, SPICNM, STATUS )

*   Reset the root flag
         CROOT = 0

      ENDIF

*   Flush HDS if database file has been updated
      IF ( FLUSH ) THEN
*         CALL HDS_FREE( DABLOC, STATUS )
         FLUSH = .FALSE.
      ENDIF

  99  CONTINUE

*      print*, '+++++ AGI_RCS +++++'
*      call HDS_SHOW( 'FILES', status )
*      call HDS_SHOW( 'LOCATORS', status )

      END

