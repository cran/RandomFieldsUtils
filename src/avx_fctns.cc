
/* 
 Authors
 Martin Schlather, schlather@math.uni-mannheim.de

 Collection of system specific auxiliary functions

 Copyright (C) 2001 -- 2021 Martin Schlather

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURSE.  See the
GNU General Public License for more details.
g
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "Basic_utils_local.h"
#include <R_ext/Lapack.h>
#include <R_ext/Linpack.h>
#include "RandomFieldsUtils.h"
#include "kleinkram.h"
#include "options.h"
#include "Utils.h"
#include "xport_import.h"
#include "extern.h"


#if defined AVX

ASSERT_SIMD(avx_fctns, avx);

#define algn_general(X)  ((1U + (uintptr_t) (((uintptr_t) X - 1U) / BytesPerBlock)) * BytesPerBlock)
double static inline *algn(double *X) {
  assert(algn_general(X)>=(uintptr_t)X); return (double *) algn_general(X);
}

void colMaxsI256(double *M, Long r, Long c, double *ans) {
  if (r < 16
#if defined AVX
       || !avxAvail
#elif defined  SSE2
      || !sse2Avail
#endif      
      ) {
    for (Long i=0; i<c; i++) {
      double *m = M + r * i,
	dummy = m[0];    
      for (Long j=1; j<r; j++) dummy = MAX(dummy, m[j]);
      ans[i] = dummy;
    }
    return;
  }
#ifdef DO_PARALLEL
#pragma omp parallel for num_threads(CORES) schedule(static)
#endif  
  for (Long i=0; i<c; i++) {
    double dummy,
      *m = M + r * i;
#if defined SSE2
    double *start = algn(m),
      *end = m + r;
    uintptr_t End = (uintptr_t) (end - doubles);
    if ((uintptr_t) start < End) {
      Double * m0 = (Double*) start,
	Dummy = LOADuDOUBLE((double *)m0);
      for (m0++ ; (uintptr_t) m0 < End; m0++) {
	Dummy = MAXDOUBLE(Dummy, (Double) LOADuDOUBLE((double*) m0));
      }
      double *d = (double *) &Dummy;
      dummy = d[0];
      dummy = MAX(dummy, d[1]);
#if defined AVX
      dummy = MAX(dummy, d[2]);
      dummy = MAX(dummy, d[3]);
#endif
      for ( ; m<start; m++) dummy = MAX(dummy, *m);
      m = (double *) m0;
      for ( ; m<end; m++) dummy = MAX(dummy, *m);
    } else {
      dummy = m[0];    
      for (Long j=1; j<r; j++) dummy = MAX(dummy, m[j]);
    }
#else
    dummy = m[0];    
    for (Long j=1; j<r; j++) dummy = MAX(dummy, m[j]);
#endif    
    ans[i] = dummy;
  }
}


#define size 8
#define vectorlen (256 / (size * 8))
#define repet 8
#define atonce (vectorlen * repet)
#define SET_0(NR) sum##NR = _mm256_setzero_pd()
#define P_0(NR) prod##NR = _mm256_setzero_pd()
#define MUL(NR)								\
  STOREuDOUBLE(inout + i + NR * vectorlen,				\
	       ADDDOUBLE(LOADuDOUBLE(inout + i + NR * vectorlen),	\
			 MULTDOUBLE(LOADuDOUBLE(x + i + NR * vectorlen), y)))
			  
#if (7 != repet - 1)
  wrong repet length
#endif
#if (3 != vectorlen - 1)
  wrong vector length
#endif

void avx_linearprodD(double * x, double  Y, Long len, double *inout) {
  Long i = 0,
    lenM = len - (atonce - 1);  
  __m256d y = _mm256_set1_pd(Y);
 
  for (; i < lenM; i += atonce) {
    MUL(0); MUL(1); MUL(2); MUL(3); MUL(4); MUL(5); MUL(6); MUL(7);
    // for (Long k=0; k<atonce; k++) printf("k=%d %10g %10g %10g\n", i+k, inout[i+k], Y, x[i+k]);
  }
 

  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) {
    MUL(0);
  }

  for (; i < len; i++) inout[i] += x[i] * Y;
 }

// ***********************************************************************
// SCALAR
// ***********************************************************************


#define ADDN(NR)							\
  prod##NR = MULTDOUBLE(LOADuDOUBLE(x + i + NR * vectorlen),		\
			LOADuDOUBLE(y + i + NR * vectorlen));		\
  sum##NR = ADDDOUBLE(sum##NR, prod##NR) 
#define SUMUP(NR, nr) sum##NR = ADDDOUBLE(sum##NR, sum##nr)

/*
#ifdef FMA_AVAILABLE
#define ADDF(NR) \
  sum##NR = _mm256_fmadd_pd(LOADuDOUBLE(x + i + NR * vectorlen),\
			    LOADuDOUBLE(y + i + NR * vectorlen), sum##NR)
double avx_scalarprodDfma(double * x, double * y, Long len) {
 Long i = 0,
    lenM = len - (atonce - 1);  
   __m256d SET_0(0);
   double *D  = (double *) &sum0;

  if (len >= atonce) {
    __m256d SET_0(1), SET_0(2), SET_0(3), SET_0(4), SET_0(5), SET_0(6),SET_0(7);
   for (; i < lenM; i += atonce) { 
     ADDF(0); ADDF(1); ADDF(2); ADDF(3); ADDF(4); ADDF(5); ADDF(6); ADDF(7); 
   }
   SUMUP(0, 1); SUMUP(2, 3); SUMUP(4, 5); SUMUP(6, 7);
   SUMUP(0, 2); SUMUP(4, 6); SUMUP(0, 4);
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) { ADDF(0);  }
  double sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += x[i] * y[i];
  return sum;
}
#endif
  */
  
double avx_scalarprodDnearfma(double * x, double * y, Long len) {
  // deutlich genauer zum 0 tarif
  Long i = 0,
     lenM = len - (atonce - 1);  
  __m256d SET_0(0), SET_0(1), SET_0(2), SET_0(3), SET_0(4), SET_0(5),
    SET_0(6),SET_0(7),
    P_0(0), P_0(1), P_0(2), P_0(3), P_0(4), P_0(5), P_0(6), P_0(7);
  double *D  = (double *) &sum0;

  if ( len >= atonce) {
    for (; i < lenM; i += atonce) {
      ADDN(0); ADDN(1); ADDN(2); ADDN(3); ADDN(4); ADDN(5); ADDN(6); ADDN(7); 
    }
    SUMUP(0, 1); SUMUP(2, 3); SUMUP(4, 5); SUMUP(6, 7);
    SUMUP(0, 2); SUMUP(4, 6); SUMUP(0, 4);
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) {  ADDN(0);  }
  double sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += x[i] * y[i];
  
  return sum;
}
 


#define ADDM(NR)							\
  prod0 = MULTDOUBLE(LOADuDOUBLE(x + i + NR * vectorlen),		\
		     LOADuDOUBLE(y + i + NR * vectorlen));		\
  sum0 = ADDDOUBLE(sum0, prod0)


double avx_scalarprodD(double * x, double * y, Long len){
  Long i = 0,
    lenM = len - (atonce - 1);  
  __m256d SET_0(0), P_0(0);
   double *D  = (double *) &sum0;

  if ( len >= atonce) {
  for (; i < lenM; i += atonce) {
    ADDM(0); ADDM(1); ADDM(2); ADDM(3); ADDM(4); ADDM(5); ADDM(6); ADDM(7); 
    }
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) { ADDM(0); } 
  double sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += x[i] * y[i];
  return sum;
}

/*
// #pragma clang optimize on|off.
//double avx_scalarprodDopt(double * x, double * y, Long len)  __attribute__ ((optimize(3)));
//
#pragma GCC push_options
#pragma GCC optimize ("Os") // aggressive
// aggressive
double avx_scalarprodDopt(double * x, double * y, Long len) {
  Long i = 0,
    lenM = len - (atonce - 1);  
  __m256d SET_0(0), P_0(0);
   double *D  = (double *) &sum0;

  if ( len >= atonce) {
  for (; i < lenM; i += atonce) {
    ADDM(0); ADDM(1); ADDM(2); ADDM(3); ADDM(4); ADDM(5); ADDM(6); ADDM(7); 
    }
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) { ADDM(0); } 
  double sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += x[i] * y[i];
  return sum;
}
#pragma GCC pop_options


#define ADDMM(NR)							\
  x0 = LOADuDOUBLE(X0 + i + NR * vectorlen);				\
  y0 = LOADuDOUBLE(Y0 + i + NR * vectorlen);				\
  prod0 = MULTDOUBLE(x0, y0);						\
  sum0 = ADDDOUBLE(sum0, prod0);					\
  x1 = LOADuDOUBLE(X1 + i + NR * vectorlen);				\
  prod0 = MULTDOUBLE(x1, y0);						\
  sum1 = ADDDOUBLE(sum1, prod0);					\
  y1 = LOADuDOUBLE(Y1 + i + NR * vectorlen);				\
  prod0 = MULTDOUBLE(x0, y1);						\
  sum2 = ADDDOUBLE(sum2, prod0);					\
  prod0 = MULTDOUBLE(x1, y1);						\
  sum3 = ADDDOUBLE(sum3, prod0);					\
  x2 = LOADuDOUBLE(X2 + i + NR * vectorlen);				\
  prod0 = MULTDOUBLE(x2, y0);						\
  sum4 = ADDDOUBLE(sum4, prod0);					\
  prod0 = MULTDOUBLE(x2, y1);						\
  sum5 = ADDDOUBLE(sum5, prod0);					\
  y0 = LOADuDOUBLE(Y2 + i + NR * vectorlen);				\
  prod0 = MULTDOUBLE(x0, y0);						\
  sum6 = ADDDOUBLE(sum6, prod0);					\
  prod0 = MULTDOUBLE(x1, y0);						\
  sum7 = ADDDOUBLE(sum7, prod0);					\
  prod0 = MULTDOUBLE(x2, y0);						\
  sum8 = ADDDOUBLE(sum8, prod0);					\


#pragma GCC optimize ("O1") // aggressive
void avx_scalarprodM(double * X0, double * Y0, Long len, double *res) {
  Long i = 0,
    lenM = len - (atonce - 1);  
 __m256d SET_0(0),
    SET_0(1),
    SET_0(2),
    SET_0(3),
    SET_0(4),
    SET_0(5),
    SET_0(6),
    SET_0(7),
    SET_0(8),
    x0, x1, x2,
    y0, y1,// y2, 
    P_0(0);
  double 
    *X1 = X0 + len,
    *Y1 = Y0 + len,
    *X2 = X0 + 2 * len,
    *Y2 = Y0 + 2 * len;

  if ( len >= atonce) {
  for (; i < lenM; i += atonce) {  
    ADDMM(0); ADDMM(1); ADDMM(2); ADDMM(3); ADDMM(4); ADDMM(5); ADDMM(6); ADDMM(7); 
    }
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) { ADDMM(0); }
  double
    *D  = (double *) &sum0,
    sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += X0[i] * Y0[i];
  res[0] = sum;
}

*/

double avx_scalarprodDP(double * x, double * y, Long len) {
  Long i = 0,
     lenM = len - (atonce - 1);  
  __m256d SET_0(0), SET_0(1), P_0(0);
   double *D  = (double *) &sum1;
  if ( len >= atonce) {
    for (; i < lenM; ) {
      Long lenMM = i + vectorlen * (repet * 10L + 1L);
      if (lenMM > lenM) lenMM = lenM;
      sum0 = MULTDOUBLE(LOADuDOUBLE(x + i), LOADuDOUBLE(y + i));
      i += vectorlen;
      for (; i < lenMM; i += atonce) {
	ADDM(0); ADDM(1); ADDM(2); ADDM(3); ADDM(4); ADDM(5); ADDM(6); ADDM(7); 
      }
      sum1 = ADDDOUBLE(sum0, sum1);
    }
  }
  
 lenM = len - vectorlen + 1;
 for (; i < lenM; i += vectorlen) { 
    prod0 = MULTDOUBLE(LOADuDOUBLE(x + i), LOADuDOUBLE(y + i));
    sum1 = ADDDOUBLE(sum1, prod0);
  }
  double sum = D[0] + D[1] + D[2] + D[3];
  for (; i < len; i++) sum += x[i] * y[i];
  return sum;
}


#define ADDK(NR)							\
  prod0 = MULTDOUBLE(LOADuDOUBLE(x + i + NR * vectorlen),		\
		     LOADuDOUBLE(y + i + NR * vectorlen));		\
  sum2 = SUBDOUBLE(prod0, sum1);					\
  sum3 = ADDDOUBLE(sum0, sum2);						\
  sum1 = SUBDOUBLE(sum3, sum0);						\
  sum0 = sum3;								\
  sum1 = SUBDOUBLE(sum1, sum2);
double avx_scalarprodDK(double * x, double * y, Long len) {
  // Kahan
  Long i = 0,
    lenM = len - (atonce - 1);  
  __m256d SET_0(0), // sum
    SET_0(1), 
    SET_0(2), // y
    SET_0(3), // t
    P_0(0),
    P_0(1);
  double *D  = (double *) &sum0;  
  if ( len >= atonce) {
    for (; i < lenM; i += atonce) {
      ADDK(0); ADDK(1); ADDK(2); ADDK(3); ADDK(4); ADDK(5); ADDK(6); ADDK(7);
    }
  }
  lenM = len - vectorlen + 1;
  for (; i < lenM; i += vectorlen) { ADDK(0); }
  sum0 = ADDDOUBLE(sum0, prod1);
  double sum = D[0] + D[1] + D[2] + D[3];
  
  for (; i < len; i++) sum += x[i] * y[i];
  return sum;
}


#else

void colMaxsI(double *M, Long r, Long c, double *ans);
void colMaxsI256(double *M, Long r, Long c, double *ans) {colMaxsI(M, r, c, ans);}

void linearprod2by2(double * x, double  y, Long len, double *inout);
void avx_linearprodD(double * x, double  y, Long len, double *inout) {
  linearprod2by2(x, y, len, inout);}

double scalarprod4by4( double * v1, double * v2, Long N);
double avx_scalarprodDnearfma(double * x, double * y, Long L) {
  return scalarprod4by4(x,y,L);}
double avx_scalarprodD(double * x, double * y, Long L) {
 return scalarprod4by4(x,y,L);}
double avx_scalarprodDP(double * x, double * y, Long L) {
  return scalarprod4by4(x,y,L);}
double avx_scalarprodDK(double * x, double * y, Long L) {
  return scalarprod4by4(x,y,L);}

SIMD_MISS(avx_fctns, avx);

#endif



