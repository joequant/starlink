*+PAR_GETUC Parameter get with U case conversion
	SUBROUTINE PAR_GETUC (PAR, VAL, STATUS)
	IMPLICIT NONE
* Input
	CHARACTER*(*)	PAR
* Output
	CHARACTER*(*) 	VAL
	INTEGER		STATUS
        integer         k 
 
	IF (STATUS .NE. 0) RETURN
 
	CALL PAR_GET0C (PAR, VAL, STATUS)
	CALL CHR_UCASE(VAL)


	END

