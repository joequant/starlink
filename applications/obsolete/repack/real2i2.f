*+REAL2I2	Converts REAL array to INTEGER*2
	SUBROUTINE REAL2I2(NPTS, RDATA, IDATA)
	INTEGER NPTS		!input
	REAL RDATA(NPTS)	!input
	INTEGER*2 IDATA(NPTS)	!output
*-Author	Clive Page	1990-NOV-9
	INTEGER J
*
	DO J = 1,NPTS
	    IDATA(J) = NINT(RDATA(J))
	END DO
	END
