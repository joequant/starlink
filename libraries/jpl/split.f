C------------------------------- SUBROUTINE 'SPLIT' --------------------
C
C+++++++++++++++++++++++++
C
      SUBROUTINE SPLIT(TT,FR)
C
C+++++++++++++++++++++++++
C
C     THIS SUBROUTINE BREAKS A D.P. NUMBER INTO A D.P. INTEGER
C     AND A D.P. FRACTIONAL PART.
C
C     CALLING SEQUENCE PARAMETERS:
C
C       TT = D.P. INPUT NUMBER
C
C       FR = D.P. 2-WORD OUTPUT ARRAY.
C            FR(1) CONTAINS INTEGER PART
C            FR(2) CONTAINS FRACTIONAL PART
C
C            FOR NEGATIVE INPUT NUMBERS, FR(1) CONTAINS THE NEXT
C            MORE NEGATIVE INTEGER; FR(2) CONTAINS A POSITIVE FRACTION.
C
C       CALLING SEQUENCE DECLARATIONS
C
      DOUBLE PRECISION TT
      DOUBLE PRECISION FR(2)
C
C
C       MAIN ENTRY -- GET INTEGER AND FRACTIONAL PARTS
C
      FR(1)=DINT(TT)
      FR(2)=TT-FR(1)
C
      IF(TT.GE.0.D0 .OR. FR(2).EQ.0.D0) RETURN
C
C       MAKE ADJUSTMENTS FOR NEGATIVE INPUT NUMBER
C
      FR(1)=FR(1)-1.D0
      FR(2)=FR(2)+1.D0
C
      RETURN
C
      END
