      SUBROUTINE ARD_WCS( IWCS, STATUS )
*+
*  Name:
*     ARD_WCS

*  Purpose:
*     Specify WCS information to be used in future calls to ARD_WORK.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL ARD_WCS( IWCS, STATUS )

*  Description:
*     This routine can be used to specify the cooordinate systems
*     which can be used in subsequent calls to ARD_WORK. The supplied
*     FrameSet must include a Frame with Domain "PIXEL". ARD expressions
*     passed to subsequent calls to ARD_WORK can include positions in any
*     of the Frames included in the supplied FrameSet (the ARD expression
*     should include suitable COFRAME or WCS statements to indicate which
*     coordinate system is being used).
*   
*     If this routine is not called prior to ARD_WORK (or if it is
*     called with IWCS set AST__NULL), then the ARD expression must
*     provide (either directly or through a WCS statement) positions in 
*     pixel coordinates.
*
*     The FrameSet pointer supplied is simply stored by this routine.
*     If any changes are subsequently made to the FrameSet by the calling 
*     routine, then these changes will be visible within ARD_WORK. In 
*     particular, if the calling routine annulls the FrameSet pointer, 
*     then ARD_WORK will fail.
*
*     The supplied FrameSet will be used by all subsequent calls to ARD_WORK 
*     until a new FrameSet is specified by calling ARD_WCS again. 

*  Arguments:
*     IWCS = INTEGER (Given)
*        An AST pointer to a FrameSet, or AST__NULL.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Authors:
*     DSB: David Berry (STARLINK)
*     {enter_new_authors_here}

*  History:
*     17-JUL-2001 (DSB):
*        Original version.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-
      
*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'ARD_ERR'          ! ARD error constants
      INCLUDE 'ARD_CONST'        ! ARD private constants
      INCLUDE 'AST_PAR'          ! AST constants and function declarations

*  Global Constants:
      INCLUDE 'ARD_COM'          ! ARD common blocks
*        CMN_AWCS = INTEGER (Write)
*           A pointer to the application FrameSet.

*  Arguments Given:
      INTEGER IWCS

*  Status:
      INTEGER STATUS             ! Global status

*  External References:
      EXTERNAL ARD1_INIT         ! Initialise ARD common blocks

*  Local Variables:
      LOGICAL OK                 ! Is the pointer usable?
*.

*  Check the inherited global status.
      IF( STATUS .NE. SAI__OK ) RETURN

*  Start an new error reporting context.
      CALL ERR_MARK     

*  If the supplied value is not AST__NULL or a FrameSet, set a flag.
      IF( IWCS .NE. AST__NULL ) THEN 
         OK = AST_ISAFRAMESET( IWCS, STATUS ) 
      ELSE
         OK = .TRUE.
      END IF

*  If an error occurred above, annull it.
      IF( STATUS .NE. SAI__OK ) CALL ERR_ANNUL( STATUS )

*  End the error reporting context.
      CALL ERR_RLSE

*  If a valid FrameSet pointer (or AST__NULL) was supplied, store it.
      IF( OK ) THEN 
         CMN_AWCS = IWCS

*  Otherwise, report an error.
      ELSE
         STATUS = ARD__BADAR
         CALL MSG_SETI( 'I', IWCS )
         CALL ERR_REP( 'ARD_WCS_ERR', 'ARD_WCS: The supplied value '//
     :                 '(^I) is not a valid FrameSet pointer '//
     :                 '(programming error).', STATUS )
      END IF

      END
