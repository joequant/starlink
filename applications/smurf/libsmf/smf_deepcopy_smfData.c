/*
*+
*  Name:
*     smf_deepcopy_smfData

*  Purpose:
*     Copy all elements of a smfData structure

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     Subroutine

*  Invocation:
*     new = smf_deepcopy_smfData( const smfData *old, const int rawconvert,
*              const int flags,  int * status );

*  Arguments:
*     old = const smfData* (Given)
*        Pointer to smfData to be copied
*     rawconvert = const int (Given)
*        Flag to denote whether to convert integer to double
*     flags = const int (Given)
*        Control which items are copied. The following flag values
*        are supported (and can be combined):
*          - SMF__NOCREATE_HEAD     Do not copy the smfHead
*          - SMF__NOCREATE_FILE     Do not copy the smfFile
*          - SMF__NOCREATE_DA       Do not copy the smfDA
*          - SMF__NOCREATE_DATA     Do not copy DATA component
*          - SMF__NOCREATE_VARIANCE Do not copy VARIANCE component
*          - SMF__NOCREATE_QUALITY  Do not copy QUALITY component
*
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Return Value:
*     smf_deepcopy_smfData = smfData*
*        Pointer to newly created smfData. NULL on error.

*  Description:
*     This function copies all information from an existing smfData
*     structure and all the internal structures to a new smfData
*     structure.

*  Notes:
*     - Free this memory using smf_close_file, via a smfData structure.
*     - Can be freed with a smf_free if header resources are freed first.
*     - The mapped smfData is no longer associated with a file so the
*       output smfFile component will not include an NDF identifier.

*  Authors:
*     Tim Jenness (TIMJ)
*     Andy Gibb (UBC)
*     Ed Chapin (UBC)
*     {enter_new_authors_here}

*  History:
*     2006-03-23 (AGG):
*        Initial version.
*     2006-03-24 (TIMJ):
*        Deal with different data types and quality
*     2006-03-28 (AGG):
*        Change API to allow copying of raw integer data to a double
*        output array
*     2006-04-21 (AGG):
*        - Update API to accept a flags to determine whether the smfFile,
*          smfDA and smfHead elements should be copied
*        - Make use of these SMF__NOCREATE_* flags
*        - Copy history element only if present
*     2006-05-16 (AGG):
*        Check that ncoeff is non-zero before attempting to copy poly
*        coefficients, add checking of malloc'ed pointers
*     2008-07-16 (TIMJ):
*        Document flags. Do not copy the NDF id (otherwise smf_close_file
*        will not try to free the malloced memory)
*     2008-07-22 (EC):
*        -Implements SMF__NOCREATE_DATA/VARIANCE/QUALITY
*        -Copy isTordered flag
*     2008-07-23 (EC):
*        Correctly calculate npts if isTordered=0
*     2008-08-26 (TIMJ):
*        Need to trap VAL__BADI
*     {enter_further_changes_here}

*  Copyright:
*     Copyright (C) 2008 Science and Technology Facilities Council.
*     Copyright (C) 2006 Particle Physics and Astronomy Research
*     Council. University of British Columbia. All Rights Reserved.

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

/* System includes */
#include <stdlib.h>
#include <string.h>

/* Starlink includes */
#include "sae_par.h"
#include "mers.h"
#include "ndf.h"
#include "prm_par.h"

/* SMURF routines */
#include "smf.h"
#include "smf_typ.h"
#include "smf_err.h"

#define FUNC_NAME "smf_deepcopy_smfData"

smfData *
smf_deepcopy_smfData( const smfData *old, const int rawconvert,
                      const int flags, int * status ) {

  int create[3];              /* Flag for copying each component */
  smfDA *da = NULL;           /* New smfDA */
  dim_t dims[NDF__MXDIM];     /* Dimensions of each axis of data array */
  smf_dtype dtype;            /* Data type */
  smfFile *file = NULL;       /* New smfFile */
  smfHead *hdr = NULL;        /* New smfHead */
  size_t i;                   /* Loop counter */
  int isTordered;             /* Data order */
  size_t j;                   /* Loop counter */
  size_t nbytes;              /* Number of bytes in data type */
  dim_t ncoeff = 0;           /* Number of coefficients */
  size_t ndims;               /* Number of dimensions in data array */
  smfData *new = NULL;        /* New smfData */
  size_t npts;                /* Number of data points */
  double *outdata;            /* Pointer to output DATA */
  void *pntr[3];              /* Data, variance and quality arrays */
  double *poly = NULL;        /* Polynomial coefficients */
  int *tstream;               /* Pointer to raw time series data */
  int virtual;                /* Is it a virtual smfData? */
  AstKeyMap *history = NULL;  /* Pointer to history */

  if (*status != SAI__OK) return NULL;

  /* Copy elements */
  ndims = old->ndims;
  ncoeff = old->ncoeff;
  virtual = old->virtual;
  dtype = old->dtype;
  isTordered = old->isTordered;

  npts = 1;
  for (i=0; i<ndims; i++) {
    dims[i] = (old->dims)[i];
    npts *= dims[i];
  }

  /* Copy all history components provided one exists */
  if ( old->history != NULL ) {
    history = astCopy( old->history );
  } else {
    msgOutif(MSG__DEBUG," ", "No history to copy. Continuing, but this may cause problems later", status);
  }

  /* Set elements of create to reflect SMF__NOCREATE flags */

  if( flags & SMF__NOCREATE_DATA ) {
    create[0] = 0;
  } else {
    create[0] = 1;
  }

  if( flags & SMF__NOCREATE_VARIANCE ) {
    create[1] = 0;
  } else {
    create[1] = 1;
  }

  if( flags & SMF__NOCREATE_QUALITY ) {
    create[2] = 0;
  } else {
    create[2] = 1;
  }

  /* DATA and VARIANCE */
  for (i=0; i<2; i++) {
    if ( ((old->pntr)[i] != NULL) && create[i] ) {
      /* Check if we are converting from integer to double */
      if (rawconvert && (old->dtype == SMF__INTEGER) ) {
        nbytes = sizeof(double);
        dtype = SMF__DOUBLE;
        pntr[i] = smf_malloc( npts, nbytes, 0, status);
        outdata = pntr[i];
        tstream = (old->pntr)[i];
        /* Input data are ints: must re-cast as double */
        for (j=0; j<npts; j++) {
          if (tstream[j] != VAL__BADI) {
            outdata[j] = (double)tstream[j];
          } else {
            outdata[j] = VAL__BADD;
          }
        }
        pntr[i] = outdata;
      } else {
        nbytes = smf_dtype_size(old, status);
        pntr[i] = smf_malloc( npts, nbytes, 0, status);
        if ( pntr[i] == NULL ) {
          if ( i == 0) {
            msgSetc("C", "Data");
          } else if ( i == 1 ) {
            msgSetc("C", "Variance");
          } else {
            if ( *status == SAI__OK ) {
              *status = SAI__ERROR;
              errRep(FUNC_NAME, "Loop counter out of range. Possible programming error?", status);
              return NULL;
            }
          }
          *status = SAI__ERROR;
          errRep(FUNC_NAME, "Unable to allocate memory for ^C component", status);
          return NULL;
        }
        memcpy( pntr[i], (old->pntr)[i], nbytes*npts);
      }
    } else {
      pntr[i] = NULL;
    }
  }
  /* Quality */
  if ( ((old->pntr)[2] != NULL) && create[2] ) {
    pntr[2] = smf_malloc( npts, 1, 0, status);
    if ( pntr[2] == NULL ) {
      *status = SAI__ERROR;
      errRep(FUNC_NAME, "Unable to allocate memory for Quality component", status);
      return NULL;
    }
    memcpy( pntr[2], (old->pntr)[2], npts );
  } else {
    pntr[2] = NULL;
  }

  /* Redefine npts for polynomial coefficients */
  if ( ncoeff != 0 ) {
    if( isTordered ) {
      npts = dims[0]*dims[1]*ncoeff;
    } else {
      npts = dims[1]*dims[2]*ncoeff;
    }
    poly = smf_malloc( npts, sizeof(double), 0, status);
    if ( *status != SAI__OK ) {
      errRep(FUNC_NAME,
             "Unable to allocate memory for polynomial coefficients", status);
      return NULL;
    }
    memcpy( poly, old->poly, npts*sizeof(double));
  } else {
    msgOutif(MSG__DEBUG," ", "Skipping copy of SCANFIT coefficients, ncoeff = 0", status);
  }

  /* Copy smfHead if desired */
  if (! (flags & SMF__NOCREATE_HEAD) )
    hdr = smf_deepcopy_smfHead( old->hdr, status );
  /* Copy smfFile if desired */
  if (! (flags & SMF__NOCREATE_FILE) ) {
    file = smf_deepcopy_smfFile( old->file, status );
    /* annul the cloned NDF identifier if it is there */
    if (file->ndfid != NDF__NOID) ndfAnnul( &(file->ndfid), status );
  }

  /* Copy smfDA if desired */
  if (! (flags & SMF__NOCREATE_DA) )
    da = smf_deepcopy_smfDA( old, status );

  /* Construct the new smfData */
  new = smf_construct_smfData( new, file, hdr, da, dtype, pntr, isTordered,
                               dims, old->lbnd, ndims, virtual, ncoeff, poly, history,
                               status);

  return new;
}
