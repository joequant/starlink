*+PAR_GETLC Parameter get with U case conversion
	SUBROUTINE PAR_GETLC (PAR, VAL, STATUS)
	IMPLICIT NONE
* Input
	CHARACTER*(*)	PAR
* Output
	CHARACTER*(*) 	VAL
	INTEGER		STATUS
        integer         k 
 
	IF (STATUS .NE. 0) RETURN
 
	CALL PAR_GET0C (PAR, VAL, STATUS)
	CALL CHR_LCASE(VAL)


	END

