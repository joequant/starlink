# post-process bbl files generated by plainhtml.bst
# $Id$
:redo
/%$/{N
s/%\n//
b redo
}
s/%/\\%/g
s/[{}~]/ /g
