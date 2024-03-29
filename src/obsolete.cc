/*
 Authors 
 Martin Schlather, schlather@math.uni-mannheim.de


 Copyright (C) 2016 -- 2021 Martin Schlather

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.  
*/

#define RFU_NEED_OBSOLETE 1
#include "Basic_utils.h"
#include "options.h"
#include "extern.h"
#include "zzz_RandomFieldsUtils.h"
#include "xport_import.h"
extern void strcopyN(char *dest, const char *src, int n);


#ifdef __cplusplus
extern "C" {
#endif

  typedef char errorstring_type[MAXERRORSTRING];
  bool RELAX_UNKNOWN_RFOPTION=false; // auf keinen Fall aendern!
  void relaxUnknownRFoption(bool relax){ RELAX_UNKNOWN_RFOPTION = relax; }
  
  void getErrorString(errorstring_type errorstring){
    STRCPY(errorstring, "error occurred in package RandomFieldsUtils");
  }
  
  void setErrorLoc(errorstring_type VARIABLE_IS_NOT_USED errorloc){
  }
  
#define MAXNLIST 7
#define PKGNAMELENGTH (MAXCHAR - 1)
  extern int NList;
  extern int noption_class_list,
    AllprefixN[MAXNLIST],
    *AllallN[MAXNLIST];
  extern  const char  *option_class_list[MAXNLIST],
    **Allprefix[MAXNLIST],
    ***Allall[MAXNLIST];
  extern name_type pkgnames;
  extern setoptions_fctn setoption_fct_list[MAXNLIST][MAXNLIST];
  extern getoptions_fctn getoption_fct_list[MAXNLIST][MAXNLIST];
  extern finalsetoptions_fctn finaloption_fct_list[MAXNLIST];
  extern deleteoptions_fctn deloption_fct_list[MAXNLIST];
  extern bool installed [MAXNLIST];
  extern install_modes min_simd_needs[MAXNLIST],
    min_gpu_needs[MAXNLIST];
  extern Uint simd_infos [MAXNLIST];
  extern bool obsolete_package_in_use;

  setparameterfct setparam[MAXNLIST] = {NULL, NULL, NULL, NULL, NULL};
  getparameterfct getparam[MAXNLIST] = {NULL, NULL, NULL, NULL, NULL};
  finalsetparameterfct finalparam[MAXNLIST] = { NULL, NULL, NULL, NULL, NULL };
  deleteparameterfct delparam[MAXNLIST] = { NULL, NULL, NULL, NULL, NULL };
    
  void attachRFoptions(const char **PKGprefixlist, int N, 
		       const char ***PKGall, int *PKGallN,
		     setparameterfct set, finalsetparameterfct final,
		       getparameterfct get, deleteparameterfct del,
		       int pl_offset, bool basicopt) {
    char pkgname[] = "obsolete package";
    obsolete_package_in_use = true;
    OPTIONS.solve.eigen2zero = 1e-10;
    OPTIONS.installNrun.la_mode = 
      OPTIONS.installNrun.la_usr = LA_INTERN;
     
    for (int ListNr=0; ListNr<NList; ListNr++) {    
      if (AllprefixN[ListNr] == N && 
	STRCMP(Allprefix[ListNr][0], PKGprefixlist[0]) == 0) {
	if (PL > 0) {
	  PRINTF("options starting with prefix '%.50s' have been already attached.",
		 PKGprefixlist[0]);
	}
	return;    
      }
    }
    if (basicopt) option_class_list[noption_class_list++] = PKGprefixlist[0];
    if (NList >= MAXNLIST) BUG;
    strcopyN(pkgnames[NList], pkgname, PKGNAMELENGTH);
    Allprefix[NList] = PKGprefixlist;
    AllprefixN[NList] = N;
    Allall[NList] = PKGall;
    AllallN[NList] = PKGallN;
    
    setoption_fct_list[NList][NList] = NULL;
    getoption_fct_list[NList][NList] = NULL;
    finaloption_fct_list[NList] = NULL;
    deloption_fct_list[NList] = NULL;
    
    setparam[NList] = set;
    finalparam[NList] = final;
    getparam[NList] = get;
    delparam[NList] = del;
    
    min_simd_needs[NList] = min_gpu_needs[NList] = Inone;
     
    NList++;
    PLoffset = pl_offset;
    PL = OPTIONS.basic.Cprintlevel = OPTIONS.basic.Rprintlevel + PLoffset;
    CORES = OPTIONS.basic.cores;
   }
  
  void detachRFoptions(const char **PKGprefixlist, int N) { detachRFUoptions(PKGprefixlist,  N); }

  void getUtilsParam(utilsoption_type **global) { 
    *global = &OPTIONS; // OK!
  }

  
  bool is_positive_definite( double * C, int  dim) { return Is_positive_definite( C, dim, 1);}
  double detPosDef(double * M,  int size) { return DetPosDef( M,  size, 1) ;} 
  int invertMatrix(double * M, int size) { return  InvertMatrix( M, size, 1);}
  double detPosDefsp(double * M, int size, solve_options * sp) { return DetPosDefsp( M, size,  sp, 1) ;}
 int XCinvXdet(double* M, int size, double *X, int X_cols,
	  double * XCinvX, double * det, bool log, solve_storage *PT) { return xCinvXdet( M, size, X, X_cols,XCinvX,  det, log, PT, 1)  ;}
 int XCinvYdet(double* M, int size, bool posdef,
	    double * X, double * Y, int cols,
	    double * XCinvY, double * det, bool log, solve_storage *PT) { return
     xCinvYdet( M, size, posdef, X,  Y, cols,XCinvY,  det, log, PT, 1)   ;}
 int chol(double * MPT, int size) { return cholesky( MPT, size, 1)  ;}
 int solvePosDef(double* M, int size, bool posdef, 
	   double * rhs, int rhs_cols, double * logdet, solve_storage * PT) { return  SolvePosDef( M, size, posdef, rhs, rhs_cols,  logdet,  PT, 1);}
  int solvePosDefSp(double * M, int size, bool posdef,
	   double * rhs, int rhs_cols, double *logdet,
	   solve_storage * Pt, solve_options *sp) { return   SolvePosDefSp( M, size, posdef,rhs, rhs_cols, logdet, Pt, sp, 1);}
  int sqrtPosDefFree(double * M, int size, solve_storage * pt,
	   solve_options * sp) { return  SqrtPosDefFree( M, size,  pt,sp, 1) ;}
 

  
  
#ifdef __cplusplus
}
#endif
