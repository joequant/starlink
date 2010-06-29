/*
*+
*  Name:
*     smf_find_gains

*  Purpose:
*     Using a given template, find the gain and offset for each bolometer
*     in a smfData and reject aberrant bolometers.

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     C function

*  Invocation:
*     int smf_find_gains( smfWorkForce *wf, smfData *data,
*                         double *template, AstKeyMap *keymap,
*                         smf_qual_t goodqual, smf_qual_t badqual,
*                         smf_qual_t *quality, smfData *gain,
*                         int *nrej, int *status )

*  Arguments:
*     wf = smfWorkForce * (Given)
*        Pointer to a pool of worker threads (can be NULL)
*     data = smfData * (Given)
*        The input data. Each bolometer time series will be compared to
*        the template.
*     template = double * (Given)
*        The 1-dimensional template. The length of this array should
*        equal the number of time slices in "data".
*     keymap = AstKeyMap * (Given)
*        An AST KeyMap holding values for the parameters required by this
*        function:
*
*        - corr_abstol (double): A bolometer is rejected if its correlation
*        coefficient is lower corr_abstol.
*
*        - corr_tol (double): A bolometer is rejected if its correlation
*        coefficient is lower than the mean correlation coefficient by more
*        than corr_tol times the standard deviation of the correlation
*        coefficients. Note, all correlation coefficients lower than
*        "corr_abstol" are removed before finding the mean and standard
*        deviation of the correlation coefficients.
*
*        - fit_box (int): The number of adjacent time slices that are
*        to be used when fitting the data to the template. fit_box should
*        be no smaller than gain_box.
*
*        - gain_abstol (double): A bolometer is rejected if its log(gain)
*        value more than gain_abstol from the mean log(gain) value. Note, all
*        negative gains are removed before finding the mean of the log(gain)
*        values.
*
*        - gain_box (int): The number of adjacent time slices that are
*        to be described using a single gain and offset.
*
*        - gain_fgood (double): The minimum number of good blocks required
*        for a usable bolometer, expressed as fraction in the range zero
*        to one.
*
*        - gain_is_one (int): If non-zero, the bolometer gains determined
*        by the least squares linear fit are ignored, and a value of unity
*        is used for all gains. The offsets and correlation coefficients
*        are still determined.
*
*        - gain_rat (double): The square root of the ratio of the maximum
*        acceptable block gain within a bolometer to the minimum acceptable
*        block gain. It is used when checking for consistency amongst the
*        blocks of a single bolometer.
*
*        - gain_tol (double): A bolometer is rejected if its log(gain)
*        value more than gain_tol standard deviations from the mean
*        log(gain) value. Note, all negative gains are removed before
*        finding the mean and standard deviation of the log(gain) values.
*
*        - offset_is_zero (int): If non-zero, the bolometer offsets are
*        forced to zero within the least squares linear fit. The gains and
*        correlation coefficients are still determined.
*
*     goodqual = smf_qual_t (Given)
*        The quality value to be used when checking for values to be
*        included in the comparison.
*     badqual = smf_qual_t (Given)
*        The quality value to be assigned to samples that are found to be
*        aberrant. In addition, if an entire bolometer is set bad, its
*        first sample will be flagged with SMF__Q_BADB.
*     quality = smf_qual_t * (Given and Returned)
*        If non-NULL, use this array instead of the QUALITY associated with
*        "data". Samples rejected as aberrant will be flagged using "badqual".
*     gain = smfData * (Given & Returned)
*        This holds the gains, offsets and correlation coefficients that
*        scale the template values into the bolometer values for each block:
*
*        data ~= scale*template + offset
*
*        In this smfData, the time axis is subverted to enumerate the
*        coefficients, and should have a length of 3*gain_box. For each
*        bolometer, the first "gain_box" values on the time axis contain
*        the gain values for the bolometer, the next block of "gain_box"
*        values are the offsets for the bolometer, and the final block of
*        "gain_box" values are the correlation coefficients.
*
*        The array should be filled with zeros before calling this function
*        for the first time. New gains, offsets, and correlation coefficients
*        for each bolometer in each block are found and returned in this
*        smfData. The exception tp this is that the values for any block that
*        has already converged (i.e. any block from which no bolometers were
*        rejected on the previous invocation of this function -see argument
*        "nrej") are left unchanged. Also, any values that are VAL__BADD
*        on entry (i.e. previously rejected bolometer blocks) are left
*        unchanged
*
*        On exit, a gain of VAL__BADD is assigned to any blocks that could
*        not be fit to the template, or that were rejected.
*     nrej = int * (Given and Returned)
*        Pointer to an array with one element for each bolometer block.
*        That is, the size of this array should equal the number of
*        blocks per bolometer, (i.e. one third of the size of the "time"
*        axis in "gai"). The array should be filled with arbitrary non
*        zero values before calling this function for the first time. On
*        exit, each element holds the number of bolometers rejected from
*        the corresponding block during the current invocation of this
*        function (i.e. bolometers rejected on previous invocations are
*        not included in the returned counts). Thus if a value of zero is
*        returned for a particular block, then it means that no further
*        bolometers have been rejected from that block. The supplied
*        values are used to determine whether it is necessary to
*        re-calculate the gains and offsets for the bolometers in each
*        block - if a block has a supplied value of zero in this array,
*        and the two neighbouring blocks are also supplied as zero, then
*        the central block of these three has converged, and so the
*        bolometer gains and offsets in the central block (supplied
*        within "gai") are left unchanged on exit. Note, the supplied
*        values are treated as boolean zero/non-zero values - the specific
*        non-zero value supplied is insignificant.
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Returned value:
*     The total number of bolometer blocks rejected by the current
*     invocation of this function (i.e. the sum of the values returned in
*     the "nrej" array).

*  Description:
*     Each bolometer time series in the supplied smfData is split into
*     blocks, and each block that corresponds to a non-zero value in "nrej"
*     (or a non-zero neighbouring value in "nrej") is compared against the
*     corresponding samples in the supplied template. The number of
*     blocks into which each bolometer time series is to be split is given
*     by config parameter GAIN_BOX (read from the supplied KeyMap)
*
*     Each such comparison performs a least square linear fit relating the
*     template values to the bolometer values, and gain, offset and
*     correlation coefficient are returned in the "gai" smfData. Any blocks
*     that have unusal gains, or that are a poor fit to the template, are
*     flagged by setting the quality bits indicated by "badqual" in the
*     returned quality array. In addition, such bad bolometer blocks are
*     assigned a gain value of VAL__BADD in "gai" upon exit.

*  Authors:
*     David S Berry (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     13-JUN-2010 (DSB):
*        Original version.
*     {enter_further_changes_here}

*  Copyright:
*     Copyright (C) 2010 Science & Technology Facilities Council.
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

/* Starlink includes */
#include "ast.h"
#include "mers.h"
#include "sae_par.h"
#include "prm_par.h"

/* SMURF includes */
#include "libsmf/smf.h"
#include "libsmf/smf_typ.h"
#include "libsmf/smf_err.h"

/* Local constants: */
#define NREASON 11
#define NOFIT VAL__MAXD/2

/* Local data types: */
typedef struct smfFindGainsJobData {
   dim_t b1;
   dim_t b2;
   size_t bstride;
   int *converged;
   double *dat;
   dim_t fit_box;
   double *gai;
   dim_t gain_box;
   size_t gbstride;
   size_t gcstride;
   smf_qual_t goodqual;
   dim_t nblock;
   dim_t nbolo;
   int nogains;
   int nooffs;
   dim_t ntslice;
   smf_qual_t *qua;
   double *template;
   size_t tstride;
} smfFindGainsJobData;


/* Local function prototypes: */
static void smf1_find_gains_job( void *job_data, int *status );


/* Main entry */
int smf_find_gains( smfWorkForce *wf, smfData *data, double *template,
                    AstKeyMap *keymap, smf_qual_t goodqual,
                    smf_qual_t badqual, smf_qual_t *quality,
                    smfData *gain, int *nrej, int *status ){

/* Local Variables: */
   dim_t block_size;
   dim_t corr_offset;
   dim_t fit_box;
   dim_t gain_box;
   dim_t glim;
   dim_t iblock;
   dim_t ibolo;
   dim_t itime;
   dim_t nblock;
   dim_t nbolo;
   dim_t ntime;
   dim_t ntslice;
   dim_t off_offset;
   double *corr;
   double *dat;
   double *gai;
   double *glog;
   double c;
   double cmean;
   double corr_abstol;
   double corr_tol;
   double csig;
   double csum;
   double g;
   double gain_abstol;
   double gain_fgood;
   double gain_rat;
   double gain_tol;
   double gmax;
   double gmean;
   double gmin;
   double gsig;
   int *converged;
   int badbol;
   int conv;
   int ib;
   int ib_hi;
   int ib_lo;
   int ireason;
   int ival;
   int iworker;
   int nbad;
   int nboff;
   int nconverged;
   int nogains;
   int nooffs;
   int nworker;
   int reason[ NREASON ];
   int step;
   int totgood;
   size_t bstride;
   size_t cgood;
   size_t gbstride;
   size_t gcstride;
   size_t ggood;
   size_t ibase;
   size_t igbase;
   size_t tstride;
   smfFindGainsJobData *job_data;
   smfFindGainsJobData *pdata;
   smf_qual_t *qua;

   const char *reason_text[ NREASON ] = {
                                   "fit failed",
                                   "gain is negative",
                                   "corr_abstol test failed",
                                   "corr_tol test failed",
                                   "too few good bolometers remain",
                                   "gain_tol test failed",
                                   "gain_abstol test failed",
                                   "unknown cause",
                                   "gain_rat test failed - gain too high",
                                   "gain_rat test failed - gain too low",
                                   "gain_fgood test failed" };

/* Initialise */
   nbad = 0;
   pdata = NULL;

/* Check the inherited status. */
   if( *status != SAI__OK ) return nbad;

/* Check we have double precision data. */
   smf_dtype_check_fatal( data, NULL, SMF__DOUBLE, status );
   smf_dtype_check_fatal( gain, NULL, SMF__DOUBLE, status );

/* Get pointers to data, quality and gain arrays. */
   dat = data->pntr[ 0 ];
   if( quality ) {
      qua = quality;
   } else {
      qua = data->pntr[ 2 ];
   }
   gai = gain->pntr[ 0 ];

/* Report an error if any are missing. */
   if( !qua ) {
      *status = SAI__ERROR;
      errRep( " ", "smf_find_gains: No valid QUALITY array was provided",
              status );

   } else if( !dat ) {
      *status = SAI__ERROR;
      errRep( " ", "smf_find_gains: 'smfData *data' does not contain a DATA "
              "component", status );

   } else if( !gai ) {
      *status = SAI__ERROR;
      errRep( " ", "smf_find_gains: 'smfData *gain' does not contain a DATA "
              "component", status );
   }

/* Get the data array dimensions and strides. */
   smf_get_dims( data,  NULL, NULL, &nbolo, &ntslice, NULL, &bstride,
                 &tstride, status );

/* Get the gain array strides. Also find the number of blocks per
   bolometer. The length of the "time" axis is 3 times the number of
   blocks. */
   smf_get_dims( gain,  NULL, NULL, NULL, &nblock, NULL, &gbstride, &gcstride,
                 status );
   nblock /= 3;

/* Get the required configuration parameters. */
   astMapGet0D( keymap, "CORR_TOL", &corr_tol );
   astMapGet0D( keymap, "CORR_ABSTOL", &corr_abstol );
   astMapGet0D( keymap, "GAIN_TOL", &gain_tol );
   astMapGet0D( keymap, "GAIN_FGOOD", &gain_fgood );
   astMapGet0D( keymap, "GAIN_RAT", &gain_rat );
   astMapGet0D( keymap, "GAIN_ABSTOL", &gain_abstol );
   astMapGet0I( keymap, "GAIN_IS_ONE", &nogains );
   astMapGet0I( keymap, "OFFSET_IS_ZERO", &nooffs );
   astMapGet0I( keymap, "GAIN_BOX", &ival );
   gain_box = ival;
   if( *status == SAI__OK && gain_box == 0 ) {
     *status = SAI__ERROR;
     errRep( "", "GAIN_BOX must be greater than 0", status );
   }

/* FIT_BOX defaults to GAIN_BOX (it should be null in the defaults file). */
   if( astMapGet0I( keymap, "FIT_BOX", &ival ) ) {
      fit_box = ival;
      if( fit_box < gain_box && *status == SAI__OK ) {
         *status = SAI__ERROR;
        msgSeti( "F", (int) fit_box );
        msgSeti( "G", (int) gain_box );
        errRep( "", "FIT_BOX (^F) must not be smaller than GAIN_BOX "
                "(^G)", status );
      }
   } else {
      fit_box = gain_box;
   }

/* Find the minimum number of good blocks required for a usable bolometer. */
   glim = ceil( nblock*gain_fgood );
   if( glim < 1 ) glim = 1;

/* Allocate work space to hold the convergence flag for each block. */
   converged = astMalloc( nblock*sizeof( *converged ) );

/* Allocate work space to hold gain and correlation coefficients for each
   bolometer within a single block. */
   glog = astMalloc( nbolo*sizeof( *glog ) );
   corr = astMalloc( nbolo*sizeof( *corr ) );

/* Create structures used to pass information to the worker threads. */
   nworker = wf ? wf->nworker : 1;
   job_data = astMalloc( nworker*sizeof( *job_data ) );

/* Check pointers can be used safely. */
   if( *status == SAI__OK ) {

/* Get the number of blocks to check on either side of a central block
   when checking the central block for convergence. The central block has
   converged if:

   1) no bolometers were rejected from the central block on the previous
      invocation of this function

   2) no bolometers were rejected from either of the neighbouring blocks
      on the previous invocation of this function (this is because linear
      interpolation between neighbouring blocks is used to determine the
      gain and offset for any particular time slice).

   3) no bolometers were rejected from any block contained within the
      span of the fit box (this is because all blocks within the fit box
      affect the gain and offset of the central block). */
      nboff = ( fit_box + gain_box - 1 )/( 2*gain_box );
      if( nboff < 1 ) nboff = 1;

/* Now calculate and store the convergence flag for each block. */
      for( iblock = 0; iblock < nblock && *status == SAI__OK; iblock++ ){
         ib_lo = iblock - nboff;
         if( ib_lo < 0 ) ib_lo = 0;

         ib_hi = iblock + nboff;
         if( ib_hi >= (int) nblock ) ib_hi = nblock - 1;

         conv = 1;
         for( ib = ib_lo; ib <= ib_hi; ib++ ) {
            if( nrej[ ib ] > 0 ) conv = 0;
         }
         converged[ iblock ] = conv;
      }

/* Determine which bolometers are to be processed by which threads. */
      step = nbolo/nworker;
      if( step < 1 ) step = 1;

      for( iworker = 0; iworker < nworker; iworker++ ) {
         pdata = job_data + iworker;
         pdata->b1 = iworker*step;
         pdata->b2 = pdata->b1 + step - 1;
      }

/* Ensure that the last thread picks up any left-over bolometers */
      pdata->b2 = nbolo - 1;

/* Store all the other info needed by the worker threads, and submit the
   jobs to work out the gain, offset and correlation coefficient for
   each bolo block that has not already converged, and then wait for them
   to complete. */
      for( iworker = 0; iworker < nworker; iworker++ ) {
         pdata = job_data + iworker;

         pdata->bstride = bstride;
         pdata->dat = dat;
         pdata->fit_box = fit_box;
         pdata->gai = gai;
         pdata->gain_box = gain_box;
         pdata->gbstride = gbstride;
         pdata->gcstride = gcstride;
         pdata->goodqual = goodqual;
         pdata->nblock = nblock;
         pdata->nbolo = nbolo;
         pdata->nogains = nogains;
         pdata->nooffs = nooffs;
         pdata->ntslice = ntslice;
         pdata->qua = qua;
         pdata->template = template;
         pdata->tstride = tstride;
         pdata->converged = converged;

         smf_add_job( wf, SMF__REPORT_JOB, pdata, smf1_find_gains_job, NULL,
                      status );
      }
      smf_wait( wf, status );

/* For debugging purposes, we record the reasons why any bolo-blocks are
   rejected. Initialise the number of bolo-blocks rejected for each
   reason. */
      memset( reason, 0, NREASON*sizeof( *reason ) );

/* Zero the returned "nrej" array. */
      memset( nrej, 0, nblock*sizeof( *reason ) );

/* Get the vector offsets within "gai", from a gain value to the
   corresponding offset value and correlation coefficient. */
      off_offset = 1*nblock*gcstride;
      corr_offset = 2*nblock*gcstride;

/* Initialise the number of time slices left to be processed. */
      ntime = ntslice;

/* We now reject bolo-blocks that have unusual gains or correlation
   coefficients compared to the other bolo-blocks in the same block.
   Do each block in turn. */
      for( iblock = 0; iblock < nblock && *status == SAI__OK; iblock++ ) {

/* Determine the number of time slices in the current block. The final
   block picks up any left over times slices. Update the number left to
   be processed. */
         block_size = ( iblock < nblock - 1 ) ? gain_box : ntime;
         ntime -= block_size;

/* Skip blocks that have already converged. */
         if( ! converged[ iblock ] ) {

/* Detection of unusual gain values is based on the logarithm of the gain
   values rather than the gain values themselves. So we need an array
   holding these log values that can be passed to smf_stats1D below. At
   the same time, we filter out gains or correlation coefficients that are
   below the lower absolute limit, so that they are not included in the
   statistics. Loop round all bolometers. */
            igbase = iblock*gcstride;
            for( ibolo = 0; ibolo < nbolo; ibolo++ ) {

/* Assume the bolometer should not be included in the statistics. */
               glog[ ibolo ] = VAL__BADD;
               corr[ ibolo ] = VAL__BADD;

/* Ignore bad bolometers. */
               if( !( qua[ ibolo*bstride ] & SMF__Q_BADB) ) {

/* Get the gain and correlation coefficient for the current bolometer at
   the current block. */
                  g = gai[ igbase ];
                  c = gai[ igbase + corr_offset ];

/* Ignore bolo-blocks rejected by previous invocations of this function. */
                  if( g == VAL__BADD ) {

/* Ignore failed fits, negative gains and very low correlation
   coefficients, recording the reason why the bolometer was rejected. */
                  } else if( g == NOFIT ){
                     reason[ 0 ]++;

                  } else if( g <= 0 ){
                     reason[ 1 ]++;

                  } else if( c <= corr_abstol ){
                     reason[ 2 ]++;

/* Otherwise, store the correlation coefficient and the logarithm of the
   gain. */
                  } else {
                     glog[ ibolo ] = log( g );
                     corr[ ibolo ] = c;
                  }
               }

/* Get the vector index of the gain value for the next bolometer. */
               igbase += gbstride;
            }

/* Find the mean and standard deviation of the log(gain) values and the
   correlation coefficients. Maybe doing some simga-clipping within
   smf_stats1D would produce better estimates of mean and sigma, and so
   reduce the number of iterative calls to this function needed to achieve
   convergence? */
            smf_stats1D( glog, 1, nbolo, NULL, 0, 0, &gmean, &gsig, &ggood,
                         status );
            smf_stats1D( corr, 1, nbolo, NULL, 0, 0, &cmean, &csig, &cgood,
                         status );

/* If there are insufficient usable bolometers left to find the statistics,
   annull the error and set all gain values bad. */
            if( *status == SMF__INSMP ) {
               errAnnul( status );

               for( ibolo = 0; ibolo < nbolo; ibolo++ ) {
                  glog[ ibolo ] = VAL__BADD;
               }

               reason[ 4 ] += ggood;
            }

/* We now go on to identify bolometers that have unusual gains or
   correlation coefficients for the current block (compared to the other
   bolometers in the current block), and reject them. Loop round every
   bolometer. */
            igbase = iblock*gcstride;
            ibase = iblock*gain_box*tstride;

            for( ibolo = 0; ibolo < nbolo && *status == SAI__OK; ibolo++ ) {

/* If the bolometer has no gain, it cannot be used. */
               if( glog[ ibolo ] == VAL__BADD ) {
                  badbol = 1;

/* If the bolometer has not yet been rejected... */
               } else {
                  badbol = 0;

/* Reject blocks that have an unusually low correlation coefficient
   compared to the spread of correlation coefficients in other bolometers.
   Note, unusually high correlation coefficients are accepted as OK! */
                  if( cmean - corr[ ibolo ] > corr_tol*csig ) {
                     badbol = 1;
                     reason[ 3 ]++;

/* Reject blocks that have an unusually low or high gain compared to
   the spread of gain values in other bolometers. */
                  } else if( fabs( gmean - glog[ ibolo ] ) > gain_tol*gsig ) {
                     badbol = 1;
                     reason[ 5 ]++;

/* Reject blocks that have an unusually low or high gain compared to
   the mean gain in other bolometers. */
                  } else if( fabs( gmean - glog[ ibolo ] ) > gain_abstol ) {
                     badbol = 1;
                     reason[ 6 ]++;
                  }
               }

/* If the current bolometer cannot be used, but it was not flagged as
   rejected on entry, reject it now. */
               if( badbol && gai[ igbase ] != VAL__BADD ) {

/* Set the requested bit in the quality array for all samples for the
   current bolometer in the current block. */
                  for( itime = 0; itime < block_size; itime++ ) {
                     qua[ ibase + itime*tstride ] |= badqual;
                  }

/* Return a bad gain value */
                  gai[ igbase ] = VAL__BADD;

/* Increment the number of bolometers rejected from this block. */
                  nrej[ iblock ]++;

/* Increment the total number of new rejected bolo-blocks. */
                  nbad++;
               }

/* Prepare for the next bolometer. */
               igbase += gbstride;
               ibase += bstride;
            }
         }
      }

/* Initialise the number of remaining good bolo-blocks. */
      totgood = 0;

/* We now consider the consistency of the 'nblock' gain values for each
   individual bolometer. We reject blocks for which the gain value is very
   different to the other gain values for the same bolometer. We also
   reject the entire boloemeter if it has too few good blocks. Loop round
   all remaining bolometers. */
      for( ibolo = 0; ibolo < nbolo && *status == SAI__OK; ibolo++ ) {

/* Skip entirely bad bolometers. */
         ibase = ibolo*bstride;
         if( !( qua[ ibase ] & SMF__Q_BADB) ) {

/* Get the index within "gai" of the gain value for the first block for
   the current bolometer. */
            igbase = ibolo*gbstride;

/* For the current bolometer, find the count of blocks with good gain values,
   and find the mean gain value for this bolometer, use the correlation
   coefficients as weights. */
            gmean = 0.0;
            ggood = 0;
            csum = 0.0;

            for( iblock = 0; iblock < nblock; iblock++ ) {
               g = gai[ igbase ];
               if( g != VAL__BADD ) {
                  c = gai[ igbase + corr_offset ];
                  gmean += g*c;
                  csum += c;
                  ggood++;
               }
               igbase += gcstride;
            }

/* Check there were sufficient good blocks. */
            if( ggood >= glim && csum > 0.0 ) {
               gmean /= csum;

/* Get the maximum and minimum acceptable gain for this bolometer. */
               gmax = gmean*gain_rat;
               gmin = gmean/gain_rat;

/* Loop round all blocks, rejecting any that have gains outside the above
   limits. */
               ntime = ntslice;
               igbase = ibolo*gbstride;
               for( iblock = 0; iblock < nblock; iblock++ ) {
                  block_size = ( iblock < nblock - 1 ) ? gain_box : ntime;
                  ntime -= block_size;

                  g = gai[ igbase ];
                  if( g != VAL__BADD ) {

                     if( g > gmax || g < gmin ) {
                        for( itime = 0; itime < block_size; itime++ ) {
                           qua[ ibase + itime*tstride ] |= badqual;
                        }

                        gai[ igbase ] = VAL__BADD;
                        nrej[ iblock ]++;
                        nbad++;

                        if( g > gmax ) {
                           reason[ 8 ]++;
                        } else {
                           reason[ 9 ]++;
                        }

                     } else {
                        totgood++;
                     }
                  }

/* Prepare for the next block. */
                  ibase += block_size*tstride;
                  igbase += gcstride;
               }

/* If there were insufficient good blocks for the current bolometer,
   reject all blocks in the current bolometer. */
            } else {

               for( itime = 0; itime < ntslice; itime++ ) {
                  qua[ ibase ] |= ( SMF__Q_BADB | badqual );
                  ibase += tstride;
               }

               igbase = ibolo*gbstride;
               for( iblock = 0; iblock < nblock; iblock++ ) {
                  if( gai[ igbase ] != VAL__BADD ) {
                     nrej[ iblock ]++;
                     gai[ igbase ] = VAL__BADD;
                     reason[ 10 ]++;
                     nbad++;
                  }
                  igbase += gcstride;
               }
            }
         }
      }

/* Calculate and report debugging info only if needed. */
      if( msgIflev( NULL, status ) >= MSG__VERB ) {

/* Find the number of blocks that have now converged. */
         nconverged = 0;
         for( iblock = 0; iblock < nblock && *status == SAI__OK; iblock++ ){
            ib_lo = iblock - nboff;
            if( ib_lo < 0 ) ib_lo = 0;

            ib_hi = iblock + nboff;
            if( ib_hi >= (int) nblock ) ib_hi = nblock - 1;

            conv = 1;
            for( ib = ib_lo; ib <= ib_hi; ib++ ) {
               if( nrej[ ib ] > 0 ) conv = 0;
            }
            if( conv ) nconverged++;
         }

/* Display it. */
         msgSeti( "N", nconverged );
         msgSeti( "M", nblock );
         msgOutif( MSG__VERB, "",
                   "    ^N out of ^M time-slice blocks have now converged", status );

         msgSeti( "NEW", nbad );
         msgOutif( MSG__VERB, "",
                   "    flagged ^NEW new bad bolo time-slice blocks", status );

         for( ireason = 0; ireason < NREASON; ireason++ ) {
            if( reason[ ireason ] > 0 ) {
               msgSeti( "N", reason[ ireason ] );
               msgSetc( "T", reason_text[ ireason ] );
               msgOutif( MSG__DEBUG, "",
                         "       (^N were flagged because ^T)", status );
            }
         }

         msgSeti( "NG", totgood );
         msgSeti( "T", nblock*nbolo );
         msgOutif( MSG__VERB, "",
                   "    Out of ^T, ^NG bolo time-slice blocks are still good",
                   status );
      }
   }

/* Free resources. */
   converged = astFree( converged );
   glog = astFree( glog );
   corr = astFree( corr );
   job_data = astFree( job_data );

/* Return the number of rejected bolo blocks. */
   return nbad;
}



static void smf1_find_gains_job( void *job_data, int *status ) {
/*
*  Name:
*     smf1_find_gains_job

*  Purpose:
*     Find the gain and offset for each bolo block.

*  Invocation:
*     void smf1_find_gains_job( void *job_data, int *status )

*  Arguments:
*     job_data = void * (Given)
*        Pointer to the data needed by the job. Should be a pointer to a
*        smfFindGainsJobData structure.
*     status = int * (Given and Returned)
*        Pointer to global status.

*  Description:
*     This routine performs a least squares linear fit between the
*     data values in each bolo block, and the supplied template. It
*     runs within a thread instigated by smf_find_gains.

*/

/* Local Variables: */
   dim_t b1;
   dim_t b2;
   dim_t block_cstride;
   dim_t block_size;
   dim_t block_tstride;
   dim_t fit_box;
   dim_t gain_box;
   dim_t iblock;
   dim_t ibolo;
   dim_t nblock;
   dim_t nbolo;
   dim_t ntime;
   dim_t ntslice;
   double *dat;
   double *gai;
   double *m;
   double *m_box;
   double *template;
   int *converged;
   int delta_box;
   int fit_end;
   int fit_off;
   int fit_size;
   int fit_start;
   int nogains;
   int nooffs;
   size_t box_end;
   size_t box_start;
   size_t bstride;
   size_t gbstride;
   size_t gcstride;
   size_t ibase;
   size_t ibase_box;
   size_t igbase;
   size_t tstride;
   smfFindGainsJobData *pdata;
   smf_qual_t *qua;
   smf_qual_t goodqual;
   struct timeval tv1;
   struct timeval tv2;

/* Check inherited status */
   if( *status != SAI__OK ) return;

/* Get a pointer to the job data, and then extract its contents into a
   set of local variables. */
   pdata = (smfFindGainsJobData *) job_data;

   b1 = pdata->b1;
   b2 = pdata->b2;
   bstride = pdata->bstride;
   converged = pdata->converged;
   dat = pdata->dat;
   fit_box = pdata->fit_box;
   gai = pdata->gai;
   gain_box = pdata->gain_box;
   gbstride = pdata->gbstride;
   gcstride = pdata->gcstride;
   goodqual = pdata->goodqual;
   nblock = pdata->nblock;
   nbolo = pdata->nbolo;
   nogains = pdata->nogains;
   nooffs = pdata->nooffs;
   ntslice = pdata->ntslice;
   qua = pdata->qua;
   template = pdata->template;
   tstride = pdata->tstride;

/* Check we have something to do. */
   if( b1 < nbolo ) {

/* Debugging message indicating thread started work */
      msgOutiff( MSG__DEBUG, "", "smfFindGains: thread starting on bolos %zu -- %zu",
                 status, b1, b2 );
      smf_timerinit( &tv1, &tv2, status);

/* Get the number of samples within the fit_box that overhang at each end
   of the gain_box. */
      delta_box = ( fit_box - gain_box )/2;

/* Get the strides between coefficients for adjacent blocks within "gai". */
      block_cstride = gcstride*nblock;

/* Get the strides between first time slices for adjacent blocks within
   "dat". */
      block_tstride = gain_box*tstride;

/* Loop round all the bolometers to be processed by this thread. */
      for( ibolo = b1; ibolo <= b2 && *status == SAI__OK; ibolo++ ) {

/* Get the vector offset within "dat" at the start of the first block in
   the current bolometer. */
         ibase = ibolo*bstride;

/* Get the vector offset within "gai" for the gain value for the first block
   of the current bolometer. */
         igbase = ibolo*gbstride;

/* Check the bolometer is not entirely bad. */
         if( !( qua[ ibase ] & SMF__Q_BADB ) ) {

/* Initialise the number of time slices still to be processed, and then
   loop round each block of time slices, fitting the template to the
   bolometer signal and finding a gain, offset and correlation for each
   block. The final block may contain more than "gain_box" slices. */
            ntime = ntslice;
            m = template;
            for( iblock = 0; iblock < nblock && *status == SAI__OK; iblock++ ){

/* If the block has converged, then we do not need to re-calculate the
   gains and offsets since they will not have changed. Also do not
   re-calculate the gain and offset for any bolo-block that has
   previously been rejected. */
               if( ! converged[ iblock ] && gai[ igbase ] != VAL__BADD ) {

/* Calculate the number of time slices in this block. The last block (index
   iblock-1 ) contains all remaining time slices, which may not be gain_box
   in number. */
                  block_size = ( iblock < nblock - 1 ) ? gain_box : ntime;

/* Find the bounds of the data to include in the fit for this block (the
   fit extends by delta_box samples beyond the block at start and end of
   the block). */
                  box_start = ntslice - ntime;
                  box_end = box_start + block_size - 1;

                  fit_start = box_start - delta_box;
                  fit_end = box_end + delta_box;

                  if( fit_start < 0 ) fit_start = 0;
                  if( fit_end >= (int) ntslice ) fit_end = ntslice - 1;

                  fit_off = box_start - fit_start;
                  fit_size = fit_end - fit_start + 1;

/* Get the index for the first data value to be included in the fit. Also get
   a pointer to the first template value. */
                  ibase_box = ibase - fit_off*tstride;
                  m_box = m - fit_off;

/* Do the fit. */
                  smf_templateFit1D( dat + ibase_box, qua + ibase_box,
                                     goodqual, 0, fit_size, tstride, m_box, 0,
                                     nooffs, gai + igbase,
                                     gai + block_cstride + igbase,
                                     gai + 2*block_cstride + igbase, status );

/* If no fit could be performed, annul the error and store a gain value
   of NOFIT. */
                  if( *status == SMF__DIVBZ || *status == SMF__INSMP ) {
                     errAnnul( status );
                     gai[ igbase ] = NOFIT;

/* If we are ignoring gains, force gains to 1.0. Retain the correlation
   coeffs in order to flag bad blocks. The ignoring of offsets is handled
   by smf_templateFit1D above. */
                  } else if( nogains ) {
                     gai[ igbase ] = 1.0;
                  }
               }

/* Move on to the next block. */
               ntime -= gain_box;
               ibase += block_tstride;
               m += gain_box;
               igbase += gcstride;
            }

/* Store bad gain values for every block if the whole bolometer is bad.
   So later code sees this as being equivalent to "rejected on entry". */
         } else {
            for( iblock = 0; iblock < nblock; iblock++ ){
               gai[ igbase ] = VAL__BADD;
               igbase += gcstride;
            }
         }
      }

/* Report the time taken in this thread. */
      msgOutiff( MSG__DEBUG, "", "smfFindGains: thread finishing bolos %zu -- %zu (%.3f sec)",
                 status, b1, b2, smf_timerupdate( &tv1, &tv2, status ) );
   }
}

