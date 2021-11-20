/*
Authors
Martin Schlather, schlather@math.uni-mannheim.de

main library for unconditional simulation of random fields

 Copyright (C) 2021 -- 2021 Martin Schlather

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
  
#ifndef rfutils_options_H
#define rfutils_options_H 1

//#include "AutoRandomFieldsUtilsLocal.h"
#include "RFU.h"

#define basicN 14
// IMPORTANT: all names of basic must be have least 3 letters !!!
#define BASIC_WARN_OPTION 9
typedef // benoetigt
struct basic_options {
  int  
  Rprintlevel, Cprintlevel, seed, cores, warn_unknown_option,
    LaMaxTakeIntern;
  install_modes install;
  bool skipchecks, asList /* hidden:verbose */, kahanCorrection, helpinfo,
    warn_parallel, installPackages;
  la_modes la_usr, la_mode;
} basic_options;
#define basic_START \
  { R_PRINTLEVEL, C_PRINTLEVEL,		\
   NA_INTEGER, INITCORES, WARN_UNKNOWN_OPTION_ALL,	\
      MAXINT, \
      Iask,							\
      false, true, false, true, true, false, 			\
      LA_AUTO, LA_R /*LA_R  */					\
      }


#define SOLVE_SVD_TOL 3
#define solveN 21
typedef // benoetigt
struct solve_options {
  usr_bool sparse, pivot_check;
  bool det_as_log, pivot_partialdet, pseudoinverse;
  double spam_tol, spam_min_p[2], svd_tol, eigen2zero, pivot_relerror,
    max_deviation, max_reldeviation;
  InversionMethod Methods[SOLVE_METHODS];
  int spam_min_n[2], spam_sample_n, spam_factor, pivotsparse, max_chol,
    max_svd,
    pivot, // obsolete
     actual_size,
    *pivot_idx, pivot_idx_n,//permutation; phys+logi laenge
    tinysize;
  //  bool tmp_delete;
  pivot_modes actual_pivot,pivot_mode;
} solve_options;
#ifdef SCHLATHERS_MACHINE
#define svd_tol_start 1e-08
#else
#define svd_tol_start 0
#endif
#define solve_START							\
  False, False, true, false, false,					\
    2.220446e-16, {0.8, 0.9}, svd_tol_start, 1e-12, 1e-11,		\
    1e-10, 1e-10,							\
    {NoInversionMethod,  NoFurtherInversionMethod},			\
    {400, 10000}, 500, 4294967, PIVOTSPARSE_MMD, 16384,			\
    10000,  /* never change -- see RFoptions.Rd */			\
    PIVOT_NONE, /* obsolete */						\
    0, NULL, 0, 3,							\
    PIVOT_UNDEFINED, PIVOT_AUTO /* PIVOT_NONE */
    

typedef // benoetigt
struct utilsoption_type{
  basic_options basic;
  solve_options solve;
} utilsoption_type;



#if defined OBSOLETE_RFU && ! defined obsolete_miraculix
#else
#define ADD(ELT) SET_VECTOR_ELT(sublist, k++, ELT)
#define ADDCHAR(ELT) x[0] = ELT; ADD(ScalarString(mkChar(x)))
#endif

//int own_chol_up_to(int size, int maxtime);
//int own_chol_up_to();
void SetLaMode();
void SetLaMode(la_modes);
void solve_DELETE0(solve_storage *x);
void resetInstalled();


#endif
