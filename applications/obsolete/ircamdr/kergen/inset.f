*+  INSET - will be .TRUE. if STRING is a member of the "set" SET
      LOGICAL FUNCTION INSET( SET, STRING )
*    Description :
*     Checks a character variable, STRING, for being equal to any of a number
*     of possible values given in the pseudo set, SET, which is also a
*     character variable. The possible values can be any length and
*     can differ in length throughout the set. They should be separated
*     by a comma. Upper and lower case are treated as being equivalent.
*    Invocation :
*     RESULT = INSET( SET, STRING )
*    Parameters :
*     SET = CHAR*(*)( READ )
*           The pseudo set of possible values for string. This "set" takes the
*           following form : 'string1,string2,.......,stringN' where each of
*           the substrings string1 up to stringN can be of different
*           lengths.
*     STRING = CHAR*(*)( READ )
*           The string to be checked for membership of the pseudo set.
*    Result :
*     INSET = LOGICAL
*           .TRUE. if STRING is a member of the "set" SET, otherwise .FALSE.
*    Method :
*     Set the default return value to .FALSE.
*     Find out the stripped lengths of STRING and SET.
*     Initialise the current string start position
*     Initialise the END logical to .FALSE.
*     Do until end-of-set or INSET = .TRUE.
*        Find position of next comma relative to current string start position
*        If no more commas
*           Set comma position to be end of set
*           Set END to .TRUE.
*        Endif
*        INSET = .TRUE. if sub-string of SET between CURR and COMMA-1
*        matches the input test string STRING
*        Work out next CURR position from COMMA+1
*     Enddo
*     Return
*    Authors :
*     Dave Baines (ROE::ASOC5)
*     Mark McCaughrean UoE (REVA::MJM)
*    History :
*     06/10/1983 : Original version                     (ROE::ASOC5)
*     17/02/1984 : Documentation brought up to standard (ROE::ASOC5)
*     11-12-1985 : Modified considerably to allow the various
*                : members of SET to be any and different lengths.
*                : (UKTH::MARK)
*    Type Definitions :
      IMPLICIT NONE
*    Global constants :
      INCLUDE 'SAE_PAR'
*    Import :
      CHARACTER*(*)
     :  SET,   ! "set" of possible values
     :  STRING ! string to be tested for inclusion in the "set"
*    External references :
      LOGICAL
     :  CHR_SIMLR   ! true if two strings given match
      INTEGER
     :  CHR_LEN,    ! length of string ignoring stripped blanks
     :  CHR_INDEX   ! index position of character in string
*    Local variables :
      INTEGER
     :  SETLEN, ! stripped length of "set" character variable
     :  STRLEN, !    "        "   "  string     "       "
     :  CURR,   ! absolute position of start of next char value in SET
     :  COMMA   ! relative offset of next comma from CURR in SET
      LOGICAL
     :  END     ! true if end of set has been reached
*-

*    set up default return value
      INSET = .FALSE.

*    get the length of the "set", stripping trailing blanks
      SETLEN = CHR_LEN( SET )

*    get the length of the string, stripping trailing blanks
      STRLEN = CHR_LEN( STRING )

*    initialise the current position pointer - points to the
*    first character of the next string
      CURR = 1

*    set up 'end-of-set' marker
      END = .FALSE.

*    start search loop - continue until end of set is reached, or
*    string is matched
      DO WHILE( ( .NOT. END ) .AND. ( .NOT. INSET ) )

*       find the position of the next comma relative to the position
*       of CURR, i.e. how many characters away it is from CURR
         COMMA = CHR_INDEX( SET( CURR:SETLEN ), ',' )

*       if this value is zero, then there are no more commas in the
*       set, so set COMMA to be the number of characters left from
*       the current position of CURR to the end of the SET plus 2,
*       Also, set END to be .TRUE.
         IF( COMMA .EQ. 0 ) THEN
            COMMA = SETLEN - CURR + 2
            END = .TRUE.
         END IF

*       now test to see whether the string between CURR and COMMA-1
*       matches up to the value of STRING - if so, INSET = .TRUE.
         INSET = CHR_SIMLR(SET(CURR:CURR+COMMA-2),STRING(1:STRLEN))

*       update the position of CURR for next pass through loop
         CURR = CURR + COMMA

      END DO

*    that's it - return

      END
