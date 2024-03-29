


/*
 Authors 
 Martin Schlather, schlather@math.uni-mannheim.de


 Copyright (C) 2015 -- 2021 Martin Schlather

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


#ifndef RFutils_public_H
#define RFutils_public_H 1

#ifdef __cplusplus
extern "C" {
#endif 
  SEXP scalarR(SEXP x, SEXP y, SEXP mode);
  SEXP struve(SEXP X, SEXP Nu, SEXP Factor_Sign, SEXP Expscaled);
  SEXP besselk_simd(SEXP X, SEXP Nu);
  SEXP I0ML0(SEXP X);
  SEXP gaussr(SEXP X, SEXP Derivative); 
  SEXP WMr(SEXP X, SEXP Nu, SEXP Derivative, SEXP Factor);
  SEXP logWMr(SEXP X, SEXP Nu1, SEXP Nu2, SEXP Factor);

  SEXP SolvePosDefR(SEXP M, SEXP rhs, SEXP logdet);
  SEXP Chol(SEXP M);
  
  SEXP RFoptions(SEXP options);

 void loadoptions();
  void detachoptions();
  
  SEXP sortX(SEXP Data, SEXP From, SEXP To, SEXP NAlast);
  SEXP orderX(SEXP Data, SEXP From, SEXP To, SEXP NAlast);

  SEXP colMaxs(SEXP M);
  SEXP rowMeansX(SEXP M, SEXP Factor);
  SEXP rowProd(SEXP M);
  SEXP chol2mv(SEXP Chol, SEXP N);
  SEXP tcholRHS(SEXP C, SEXP RHS);
  SEXP DivByRow(SEXP M, SEXP V);
  SEXP quadratic(SEXP x, SEXP A);
  SEXP dbinorm(SEXP X, SEXP Sigma);
  SEXP dotXV(SEXP M, SEXP V);
  //  void Ordering(double *d, int *len, int *dim, int *pos);
  SEXP crossprodX(SEXP X, SEXP Y, SEXP mode);

  SEXP DebugCall();
  SEXP getPackagesToBeInstalled(SEXP Force);
  SEXP isGPUavailable();
  SEXP isNEONavailable();
  SEXP isX86_64();
  void setCPUs(int *n);
  void recompilationNeeded(int *n);
  SEXP SIMDmessages(SEXP pkgs);
  SEXP debuggingLevel();
  SEXP gpu_info(SEXP DEVICES);
  SEXP instruction_set(SEXP which,  SEXP pkgs, SEXP used);

#ifdef __cplusplus
}
#endif



#endif 
 
