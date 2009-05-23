#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vicNl.h>

static char vcid[] = "$Id$";

void initialize_global() {
/*********************************************************************
  initialize_global              Keith Cherkauer       March 1998

  This subroutine initalizes all global parameters before they are 
  called by the model.

  option_strudt:           structure containing all global model options
  options.FULL_ENERGY    = TRUE - compute full energy balance
  options.FROZEN_SOIL    = TRUE - compute frozen soils
  options.DIST_PRCP      = TRUE - use distributed precipitation
  options.INIT_SOIL      = TRUE - use file to initialize soil column
  options.CORRPREC       = TRUE - correct precipitation measurements to
                           account for gauge loss due to wind
  options.MOISTFRACT     = TRUE - output moisture as fractional moisture
                           content instead of total water in mm
  options.BINARY_OUTPUT  = TRUE - create binary output fles
  options.BLOWING        = TRUE - calculate sublimation from blowing snow
  options.Nlayer         = Number of soil layers to use in model (at least
                           3 must be used for the energy balance model
  options.GRID_DECIMAL   = Number of decimal places used in the gridded
                           input and output file names
  options.SNOW_BAND      = Number of elevation bands over which to solve the
                           enery balance snow model
 
  debug_struct:            Structure cantains all debugging flags
  debug.DEBUG            = TRUE - turn on all debugging
  debug.PRT_SOIL         = TRUE - print soil parameter debugging files
  debug.PRT_VEGE         = TRUE - print vegetation parameter debugging files
  debug.PRT_GLOBAL       = TRUE - print global parameter debugging files
  debug.PRT_ATMOS        = TRUE - print forcing data debugging files
  debug.PRT_SNOW         = TRUE - print snow pack debugging files
  debug.PRT_FLUX         = TRUE - print energy flux debugging files
  debug.PRT_VAR          = TRUE - print variable debugging files
  debug.PRT_TEMP         = TRUE - 
  debug.PRT_MOIST        = TRUE - 
  debug.PRT_KAPPA        = TRUE - 
  debug.PRT_BALANCE      = TRUE - 
  debug.PRT_GRID         = TRUE - 
  debug.debug_dir        = 

  param_set.ALBEDO       = FALSE;
  param_set.AIR_TEMP     = FALSE;
  param_set.CRAINF       = FALSE;
  param_set.CSNOWF       = FALSE;
  param_set.DENSITY      = FALSE;
  param_set.LONGWAVE     = FALSE;
  param_set.LSRAINF      = FALSE;
  param_set.LSSNOWF      = FALSE;
  param_set.PREC         = FALSE;
  param_set.PRESSURE     = FALSE;
  param_set.QAIR         = FALSE;
  param_set.RAINF        = FALSE;
  param_set.REL_HUMID    = FALSE;
  param_set.SHORTWAVE    = FALSE;
  param_set.SNOWF        = FALSE;
  param_set.TMAX         = FALSE;
  param_set.TMIN         = FALSE;
  param_set.TSKC         = FALSE;
  param_set.VP           = FALSE;
  param_set.WIND         = FALSE;
  param_set.WIND_E       = FALSE;
  param_set.WIND_N       = FALSE;

  Modifications:
  11-18-02 Added BLOWING_SNOW and PRT_LAKE to global file 
           initialization.                                        KAC
  04-22-03 Removed LAKS and LAKE_PROFILE from pre-processor
           statements that removed them is LAKE_MODEL was FALSE.
           This causes problems with various checks and saves no
           appreciable time of memory.                            KAC
  10-May-04 Initialize ARC_SOIL, COMPRESS, and ARNO_PARAMS to FALSE.
	    Also changed limit on loop over forcing types from
	    hard-coded 17 to variable N_FORCING_TYPES.				TJB
  2005-Mar-24 Modified to handle ALMA-specific global options.			TJB
  2005-Apr-23 Changed ARNO_PARAMS to NIJSSEN2001_BASEFLOW.			TJB
  2005-May-01 Added the ALMA vars CRainf, CSSnowf, LSRainf, and LSSnowf.	TJB
  2005-May-02 Added the ALMA vars Wind_E, Wind_N.				TJB
  2005-11-29  Added SAVE_STATE (option is now set in global param file) 	GCT
  2006-0913  Replaced NIJSSEN2001_BASEFLOW with BASEFLOW option.		TJB/GCT
  2006-Sep-23 Implemented flexible output configuration; added new
              options.Noutfiles and organized the options according
              to function.							TJB
  2006-Dec-29 Added REL_HUMID to the list of supported met input variables.	TJB
  2007-Jan-02 Added ALMA_INPUT option; removed TAIR and PSURF from list
	      of supported met input variables.					TJB
  2007-Jan-15 Added PRT_HEADER option.						TJB
  2007-Apr-25 Added IMPLICIT option.						JCA
  2007-Apr-25 Added EXP_TRANS option.						JCA
  2007-Sep-14 Replaced initialization of param_set.TYPE[i].SUPPLIED with a loop,
	      and added initialization of param_set.TYPE[i].SIGNED and
	      param_set.TYPE[i].multiplier.                                     TJB
  2008-Apr-21 Added SNOW_ALBEDO option.						KAC via TJB
  2008-Apr-21 Added SNOW_DENSITY option.					TJB
  2009-Jan-12 Added COMPUTE_TREELINE option.					TJB
  2009-Jan-16 Added options.AERO_RESIST_CANSNOW.				TJB
  2009-May-17 Added options.MIN_LIQ.						TJB
  2009-May-18 Added options.PLAPSE.						TJB
  2009-May-20 Set default of options.AERO_RESIST_CANSNOW to AR_406_FULL.	TJB
  2009-May-20 Added options.GRND_FLUX_TYPE.					TJB

*********************************************************************/

  extern option_struct options;
#if LINK_DEBUG
  extern debug_struct debug;
#endif
  extern param_set_struct param_set;

  int i, j;

  /** Initialize model option flags **/

  // simulation modes
  options.AboveTreelineVeg      = -1;
  options.AERO_RESIST_CANSNOW   = AR_406_FULL;
  options.BLOWING               = FALSE;
  options.COMPUTE_TREELINE      = FALSE;
  options.CORRPREC              = FALSE;
  options.DIST_PRCP             = FALSE;
  options.EQUAL_AREA            = FALSE;
  options.EXP_TRANS             = FALSE;
  options.FROZEN_SOIL           = FALSE;
  options.FULL_ENERGY           = FALSE;
  options.GRND_FLUX             = FALSE;
  options.GRND_FLUX_TYPE        = GF_FULL;
  options.IMPLICIT              = FALSE;
  options.JULY_TAVG_SUPPLIED    = FALSE;
  options.LAKES                 = FALSE;
  options.LAKE_PROFILE          = FALSE;
  options.MIN_LIQ               = FALSE;
  options.MIN_WIND_SPEED        = 0.0;
  options.Nlayer                = 2;
  options.Nnode                 = 3;
  options.NOFLUX                = FALSE;
  options.PLAPSE                = FALSE;
  options.PREC_EXPT             = 0.6;
  options.QUICK_FLUX            = TRUE;
  options.QUICK_SOLVE           = FALSE;
  options.ROOT_ZONES            = MISSING;
  options.SNOW_ALBEDO           = USACE;
  options.SNOW_BAND             = 1;
  options.SNOW_DENSITY          = DENS_BRAS;
  options.SNOW_STEP             = 1;
  // input options
  options.ARC_SOIL              = FALSE;
  options.BASEFLOW              = ARNO;
  options.GLOBAL_LAI            = FALSE;
  options.GRID_DECIMAL          = 2;
  // state options
  options.BINARY_STATE_FILE     = TRUE;
  options.INIT_STATE            = FALSE;
  options.SAVE_STATE            = FALSE;
  // output options
  options.ALMA_OUTPUT           = FALSE;
  options.BINARY_OUTPUT         = FALSE;
  options.COMPRESS              = FALSE;
  options.MOISTFRACT            = FALSE;
  options.Noutfiles             = 2;
  options.PRT_HEADER            = FALSE;
  options.PRT_SNOW_BAND         = FALSE;

#if LINK_DEBUG 

  /** Initialize debugging control flags **/

  debug.DEBUG       = FALSE;
  debug.PRT_SOIL    = FALSE;
  debug.PRT_VEGE    = FALSE;
  debug.PRT_GLOBAL  = FALSE;
  debug.PRT_ATMOS   = FALSE;
  debug.PRT_SNOW    = FALSE;
  debug.PRT_FLUX    = FALSE;
  debug.PRT_VAR     = FALSE;
  debug.PRT_TEMP    = FALSE;
  debug.PRT_MOIST   = FALSE;
  debug.PRT_LAKE    = FALSE;
  debug.PRT_KAPPA   = FALSE;
  debug.PRT_BALANCE = FALSE;
  debug.PRT_GRID    = FALSE;
  strcpy(debug.debug_dir,"./");

#endif // LINK_DEBUG

  /** Initialize forcing file input controls **/

  for(j=0;j<N_FORCING_TYPES;j++) {
    param_set.TYPE[j].SUPPLIED = FALSE;
    param_set.TYPE[j].SIGNED   = 1;
    param_set.TYPE[j].multiplier = 1;
  }
  for(i=0;i<2;i++) {
    param_set.FORCE_DT[i] = MISSING;
    param_set.N_TYPES[i] = MISSING;
    param_set.FORCE_FORMAT[i] = MISSING;
    for(j=0;j<N_FORCING_TYPES;j++) param_set.FORCE_INDEX[i][j] = MISSING;
  }

}
