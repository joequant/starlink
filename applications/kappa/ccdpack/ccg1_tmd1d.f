      SUBROUTINE CCG1_TMD1D( ALPHA, STACK, NPIX, NLINES, VARS, MINPIX,
     :                         COVEC, NMAT, RESULT, RESVAR, WRK1, NCON,
     :                         POINT, USED, STATUS )
*+
*  Name:
*     CCG1_TMD1

*  Purpose:
*     Combines data lines using a trimmed mean.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL CCG1_TMD1( ALPHA, STACK, NPIX, NLINES, VARS, MINPIX, COVEC,
*                     NMAT, RESULT, RESVAR, WRK1, NCON, POINT, USED,
*                     STATUS )

*  Description:
*     This routine accepts an array consisting a series of (vectorised)
*     lines of data. The data values in the lines are then combined to
*     form an alpha trimmed mean line. The output means are returned in
*     the array RESULT.  The variances are propagation through the
*     combination process and returned in the RESVAR array.

*  Arguments:
*     ALPHA = REAL (Given)
*        The fraction of data to trim from upper and lower orders.
*        (MUST BE GREATER THAN 0.0 AND LESS 0.5)
*     STACK( NPIX, NLINES ) = DOUBLE PRECISION (Given)
*        The array of lines which are to be combined into a single line.
*     NPIX = INTEGER (Given)
*        The number of pixels in a line of data.
*     NLINES = INTEGER (Given)
*        The number of lines of data in the stack.
*     VARS( NPIX, NLINES ) = DOUBLE PRECISION (Given)
*        The data variances.
*     MINPIX = INTEGER (Given)
*        The minimum number of pixels required to contribute to an
*        output pixel.
*     COVEC( NMAT, NLINES ) = DOUBLE PRECISION (Given)
*        The packed variance-covariance matrix of the order statistics
*        from a normal distribution of sizes up to NLINES, produced by
*        CCD1_ORVAR.
*     NMAT = INTEGER (Given)
*        Size of the first dimension of COVEC.
*     RESULT( NPIX ) = DOUBLE PRECISION (Returned)
*        The output line of data.
*     RESVAR( NPIX ) = DOUBLE PRECISION (Returned)
*        The output variance.
*     WRK1( NLINES ) = DOUBLE PRECISION (Given and Returned)
*        Workspace for calculations.
*     NCON( NLINES ) = DOUBLE PRECISION (Given and Returned)
*        The actual number of contributing pixels.
*     POINT( NLINES ) = INTEGER (Given and Returned)
*        Workspace to hold pointers to the original positions of the
*        data before extraction and conversion in to the WRK1 array.
*     USED( NLINES ) = LOGICAL (Given and Returned)
*        Workspace used to indicate which values have been used in
*        estimating a resultant value.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Authors:
*     PDRAPER: Peter Draper (STARLINK)
*     BRADC: Brad Cavanagh (JAC)
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}

*  History:
*     21-MAY-1992 (PDRAPER):
*        Original version.
*     11-OCT-2004 (BRADC):
*        No longer use NUM_CMN.
*     2006 August 6 (MJC):
*        Exclude data with non-positive variance.
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! PRIMDAT constants

*  Arguments Given:
      REAL ALPHA
      INTEGER NPIX
      INTEGER NLINES
      INTEGER MINPIX
      INTEGER NMAT
      DOUBLE PRECISION STACK( NPIX, NLINES )
      DOUBLE PRECISION VARS( NPIX, NLINES )
      DOUBLE PRECISION COVEC( NMAT, NLINES )

*  Arguments Given and Returned:
      DOUBLE PRECISION NCON( NLINES )
      DOUBLE PRECISION WRK1( NLINES )

*  Arguments Returned:
      DOUBLE PRECISION RESULT( NPIX )
      DOUBLE PRECISION RESVAR( NPIX )
      LOGICAL USED( NLINES )
      INTEGER POINT( NLINES )

*  Status:
      INTEGER STATUS             ! Global status

*  Global Variables:


*  External References:
      EXTERNAL NUM_WASOK
      LOGICAL NUM_WASOK          ! Was numeric operation ok?
      EXTERNAL NUM_TRAP
      INTEGER NUM_TRAP           ! Numerical error handler

*  Local Variables:
      INTEGER I                  ! Loop variable
      INTEGER J                  ! Loop variable
      DOUBLE PRECISION VAL       ! Weighted median
      DOUBLE PRECISION VAR       ! Population variance
      DOUBLE PRECISION SUM       ! Sum of inverse variance
      INTEGER NGOOD              ! Number of good pixels

*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! NUM_ type conversion functions
      INCLUDE 'NUM_DEF_CVT'      ! Define functions...

*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Set the numeric error and set error flag value.
      CALL NUM_HANDL( NUM_TRAP )

*  Loop over for all possible output pixels.
      DO 1 I = 1, NPIX
         NGOOD = 0
         CALL NUM_CLEARERR()
         SUM = 0.0D0

*  Loop over all possible contributing pixels.
         DO 2 J = 1, NLINES
            IF ( STACK( I, J ) .NE. VAL__BADD .AND.
     :            VARS( I, J ) .NE. VAL__BADD .AND.
     :            VARS( I, J ) .GT. VAL__SMLD ) THEN

*  Increment good value counter.
               NGOOD = NGOOD + 1

*  Update the pointers to the positions of the unextracted data.
               POINT( NGOOD ) = J

*  Convert input type floating point value.
               WRK1( NGOOD ) = NUM_DTOD( STACK( I, J ) )

*  Sum inverse variances.
               SUM = SUM + 1.0D0 / NUM_DTOD( VARS( I, J ) )

*  Trap conversion failures.
               IF ( .NOT. NUM_WASOK() ) THEN

*  Decrement NGOOD, do not let it go below zero.
                  NGOOD = MAX( 0, NGOOD - 1 )
                  CALL NUM_CLEARERR()
               END IF
            END IF
 2       CONTINUE

*  Continue if more than one good value.
         IF ( NGOOD .GT. 1 ) THEN

*  Sort these values into increasing order.
            CALL CCG1_IS2D( WRK1, POINT, NGOOD, STATUS )

*  Create population variance.
            SUM = 1.0D0 / SUM

*  Find the trimmed mean.
            CALL CCG1_TRM2D( ALPHA, WRK1, SUM, NGOOD, USED,
     :                       COVEC( 1, NGOOD ), VAL, VAR, STATUS )

*  Update the used line counters.
            DO 3 J = 1, NGOOD
               IF ( USED ( J ) ) THEN
                  NCON( POINT( J ) ) = NCON( POINT( J ) ) + 1.0D0
               END IF
 3          CONTINUE
         ELSE IF ( NGOOD .EQ. 1 ) THEN 

*  Population of one value - just use it.
            VAR = 1.0D0 / SUM
            VAL = WRK1( NGOOD )

         END IF

*  If there are sufficient good pixels output the result.
         IF ( NGOOD .GE. MINPIX ) THEN
            RESULT( I ) = VAL
            RESVAR( I ) = VAR

*  Trap numeric errors.
            IF ( .NOT. NUM_WASOK() ) THEN
               RESULT( I ) = VAL__BADD
               RESVAR( I ) = VAL__BADD
            END IF
         ELSE

*  Not enough contributing pixels, set output invalid.
            RESULT( I ) = VAL__BADD
            RESVAR( I ) = VAL__BADD
         END IF
 1    CONTINUE

*  Remove the numerical error handler.
      CALL NUM_REVRT

      END
* $Id$
