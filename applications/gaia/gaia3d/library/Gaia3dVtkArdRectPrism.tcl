#+
#  Name:
#     Gaia3dArdRectPrism

#  Type of Module:
#     [incr Tcl] class

#  Purpose:
#     Create and manipulate a rectangular ARD prism.

#  Description:
#     Class that extends Gaia3dVtkArdPrism to support rectangular shapes.
#     A rectangle is axis aligned.

#  Copyright:
#     Copyright (C) 2007 Science and Technology Facilities Council
#     All Rights Reserved.

#  Licence:
#     This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License as
#     published by the Free Software Foundation; either version 2 of the
#     License, or (at your option) any later version.
#
#     This program is distributed in the hope that it will be
#     useful, but WITHOUT ANY WARRANTY; without even the implied warranty
#     of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
#     02111-1307, USA

#  Authors:
#     PWD: Peter Draper (JAC, Durham University)
#     {enter_new_authors_here}

#  History:
#     07-DEC-2007 (PWD):
#        Original version.
#     {enter_further_changes_here}

#-

#.

itcl::class ::gaia3d::Gaia3dVtkArdRectPrism {

   #  Inheritances:
   #  -------------
   inherit gaia3d::Gaia3dVtkArdPrism

   #  Constructor:
   #  ------------
   constructor {args} {

      #  Set any configuration variables.
      eval configure $args
   }

   #  Destructor:
   #  -----------
   destructor  {
   }

   #  Methods and procedures:
   #  -----------------------

   #  Create the polygon for the rectangle locus.
   protected method create_polygon_ {} {

      $points_ Reset
      $cells_ Reset
      $cells_ InsertNextCell 4

      if { $axis == 1 } {
         $points_ InsertPoint 0 0.0 $x0 $y0
         $points_ InsertPoint 1 0.0 $x0 $y1
         $points_ InsertPoint 2 0.0 $x1 $y1
         $points_ InsertPoint 3 0.0 $x1 $y0
      } elseif { $axis == 2 } {
         $points_ InsertPoint 0 $x0 0.0 $y0
         $points_ InsertPoint 1 $x0 0.0 $y1
         $points_ InsertPoint 2 $x1 0.0 $y1
         $points_ InsertPoint 3 $x1 0.0 $y0
      } else {
         $points_ InsertPoint 0 $x0 $y0 0.0
         $points_ InsertPoint 1 $x0 $y1 0.0
         $points_ InsertPoint 2 $x1 $y1 0.0
         $points_ InsertPoint 3 $x1 $y0 0.0
      }
      $cells_ InsertCellPoint 0
      $cells_ InsertCellPoint 1
      $cells_ InsertCellPoint 2
      $cells_ InsertCellPoint 3

      $polydata_ SetPoints $points_
      $polydata_ SetPolys $cells_
   }

   #  Configuration options: (public variables)
   #  ----------------------

   #  X coordinate, lower left.
   public variable x0 0.0

   #  Y coordinate, lower left.
   public variable y0 0.0

   #  X coordinate, upper right.
   public variable x1 1.0

   #  Y coordinate, upper right.
   public variable y1 1.0

   #  Protected variables: (available to instance)
   #  --------------------

   #  Common variables: (shared by all instances)
   #  -----------------

#  End of class definition.
}
