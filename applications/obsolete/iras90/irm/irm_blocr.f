      SUBROUTINE IRM_BLOCR( BAD, SAMBAD, VAR, NX, NY, A, IBOXX,
     :                      IBOXY, NLIM, B, BADOUT, ASUM, NSUM,
     :                      STATUS )
*+
*  Name:
*     IRM_BLOCR

*  Purpose:
*     Smooth a 2-dimensional image using a rectangular box filter.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL IRM_BLOCR( BAD, SAMBAD, VAR, NX, NY, A, IBOXX,
*                     IBOXY, NLIM, B, BADOUT, ASUM, NSUM, STATUS )

*  Description:
*     The routine smooths a 2-dimensional image using a rectangular box
*     filter; each pixel is replaced by the mean of those good
*     neighbours which lie within a box of specified size.

*  Arguments:
*     BAD = LOGICAL (Given)
*        Whether it is necessary to check for bad pixels in the input
*        image.
*     SAMBAD = LOGICAL (Given)
*        If a .TRUE. value is given for this argument, then bad input
*        pixels will be propagated to the output image unchanged (a
*        smoothed output value will be calculated for all other
*        pixels). If a .FALSE. value is given, then the NLIM argument
*        determines whether an output pixel is good or bad.  The value
*        of NLIM is not relevant if SAMBAD is .TRUE..
*     VAR = LOGICAL (Given)
*        If a .FALSE. value is given for this argument, then the
*        smoothing applied will be appropriate to a data image. If a
*        .TRUE. value is given, then the smoothing will be appropriate
*        to an image containing variance values. In the latter case the
*        output values will be (on average) smaller than the input
*        values to take account of the variance-reducing effect which
*        smoothing produces.
*     NX = INTEGER (Given)
*        First dimension of the image to be smoothed.
*     NY = INTEGER (Given)
*        Second dimension of the image to be smoothed.
*     A( NX, NY ) = REAL (Given)
*        Input image to be smoothed.
*     IBOXX = INTEGER (Given)
*        Half-size of the smoothing box in pixels in the X direction
*        (the actual size of the box used will be 2*IBOXX+1 pixels).
*     IBOXY = INTEGER (Given)
*        Half-size of the smoothing box in pixels in the Y direction
*        (the actual size of the box used will be 2*IBOXY+1 pixels).
*     NLIM = INTEGER (Given)
*        Minimum number of good pixels which must be present in the
*        smoothing box in order to calculate a smoothed output pixel.
*        If this minimum number is not satisfied, then a bad output
*        pixel will result. A value between 1 and the total number of
*        pixels in the smoothing box should be supplied.
*     B( NX, NY ) = REAL (Returned)
*        The smoothed output image.
*     BADOUT = LOGICAL (Returned)
*        Whether bad pixels are present in the output image.
*     ASUM( NX ) = REAL (Returned)
*        Workspace for the pixel sums.
*     NSUM( NX ) = INTEGER (Returned)
*        Workspace for counting good pixels.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     DSB: David Berry (STARLINK)
*     {enter_new_authors_here}

*  History:
*     29-SEP-1992 (DSB):
*        Original version, copied from KAPPA routine KPG1_BLOCx written
*        by RFWS.
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! PRIMDAT primitive data constants

*  Arguments Given:
      LOGICAL BAD
      LOGICAL SAMBAD
      LOGICAL VAR
      INTEGER NX
      INTEGER NY
      REAL    A( NX, NY )
      INTEGER IBOXX
      INTEGER IBOXY
      INTEGER NLIM

*  Arguments Returned:
      REAL    B( NX, NY )
      LOGICAL BADOUT
      REAL    ASUM( NX )
      INTEGER NSUM( NX )

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      REAL ASUM0                 ! Good pixel sum
      REAL DENOM                 ! Denominator for smoothed output value
      REAL NPIX                  ! Number of pixels in smoothing box
      INTEGER IIX                ! Loop counter for summing over pixels
      INTEGER IIY                ! Loop counter for summing over lines
      INTEGER ILINE              ! Contributing line number
      INTEGER IPIX               ! Contributing pixel number
      INTEGER IX                 ! First array index
      INTEGER IY                 ! Second array index
      INTEGER NEWX               ! X position of new pixel
      INTEGER NEWY               ! Y position of new pixel
      INTEGER NSUM0              ! Good pixel count
      INTEGER OLDX               ! X position of old pixel
      INTEGER OLDY               ! Y position of old line

*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Initialise the bad output pixel flag and the workspace arrays.
      BADOUT = .FALSE.
      DO 1 IX = 1, NX
         ASUM( IX ) = 0.0
         NSUM( IX ) = 0
 1    CONTINUE

*  Calculate the number of pixels in the smoothing box.
      NPIX = ( 2 * IBOXX + 1 ) * ( 2 * IBOXY + 1 )

*  Loop to initialise elements of the two workspace arrays so that they
*  contain sum for the good pixels in a box of size 1x(2*IBOXY+1)
*  centred on pixel location (IX,0). First loop over all the possible
*  contributing lines.
      DO 4 IIY = -IBOXY, IBOXY

*  If a line lies outside the input image, then use the boundary line
*  instead.
         IF ( IIY .LT. 1 ) THEN
            ILINE = 1
         ELSE IF ( IIY .GT. NY ) THEN
            ILINE = NY
         ELSE
            ILINE = IIY
         END IF

*  Accumulate sums for each box in the workspace arrays. If bad pixels
*  are present, then test for them.
         IF ( BAD ) THEN
            DO 2 IX = 1, NX
               IF ( A( IX, ILINE ) .NE. VAL__BADR ) THEN
                  ASUM( IX ) = ASUM( IX ) + A( IX, ILINE )
                  NSUM( IX ) = NSUM( IX ) + 1
               END IF
 2          CONTINUE

*  If no bad pixels are present, then omit the test for them.
         ELSE
            DO 3 IX = 1, NX
               ASUM( IX ) = ASUM( IX ) + A( IX, ILINE )
 3          CONTINUE
         END IF
 4    CONTINUE

*  Smooth down the image.
*  =====================
*  Loop to form smoothed output values for each image line by moving the
*  accumulated sums for each initial box down the image.
      DO 9 IY = 1, NY

*  Find the position of the old input line which is lost from the boxes
*  as they are moved down the image and the position of the new line
*  which enters the boxes. Allow for the image boundaries.
         OLDY = IY - IBOXY - 1
         IF ( OLDY .LT. 1 ) OLDY = 1
         NEWY = IY + IBOXY
         IF ( NEWY .GT. NY ) NEWY = NY

*  Remove the pixels in the old line from the accumulated sums and add
*  the pixels in the new line.  If bad pixels are present, then test
*  for them.
         IF ( BAD ) THEN
            DO 5 IX = 1, NX
               IF ( A( IX, OLDY ) .NE. VAL__BADR ) THEN
                  ASUM( IX ) = ASUM( IX ) - A( IX, OLDY )
                  NSUM( IX ) = NSUM( IX ) - 1
               END IF
               IF ( A( IX, NEWY ) .NE. VAL__BADR ) THEN
                  ASUM( IX ) = ASUM( IX ) + A( IX, NEWY )
                  NSUM( IX ) = NSUM( IX ) + 1
               END IF
 5          CONTINUE

*  If bad pixels are absent, then there is no need to test for them.
*  There is also no need to keep track of the number of good pixels in
*  the box, as this will always be equal to the total number of pixels
*  in the smoothing box.
         ELSE
            DO 6 IX = 1, NX
               ASUM( IX ) = ASUM( IX ) + ( A( IX, NEWY ) -
     :                                     A( IX, OLDY ) )
 6          CONTINUE
         END IF

*  Smooth along each image line.
*  ============================
*  Form initial sums for a box of size (2*IBOXX+1)x1 centred on pixel
*  (0,IY).
         ASUM0 = 0.0
         NSUM0 = 0
         DO 7 IIX = -IBOXX, IBOXX

*  Allow for the image boundaries.
            IF ( IIX .LT. 1 ) THEN
               IPIX = 1
            ELSE IF ( IIX .GT. NX ) THEN
               IPIX = NX
            ELSE
               IPIX = IIX
            END IF

*  Use the current values stored in the workspace arrays to form these
*  initial sums.
            ASUM0 = ASUM0 + ASUM( IPIX )
            IF ( BAD ) NSUM0 = NSUM0 + NSUM( IPIX )
 7       CONTINUE

*  Loop to move these sums along the current image line.
         DO 8 IX = 1, NX

*  Find the position of the old pixel which is lost from the box as it
*  moves along the line and the position of the new one which enters
*  it. Allow for the image boundaries.
            OLDX = IX - IBOXX - 1
            IF ( OLDX .LT. 1 ) OLDX = 1
            NEWX = IX + IBOXX
            IF ( NEWX .GT. NX ) NEWX = NX

*  Update the sums by subtracting the old pixel and adding the new one.
            ASUM0 = ASUM0 + ( ASUM( NEWX ) - ASUM( OLDX ) )
            IF ( BAD ) NSUM0 = NSUM0 - NSUM( OLDX ) + NSUM( NEWX )

*  Form the denominator for calculating the smoothed output value. If
*  there are no bad pixels present, then this is equal to the number of
*  pixels in the smoothing box. Otherwise, use the accumulated sum of
*  good pixels.
            IF ( .NOT. BAD ) THEN
               DENOM = NPIX
            ELSE
               DENOM = NSUM0
            END IF

*  If smoothing a variance image, then use the square of the number of
*  good pixels (this takes account of the variance-reducing effect that
*  smoothing introduces).
            IF ( VAR ) DENOM = DENOM * DENOM

*  Calculate the smoothed output value.
*  ===================================
*  This is valid if there are no bad pixels present.
            IF ( .NOT. BAD ) THEN
               B( IX, IY ) = ASUM0 / DENOM

*  It is also valid if SAMBAD is .TRUE. and the corresponding input
*  pixel is not bad.
            ELSE IF ( SAMBAD ) THEN
               IF ( A( IX, IY ) .NE. VAL__BADR ) THEN
                  B( IX, IY ) = ASUM0 / DENOM

*  It is bad if SAMBAD is .TRUE. and the corresponding input pixel is
*  bad.  Note that bad pixels are present in the output image.
               ELSE
                  B( IX, IY ) = VAL__BADR
                  BADOUT = .TRUE.
               END IF

*  It is valid if SAMBAD is .FALSE. and the NLIM criterion is met.
            ELSE
               IF ( NSUM0 .GE. NLIM ) THEN
                  B( IX, IY ) = ASUM0 / DENOM

*  It is bad if the NLIM criterion is not met. Note that bad pixels are
*  present in the output image.
               ELSE
                  B( IX, IY ) = VAL__BADR
                  BADOUT = .TRUE.
               END IF
            END IF
 8       CONTINUE
 9    CONTINUE

      END
