
#  N.B. the previous line should be blank.
#+
#  Name:
#     tabbedgaia

#  Purpose:
#     Starts the tabbed version of GAIA.

#  Type of Module:
#     Bourne shell script

#  Usage:
#     gaia

#  Description:
#     This command starts the tabbed version of GAIA. This manages
#     clone windows by including them in a single pane, rather
#     that different windows. It's a choice.

#  Authors:
#     PWD: P.W. Draper (STARLINK, Durham University)
#     {enter_new_authors_here}

#  History:
#     10-OCT-2003 (PWD):
#        Original version.
#     {enter_further_changes_here}

#  Bugs:
#     {note_any_bugs_here}

#-

#  Check that GAIA_DIR is around.

if test "$GAIA_DIR" = ""; then
  echo "!! Cannot start gaia; the GAIA_DIR environment variable is not set."
  exit 1
fi

#  Check that GAIA is installed.
if test -x $GAIA_DIR/gaia_swish; then

#  Try to stop problems with the AMS rendevous files by creating a
#  new directory as ADAM_USER.
  OLD_ADAM_USER=${ADAM_USER:-"${HOME}/adam"}
  ADAM_USER=${OLD_ADAM_USER}/gaia_$$
  export ADAM_USER

#  Make sure directory exists.
  if test -f $ADAM_USER; then
    rm -r -f $ADAM_USER
  fi
  mkdir -p $ADAM_USER

#  Make sure we remove this all on exit.
  trap 'rm -r -f $ADAM_USER;exit' 0 1 2 3 9 15

#  Set the BLT_LIBRARY variable. This is needed to locate some .pro files.
  BLT_LIBRARY=$GAIA_DIR
  export BLT_LIBRARY

#  And run up the interface.
  $GAIA_DIR/gaia_swish ${1+"$@"} -tabbedgaia 1

else
  echo "!! Cannot start gaia; failed to locate $GAIA_DIR/gaia_swish."
  exit 1
fi
