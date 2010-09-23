/*
*+
*  Name:
*     smf_filter_fromkeymap

*  Purpose:
*     Build a smfFilter from parameters given in an astKeyMap

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     C function

*  Invocation:
*     smf_filter_fromkeymap( smfFilter *filt, AstKeyMap *keymap, smfHead *hdr,
*                            int *dofilt, int *status )

*  Arguments:
*     filt = smfFilter * (Given and Returned)
*        Pointer to smfFilter to be modified
*     keymap = AstKeyMap* (Given)
*        keymap containing parameters
*     hdr = smfHead *(Given)
*        Required if filt_edge_smallscale/largscale requested, otherwise can
*        be NULL.
*     dofilt = int* (Returned)
*        If true, frequency-domain filtering is required
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Description:
*     This function builds a filter using parameters stored in an astKeyMap:
*     filt_edgelow, filt_edgehigh, filt_notchlow, filt_notchhigh
*     See description of these parameters in smf_get_cleanpar header.
*     If none of these parameters are defined, nothing is done to the
*     supplied smfFilter.
*
*  Authors:
*     EC: Edward Chapin (UBC)
*     DSB: David S Berry (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     2009-04-17 (EC):
*        Initial version.
*     2010-06-21 (DSB):
*        Store a a value for the "wlim" component of the filter structure.
*     2010-06-25 (DSB):
*        Store a value for the "apod_length" component of the filter structure.
*     2010-07-22 (DSB):
*        Use a dynamic default for APOD based on frequency.
*    2010-09-23 (EC):
*        Choose filter edges based on scanvel (stored in hdr)
*     {enter_further_changes_here}

*  Notes:

*  Copyright:
*     Copyright (C) 2009-2010 University of British Columbia
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 3 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public
*     License along with this program; if not, write to the Free
*     Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*     MA 02111-1307, USA

*  Bugs:
*     {note_any_bugs_here}
*-
*/

#include <stdio.h>

/* Starlink includes */
#include "ast.h"
#include "mers.h"
#include "ndf.h"
#include "sae_par.h"
#include "prm_par.h"
#include "par_par.h"

/* SMURF includes */
#include "libsmf/smf.h"
#include "libsmf/smf_err.h"

#define FUNC_NAME "smf_filter_fromkeymap"

void smf_filter_fromkeymap( smfFilter *filt, AstKeyMap *keymap,
                            const smfHead *hdr, int *dofilt, int *status ) {

  int dofft=0;              /* Set if freq. domain filtering the data */
  double f_edgelow;         /* Freq. cutoff for low-pass edge filter */
  double f_edgehigh;        /* Freq. cutoff for high-pass edge filter */
  double f_edgesmall;       /* Select low-pass based on spatial scale */
  double f_edgelarge;       /* Select high-pass based on spatial scale */
  double f_low;             /* Lowest edge frequency */
  double f_notchlow[SMF__MXNOTCH]; /* Array low-freq. edges of notch filters */
  double f_notchhigh[SMF__MXNOTCH];/* Array high-freq. edges of notch filters */
  int f_nnotch=0;           /* Number of notch filters in array */
  int i;                    /* Loop count */
  int ival;                 /* Dummy integer argument */

  /* Main routine */
  if (*status != SAI__OK) return;

  /* Search for filtering parameters in the keymap */
  smf_get_cleanpar( keymap, NULL, NULL, NULL, NULL, NULL, NULL,
                    NULL, &f_edgelow, &f_edgehigh, &f_edgesmall,
                    &f_edgelarge, f_notchlow, f_notchhigh, &f_nnotch, &dofft,
                    NULL, NULL, NULL, NULL, NULL, NULL, status );

  /* Modify edge filters if spacial scales were requested */
  if( f_edgesmall || f_edgelarge ) {
    if( !hdr || (hdr->scanvel<=0) ) {
      *status = SAI__ERROR;
      errRep( "", FUNC_NAME
              ": FILT_EDGE_SMALLSCALE or FILT_EDGE_LARGESCALE, but "
              "no smfHead supplied or invalid smfHead->scanvel", status );
      return;
    } else {
      msgOutiff( MSG__VERB, "", FUNC_NAME
                 ": Based on a slew speed of %.1lf arcsec/sec, setting:",
                 status, hdr->scanvel );

      if( f_edgesmall ) {
        f_edgelow = hdr->scanvel / f_edgesmall;
        msgOutiff( MSG__VERB, "", FUNC_NAME
                   ": FILT_EDGELOW = %.3lf Hz (> %.1lf arcsec scales)",
                   status, f_edgelow, f_edgesmall );
      }

      if( f_edgesmall ) {
        f_edgehigh = hdr->scanvel / f_edgelarge;
        msgOutiff( MSG__VERB, "", FUNC_NAME
                   ": FILT_EDGEHIGH = %.3lf Hz (< %.1lf arcsec scales)",
                   status, f_edgehigh, f_edgelarge );
      }
    }
  }



  /* Return dofilt if requested */
  if( dofilt ) {
    *dofilt = dofft;
  }

  if( f_edgelarge ) {

  }

  /* If filtering parameters given, create filter  */
  if( dofft ) {
    if( f_edgelow ) {
      smf_filter_edge( filt, f_edgelow, 1, status );
    }

    if( f_edgehigh ) {
      smf_filter_edge( filt, f_edgehigh, 0, status );
    }

    if( f_nnotch ) {
      smf_filter_notch( filt, f_notchlow, f_notchhigh, f_nnotch, status );
    }

    if( ! astMapGet0D( keymap, "FILT_WLIM", &(filt->wlim) ) ) {
       filt->wlim = VAL__BADD;
    }


    /* If no apodisation length has been given, use a default of
       1/(steptime*freq) where freq is the lowest edge or notch
       frequency. */

    if( astMapGet0I( keymap, "APOD", &ival ) ) {
       filt->apod_length = ival;

    } else {

      f_low = ( f_edgehigh > 0.0 ) ? f_edgehigh : VAL__MAXD;
      if( f_edgelow > 0.0 && f_edgelow < f_low ) f_low = f_edgelow;
      for( i = 0; i < f_nnotch; i++ ) {
        if( f_notchlow[ i ] > 0.0 && f_notchlow[ i ] < f_low ) f_low = f_notchlow[ i ];
      }

      if( f_low != VAL__MAXD ) {
         filt->apod_length = 0.5*filt->df*filt->ntslice/f_low;
      } else {
         filt->apod_length = 0;
      }

      msgSeti( "P", (int) filt->apod_length );
      msgOutif( MSG__VERB, "", "Apodising ^P samples at start and end of "
                "each time stream.", status );
    }
  }
}
