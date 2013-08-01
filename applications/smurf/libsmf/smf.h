/*  -*- C -*-
 *+
 *  Name:
 *     smf.h

 *  Purpose:
 *     Prototypes for the libsmf library

 *  Language:
 *     Starlink ANSI C

 *  Type of Module:
 *     Header File

 *  Invocation:
 *     #include "smf.h"

 *  Description:
 *     Prototypes used by the libsmf functions.

 *  Authors:
 *     Andy Gibb (UBC)
 *     Tim Jenness (JAC, Hawaii)
 *     Edward Chapin (UBC)
 *     Malcolm J. Currie (Starlink)
 *     Coskun Oba (COBA, UoL)
 *     Remo Tilanus (JAC, Hawaii)
 *     Matt Sherwood (MS, UoL)
 *     {enter_new_authors_here}

 *  History:
 *     2005-09-27 (AGG):
 *        Initial test version
 *     2005-11-04 (AGG):
 *        Add smf_open_file, smf_fits_rdhead and smf_fits_crchan
 *     2005-11-07 (TIMJ):
 *        Alphabetize.
 *        Add smf_tslice_ast, add smf_fits_getI
 *     2005-11-28 (TIMJ):
 *        Add smf_close_file
 *     2005-12-05 (AGG)
 *        Add smf_flatfield and smf_check_flat
 *     2005-12-06 (AGG)
 *        Add smf_flatten
 *     2005-12-09 (AGG)
 *        Add smf_clone_data
 *     2006-01-09 (AGG)
 *        Add smf_tslice and smf_insert_tslice
 *     2006-01-10 (AGG)
 *        Add smf_scale_tau and smf_fits_getF
 *     2006-01-24 (TIMJ):
 *        Add smf_fits_getS
 *     2006-01-24 (AGG):
 *        Change floats to doubles in smf_correction_extintion, smf_scale_tau
 *     2006-01-25 (AGG):
 *        Add smf_dtype_check_fatal
 *     2006-01-25 (TIMJ):
 *        Add smf_malloc, smf_free.
 *        Remove smf_fits_rdhead
 *     2006-01-25 (TIMJ):
 *        Add smf_create_*
 *        Add smf_construct_*
 *        Add smf_dtype_tostring
 *     2006-01-27 (TIMJ):
 *        Change API for smf_construct_smfFile, smf_construct_smfDA
 *        and smf_construct_smfHead.
 *     2006-02-02 (EC):
 *        Add smf_mapbounds
 *        Add smf_rebinmap
 *     2006-02-03 (AGG):
 *        Change API for smf_scale_tau, smf_correct_extinction
 *     2006-02-17 (AGG):
 *        Add smf_subtract_poly
 *     2006-02-24 (AGG):
 *        Add smf_subtract_plane
 *     2006-03-23 (AGG):
 *        Update API for smf_rebinmap, smf_construct_smfData,
 *        smf_construct_smfHead
 *        Add smf_mapbounds approx, smf_deepcopy_smfHead & smf_deepcopy_smfData
 *     2006-03-28 (AGG):
 *        Update API for smf_deepcopy_smfData, add smf_deepcopy_smfDA
 *     2006-03-30 (AGG):
 *        Add smf_deepcopy_smfFile
 *     2006-04-05 (AGG):
 *        - Change API for smf_deepcopy_smfDA to accept a smfData
 *          rather than smfDA
 *        - Add smf_check_smfData, smf_check_smfDA, smf_check_smfFile and
 *          smf_check_smfHead
 *     2006-04-21 (AGG):
 *        - Change API for smf_check_smfData, smf_deepcopy_smfData
 *        - Add history to smf_construct_smfData
 *        - Add smf_history_add, smf_history_read
 *     2006-05-01 (EC):
 *        - Add smf_mapcoordinates
 *     2006-05-09 (AGG):
 *        Add smf_get_xloc and smf_get_ndfid
 *     2006-05-09 (EC):
 *        Renamed smf_mapcoord to smf_calc_mapcoord
 *     2006-07-07 (AGG):
 *        Add smf_grp_related, smf_construct_smfGroup,
 *        smf_open_related, smf_close_related and smf_close_smfGroup
 *     2006-07-11 (EC):
 *        Add smf_model_create, smf_model_getname
 *     2006-07-26 (TIMJ):
 *        Replace sc2head with JCMTState.
 *     2006-07-28 (TIMJ):
 *        Add tswcs argument to smf_construct_smfHead
 *     2006-07-31 (TIMJ):
 *        Add smf_inst_get
 *     2006-08-02 (AGG):
 *        Add smf_open_newfile and smf_open_ndf
 *     2006-08-02 (TIMJ):
 *        smf_open_newfile should take dim_t
 *     2006-08-16 (EC):
 *        changed interface for smf_model_getname
 *     2006-09-06 (EC):
 *        added smf_calc_telpos, smf_create_lutwcs and smf_telpos_get
 *     2006-09-15 (AGG):
 *        Added smf_dreamsolve, smf_create_smfDream,
 *        smf_construct_smfDream, smf_close_smfDream, smd_dream_setjig,
 *        smf_store_image
 *     2006-09-15 (AGG):
 *        Add smf_dream_getgrid, smf_dream_calcweights
 *     2006-10-11 (AGG):
 *        Update API for smf_open_newfile
 *     2006-10-13 (JB):
 *        Add smf_bbrebinmap
 *     2006-10-26 (AGG):
 *        Add smf_average_data, smf_calc_stareimage, update API to
 *        smf_store_image
 *     2006-11-2 (DSB):
 *        Add smf_geod.
 *     2006-11-3 (DSB):
 *        Add smf_instap_get.
 *     2006-11-20 (DSB):
 *        Add smf_cubegrid and change smf_cubebounds.
 *     2006-11-30 (DSB):
 *        Add smf_sparsebounds.
 *     2007-01-19 (AGG):
 *        - Change API to smf_mapbounds: now returns the moving flag
 *        - Change API to smf_rebinmap: takes the moving flag as an argument
 *     2007-01-30 (AGG):
 *        Update API to smf_mapbounds_approx to return moving flag
 *     2007-02-06 (AGG):
 *        Add tsys to smf_construct_smfHead
 *     2007-02-12 (EC):
 *        Add smf_model_getptr
 *     2007-02-12 (DSB):
 *        Add "hasoffexp" argument to smf_cubebounds.c
 *     2007-02-23 (AGG):
 *        Add instap to smf_construct_smfHead.c
 *     2007-03-05 (EC):
 *        Changed smf_correct_extinction interface
 *        Add smf_model_gettype
 *        Add smf_calcmodel_ext
 *     2007-03-20 (TIMJ):
 *        Add smf_fits_outhdr
 *     2007-04-14 (DSB):
 *        Add "int *nreject" to smf_rebincube.
 *     2007-04-23 (DSB):
 *        Big changes to smf_rebincube_xxx functions.
 *     2007-04-23 (EC):
 *        Add smf_terr
 *     2007-06-13 (EC):
 *        Add smf_open_file and smf_dtype_sz.c
 *     2007-06-13 (EC):
 *        Add smf_model_NDFexport
 *     2007-06-22 (TIMJ):
 *        Add string arg to smf_fits_add_prov
 *     2007-07-05 (TIMJ):
 *        Add smf_accumulate_prov.
 *     2007-07-10 (EC):
 *        -Add smf_open_related_model
 *        -changed interface for smf_create_smfArray
 *        -changed interface for smf_model_create
 *        -changed interface for smf_calcmodel_*
 *        -changed return value of smf_calcmodelptr
 *     2007-07-12 (EC):
 *        -Added moving to smf_bbrebinmap and smf_calc_mapcoord
 *        -Changed name of smf_rebincube_totmap to smf_rebin_totmap
 *     2007-07-16 (EC):
 *        -Added copysubgroups to smf_construct_smfGroup
 *     2007-08-09 (EC):
 *        -Changed interface for smf_model_create
 *     2007-08-17 (EC):
 *        -Added nofile to smf_model_create interface
 *     2007-09-13 (EC):
 *        -Added smf_dataOrder
 *     2007-09-13 (EC):
 *        -Added smf_fft_filter and smf_concat_smfGroup
 *     2007-10-12 (DSB):
 *        -Added smf_choosepolbins, smf_freepolbins and smf_polext.
 *     2007-10-19 (DSB):
 *        -Added specunion to smf_cubebounds.
 *     2007-10-25 (DSB):
 *        -Added smf_checkdets.
 *     2007-10-31 (EC):
 *        -Added mode to smf_open_mapcoord interface
 *     2007-11-8 (DSB):
 *        -Added smf_sortd and smf_reorder<x>.
 *     2007-11-15 (EC):
 *        -Added projection information to interfaces of
 *         smf_iteratemap and smf_concat_smfGroup
 *        -Added iarray to smf_model_create interface
 *     2007-11-23 (DSB):
 *        Added smf_updateprov.
 *     2007-11-26 (DSB):
 *        Added wcsout to smf_choosetiles argument list.
 *     2007-11-27 (EC):
 *        Added smf_fits_getL, smf_fits_setL
 *     2007-12-054 (DSB):
 *        Added trim to smf_choosetiles argument list.
 *     2007-12-14 (EC):
 *        Added flags to smf_calc_mapcoord interface.
 *     2007-12-18 (DSB):
 *        -Added smf_getrefwcs.
 *        -Added specrefwcs and spacerefwcs to smf_cubebounds.
 *     2007-12-18 (AGG):
 *        New smf_free behaviour: now returns a NULL pointer if successful
 *     2008-1-14 (DSB):
 *        Added argument "border" to smf_choosetiles, and "trim" to
 *        smf_reshapendf.
 *     2008-1-15 (DSB):
 *        Remove argument "trim" from smf_reshapendf.
 *     2008-1-17 (DSB):
 *        Added argument "alignsys" to smf_cubegrid.
 *     2008-1-21 (DSB):
 *        Added argument "polobs" to smf_cubebounds and smf_sparsebounds.
 *     2008-01-22 (EC):
 *        Added hitsmap to smf_simplerebinmap, smf_iteratemap
 *     2008-01-25 (EC):
 *        Added map projection information to smf_model_create interface
 *     2008-2-1 (DSB):
 *        Added smf_resampcube* functions.
 *     2008-2-8 (EC):
 *        Added smf_update_quality
 *     2008-2-12 (DSB):
 *        Changed interface to smf_choosepolbins, smf_rebincube*, and
 *        smf_rebinsparse.
 *     2008-02-12 (AGG):
 *        Update API to smf_rebinmap, deprecate smf_bbrebinmap
 *     2008-02-13 (AGG):
 *        - smf_rebinmap: add parameters for pixel spreading scheme
 *        - smf_get_spread: new routine to get parameters for chosen
 *          pixel-spreading scheme
 *     2008-02-26 (AGG):
 *        Add rel parameter to smf_subtract_poly to get subtraction
 *        relative to first time slice
 *     2008-03-03 (EC):
 *        Added target to smf_update_quality interface
 *     2008-03-04 (EC):
 *        Updated smf_calcmodel* routines to use smfDIMMData
 *     2008-03-10 (AGG):
 *        Add smf_create_qualname
 *     2008-03-12 (EC):
 *        - Updated smf_update_quality interface
 *        - Added smf_correct_steps / smf_simple_stats
 *     2008-03-14 (DSB):
 *        - Added smf_sorti, smf_ext2km and smf_km2ext.
 *     2008-03-25 (EC):
 *        - Added syncbad to smf_update_quality interface
 *     2008-03-27 (DSB):
 *        - Added smf_getobsidss.
 *     2008-03-28 (DSB):
 *        - Added smf_calc_telres.
 *     2008-03-31 (JB):
 *        - Added smf_get_moltrans.
 *     2008-03-31 (EC):
 *        - Added smf_quick_noise
 *     2008-04-02 (EC):
 *        - Added smf_flag_spikes
 *     2008-04-03 (EC):
 *        - Added QUALITY to smf_simplerebinmap
 *        - Added QUALITY to smf_scanfit, smf_fit_poly, smf_subtract_poly,
 *     2008-04-09 (TIMJ):
 *        fix smf_created_qualname.
 *        smf_create_lutwcs and smf_detpos_wcs no longer needs steptime argument
 *     2008-04-14 (EC):
 *        - Updated interface for smf_flag_spikes and smf_boxcar1
 *     2008-04-16 (EC):
 *        - Added optional external QUALITY and VARIANCE to smf_model_NDFexport
 *        - Added chunk to smf_construct_smfGroup
 *        - Added smf_simpleaddmap
 *     2008-04-16 (AGG):
 *        Add genvar to smf_rebinmap
 *     2008-04-17 (EC):
 *        Modified smf_grp_related interface
 *     2008-04-18 (EC):
 *        Modified smf_flag_spikes interface
 *     2008-04-23 (EC):
 *        -Added sampvar to smf_simplerebinmap
 *        -Added hdr to smf_model_NDFexport
 *     2008-04-24 (EC):
 *        -Added smf_check_mapsize (then renamed smf_checkmem_map)
 *        -Added maxmem to smf_iteratemap
 *     2008-04-28 (AGG):
 *        Add meansky parameter to smf_subtract_plane1/2
 *     2008-04-28 (EC):
 *        -Added maxconcatlen to smf_grp_related interface
 *        -Added smf_checkmem_dimm
 *     2008-04-28 (AGG):
 *        Add smf_dump_smfData
 *     2008-04-30 (TIMJ):
 *        Add _clabels routines.
 *     2008-05-01 (TIMJ):
 *        Add check_units
 *     2008-05-14 (EC):
 *        -Added smf_get_projpar
 *        -Modified smf_mapbounds interface to use par
 *        -Moved NINT macro here from smf_cubegrid
 *     2008-05-23 (TIMJ):
 *        Add smf_get_taskname
 *     2008-05-26 (EC):
 *        Added is2d to smf_choosetiles
 *     2008-05-28 (TIMJ):
 *        Change smf_accumulate_prov
 *     2008-06-03 (TIMJ):
 *        Add smf_display_projpars
 *     2008-06-04 (TIMJ):
 *        smf_get_projpar new API.
 *        Add smf_calc_skyframe
 *     2008-06-05 (EC):
 *        Removed is2d from smf_choosetiles
 *     2008-06-06 (EC):
 *        -Renamed smf_fft_filter to smf_filter_execute and changed interface
 *        -Added smf_create_smfFilter, smf_filter_ident, smf_free_smfFilter
 *     2008-06-10 (EC):
 *        -Added smf_filter_r2c
 *     2008-06-11 (EC):
 *        -Added smf_filter_edge and smf_filter_notch
 *        -Renamed smf_model_NDFexport to smf_NDFexport
 *     2008-06-24 (EC):
 *        Added padStart & padEnd to smf_concat_smfGroup
 *     2008-07-03 (EC):
 *        Switched to using dim_t in a number of function interfaces:
 *        smf_calc_stats, smf_construct_smfGroup, smf_model_create,
 *        smf_open_related, smf_simplerebinmap, smf_open_related_model,
 *        smf_simpleaddmap
 *     2008-07-30 (EC):
 *        Add smf_grpCopy
 *     2008-08-27 (AGG):
 *        Add smf_set_moving
 *     2008-08-27 (EC):
 *        -Rename smf_simple_stats, smf_simplerebinmap, smf_simpleaddmap
 *         to     smf_stats1, smf_rebinmap1, smf_addmap1
 *        -Add smf_get_dims
 *     2008-08-28 (EC):
 *        -Renamed to smf.h.source in line with using cgeneric macros
 *     2009-03-12 (EC):
 *        -Added smf_calcmodel_flt
 *     2009-03-27 (TIMJ):
 *        Add smf_create_respfile
 *     2009-04-15 (DSB):
 *        Added smf_check_detpos.
 *     2009-04-15 (EC):
 *        Add smf_get_cleanpar
 *     2009-04-17 (EC):
 *        Add smf_filter_fromkeymap
 *     2009-04-23 (EC):
 *        Add smf_copytime, smf_timerinit, timerupdate
 *     2009-04-28 (EC):
 *        Alphabetize and reformat to fit in 80 columns
 *     2009-08-06 (EC):
 *        Add smf_map_spikes
 *     2009-08-25 (MJC):
 *        Add star/irq.h include as it is no longer in star/kaplibs.h.
 *     2009-09-29 (EC):
 *        Add smf_get_extpar
 *     2009-11-17 (EC):
 *        Add smf_get_goodrange
 *     2010-03-18 (EC):
 *        Add smf_qual_str, smf_qualstats_report
 *     2010-05-26 (TIMJ):
 *        Remove SMURF memory allocation routine prototypes. Use AST
 *        routines natively. Provide debugging fallback using standard
 *        routines.
 *     2010-08-13 (EC):
 *        Add smf_write_itermap
 *     2010-08-20 (EC):
 *        Add smf_write_bolomap, smf_write_shortmap
 *     2010-08-31 (EC):
 *        Add smf_dataOrder_array
 *     2010-09-15 (DSB):
 *        Remove  smf_flag_spikes.
 *     2010-09-17 (COBA):
 *        - Add smf_create_smfFts
 *        - Add smf_construct_smfFts
 *        - Add smf_deepcopy_smfFts
 *        - Update smf_construct_smfData
 *     2010-09-23 (EC):
 *        - Rename smf_flag_stationary to smf_flag_slewspeed
 *        - add smf_filter_complement
 *     2010-09-27 (EC):
 *        Add smf_write_flagmap
 *     2010-10-12 (EC):
 *        Add smf_whiten
 *     2010-10-18 (COBA):
 *        Add smf_check_smfFts
 *     2010-10-20 (EC):
 *        Add smf_downsamp_smfData
 *     2010-10-06 (RPT)
 *        Added smf_dataOrder_ndims.c
 *     2010-10-05 (RPT)
 *        Added smf_fit_profile and smf_lsqfit
 *     2010-10-19 (RPT)
 *        Added smf_gauest.c
 *     2011-02-02 (RPT)
 *        Added smf_math_functions, smf_math_fvalue, smf_math_fpderv,
 *              smf_math_fnpar
 *     2011-02-16 (RPT)
 *        Added smf_dtype_arraycopy
 *     2011-06-08 (EC)
 *        Add smf_raw2current.c
 *     2011-10-06 (EC)
 *        Add smf_fit_pspec
 *     2012-07-19 (RPT)
 *        Extract smf_math_cmplxerrfunc from smf_math_functions as separate
 *              routine
 *     2013-07-31 (MS)
 *        Add smf_fit_poly1d_chisq
 *     {enter_further_changes_here}

 *  Copyright:
 *     Copyright (C) 2005-2007 Particle Physics and Astronomy Research Council.
 *     Copyright (C) 2007-2009 Science and Technology Facilities Council.
 *     Copyright (C) 2005-2010 University of British Columbia.
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
 *     Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *     MA 02110-1301, USA

 *  Bugs:
 *     {note_any_bugs_here}
 *-
 */

#ifndef SMF_DEFINED
#define SMF_DEFINED

#include <sys/time.h>    /* for struct timeval */
#include <stdint.h>      /* for int64_t */

#include "ast.h"
#include "star/thr.h"
#include "jcmt/state.h"  /* for inst_t */
#include "smurf_typ.h"
#include "smurf_par.h"
#include "star/grp.h"
#include "star/ndg.h"
#include "smf_typ.h"
#include "msg_par.h"
#include "star/irq.h"
#include "star/kaplibs.h"
#include "sc2da/sc2ast_typ.h"

/* Handle GCC __attribute__ */
#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING*/
#endif

/* Returns nearest integer to "x" */
#define NINT(x) ( ( (x) > 0 ) ? (int)( (x) + 0.5 ) : (int)( (x) - 0.5 ) )

/* Evaluates a polynomial at coordinate "x" and stores the result in "result".
   coeffs is an array of size order+1 */
#define EVALPOLY( result, x, order, coeffs ) { \
  size_t _j; \
  double _temp; \
  double _xx = x; \
  _temp = coeffs[0]; \
  if (_temp != VAL__BADD) { \
    for ( _j = 1; _j <= order; _j++ ) { \
      _temp += coeffs[_j] * pow( _xx, _j ); \
    } \
  } \
  result = _temp; \
}

/* Macro to convert a pixel index in an FFT to a frequency index: if there
   are N samples in real space, the indices up to N/2+1 in frequency space
   are unique, and then the remaining samples are the complex conjugate of
   the first half (negative frequencies). Since the return value can be
   negative we type cast everything as a signed int. thanks GM */

#define FFT_INDEX_TO_FREQ(index,N) \
  ((((int)index + ((int)N-1)/2) % (int)N) - ((int)N-1)/2)

/* These macros can be enabled to replace the AST malloc routines with
   system default routines. This can sometimes help debuggers and tools
   like valgrind. The AST malloc routine can get in the way as it uses
   local caching. Note that we ignore the "init" parameter for astCalloc
   and always force initialisation in this simplified macro.

   Make sure that no SMURF routines include ast.h after including smf.h
*/

#define MALLOC_DEBUG 0
#if MALLOC_DEBUG

#undef astMalloc
#undef astFree
#undef astCalloc
#undef astRealloc
#undef astGrow
#undef astStore

#define astMalloc(size)  malloc(size)
#define astCalloc(nmemb, size, init) calloc(nmemb, size)
#define astRealloc(ptr, size) realloc(ptr, size)
#define astGrow( ptr, n, size ) realloc(ptr, n*size)

/* The prototype differs for system free(). This will trigger a "defined but not used"
   warning when compiling in any routine that does not use smf_free but that can be
   ignored since this is only for debugging. */
#define astFree(ptr) debug_astFree(ptr)
static inline void * debug_astFree( void * ptr );
static inline void * debug_astFree( void * ptr ) { if (ptr) free(ptr); return NULL; }

/* astStore is realloc+memcpy */
#define astStore( ptr, data, size ) debug_astStore( ptr, data, size )

static inline void * debug_astStore( void * ptr, const void * data, size_t size );
static inline void * debug_astStore( void * ptr, const void * data, size_t size ) {
  ptr = realloc( ptr, size );
  if (ptr) memcpy( ptr, data, size );
  return ptr;
}

#endif


/* Function Prototypes */

void smf_accumulate_prov( const smfData * data, const Grp* igrp, size_t index,
                          int ondf, const char *creator,
                          NdgProvenance ** modprov, int * status );

void smf_add_grp_metadata( Grp *grp, const char *name, const char *value,
                           int *status );

void smf_add_smu_pcorr( JCMTState *state, int isazel, double dlon,
                        double dlat, int *status);

void smf_add_spectral_axis( int indf, AstFitsChan *fc, int *status );

void smf_addmap1( double *map1, double *mapweight1, int *hitsmap1,
                  double *mapvar1, smf_qual_t *mapqual1, double *map2,
                  double *mapweight2, int *hitsmap2, double *mapvar2,
                  smf_qual_t *mapqual2, dim_t msize, int *status );

void smf_addto_smfArray( smfArray *ary, smfData *data, int *status );

void smf_apodize( smfData *data, size_t len, int forward, int *status );

int smf_apply_dark( smfData *indata, const smfArray *darks,
                     int *status);

void smf_apply_mask( smfData *indata,
                     const smfArray *bpms, smf_bbm_meth method,
                     smf_qual_t addqual, int *status );

int smf_block_end( smfData *data, int block_start, int ipolcrd,
                   float arcerror, int maxsize, int *status );

void smf_bolonoise( ThrWorkForce *wf, smfData *data,
                    size_t window, double f_low,
                    double f_white1, double f_white2,
                    int nep, size_t len, double *whitenoise, double *fratio,
                    smfData **fftpow,int *status );

double smf_calc_covar( const smfData *data, const size_t i, const size_t j,
                       size_t lo, size_t hi, int *status);

void smf_calc_csofit( const smfData * data, AstKeyMap* extpars, double **tau,
                      size_t *nframes, int *status );

double smf_calc_fcon( smfData *data, dim_t nchan, int report,
                      AstMapping **specmap, AstFrame **specframe,
                      int *status );

void smf_calc_mapcoord( ThrWorkForce *wf, AstKeyMap *config, smfData *data,
                        AstFrameSet *outfset, int moving, int *lbnd_out,
                        int *ubnd_out, fts2Port fts_port, int flags,
                        int *status );

double smf_calc_mappa( smfHead *hdr, const char *system, AstFrame *sf,
                       int *status );

double smf_calc_meantau( const smfHead *hdr, int * status );

void smf_calc_mmapsize( size_t headsize, const smfData * data,
                        size_t *headlen, size_t *datalen,
                        size_t *buflen, int * status );

void smf_calc_mode( smfHead * hdr, int * status );

void smf_calc_iqu( ThrWorkForce *wf, smfData *data, int block_start, int block_end,
                   int ipolcrd, int qplace, int uplace, int iplace, NdgProvenance *oprov,
                   AstFitsChan *fc, int pasign, double paoff, double angrot,
                   int submean, int harmonic, int *status );

void smf_calc_skyframe( const AstFrame * skyin,
                        const char * system, const smfHead* hdr,
                        int alignsys, AstSkyFrame ** skyframe,
                        double skyref[2],
                        int * moving, int * status );

void smf_calc_smoothedwvm ( ThrWorkForce * wf, const smfArray * alldata,
                            const smfData * adata, AstKeyMap* extpars, double **wvmtau,
                            size_t *nelems, size_t *ngood, int * status );

void smf_calc_stareimage( smfData *data, const int naver, int *status);

void smf_calc_stats( const smfData *data, const char *mode, const dim_t index,
                     dim_t lo, dim_t hi,  int nclip, const float clip[],
                     double *mean, double *sigma,
                     int *status);

smf_subinst_t smf_calc_subinst ( const smfHead * hdr, int * status );

void smf_calc_telpos( const double obsgeo[3], const char telName[],
                      double telpos[3], int *status );

float smf_calc_telres( AstFitsChan *hdr, int *status );

double smf_calc_wvm( const smfHead *hdr, double approxam, AstKeyMap * extpars, int *status );


void smf_calcmodel_ast( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status);

void smf_calcmodel_com( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status);

void smf_calcmodel_com_old( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                            AstKeyMap *keymap, smfArray **allmodel, int flags,
                            int *status);

void smf_calcmodel_dks( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status);

void smf_calcmodel_ext( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

void smf_calcmodel_flt( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

void smf_calcmodel_gai( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status);

void smf_calcmodel_noi( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status);

void smf_calcmodel_pln( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

void smf_calcmodel_smo( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

void smf_calcmodel_tmp( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

void smf_calcmodel_two( ThrWorkForce *wf, smfDIMMData *dat, int chunk,
                        AstKeyMap *keymap, smfArray **allmodel, int flags,
                        int *status );

int smf_check_detpos( smfData *data, int report, int *status );

void smf_check_flat ( const smfData *data, int *status );

size_t smf_check_quality( ThrWorkForce *wf, smfData *data, int showbad, int *status );

void smf_check_smfDA ( const smfData *idata, smfData *odata, int *status );

void smf_check_smfData ( const smfData *idata, smfData *odata, const int flags,
                         int *status );

void smf_check_smfFile ( const smfData *idata, smfData *odata, int *status );

void smf_check_smfFts ( const smfData *idata, smfData *odata, int *status );

void smf_check_smfHead ( const smfData *idata, smfData *odata, int *status );

void smf_check_units( int count, char current[], smfHead* hdr, int *status );

void smf_checkdets( Grp *detgrp, smfData *data, int *status );

void smf_checkmem_dimm( dim_t maxlen, inst_t instrument, int nrelated,
            smf_modeltype *modeltyps, dim_t nmodels, dim_t msize,
                        AstKeyMap *keymap, size_t available, dim_t maxfilelen,
                        size_t *necessary, int *status );

void smf_checkmem_map( const int lbnd[2], const int ubnd[2], int rebin,
                       size_t available, size_t *necessary, int *status );

void smf_clipped_stats1D( const double *data, size_t nclips,
                          const float clips[], size_t stride, size_t nsamp,
                          const smf_qual_t *quality, size_t qstride,
                          smf_qual_t mask, double *mean, double *sigma,
                          double *median, int usemedian, size_t *ngood,
                          int *status );

void smf_choose_closest( const smfArray *alldata, const smfData *indata,
                         size_t *previdx, size_t *nextidx, int * status );

void smf_choose_darks( const smfArray *darks, const smfData *indata,
                       size_t *dark1, size_t *dark2, int * status );

void smf_choose_flat( const smfArray *flats, const smfData *indata,
                      size_t *flatidx, int * status );

int ***smf_choosepolbins( Grp *igrp, int size, float binsize, float binzero,
                          AstFrameSet *wcsout2d, int *npbin, double **pangle,
                          int *status );

smfTile *smf_choosetiles( Grp *igrp,  int size, int *lbnd,
                          int *ubnd, smfBox *boxes, int spread,
                          const double params[], AstFrameSet *wcsout,
                          int tile_size[ 2 ], int trim, int border,
                          size_t *ntiles, int *status );

void smf_clean_dksquid( smfData *indata, smf_qual_t mask, size_t window,
                        smfData *model, int calcdk, int nofit, int replacebad,
                        int *status );

void smf_clean_pca( ThrWorkForce *wf, smfData *data, size_t t_first,
                    size_t t_last, double thresh, smfData **components,
                    smfData **amplitudes, int flagbad, AstKeyMap *keymap,
                    int *status );

void smf_clean_pca_chunks( ThrWorkForce *wf, smfData *data, size_t chunklen,
                           double thresh, AstKeyMap *keymap, int *status );

void smf_clean_smfArray( ThrWorkForce *wf, smfArray *array,
                         smfArray **noisemaps, smfArray **com, smfArray **gai,
                         AstKeyMap *keymap, int *status );

void smf_clipnoise( double *clipdata, size_t ndata, int cliplog,
                    double cliplow, double cliphigh, size_t *nclipped,
                    int *status );

void smf_clone_data ( const smfData *idata, smfData **odata, int *status );

void smf_close_file( smfData **, int *status);

void smf_close_mapcoord( smfData *data, int *status );

void smf_close_related( smfArray **relfiles, int *status );

void smf_close_smfDream( smfDream **dream, int * status );

void smf_close_smfGroup( smfGroup **group, int *status );

void smf_collapse_quality( const smf_qual_t *inqual, smf_qfam_t qfamily,
                           dim_t nbolo, dim_t ntslice,
                           size_t bstride, size_t tstride,
                           int collapse_time, smf_qual_t **outqual,
                           int *status );

void smf_collapse_tseries( const smfData *indata, int nclip, const float clip[],
                           double snrlim, int flagconst, smf_dtype dtype,
                           smfData **outdata, int *status );

void smf_concat_smfGroup( ThrWorkForce *wf, AstKeyMap *keymap, const smfGroup *igrp,
                          const smfArray *darks, const smfArray *bpms,
                          const smfArray *flatramps, AstKeyMap * heateffmap,
                          size_t whichchunk, int ensureflat, int isTordered,
                          AstFrameSet *outfset, int moving,
                          int *lbnd_out, int *ubnd_out, fts2Port fts_port,
                          dim_t padStart,
                          dim_t padEnd, int flags, smfArray **concat,
                          smfData **first, int *status );

smfDA *smf_construct_smfDA( smfDA * tofill, smfData *dksquid, double * flatcal,
                            double * flatpar, smf_flatmeth flatmeth,
                            int nflat, double refres, double * heatval, int nheat,
                            int * status );

smfData *smf_construct_smfData( smfData * tofill, smfFile * file,
                                smfHead * hdr, smfDA * da, smfFts* fts,
                                smf_dtype dtype, void * pntr[2],
                                smf_qual_t * qual, smf_qfam_t qfamily,
                                smfData * sidequal, int isFFT, int isTordered,
                                const dim_t dims[], const int lbnd[],
                                int ndims, int virtual, int ncoeff,
                                double *poly, AstKeyMap *history,
                                int * status );

smfDream *smf_construct_smfDream( smfData *data, size_t nvert,
                                  size_t nsampcycle, const int *jigvert,
                                  const double *jigpath, int * status );

smfFile *smf_construct_smfFile(smfFile * tofill, int ndfid, int isSc2store,
                               int isTstream, const char * name,
                               int * status );

smfFts* smf_construct_smfFts( smfFts* tofill,
                              smfData* zpd, smfData* fpm, smfData* sigma,
                              int* status);

smfGroup *smf_construct_smfGroup( const Grp *igrp, size_t **subgroups,
                                  size_t *chunk, dim_t *tlen,
                                  const size_t ngroups, const size_t nrelated,
                                  const int copysubgroups, int *status );

smfHead *smf_construct_smfHead( smfHead * tofill, inst_t instrument,
                                AstFrameSet * wcs, AstFrameSet * tswcs,
                                AstFitsChan * fitshdr, JCMTState * allState,
                                dim_t curframe, const double instap[],
                                dim_t nframes, double steptime, double scanvel,
                                smf_obsmode obsmode, smf_swmode swmode,
                                smf_obstype obstype, smf_obstype seqtype,
                                smf_inbeam_t inbeam, unsigned int ndet,
                                double fplanex[], double fplaney[],
                                double detpos[], char *detname, int rpazel,
                                double tsys[], const char title[],
                                const char dlabel[], const char units[],
                                const double telpos[], char * ocsconfig,
                                const char obsidss[], int * status );

void smf_convert_bad( ThrWorkForce * wf, smfData *data, int *status );

void smf_coords_lut( smfData *data, int tstep, dim_t itime_lo,
                     dim_t itime_hi, AstSkyFrame *abskyfrm,
                     AstMapping *oskymap, int moving, int olbnd[ 2 ],
                     int oubnd[ 2 ], fts2Port fts_port, int *lut,
                     double *angle, double *lon,
                     double *lat, int *status );

void smf_copytime( struct timeval *target, struct timeval *source,
                   int *status);

int smf_correct_extinction( ThrWorkForce * wf, smfData *data, smf_tausrc *thetausrc,
                             smf_extmeth method, AstKeyMap * extpars, double tau,
                             double *allextcorr, double **wvmtaucache, int *status);

void smf_create_bolfile( const Grp * rgrp, size_t index,
                         const smfData* refdata, const char * datalabel,
                         const char *units, int hasqual, smfData **respmap, int *status );

smfCreateLutwcsCache *smf_create_lutwcs( int clearcache,
                                         const double *fplane_x,
                                         const double *fplane_y,
                                         const int n_pix,
                                         const JCMTState *state, double dut1,
                                         const double instap[2],
                                         const double telpos[3],
                                         AstFrameSet **fset,
                                         smfCreateLutwcsCache *cache,
                                         int *status );

void smf_create_qualname( const char *mode, int indf, IRQLocs **qlocs,
                          int *status);

smfArray *smf_create_smfArray( int *status );

smfDA *smf_create_smfDA( int * status );

smfData* smf_create_smfData( int flags, int * status );

smfDream *smf_create_smfDream( int * status );

smfFile* smf_create_smfFile( int * status );

smfFilter *smf_create_smfFilter( smfData *template, int *status );

smfFts* smf_create_smfFts(int* status);

smfHead* smf_create_smfHead( int * status );

void smf_create_tswcs( smfHead *hdr, AstFrameSet **frameset, int *status );

double smf_cso2filt_applycoeff( double csotau, const double coeff[2],
                                int *status );

void smf_cso2filt_coeff( const smfHead *hdr,  AstKeyMap * extpars,
                         size_t nvals, double coeffs[], size_t *ncoeff, int *status);

double smf_cso2filt_tau( const smfHead *hdr, double csotau, AstKeyMap * extpars, int *status);

void smf_cubebounds( Grp *igrp,  int size, AstSkyFrame *oskyframe,
                     int autogrid, int usedetpos, AstFrameSet *spacerefwcs,
                     AstFrameSet *specrefwcs, double par[ 7 ],
                     Grp *detgrp, int moving, int specunion, int lbnd[ 3 ],
                     int ubnd[ 3 ], AstFrameSet **wcsout, int *npos,
                     int *hasoffexp, smfBox **boxes, int *polobs, int *status );

void smf_cubegrid( Grp *igrp,  int size, char *system, int usedetpos,
                   int autogrid, int alignsys, Grp *detgrp, double par[ 7 ],
                   int *moving, AstSkyFrame **skyframe, int *sparse,
                   int *gottsys, int *status );

int smf_dataOrder( smfData *data, int isTordered, int *status );

void * smf_dataOrder_array( void * oldbuf, smf_dtype oldtype, smf_dtype newtype,
                            size_t ndata, size_t ntslice, size_t nbolo,
                            size_t tstr1, size_t bstr1,
                            size_t tstr2, size_t bstr2, int inPlace,
                            int freeOld, int * status );

void * smf_dataOrder_ndims( void * oldbuf, smf_dtype dtype, size_t ndata,
                size_t ndims, const dim_t dims[], const size_t perm[],
                int inPlace, int freeOld, int * status );

smfDA * smf_deepcopy_smfDA ( const smfData *old, int cpdks, int * status);

smfData * smf_deepcopy_smfData ( const smfData *old, const int rawconvert,
                                 const int flags, int assertOrder,
                                 int isTordered, int * status);

smfFile * smf_deepcopy_smfFile ( const smfFile *old, int * status );

smfFts* smf_deepcopy_smfFts(const smfData* old, int* status);

smfHead * smf_deepcopy_smfHead ( const smfHead *old, int * status);

void smf_detmask( const char *type, const void *in, int len, int ndim,
                  const int *dims_in, int maxis, const int *mask,
                  void *out, int *status );

void smf_detmaskc( const char *in, int len, int ndim, const int *dims_in,
                   int maxis, const int *mask, char *out, int *status );

smfDetposWcsCache *smf_detpos_wcs( smfHead *hdr, int index, double dut1,
                                   const double telpos[3],
                                   AstFrameSet **fset,
                                   smfDetposWcsCache *cache, int *status );

void smf_diag( ThrWorkForce *wf, HDSLoc *loc, int *ibolo, int irow,
               int power, int time, int isub, smfDIMMData *dat,
               smf_modeltype type, smfArray *model, int res,
               const char *root, int mask, double mingood, int cube,
               int *status );

void smf_diagnostics( ThrWorkForce *wf, int where, smfDIMMData *dat,
                      int chunk, AstKeyMap *keymap, smfArray **allmodel,
                      smf_modeltype type, int *status );

double smf_difftime( struct timeval *tv1, struct timeval *tv2, int *status);

void smf_display_projpars( AstSkyFrame * skyframe, double par[7],
                           int *status);

void smf_downsamp_smfData( ThrWorkForce *wf, const smfData *idata,
                           smfData **odata, dim_t ontslice, int todouble,
                           int method, int *status );

void smf_dream_calcweights ( smfData *data, const Grp *ogrp, const int index,
                             const double gridstep, const int ngrid,
                             const int *gridminmax, int gridpts[][2],
                             int *status );

void smf_dream_getgrid( const AstKeyMap *keymap, double *gridstep, int *ngrid,
                        int gridminmax[4], int gridpts[][2], int *status);

void smf_dream_setjig( char subarray[], int nsampcycle, double gridstep,
                       double jigpath[][2], int *status);

void smf_dreamsolve( smfData *data, int *status );

void smf_dtype_arraycopy( void *ary_out, const smf_dtype dtype_out,
                          void *ary_in, const smf_dtype dtype_in,
                          int nelem, int *status );

int smf_dtype_check( const smfData* data, const char * type, smf_dtype itype,
                     int *status );

int smf_dtype_check_fatal( const smfData* data, const char * type,
                           smf_dtype itype, int *status );

smf_dtype smf_dtype_fromstring( const char * dtype, int * status );

size_t smf_dtype_size( const smfData* data, int * status );

const char * smf_dtype_str( smf_dtype type, int * status );

const char *smf_dtype_string( const smfData* data, int * status );

size_t smf_dtype_sz( const smf_dtype dtype, int *status );

void smf_dump_smfData( const smfData *data, int showflags, int *status );

void smf_expand_tilegroup ( Grp * ogrp, size_t ntile, int npbin,
                            size_t * outsize, int * status);

void smf_expmodel_dks( const smfData *indata, smfData **outdata, int *status);

void smf_expmodel_two( const smfData *indata, smfData **outdata, int *status);

void smf_expmodel_init( const smfData *indata, smfData **outdata, int *status);

void smf_ext2km( int indf, const char *xname, AstKeyMap *keymap, int mode,
                 int *status );

void smf_extracols( smfHead *hdr, Grp *colgrp, void **cols_info,
                    AstKeyMap **km, int *status );

smfData *smf_fft_2dazav( const smfData *data, double *df, int *status );

smfData *smf_fft_avpspec( const smfData *pspec, smf_qual_t *quality,
                          size_t qstride, smf_qual_t mask, double *weights,
                          int *status );

void smf_fft_cart2pol( ThrWorkForce *wf, smfData *data, int inverse, int power, int *status );

smfData *smf_fft_data( ThrWorkForce *wf, const smfData *indata,
                       smfData *outdata, int inverse, size_t len,
                       int *status );

void  smf_fillgaps( ThrWorkForce *wf, smfData *data,
                    smf_qual_t mask, int *status );

void smf_filter_complement( smfFilter *filt, int *status );

void smf_filter_delay( smfFilter *filt, double delay, int *status );

void smf_filter_edge( smfFilter *filt, double f, double w, int lowpass, int *status );

void smf_filter_execute( ThrWorkForce *wf, smfData *data, smfFilter *filt,
                         int complement, int whiten, int *status );

void smf_filter_fromkeymap( smfFilter *filt, AstKeyMap *keymap,
                            const char *qualifier, const smfHead *hdr,
                            int *dofft, int *whiten, int *status );

void smf_filter_ident( smfFilter *filt, int complex, int *status );

void smf_filter_mce( smfFilter *filt, int noinverse, int *status );

void smf_filter_notch( smfFilter *filt, const double f_low[],
                       const double f_high[], size_t n, int *status );

void smf_filter_r2c( smfFilter *filt, int *status );

void smf_filter2d_edge( smfFilter *filt, double f, int lowpass, int *status );

void smf_filter2d_execute( ThrWorkForce *wf, smfData *data, smfFilter *filt,
                           int complement, int *status );

void smf_filter2d_gauss( smfFilter *filt, double fwhm, int *status );

void smf_filter2d_whiten( ThrWorkForce *wf, smfFilter *filt, smfData *map,
                          double minfreq, double maxfreq, size_t smooth,
                          int *status );

void smf_find_acsis_corners( smfData *data, double xc[4], double yc[4],
                             int *status );

void
smf_find_airmass_interval( const smfHead * hdr, double *amstart,
                           double * amend, double *elstart, double *elend,
                           int * status );

int *smf_find_bad_dets( Grp *igrp,  int size, int *nbaddet, int *status );

void smf_find_dateobs(  const smfHead * hdr, double *dateobs,
                        double *dateend, int *status);

int smf_find_gains( ThrWorkForce *wf, int noflag, smfData *data,
                    const unsigned char *mask, smfData *lut, double *template,
                    AstKeyMap *keymap, smf_qual_t goodqual, smf_qual_t badqual,
                    smfData *gai, int *nrej, int *status );

int smf_find_gains_array( ThrWorkForce *wf, int noflag, smfArray *data,
                          const unsigned char *mask, smfArray *lut,
                          double *template, AstKeyMap *keymap,
                          smf_qual_t goodqual, smf_qual_t badqual,
                          smfArray *gain, int *nrej, int *status );

int *smf_find_median( const float *farray, const double *darray, size_t nel,
                      int *hist, float *median, int *status );

void smf_find_science(const Grp * ingrp, Grp **outgrp, int reverttodark,
                      Grp **darkgrp, Grp **flatgrp, int reducedark, int calcflat,
                      smf_dtype darktype, smfArray ** darks, smfArray **fflats,
                      AstKeyMap **heateffmap, double * meanstep, int * status );

void smf_find_seqcount( const smfHead * hdr, int *seqcount, int *status );

void smf_find_subarray ( const smfHead * hdr, char subarray[],
                         size_t buflen, sc2ast_subarray_t *subnum, int *status );

void smf_find_thetabins( const smfData *data, int nosign, double **bins,
                         double **bincen, dim_t *nbin, int **whichbin,
                         int *status );

void smf_fit_poly( ThrWorkForce *wf, smfData *data, const size_t order,
                   int remove, double *poly, int *status);

void smf_fit_poly1d ( size_t order, size_t nelem, double clip, const double x[],
                      const double y[], const double vary[],
                      const smf_qual_t qual[], double coeffs[],
                      double varcoeffs[], double polydata[], int64_t *nused,
                      int *status );

void smf_fit_poly1d_chisq ( size_t order, size_t nelem, const double x[],
                       const double y[], const double vary[],
                       const smf_qual_t qual[], double coeffs[],
                       double varcoeffs[], double polydata[], int64_t *nused,
                       double * rchisq, int *status );

void smf_fit_pspec( const double *pspec, dim_t nf, size_t box, double df,
                    double minfreq, double whitefreq, double maxfreq,
                    double *a, double *b, double *w, int *status );

void smf_fit_profile(  smfData *data, smfArray *pardata, void *pfcntrl,
               int *status );

void smf_fits_crchan( size_t nfits, const char * headrec, AstFitsChan ** fits,
                      int *status);

void smf_fits_export2DA( AstFitsChan *fitschan, size_t *ncards,
                         char * fitsrec, int *status );

/* Do not return result since we want the interface to remain the same when a
   string is required. If we return a string we must know who should free it */

void smf_fits_getD( const smfHead * hdr, const char * cardname,
                    double * result, int * status );

void smf_fits_getI( const smfHead * hdr, const char * cardname, int * result,
                    int * status );

void smf_fits_getL( const smfHead * hdr, const char * cardname, int * result,
                    int * status );

void smf_fits_getS( const smfHead * hdr, const char * cardname,
                    char result[70], size_t len, int * status );

void smf_fits_outhdr( AstFitsChan * inhdr, AstFitsChan ** outhdr,
                      int * status );

int smf_fits_updateD( smfHead * hdr, const char * name, double value,
                      const char * comment, int *status );

int smf_fits_updateI( smfHead * hdr, const char * name, int value,
                      const char * comment, int *status );

int smf_fits_updateL( smfHead * hdr, const char * name, int value,
                      const char * comment, int *status );

int smf_fits_updateS( smfHead * hdr, const char * name, const char * value,
                      const char * comment, int *status );

int smf_fits_updateU( smfHead * hdr, const char * name,
                      const char * comment, int *status );

int smf_fix_data( msglev_t msglev, smfData *data, int * status );

int smf_fix_metadata( msglev_t msglev, smfData * data, int * status );

int smf_fix_metadata_acsis( msglev_t msglev, smfData * data, int have_fixed,
                            int *ncards, int * status );

int smf_fix_metadata_scuba2( msglev_t msglev, smfData * data, int have_fixed,
                             int *ncards, int * status );

void smf_fix_pol2( ThrWorkForce *wf,  smfArray *array, int *status );

void smf_flag_slewspeed( smfData *data, double smin, double smax,
                         size_t *nflagged, double *average_speed,
                         int *status );

void smf_flag_spikes( ThrWorkForce *wf, smfData *data, smf_qual_t mask,
                      double thresh, size_t box, size_t *nflagged,
                      int *status );

void smf_fix_steps( ThrWorkForce *wf, smfData *data, double dcthresh,
                    dim_t dcsmooth, dim_t dcfitbox, int dcmaxsteps,
                    int dclimcorr, int meanshift, size_t *nrej,
                    smfStepFix **steps, int *nstep, int *status );

void smf_flat_assign ( int use_da, smf_flatmeth inflatmeth, double refres,
                       const smfData * powval, const smfData * bolval,
                       smfData * updata, int *status );

void
smf_flat_badflat( smfData * refdata, smfData ** resp, int * status );

size_t
smf_flat_calcflat( msglev_t msglev, const char flatname[],
                   const char resistpar[],
                   const char methpar[], const char orderpar[],
                   const char resppar[], const char respmaskpar[],
                   const char snrminpar[], const Grp * prvgrp,
                   smfData *flatdata, smfData **respmapout, int *status );

void smf_flat_fastflat( const smfData * fflat, smfData **bolvald, int *status );

void smf_flat_fitpoly ( const smfData * powvald, const smfData * bolvald,
                        double snrmin, size_t order, smfData **coeffs,
                        smfData ** polyfit, int *status );

void smf_flat_malloc( size_t nheat, const smfData * refdata,
                      smfData ** powvald, smfData **bolvald, int *status );

void smf_flat_mergedata( const smfArray * heatframes,
                         const double heatval[], smfData ** bolvald,
                         int * status );

smf_flatmeth smf_flat_methcode ( const char * flatname, int * status );

const char * smf_flat_methstring( smf_flatmeth flatmeth, int * status );

int
smf_flat_override ( const smfArray *flats, smfData * indata, int *status );

void
smf_flat_params( const smfData * refdata, const char resistpar[],
                 const char methpar[], const char orderpar[], const char snrminpar[],
                 double * refohms, double **resistance, int * nrows,
                 int * ncols, smf_flatmeth  *flatmeth,
                 int * order, double * snrmin, smfData ** heateff, int * status );

void smf_flat_precondition( int allbad, smfData * powvald, smfData * bolvald,
                            int *status );

size_t smf_flat_responsivity ( smf_flatmeth method, smfData *respmap, double snrmin, size_t order,
                               const smfData * powval, const smfData * bolval,
                               double refres, smfData ** polyfit, int *status );

void smf_flat_smfData ( const smfData *data, smf_flatmeth * flatmethod,
                        double * refres, smfData ** powval, smfData **bolval,
                        int *status );

void smf_flat_standardpow( const smfData * bolvald, double refohms,
                           const double resistance[],
                           smfData ** powref, smfData ** bolref, int * status);

void smf_flat_write( smf_flatmeth flatmeth, const char * flatname,
                     double refres, const smfData * bolval,
                     const smfData * powref, const smfData * bolref,
                     const smfData * polyfit, const Grp * prvgrp, int * status );

void smf_flatfield ( const smfData *idata, const smfArray * flats, AstKeyMap * heateffmap,
                     smfData **odata, const int flags, int *status );

int smf_flatfield_smfData ( smfData *data, const smfArray * flats, AstKeyMap * heateffmap,
                            int force, int *status );

void smf_flatten ( smfData *data, AstKeyMap * heateffmap, int *status );

AstKeyMap * smf_free_effmap( AstKeyMap * effmap, int *status);

smfFilter *smf_free_smfFilter( smfFilter *filt, int *status );

int ***smf_freepolbins( int nndf, int npbin, double **pangle, int ***ptime,
                        int *status );

smfTile *smf_freetiles( smfTile *tiles, int size, int *status );

void smf_gandoff( dim_t ibolo, dim_t time0, dim_t time1, dim_t ntime,
                  dim_t gbstride, dim_t gcstride, double *gai_data,
                  dim_t nblock, dim_t gain_box, double *wg, double *woff,
                  double *wcc, int *status );

int  smf_gauest( const double y[], int n, double *p, int np, double rms,
                 double cutamp, double cutsig, int q );

void smf_geod( const double pos[3], double *phi, double *h, double *lambda );

void smf_get_cleanpar( AstKeyMap *keymap, const char *qualifier,
                       const smfData *data, double *badfrac,
                       dim_t *dcbox, int *dcbad, double *dcthresh,
                       dim_t *dcsmooth, int *dclimcorr, int *dkclean,
                       int *fillgaps, int *zeropad, double *filt_edgelow,
                       double *filt_edgehigh, double *filt_edge_smallscale,
                       double *filt_edge_largescale, double *filt_notchlow,
                       double *filt_notchhigh, int *filt_nnotch,
                       int *dofilt, double *flagslow, double *flagfast,
                       int *order, double *spikethresh, dim_t *spikebox,
                       double *noisecliphigh, double *noisecliplow,
                       int *whiten, int *compreprocess, dim_t *pcalen,
                       double *pcathresh, int *groupsubarray,
                       double *downsampscale, double *downsampfreq,
                       int *noiseclipprecom, int *deconvmce, double *delay,
                       double *filt_edgewidth, int *status );

void smf_get_dims( const smfData *data, dim_t *nrows, dim_t *ncols,
                   dim_t *nbolo, dim_t *ntslice, dim_t *ndata,
                   size_t *bstride, size_t *tstride,int *status );

void smf_get_extpar( AstKeyMap *keymap, smf_tausrc *tausrc,
                     smf_extmeth *extmeth, int *import, int *status );

size_t smf_get_findex( double f, double df, dim_t nf, int *status );

size_t smf_get_freemem ( double * mbytes, size_t *pagesize,
                         int64_t *physsize, int * status );

void smf_get_goodrange( const smf_qual_t *quality, dim_t ntslice,
                        size_t tstride, smf_qual_t mask,
                        size_t *istart, size_t *iend, int *status );

void smf_get_gridcoords( double *row, double *col, int nrow, int ncol,
                         int *status );

void smf_get_grp_metadata( const Grp *grp, const char *name, char *value,
                           int *status );

unsigned char *smf_get_mask( ThrWorkForce *wf, smf_modeltype mtype,
                             AstKeyMap *config, smfDIMMData *dat, int flags,
                             int *status );

void smf_get_moltrans ( double restFreq, const char **molecule,
                        const char **transition, int *status );

int smf_get_ndfid ( const HDSLoc *loc, const char *name, const char *accmode,
                    const char *state, const char *dattype, const int ndims,
                    const int *lbnd, const int *ubnd, int *status );

double smf_get_nsamp( AstKeyMap *keymap, const char *name,
                      const smfData *data, dim_t *nsamp, int *status );

dim_t smf_get_padding( AstKeyMap *keymap, int report,const smfHead *hdr,
                       double, int *status );

void smf_get_projpar( AstSkyFrame *skyframe, const double skyref[2],
                      int moving, int autogrid,
                      int nallpos, const double * allpos, float telres,
                      double map_pa, double par[7], int *sparse, int *useauto,
                      int *status );

HDSLoc * smf_get_smurfloc( const smfData *data, const char * accmode, int * status );

void smf_get_spread( char *pabuf, int *spread, int *nparam, int *status );

void smf_get_taskname( char * taskname, char * prvname, int * status);

HDSLoc *smf_get_xloc ( const smfData *data, const char *extname,
                       const char *extype, const char *accmode,
                       const int ndims, const int *dims, int *status );

void smf_getfitsd( const smfHead *hdr, const char *cardname,
                   double *result, int *status );

void smf_getfitsi( const smfHead *hdr, const char *cardname,
                   int *result, int *status );

void smf_getfitss( const smfHead *hdr, const char * name, char * result,
                   size_t len, int * status );

char *smf_getobsidss( AstFitsChan *hdr, char * obsid, size_t szobs,
                      char * obsidss, size_t szss, int *status );

void smf_getrefwcs( const char *param, Grp *igrp, AstFrameSet **specwcs,
                    AstFrameSet **spacewcs, int *status );

void smf_getspectralwcs( AstFrameSet *fset, int type, AstFrameSet **specwcs,
                         int *status );

AstKeyMap *smf_groupscans( const Grp *igrp,  int size, int *maxsyspop,
                           int *conform, Grp **ogrp, int *status );

Grp *smf_grp_new( const Grp *grp, const char *type, int *status );

void smf_grp_related( const Grp *igrp, const size_t grpsize,
                      const int grouping, const int checksubinst,
                      double maxlen_s, double *srate_maxlen,
                      AstKeyMap *keymap, dim_t *maxconcatlen,
                      dim_t *maxfilelen, smfGroup **group,
                      Grp **basegrp, dim_t *pad, int *status );

void smf_handler( int sig );

void smf_history_add( smfData* data, const char * appl,
                      int *status);

int smf_history_check( const smfData* data, const char * appl, int *status);

void smf_history_read( smfData* data, int *status);

void smf_history_write( const smfData* data, int *status);

void smf_import_array( smfData *refdata, const char *name, int bad,
                       int expand, double *dataptr, int *status );

void smf_inbeam_str ( smf_inbeam_t inbeam, char *inbeamstr, size_t lenstr, int * status );

int smf_initial_sky( ThrWorkForce *wf, AstKeyMap *config, smfDIMMData *dat,
                     int *iters, int *status );

inst_t smf_inst_get( const smfHead * hdr, int * status );

void smf_instap_get( smfHead * hdr, int * status );

int smf_is_wvm_usable( const smfHead * hdr, int * status );

int smf_isdark( const smfData *data, int * status );

int smf_isfft( const smfData *indata, dim_t rdims[2], dim_t *nbolo,
               dim_t fdims[2], double df[2], size_t *ndims, int *status );

void smf_iteratemap( ThrWorkForce *wf, const Grp *igrp, const Grp *iterrootgrp,
                     const Grp *bolrootgrp, const Grp *shortrootgrp,
                     const Grp *flagrootgrp, const Grp *samprootgrp,
                     AstKeyMap *keymap,
                     const smfArray *darks, const smfArray *bbms,
                     const smfArray * flatramps, AstKeyMap * heateffmap, AstFrameSet *outfset,
                     int moving, int *lbnd_out, int *ubnd_out, fts2Port fts_port, size_t maxmem,
                     double *map, int *hitsmap, double * exp_time,
                     double *mapvar, smf_qual_t *mapqual, double *weights,
                     char data_units[], double * nboloeff,
                     size_t *numcontchunks, size_t *numinsmp,
                     size_t *numcnvg, int *iters, int *status );

const char *smf_keyname( AstKeyMap *keymap, const char *basename,
                         const char *qualifier, char *buf, size_t lbuf,
                         int *status );

void smf_km2ext( int indf, const char *xname, AstKeyMap *keymap,
                 int *timeout, int *status );

void smf_kmmerge( const char *xname, AstKeyMap *keymap, int *index,
                  int ndet, int *mask, int nts, int *rts, int j0, int j1,
                  int *status );

void smf_labelunit( Grp *igrp,  int size, smfData *odata, int *status );

void smf_lock_data( smfData *data, int lock, int *status );

void smf_lock_related( smfArray *data, int lock, int *status );

int  smf_lsqfit( smf_math_function fid, const double xdat[], int xdim, const double ydat[], const float wdat[],
                 int ndat, double *fpar, double *epar, const int mpar[],
                 int npar, int ncomp, float tol, int its, float lab,
                 const int iopt[], const double dopt[] );

void smf_makefitschan( const char *system, double crpix[2], double crval[2],
                       double cdelt[2], double crota2, AstFitsChan *fc,
                       int *status );

AstMapping *smf_maketanmap( double lon, double lat, AstMapping *cache[ 2 ],
                            int *status );

double smf_map_getpixsize( const smfData *data, int *status );

void * smf_map_or_malloc( size_t nelem, smf_dtype type, int zero, int indf,
                          const char * comp, int * status );

void smf_map_spikes( ThrWorkForce *wf, smfData *data, smfData *variance,
                     int *lut, smf_qual_t mask, double *map, double *mapweight,
                     int *hitsmap, double *mapvar, double thresh,
                     size_t *nflagged, int *status );

void smf_mapbounds( int fast, Grp *igrp,  int size, const char *system,
                    const AstFrameSet * spacesrefwcs, int alignsys,
                    int *lbnd_out, int *ubnd_out, AstFrameSet **outframeset,
                    int *moving, smfBox ** boxes, fts2Port fts_port,
                    int *status );

void smf_mapbounds_approx( Grp *igrp, size_t index, char *system,
                           int *lbnd_out, int *ubnd_out,
                           AstFrameSet **outframeset, int *moving,
                           int *status );

void smf_mask_noisy( ThrWorkForce *wf, smfData *data, smfData **noise,
                     double sigcliphigh, double sigcliplow, int cliplog,
                     int zeropad, int * status );

void smf_maskacsis( int indf, int *mask, int *status );


void smf_meanshift( double *indata, double *outdata, int nel, int stride, int box1,
                    double box2, smf_qual_t *qual, smf_qual_t mask, float wlim,
                    int *status );

smf_math_function
smf_mathfunc_fromstring( const char * mathfunc, int * status );

const char * smf_mathfunc_str( smf_math_function type, int * status );

void smf_math_functions( smf_math_function fid, double xdat, const double fpar[], int ncomp,
             double *value, double *pderv, int *npar,
             const int iopt[], const double dopt[] __attribute__((unused)) );

double smf_math_fvalue( smf_math_function fid, double xdat, const double fpar[], int ncomp,
            const int iopt[], const double dopt[] );

void smf_math_fpderv( smf_math_function fid, double xdat, const double fpar[], int ncomp,
              double *epar, const int iopt[], const double dopt[] );

int smf_math_fnpar( smf_math_function fid );

void smf_math_cmplxerrfunc( double Rz, double Iz, double *Rr, double *Ir );

void smf_median_smooth( dim_t box, smf_filt_t filter_type, float wlim,
                        dim_t el, const double *dat,
                        const smf_qual_t *qua, size_t stride,
                        smf_qual_t mask, double *out, double *w1,
                        size_t *w2, int *w3, int *status );

void smf_model_create( ThrWorkForce *wf, const smfGroup *igroup,
                       smfArray **iarray, const smfArray *darks,
                       const smfArray *bbms, const smfArray *flatramps,
                       AstKeyMap * heateffmap, const smfArray *noisemaps,
                       dim_t nchunks, smf_modeltype mtype, int isTordered,
                       AstFrameSet *outfset, int moving,
                       int *lbnd_out, int *ubnd_out, fts2Port fts_port,
                       smfGroup **mgroup,
                       smfArray **mdata, AstKeyMap *keymap, int *status );

void smf_model_createHdr( smfData *model, smf_modeltype type,
                          smfData *refdata, int *status );

smf_expmodelptr smf_model_getexpptr( smf_modeltype type, int *status);

void
smf_model_dataOrder( smfDIMMData *dat, smfArray ** allmodel, int chunk, smf_modeltype toOrder,
                     int isTordered, int * status );

const char *smf_model_getname( smf_modeltype type, int *status);

smf_calcmodelptr smf_model_getptr( smf_modeltype type, int *status);

smf_modeltype smf_model_gettype( const char *modelname, int *status );

Grp *smf_ndg_copy( const Grp *grp1, size_t indxlo, size_t indxhi, int reject, int *status );

void smf_obsmap_fill( const smfData * data, AstKeyMap * obsmap,
                      AstKeyMap * objmap, int * status );

void smf_obsmap_report( msglev_t msglev, AstKeyMap * obsmap, AstKeyMap * objmap,
                        int * status );

const char * smf_obsmode_str( smf_obsmode type, int * status );

const char * smf_obstype_str( smf_obstype type, int * status );

int smf_open_and_flatfield ( const Grp *igrp, const Grp *ogrp, size_t index,
                             const smfArray * darks, const smfArray * ramps,
                             AstKeyMap * heateffmap, smfData **ffdata, int *status);

void smf_open_asdouble( const Grp *igrp, size_t index, const smfArray* darks,
                        const smfArray* flatramps, AstKeyMap * heateffmap, int ensureflat,
                        smfData **data, int *status );

void smf_open_file( const Grp * igrp, size_t index, const char * mode,
                    int withHdr, smfData ** data, int *status);

void smf_open_file_job( ThrWorkForce *wf, int wait, const Grp *igrp,
                        size_t index, const char *mode, int flags,
                        smfData **data, int *status );

void smf_open_group( const Grp * igrp, const dim_t refdims[],
                     smfArray **files, int *status );

void smf_open_mapcoord( smfData *data, const char *mode, int *status );

void smf_open_model( const Grp *igrp, int index, const char *mode,
                     smfData **data, int *status );

void smf_open_ndf( const int newndf, const char accmode[],
                   smf_dtype dtype, smfData **ndata,
                   int *status);

void smf_open_ndfname( const HDSLoc *loc, const char accmode[],
                       const char extname[],
                       const char state[], const char dattype[],
                       const int ndims, const int lbnd[], const int ubnd[],
                       const char datalabel[], const char dataunits[],
                       const AstFrameSet * wcs,
                       smfData **ndfdata, int *status);

void smf_open_newfile( const Grp * igrp, int index, smf_dtype dtype,
                       const int ndims, const int *lbnd, const int *ubnd,
                       int flags, smfData ** data, int *status);

void
smf_open_raw_asdouble( const Grp *igrp, size_t index, const smfArray* darks,
                       smfData **data, int *status );

void smf_open_related( const smfGroup *group, const dim_t subindex,
                       const char *accmode, smfArray **relfiles,
                       int *status );

void smf_open_related_model( const smfGroup *group, const dim_t subindex,
                             const char *accmode, smfArray **relfiles,
                             int *status );

FILE *smf_open_textfile( const char *param, const char *mode, const char *def,
                         int *status );

int smf_pattern_extract ( const char * sourcestr, const char * pattern,
                          double *dresult, char * sresult, size_t szstr, int * status );

void smf_pcorr( smfHead *head, const Grp *igrp, int *status );

void smf_polext( int ondf, int store_angle, double angle, const char *domain, int axis, int *status );

void smf_pread( Grp *igrp, const char *param, int *status );

void smf_puthistory( smfData *data, const char *appn, int *status );

size_t smf_qfamily_count( smf_qfam_t qfamily, int * status );

const char * smf_qfamily_str( smf_qfam_t qfamily, int * status );

smf_qual_t * smf_qual_map( int indf, const char mode[],
                           smf_qfam_t *family, size_t *nmap, int * status );

const char *smf_qual_str( smf_qfam_t family, int usebit, int bit_or_val,
                          const char **descr, int *status );

smf_qual_t smf_qual_str_to_val( const char *qname, smf_qfam_t * family, int * status );

int smf_qual_to_bit( smf_qual_t q, int *status );

smf_qual_t * smf_qual_unmap( int indf, smf_qfam_t family, smf_qual_t * qual, int * status );

void smf_qualstats( ThrWorkForce *wf, smf_qfam_t qfamily, int nopad,
                    const smf_qual_t *qual, dim_t nbolo, size_t bstride,
                    size_t ntslice, size_t tstride,  size_t qcount[SMF__NQBITS],
                    size_t *ngoodbolo, size_t *nmap, size_t *nmax, size_t * tpad,
                    int *status );

void smf_qualstats_model( ThrWorkForce *wf, smf_qfam_t qfamily, int nopad,
                          const smfArray *qua, size_t qcount[SMF__NQBITS],
                          size_t * ngoodbolo, size_t * nmap, size_t *nmax, dim_t * ntslice, size_t * ntgood,
                          size_t * tbound, size_t *tpad, int * status );

void smf_qualstats_report( ThrWorkForce *wf, msglev_t msglev, smf_qfam_t qfamily, int nopad,
                           const smfArray *qua, size_t last_qcount[SMF__NQBITS],
                           size_t *last_nmap, int init, size_t *ngood_tslice,
                           size_t *numdata, int *status );

double smf_quick_noise( const smfData *data, dim_t bolo, dim_t nsamp, dim_t nchunk,
                        smf_qual_t mask, int *status );

double smf_raw2current( smfHead *hdr, int *status );

AstMapping *smf_rebin_totmap( smfData *data, dim_t itime,
                              AstSkyFrame *abskyfrm,
                              AstMapping *oskymap, int moving,
                              fts2Port fts_port, int *status );

void smf_rebincube( ThrWorkForce *wf, smfData *data, int first, int last,
                    int *ptime, int badmask, int is2d, AstSkyFrame *abskyfrm,
                    AstMapping *oskymap, AstFrame *ospecfrm,
                    AstMapping *ospecmap, Grp *detgrp, int moving, int usewgt,
                    int lbnd_out[ 3 ], int ubnd_out[ 3 ], int spread,
                    const double params[], int genvar, float *data_array,
                    float *var_array, double *wgt_array, float *texp_array,
                    float *teff_array, double *fcon, int64_t *nused, int *nreject,
                    int *naccept, int *status );

void smf_rebincube_ast( ThrWorkForce *wf, smfData *data, int first, int last,
                        int *ptime, dim_t nchan, dim_t ndet, dim_t nslice,
                        dim_t nel, dim_t nxy, dim_t nout, dim_t dim[3],
                        AstMapping *ssmap, AstSkyFrame *abskyfrm,
                        AstMapping *oskymap, Grp *detgrp, int moving,
                        int usewgt, int spread, const double params[],
                        int genvar, double tfac, double fcon,
                        float *data_array, float *var_array,
                        double *wgt_array, float *texp_array,
                        float *teff_array, int *good_tsys, int64_t *nused,
                        int *status );

void smf_rebincube_init( int is2d, dim_t nxy, dim_t nout, int genvar,
                         float *data_array, float *var_array,
                         double *wgt_array, float *texp_array,
                         float *teff_array, int64_t *nused, int *status );

void smf_rebincube_nn( ThrWorkForce *wf, smfData *data, int first, int last,
                       int *ptime, dim_t nchan, dim_t ndet, dim_t nslice,
                       dim_t nxy, dim_t nout, dim_t dim[3],
                       int badmask, int is2d, AstMapping *ssmap,
                       AstSkyFrame *abskyfrm, AstMapping *oskymap,
                       Grp *detgrp, int moving, int usewgt, int genvar,
                       double tcon, double fcon, float *data_array,
                       float *var_array, double *wgt_array, float *texp_array,
                       float *teff_array, int64_t *nused, int *nreject,
                       int *naccept, int *good_tsys, int *status );

void smf_rebincube_norm2d( dim_t nout, dim_t nxy, int genvar,
                           float *data_array, float *var_array,
                           double *wgt_array, int *pop_array, int *status );

void smf_rebincube_norm3d( dim_t nout, int genvar, int64_t nused, float *data_array,
                           float *var_array, double *wgt_array, int *status );

void smf_rebincube_paste2d( int badmask, dim_t nchan, int nchanout,
                            int *spectab, int *specpop, dim_t iv0,
                            dim_t nxy, double wgt, int genvar,
                            double invar, float *ddata,
                            float *data_array, float *var_array,
                            double *wgt_array, int *pop_array,
                           int64_t *nused, int *nreject, int *naccept,
                            float *work, int *status );

void smf_rebincube_paste3d( dim_t nchan, dim_t nout, int *spectab, dim_t iv0,
                            dim_t nxy, double wgt, int genvar, double invar,
                            float *pdata, float *data_array,
                            float *var_array, double *wgt_array, int64_t *nused,
                            int *status );

void smf_rebincube_paste_thread( void *data, int *status );

void smf_rebincube_seqf( ThrWorkForce *workforce, int nthreads,
                         double *blk_bot, AstMapping *this, double wlim,
                         int ndim_in, const int lbnd_in[], const int ubnd_in[],
                         const float in[], const float in_var[], int spread,
                         const double params[], int flags, double tol,
                         int maxpix, float badval, int ndim_out,
                         const int lbnd_out[], const int ubnd_out[],
                         const int lbnd[], const int ubnd[], float out[],
                         float out_var[], double weights[], int64_t *nused,
                         int *status );

void smf_rebincube_spectab( dim_t nchan, dim_t nchanout, AstMapping *ssmap,
                            int **pspectab, int *status );

const double *smf_rebincube_tcon( smfHead *hdr, dim_t itime, double fcon,
                                  float *texp, float *teff, double *tcon,
                                  int *status );

void smf_rebinmap( ThrWorkForce *wf, smfData *data, double *bolovar,
                   int index, int size, AstFrameSet *outfset, int spread,
                   const double params[], int moving, int genvar, int *lbnd_out,
                   int *ubnd_out, double *map, double *variance,
                   double *weights, int64_t *nused, fts2Port fts_port,
                   int *status );

void smf_rebinmap1( ThrWorkForce *wf, smfData *data, smfData *variance, int *lut,
                    size_t tslice1, size_t tslice2, int trange,
                    int *whichmap, dim_t nmap, smf_qual_t mask, int sampvar,
                    int flags, double *map, double *mapweight,
                    double *mapweightsq, int *hitsmap, double *mapvar,
                    dim_t msize, double *scalevariance, int *status );

void smf_rebinseq_thread( void *data_ptr, int *status );

void smf_rebinslices( void *job_data_ptr, int *status );

void smf_rebinsparse( smfData *data, int first, int *ptime, AstFrame *ospecfrm,
                      AstMapping *ospecmap, AstSkyFrame *oskyframe,
                      Grp *detgrp, int lbnd_out[ 3 ], int ubnd_out[ 3 ],
                      int genvar, float *data_array, float *var_array,
                      int *ispec, float *texp_array, float *ton_array,
                      double *fcon, int *status );

void smf_reduce_dark( const smfData *indark, smf_dtype dtype,
                      smfData **outdark, int *status );

void smf_remove_grp_metadata( Grp *grp, const char *name, int *status );

void smf_reorder( const char *type, void *in, int len, int ndim, int *dims,
                  int axis, int *index, int maxis, int *mask, void *out,
                  int *status );

void smf_reportprogress( int max, int *status );

void smf_request_mask( const char *param, smfArray ** bpms, int *status);

void smf_resampcube( smfData *data, AstSkyFrame *abskyfrm, AstMapping *iskymap,
                     AstFrame *ispecfrm, AstMapping *ispecmap,
                     Grp *detgrp, int moving, int slbnd[ 3 ],
                     int subnd[ 3 ], int interp, const double params[],
                     float *in_data, float *out_data, int *overlap,
                     int *status );

void smf_resampcube_ast( smfData *data, dim_t nchan,
                         dim_t ndet, dim_t nslice, dim_t nel,
                         dim_t dim[3], AstMapping *ssmap,
                         AstSkyFrame *abskyfrm, AstMapping *iskymap,
                         Grp *detgrp, int moving, int interp,
                         const double params[], float *in_data,
                         float *out_data, int *status );

void smf_resampcube_copy( dim_t nchan, int *spectab,
                          dim_t iv0, dim_t nxy, float *ddata,
                          float *in_data, int *status );

void smf_resampcube_nn( smfData *data, dim_t nchan,
                        dim_t ndet, dim_t nslice, dim_t nxy,
                        dim_t dim[3], AstMapping *ssmap,
                        AstSkyFrame *abskyfrm, AstMapping *iskymap,
                        Grp *detgrp, int moving, float *in_data,
                        float *out_data, int *overlap, int *status );

void smf_resampmap( ThrWorkForce *wf, smfData *data, AstSkyFrame *abskyfrm,
                    AstMapping *sky2map, int moving, int slbnd[ 2 ],
                    int subnd[ 2 ], int interp, const double params[],
                    double sigma, double *in_data, double *out_data,
                    double *ang_data, int *ngood, int *status );

void smf_reshapendf( smfData **data, smfTile *tile, int *status );

void smf_rolling_fit( dim_t box, float wlim, dim_t el, dim_t start,
                      dim_t end, const double *dat, double *grad,
                      double *off, double *rms, int *status );

int smf_samedims_smfData( const smfData *data1, const smfData *data2,
                          int *status );

void smf_scale2freq( double f_edgesmall, double f_edgelarge, double f_edgewidth, const smfHead *hdr,
                     double *f_edgelow, double *f_edgehigh, double *f_width, int *status );

void smf_scale_bols( ThrWorkForce *wf, smfData *data, const smfData * scaledata,
                     const char *path, const char *param, int div, int *status );

void smf_scalar_multiply( smfData * data, double dscalar, int * status );

void smf_select_pntr( void *const pntr[2], smf_dtype dtype, double **ddata,
                      double **dvar, int **idata, int **ivar, int *status );

const smf_qual_t *
smf_select_cqualpntr( const smfData * data, smf_qfam_t * qfamily, int * status );

smf_qual_t * smf_select_qualpntr( smfData * data, smf_qfam_t * qfamily, int * status );

void smf_set_clabels( const char title[], const char label[],
                      const char units[], smfHead* hdr, int * status );

void smf_set_moving( AstFrameSet* wcs, AstFitsChan * fchan, int *status );

double smf_sigmaclip( int nval, double *val, double *wgt,
                      double nsigma, int niter, double *stddev, int *status );

void smf_smfFile_msg( const smfFile * file, const char token[],
                      int strip, const char fallback[] );

double *smf_smoothmask( ThrWorkForce * wf, smf_qual_t qual, const smf_qual_t *mapqual, int
                        dim0, int dim1, AstFrameSet *fset, double fwhm,
                        double low, int *status );

void smf_snrmask( ThrWorkForce *wf, const double *map, const double *mapvar,
                  const dim_t *dims, double snr_hi, double snr_lo,
                  unsigned char *mask, int *status );

int smf_sort_bydouble ( const void *in1, const void *in2 );

void smf_sparsebounds( Grp *igrp,  int size, AstSkyFrame *oskyframe,
                       int usedetpos, Grp *detgrp, int lbnd[ 3 ], int ubnd[ 3 ],
                       AstFrameSet **wcsout, int *hasoffexp, int *polobs,
                       int *status );

int smf_spreadwidth( int spread, const double params[], int *status );

void smf_store_image( smfData *data, HDSLoc *scu2redloc, int cycle, int ndim,
                      int dims[], int nsampcycle, int vxmin, int vymin,
                      double *image, double *zero, int *status);

void smf_store_outputbounds (int updatepars, const int lbnd_out[3],
                             const int ubnd_out[3],
                             const AstFrameSet *wcsout,
                             const AstSkyFrame *oskyfrm,
                             const AstMapping *oskymap, int *status);

void smf_stripsuffix( const char *instr, const char *suffix, char *outstr,
                      int *status );

AstKeyMap *smf_subinst_keymap( smf_subinst_t subinst, const smfData * indata,
                               const Grp * igrp, size_t index, int * status );

const char * smf_subinst_str( smf_subinst_t subinst, int * status );

void smf_subtract_dark ( smfData * indata, const smfData * dark1,
                         const smfData * dark2, smf_dark_sub_meth method,
                         int *status );

void smf_subtract_plane( smfData *data, smfArray *array, const char *fittype,
                         int *status);

void smf_subtract_plane1( smfData *data, const char *fittype, double *meansky,
                          int *status);

void smf_subtract_plane2( smfArray *array, const char *fittype, double *meansky,
                          int *status);

void smf_subtract_plane3( ThrWorkForce *wf, smfData *data,
                          const dim_t mdims[],
                          const int lut[], int * status );

void smf_subtract_poly( smfData *data, int rel, int *status );

void smf_summarize_obs( const Grp * igrp, int * status );

const char * smf_swmode_str( smf_swmode mode, int * status );

double
smf_tai2unix( double tai, int * status );

const char * smf_tausrc_str( smf_tausrc mode, int * status );

void smf_telpos_get( smfHead * hdr, int * status );

void smf_terr( double phi, double h, double lambda, double pos[3] );

void smf_timerinit( struct timeval *tv1, struct timeval *tv2, int *status);

double smf_timerupdate( struct timeval *tv1, struct timeval *tv2, int *status);

void smf_tslice_ast (smfData * data, dim_t index, int needwcs, fts2Port fts_port,
                     int * status );

void smf_uncalc_iqu( ThrWorkForce *wf, smfData *data, double *idata,
                     double *qdata, double *udata, double *angdata,
                     int pasign, double paoff, double angrot, int *status );

void smf_update_quality( smfData *data, int syncbad,
                         const int *badmask, smf_qual_t addqual, double badfrac,
                         int *status );

void smf_update_valbad( smfData *data, smf_modeltype mtype, const smf_qual_t *qual,
                        size_t qbstride, size_t qtstride, smf_qual_t mask,
                        int *status );

void smf_updateprov( int ondf, const smfData *data, int indf,
                     const char *creator, NdgProvenance ** modprov, int *status );

int smf_validate_smfData( const smfData * data, int hashdr, int hasfile,
                          int *status );

int smf_validate_smfHead( const smfHead * hdr, int hasfits, int hasstate,
                          int *status );

void smf_whiten( double *re, double *im, double df, dim_t nf, size_t box,
                 int complement, int *status );

void smf_write_bolomap( ThrWorkForce *wf, smfArray *res, smfArray *lut,
                        smfArray *qua, smfDIMMData *dat, dim_t msize,
                        const Grp *bolrootgrp, int varmapmethod,
                        const int *lbnd_out, const int *ubnd_out,
                        AstFrameSet *outfset, int *status );

void smf_write_clabels( const smfData* data, int * status );

void smf_write_flagmap( smf_qual_t mask, smfArray *lut, smfArray *qua,
                        smfDIMMData *dat, const Grp *flagrootgrp,
                        size_t contchunk, const int *lbnd_out,
                        const int *ubnd_out, AstFrameSet *outfset,
                        int *status );

void smf_write_itermap( ThrWorkForce *wf, const double *map, const double *mapvar,
                        const smf_qual_t *mapqua, dim_t msize,
                        const Grp *iterrootgrp, size_t contchunk, int iter,
                        const int *lbnd_out, const int *ubnd_out,
                        AstFrameSet *outfset, const smfHead *hdr,
                        const smfArray *qua, int *status );

void smf_write_sampcube( const smfArray *res, const smfArray *lut,
                         const smfArray *qua, const smfDIMMData *dat,
                         const int *hits, const Grp *samprootgrp,
                         size_t contchunk, const int *lbnd, const int *ubnd,
                         int *status );

void smf_write_shortmap( ThrWorkForce *wf, int shortmap, smfArray *res, smfArray *lut,
                         smfArray *qua, smfDIMMData *dat,
                         dim_t msize, const Grp *shortrootgrp, size_t contchunk,
                         int varmapmethod, const int *lbnd_out,
                         const int *ubnd_out, AstFrameSet *outfset,
                         int *status );

void smf_write_smfData ( const smfData *data, const smfData *variance,
                         const char * filename,
                         const Grp * igrp, size_t grpindex,
                         int provid, msglev_t msglev, int single, int * status );

void smf_write_smfFilter( const smfFilter *filt, const char *filename,
                          const Grp * igrp, size_t grpindex,
                          int *status );



/* Prototypes and interfaces for generic functions */
/* ----------------------------------------------- */
/* Each distributed source file in the current directory with type ".cgen"
   is processed at build time to produce a corresponding ".c" file. Each of
   the generated type-specific functions is invoked using a generic interface
   macro which hides the code for selecting the type-specfic function. These
   lines in this header file are generated at build time by the make_h script. */

void smf_average_dataD( const smfData *, dim_t, dim_t, const dim_t, double **, size_t *, int * );
void smf_average_dataF( const smfData *, dim_t, dim_t, const dim_t, float **, size_t *, int * );
void smf_average_dataI( const smfData *, dim_t, dim_t, const dim_t, int **, size_t *, int * );
void smf_average_dataW( const smfData *, dim_t, dim_t, const dim_t, short **, size_t *, int * );
void smf_average_dataUW( const smfData *, dim_t, dim_t, const dim_t, unsigned short **, size_t *, int * );
void smf_average_dataB( const smfData *, dim_t, dim_t, const dim_t, char **, size_t *, int * );
void smf_boxcar1D( double *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_boxcar1F( float *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_boxcar1I( int *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_boxcar1W( short *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_boxcar1UW( unsigned short *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_boxcar1B( char *, const size_t, size_t, smf_qual_t *, smf_qual_t, int * );
void smf_detmaskD( const double *, int, int, const int *, int, const int *, double *, int * );
void smf_detmaskF( const float *, int, int, const int *, int, const int *, float *, int * );
void smf_detmaskI( const int *, int, int, const int *, int, const int *, int *, int * );
void smf_detmaskW( const short *, int, int, const int *, int, const int *, short *, int * );
void smf_detmaskUW( const unsigned short *, int, int, const int *, int, const int *, unsigned short *, int * );
void smf_detmaskB( const char *, int, int, const int *, int, const int *, char *, int * );
void smf_downsamp1D( ThrWorkForce *, const double *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_downsamp1F( ThrWorkForce *, const float *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_downsamp1I( ThrWorkForce *, const int *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_downsamp1W( ThrWorkForce *, const short *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_downsamp1UW( ThrWorkForce *, const unsigned short *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_downsamp1B( ThrWorkForce *, const char *, size_t, int, size_t, void *, size_t, int, size_t, int, int, int, int * );
void smf_reorderD( double *, int, int, int *, int, int *, int, int *, double *, int * );
void smf_reorderF( float *, int, int, int *, int, int *, int, int *, float *, int * );
void smf_reorderI( int *, int, int, int *, int, int *, int, int *, int *, int * );
void smf_reorderW( short *, int, int, int *, int, int *, int, int *, short *, int * );
void smf_reorderUW( unsigned short *, int, int, int *, int, int *, int, int *, unsigned short *, int * );
void smf_reorderB( char *, int, int, int *, int, int *, int, int *, char *, int * );
int* smf_sortD( size_t, const double *, int *, int * );
int* smf_sortF( size_t, const float *, int *, int * );
int* smf_sortI( size_t, const int *, int *, int * );
int* smf_sortW( size_t, const short *, int *, int * );
int* smf_sortUW( size_t, const unsigned short *, int *, int * );
int* smf_sortB( size_t, const char *, int *, int * );
void smf_stats1D( const double *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_stats1F( const float *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_stats1I( const int *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_stats1W( const short *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_stats1UW( const unsigned short *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_stats1B( const char *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, double *, double *, double *, size_t *, int * );
void smf_templateFit1D( double *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_templateFit1F( float *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_templateFit1I( int *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_templateFit1W( short *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_templateFit1UW( unsigned short *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_templateFit1B( char *, smf_qual_t *, const unsigned char *, const int *, smf_qual_t, smf_qual_t, const size_t, const size_t, const double *, int, int, double *, double *, double *, int * );
void smf_tophat1D( double *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_tophat1F( float *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_tophat1I( int *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_tophat1W( short *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_tophat1UW( unsigned short *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_tophat1B( char *, int, int, smf_qual_t *, smf_qual_t, float, int * );
void smf_weightstats1D( const double *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
void smf_weightstats1F( const float *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
void smf_weightstats1I( const int *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
void smf_weightstats1W( const short *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
void smf_weightstats1UW( const unsigned short *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
void smf_weightstats1B( const char *, size_t, size_t, const smf_qual_t *, size_t, smf_qual_t, const double *, size_t, double *, double *, size_t *, int * );
#endif
