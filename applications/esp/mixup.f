      SUBROUTINE MIXUP(STATUS)             
*+
*  Name:
*     MIXUP

*  Purpose:
*     To mixup the position of all the pixels in an image.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL MIXUP( STATUS )

*  Arguments:
*     STATUS = INTEGER(Given and Returned)
*     The global status.

*  Description:
*     Creates an equivalent noise image by swapping the positions of 
*     pairs of pixels taken from an image. This ensures that
*     structure due to galaxies/stars etc is spread randomly 
*     throughout the image but the count value statistics are retained. 
*
*     The routine may be used in conjunction with SKEW, SELFC, 
*     SELFCW or CORR when searching for low contrast objects. It 
*     provides comparison noise images which may be used to determine 
*     the significance of 'objects' identified by the other applications.

*  Usage:
*     MIXUP IN OUT 

*  ADAM Parameters:
*     IN = _NDF (Read)
*        The name of the NDF that is to be scrambled.
*     OUT = _NDF (Write)
*        The name of the NDF data that will be created. 

*  Authors:
*     GJP: Grant Privett (STARLINK)
*     {enter_new_authors_here}

*  History:
*     3-JUL-1993 (GJP)
*     (Original version)
*     16-Oct-1996 (GJP)
*     NAG free version.
*     24-Feb-1997 (GJP)
*     Minor Linux problem removed.

*  Examples:
*     mixup in=ic3374 out=ic3374m
*
*        The pixel positions in the input file IC3374 will be 
*        scrambled and the resulting image output as IC3374M.

*-

*  Type Definitions:                  ! No implicit typing
      IMPLICIT NONE

*  Global Constants:
      INCLUDE 'SAE_PAR'               ! Standard SAE constants
      INCLUDE 'NDF_PAR'               ! NDF_ public constant
                     
*  Status:     
      INTEGER STATUS                  ! Global status

*  Local Variables:                                                          
      INTEGER ELEMS                   ! Number of data items in the NDF
      INTEGER LBND(7)                 ! Lower bounds for each image axis
      INTEGER NDF1                    ! Identifier for the source NDF 
      INTEGER NDF2                    ! Identifier for the results NDF   
      INTEGER NDIM                    ! Number of dimensions in the image
      INTEGER POINT1(10)              ! Pointer to the data component of
                                      ! NDF1
      INTEGER POINT2(10)              ! Pointer to the data component of
                                      ! NDF2
      INTEGER PRANGE(2)               ! Number of pixels in the image x 
                                      ! and y axes 
      INTEGER UBND(7)                 ! Upper bounds for each image axis
      INTEGER XMAX                    ! Width in pixels of the image
      INTEGER YMAX                    ! Length in pixels of the image
*.                              

*   Check the inherited global status.
      IF (STATUS.NE.SAI__OK) RETURN
                                                                 
*   Begin an NDF context.                               
      CALL NDF_BEGIN

*   Indicate that the application is running.
      CALL MSG_BLANK(STATUS)
      CALL MSG_OUT(' ','ESP MIXUP running.',STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Obtain an identifier for the NDF structure to be examined.       
      CALL NDF_ASSOC('IN','READ',NDF1,STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Display the name of the NDF.
      CALL NDF_MSG('IN',NDF1)           
      CALL MSG_OUT(' ','Filename:   ^IN',STATUS)

*   See if the title component is defined. If so, display its value.
      CALL NDF_CMSG('TITLE',NDF1,'Title',STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999
      CALL MSG_OUT('TITLE','Title:      ^TITLE',STATUS)
    
*   Get the pixel-index bounds of an NDF and store in LBND and UBND.
      CALL NDF_BOUND(NDF1,2,LBND,UBND,NDIM,STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Store the size (in pixels) of the image dimensions.
      PRANGE(1)=UBND(1)-LBND(1)+1
      PRANGE(2)=UBND(2)-LBND(2)+1

*   Display the image x and y axis sizes (pixels).
      CALL MSG_SETI('PR1',PRANGE(1))
      CALL MSG_SETI('PR2',PRANGE(2))
      CALL MSG_OUT(' ','Shape:      ^PR1 x ^PR2 pixels',STATUS)
     
*   Display the image x and y axis ranges (pixels).
      CALL MSG_SETI('L1',LBND(1))
      CALL MSG_SETI('L2',UBND(1))
      CALL MSG_SETI('L3',LBND(2))
      CALL MSG_SETI('L4',UBND(2))
      CALL MSG_OUT(' ','Bounds:     x= ^L1:^L2  y= ^L3:^L4'
     :             ,STATUS)  

*   Calculate the maximum number of pixels in the image.
      ELEMS=PRANGE(2)*PRANGE(1)
       
*   Display the image size.
      CALL MSG_SETI('ELEMS',ELEMS)
      CALL MSG_OUT(' ','Image size: ^ELEMS pixels',STATUS)

*   Map the NDF data array as _REAL values for reading.
      CALL NDF_MAP(NDF1,'Data','_REAL','READ',POINT1(1),ELEMS,STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Propogate an NDF to contain the results.
      CALL NDF_PROP(NDF1,'Data','OUT',NDF2,STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Set the data type of the output NDF to real.
      CALL NDF_STYPE('_REAL',NDF2,'Data',STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Map the results NDF data array as _REAL values for updating.  
      CALL NDF_MAP(NDF2,'Data','_REAL','UPDATE',POINT2(1),ELEMS,STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999

*   Change the propogated title.
      CALL NDF_CPUT('ESP - MIXUP Image',NDF2,'TITLE',STATUS)
      IF (STATUS.NE.SAI__OK) GOTO 9999
  
      CALL MSG_BLANK(STATUS)

*   Inform the user of what will be done, since the routine takes a 
*   long while to run.
      CALL NDF_MSG('FOUT',NDF1)           
      CALL MSG_OUT(' ','Applying MIXUP to file: ^FOUT',STATUS)
      CALL NDF_MSG('FOUT',NDF2)           
      CALL MSG_OUT(' ','Results file will be:   ^FOUT',STATUS)
      CALL MSG_BLANK(STATUS)

*   Prepare the image size values that will be passed to the subroutine.
      XMAX=PRANGE(1)
      YMAX=PRANGE(2)

*   Mixup the image pixels.
         CALL MIX1_MIXER(XMAX,YMAX,ELEMS,
     :                   %VAL(POINT1(1)),%VAL(POINT2(1)),STATUS)  
         IF (STATUS.NE.SAI__OK) GOTO 9999

 9999 CONTINUE

*   Un-map the NDF data arrays.
      CALL NDF_UNMAP(NDF1,'Data',STATUS)
      CALL NDF_UNMAP(NDF2,'Data',STATUS) 

*   End the NDF context.
      CALL NDF_END(STATUS)

      END      
                                                 

      SUBROUTINE MIX1_MIXER(XMAX,YMAX,ELEMS,ARRAY1,ARRAY2,STATUS)    
*+
*  Name: 
*     MIX1_MIXER

*  Purpose:
*     Swaps pixels in the image so that any original structure is 
*     destroyed but the pixel statistics are retained.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL MIX1_MIXER(XMAX,YMAX,ELEMS,ARRAY1,ARRAY2,STATUS)
      
*  Description:
*     Selected pixel pairs randomly and swaps their values
*     as a way of scrambling the data in the image.

*  Arguments:
*     XMAX = INTEGER (Given)
*        The length of the image x axis. Units pixels.
*     YMAX = INTEGER (Given)
*        The length of the y axis of the image. Units pixels.
*     ELEMS = INTEGER (Given)
*        The number of pixels in the image.
*     ARRAY1(ELEMS) = REAL (Given)
*        The array containing the source NDF image count values.
*     ARRAY2(ELEMS) = REAL (Returned)
*        The array that eventually contains the scrambled image.
*     STATUS = INTEGER (Given and Returned) 
*        The global status.     

*  Authors:
*     GJP: Grant Privett (STARLINK)

*  History:
*     14-Oct-1992
*     (Original version)

*  Bugs:
*     None known.

*-

*  Type Definitions:                  ! No implicit typing
      IMPLICIT NONE
                                                                        
*  Global Constants:
      INCLUDE 'SAE_PAR'               ! Standard SAE constants
               
*  Arguments Given:
      INTEGER ELEMS                   ! Number of pixels in the data array
      INTEGER XMAX                    ! Length of the image x axis
      INTEGER YMAX                    ! Length of the image y axis
      REAL ARRAY1(ELEMS)              ! Array containing input NDF 

*  Arguments Returned:
      REAL ARRAY2(ELEMS)              ! Image array containing the
                                      ! scrambled image

*  Status:     
      INTEGER STATUS                  ! Global status

*  Local variables:                 
      INTEGER I                       ! Array element index
      INTEGER J                       ! Array element index
      INTEGER NUMBER                  ! Number of swaps so far
      INTEGER PERC                    ! Percentage of work done
      REAL    N                       ! Number of pixel pairs swapped between
                                      ! displays of the progress
      REAL    VALUE                   ! Pixel count value 
      REAL    RND                     ! Random number  

*.

*   Check the inherited global status.
      IF (STATUS.NE.SAI__OK) RETURN

*   Transfer the source image into the output image.
      DO 5 I=1,ELEMS
         ARRAY2(I)=ARRAY1(I)
 5    CONTINUE

*   Initialise the random number generator.  
      CALL MIX1_RAND(0,2001,RND,STATUS)

*   Number of pixel pairs swapped between 
*   displays of the progress.
 
*   Perform the pixel pair swaps.
      N=100000.

*   Forward through the array.
      NUMBER=0
      DO 10 I=1,ELEMS

*      Increment swap counter.
         NUMBER=NUMBER+1

*      Indicate that something is happening.
         IF (NUMBER.EQ.NINT(NUMBER/N)*N) THEN 
            PERC=NINT(NUMBER*50./ELEMS)
            CALL MSG_SETI('PERC',PERC)
            CALL MSG_OUT(' ','Percentage done so far: ^PERC',STATUS)
         END IF

*      Determine second pixel to be swapped. I defines the first.
         CALL MIX1_RAND(1,666,RND,STATUS)
         J=INT(RND*ELEMS+1)
         
*      Swap the pixels.
         VALUE=ARRAY2(I)
         ARRAY2(I)=ARRAY2(J)
         ARRAY2(J)=VALUE

 10   CONTINUE   


*   Repeat again backward through array. Just in case! 
      
*   Perform the pixel pair swaps.
      DO 20 I=ELEMS,1,-1

*      Increment swap counter.
         NUMBER=NUMBER+1

*      Indicate that something is happening.
         IF (NUMBER.EQ.NINT(NUMBER/N)*N) THEN 
            PERC=NINT(NUMBER*50./ELEMS)
            CALL MSG_SETI('PERC',PERC)
            CALL MSG_OUT(' ','Percentage done so far: ^PERC',STATUS)
         END IF

*      Determine second pixel to be swapped. I defines the first.
         CALL MIX1_RAND(1,666,RND,STATUS)
         J=INT(RND*ELEMS+1)
         
*      Swap the pixels.
         VALUE=ARRAY2(I)
         ARRAY2(I)=ARRAY2(J)
         ARRAY2(J)=VALUE

 20   CONTINUE   


 9999 CONTINUE

      END
 


      SUBROUTINE MIX1_RAND(TYPE,SEED,VALUE,STATUS)                                   >
 
*+
*  Name:
*     ELP1_RAND
 
*  Purpose:
*     Provide random numbers in the range 0-1.
      
*  Language:
*     Starlink Fortran 77
 
*  Invocation:
*      CALL ELP1_RAND(TYPE,SEED,VALUE,STATUS)    
 
*  Description:
*      Crude and simple random number generator based
*      upon a NIST routine supplied by Malcolm Currie.
 
*  Arguments:               
*     TYPE = INTEGER (Given)
*        Seed or request?                  
*     SEED = INTEGER (Given)
*        Random number seed.
*     VALUE = REAL (Returned)
*        Random number created.
*     STATUS = INTEGER (Given and Returned)
*        The global status.
 
*  Authors:
*     GJP: Grant Privett (STARLINK)
 
*  History:
*     12-Oct-1996 (GJP)
*     (Original version)
 
*  Bugs:
*     None known.
 
*-
 
*  Type Definitions:                  ! No implicit typing
      IMPLICIT NONE
                                                                        
*  Global Constants:
      INCLUDE 'SAE_PAR'               ! Standard SAE constants
 
*  Arguments Given:                              
      INTEGER TYPE                    ! Seed or request?
      INTEGER SEED                    ! Seed value.
 
*  Arguments Returned:
      REAL VALUE                      ! RAndom number
 
*  Status:     
      INTEGER STATUS                  ! Global status
      DOUBLE PRECISION FACTOR
      DOUBLE PRECISION TWO28
      DOUBLE PRECISION R                        

      SAVE R
*.
 
*   Assign values.               
      FACTOR=41475557.0D0
      TWO28=268435456.0D0

*   Check the inherited global status.
      IF (STATUS.NE.SAI__OK) RETURN   
 
*   First value only.
      IF(TYPE.EQ.0) R=DBLE(FLOAT(SEED))/TWO28
          
*   Evaluate.
      R=DMOD(R*FACTOR,1.0D0)
      VALUE=SNGL(R)

      END 
 

