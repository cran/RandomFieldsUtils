AC_INIT([RandomFieldsUtils], 1.0)


##################################################
## debugging options
##################################################
#CROSS="arm32"
#CROSS="avx2"
#CROSS="nosimd"
#CROSS="noflags"
#CROSS="FALSE"
#CROSS="TRUE"
#CROSS="NA"
#USE_GPU="try"
#USE_GPU="yes"
#CXX_FLAGS="-march=native"
#CXX_FLAGS="-nonsense"
#USERASKED="TRUE"
#USERASKED="FALSE"
#MEM_IS_ALIGNED="TRUE"
#MEM_IS_ALIGNED="FALSE"

##################################################
## explicit options passed by RandomFieldsUtils
##################################################
## CXX_FLAGS (including omp)
## CROSS
## SIMD_FLAGS  # superset of what is needed and recognized
## USE_GPU
## CUDA_HOME
## USERASKED     
## MEM_IS_ALIGNED


##################################################
## package specific definitions
##################################################
#MY_SSE2
#MY_SSE3
#MY_SSSE3
#MY_SSE41
#MY_AVX
#MY_AVX2
#MY_AVX512F
#MY_MAX_SSE2
#MY_MAX_SSE3
#MY_MAX_SSSE3
#MY_MAX_SSE41
#MY_MAX_AVX
#MY_MAX_AVX2
#MY_MAX_AVX512F
#MY_CU_FILES
#MY_C_FILES

##################################################
## RandomFieldsUtils
##################################################

##CROSS=

MY_SSE2=""
MY_SSE3=""
MY_SSSE3=""
MY_SSE41=""
MY_AVX="avx_fctns.o"
MY_AVX2="avx2_fctns.o"
MY_AVX512F=""
MY_CU_FILES="solve_61.o gpu_info_61.o"
MY_C_FILES="AutoRandomFieldsUtils.o beskf.o brdomain.o extern.o kleinkram.o maths.o options.o RFoptions.o solve.o sort.o sortLong.o utils.o win_linux_aux.o xport_import.o zzz.o gpu_info.o bckslvmodified.o cholmodified.o spamown.o obsolete.o"


##################################################
## general part
##################################################
CXX=`"${R_HOME}/bin/R" CMD config CXX`
AC_PROG_CXX
AC_LANG(C++)
SAVE_CXXFLAGS="$CXX_FLAGS"
MY_CUDA_HOME="${CUDA_HOME}"
if test "x${CROSS}" == x ; then
  CROSS=noflags
elif test "x${CROSS}" == xTRUE || test "x${CROSS}" == xFALSE || test "x${CROSS}" == xNA; then
  CROSS_BOOL=yes
fi 
AC_MSG_NOTICE([value of 'CROSS' is '${CROSS}'.])

if test "x${USERASKED}" == x ; then
  USERASKED_FLAG=""
elif test "x${USERASKED}" == xTRUE ; then
  USERASKED_FLAG="-DUSERASKED=true"
else
  USERASKED_FLAG="-DUSERASKED=false"
fi 

#AC_MSG_NOTICE([value of 'MEM_IS_ALIGNED' is '${MEM_IS_ALIGNED}'.])
if test "x${MEM_IS_ALIGNED}" == x ; then
  MEM_IS_ALIGNED_FLAG="-DMEMisALIGNED=Nan"
elif test "x${MEM_IS_ALIGNED}" == xTRUE ; then
  MEM_IS_ALIGNED_FLAG="-DMEMisALIGNED=True -DMEM_IS_ALIGNED"
else
  MEM_IS_ALIGNED_FLAG="-DMEMisALIGNED=False"
fi 


######################################################################
## availability of run-time checks 
######################################################################
#include <intrin.h>;
#  int main(){int B=1, s[[4]];__cpuid(s, B);} ]])]
CPUID_FLAG=""
AC_MSG_CHECKING([whether __cpuid is available])
AC_COMPILE_IFELSE([AC_LANG_SOURCE(
  [[#]include <intrin.h>
   [#]include <inttypes.h>
   int main(){int B=1, s[[4]]; __cpuid(s, B);} ])],
   [CPUID_FLAG="-DWINCPUID"],
   [])
if test "x${CPUID_FLAG}" != x ; then
  AC_MSG_RESULT([via intrin.h])
else
  AC_MSG_RESULT([no])
  AC_MSG_CHECKING([whether cpuid works under asm])
  AC_COMPILE_IFELSE([AC_LANG_SOURCE(
    [[#]include <stdio.h>	
     [#]include <inttypes.h>
     int main(){unsigned B; uint32_t s[[4]];			
     asm volatile						
     ("cpuid": "=a"(s[[0]]), "=b"(s[[1]]),"=c"(s[[2]]),	
      "=d"(s[[3]]):"a"(B),"c"(0));	} ])],
      [CPUID_FLAG="-DLINUXCPUID"],
    [])
if test "x${CPUID_FLAG}" != x ; then
  AC_MSG_RESULT([yes])
else
AC_COMPILE_IFELSE([AC_LANG_SOURCE(
    [[#]include <cpuid.h>	
     [#]include <inttypes.h>
     int main(){uint32_t a,b,c,d,level=0;
     __cpuid(level, a, b, c, d);
     	} ])],
      [CPUID_FLAG="-DMINGWCPUID"],
    [])
if test "x${CPUID_FLAG}" != x ; then
  AC_MSG_RESULT([via cpuid.h])
else
  AC_MSG_RESULT([no])
fi
fi
fi



######################################################################
## cuda
######################################################################

# Details of CUDA compilation


MY_CUDA_LIBS=""
#MY_CUDA=""
GPU_FLAG=" -DGPU_NEEDS=Inone"
if test "x${MY_CU_FILES}" != x && test "x${CPUID_FLAG}" != "x-DWINCPUID" ; then
GPU_FLAG=" -DGPU_NEEDS=Igpu"
if test "x${USE_GPU}" != x ; then
## both cuda and graphics card available?
  AC_MSG_NOTICE([value of 'USE_GPU' is '${USE_GPU}'.])
  AC_MSG_CHECKING([whether CUDA_HOME is set])
  if test -z "${MY_CUDA_HOME}"; then
    AC_MSG_RESULT([no -- using /usr/local/cuda])
    MY_CUDA_HOME="/usr/local/cuda"
  else
    AC_MSG_RESULT(using CUDA_HOME=${MY_CUDA_HOME})
  fi
  AC_CHECK_FILE([${MY_CUDA_HOME}/bin/nvcc], CUDA_INSTALLED=yes,
  CUDA_INSTALLED=no)
  if test ${CUDA_INSTALLED} == yes ; then
    AC_MSG_NOTICE([${CUDA_INSTALLED}])
    AC_CHECK_FILE([${MY_CUDA_HOME}/lib64/libcublas.so], , 
    AC_MSG_ERROR([this package only works with 64 bit installations of CUDA]))
    GPU_FLAG="-DGPU_NEEDS=Inone -DUSEGPU"
    MY_CUDA_LIBS="-L${MY_CUDA_HOME}/lib64 -lcudart -lcublas -lcusolver -lcusolverMg"
  else
    if test "x${USE_GPU}" == xtry ; then
      MY_CU_FILES=""
    else
      AC_MSG_ERROR([No CUDA installation found, install CUDA or specify CUDA_HOME.])
    fi
  fi
else
  AC_MSG_NOTICE(['USE_GPU' has not been set.])
  MY_CU_FILES=""
fi
fi


######################################################################
### SIMD
######################################################################
#AC_MSG_NOTICE([simdflags ${SIMD_FLAGS}])


if test "x${SIMD_FLAGS}" == x ; then
  AC_MSG_CHECKING([SIMD options for some CC files]) ##
  if test "x${MY_MAX_AVX}" != x || test "x${MY_MAX_AVX2}" != x || test "x${MY_MAX_AVX512F}" != x ; then
    ANY_MAX_AVX=yes
  fi
  if test "x${MY_SSE2}" != x || test "x${MY_MAX_SSE2}" != x || test "x${MY_MAX_SSE3}" != x || test "x${MY_MAX_SSSE3}" != x || test "x${MY_MAX_SSE41}" != x || test "x${ANY_MAX_AVX}" == xyes ; then
    SIMD_FLAGS="sse2 $SIMD_FLAGS"
  fi
  if test "x${MY_SSE3}" != x || test "x${MY_MAX_SSE3}" != x || test "x${MY_MAX_SSSE3}" != x || test "x${MY_MAX_SSE41}" != x  || test "x${ANY_MAX_AVX}" == xyes ; then
    SIMD_FLAGS="sse3 $SIMD_FLAGS"
  fi
  if test "x${MY_SSSE3}" != x || test "x${MY_MAX_SSSE3}" != x || test "x${MY_MAX_SSE41}" != x || test "x${ANY_MAX_AVX}" == xyes ; then
    SIMD_FLAGS="ssse3 $SIMD_FLAGS"
  fi
  if test "x${MY_SSE41}" != x || test "x${MY_MAX_SSE41}" != x || test "x${ANY_MAX_AVX}" == xyes ; then
    SIMD_FLAGS="sse41 $SIMD_FLAGS"
  fi
  if test "x${MY_AVX}" != x || test "x${ANY_MAX_AVX}" == xyes ; then
    SIMD_FLAGS="avx $SIMD_FLAGS"
  fi
  if test "x${MY_AVX2}" != x || test "x${MY_MAX_AVX2}" != x || test "x${MY_MAX_AVX512F}" != x; then
    SIMD_FLAGS="avx2 $SIMD_FLAGS"
  fi
  if test "x${MY_AVX512F}" != x || test "x${MY_MAX_AVX512F}" != x ; then
    SIMD_FLAGS="avx512f $SIMD_FLAGS"
  fi
  AC_MSG_RESULT([${SIMD_FLAGS}])
fi
#AC_MSG_NOTICE([simdflags ${SIMD_FLAGS}])



m4_if([$2],[],[pushdef(prog, [int main(){}])], [pushdef(prog, [$2])])
## which of SIMD_FLAGS are recognized?
if test "x${CROSS_BOOL}" == xyes ; then
  ## sysctl -a | grep "cpu.features:" # for OS X
  AC_CHECK_PROG(LSCPU, [lscpu], yes, no)
  if test "x${LSCPU}" == xyes ; then
    INFO_CPU=`lscpu | grep Flags | tr "[[:upper:]]" "[[:lower:]]"`
  else
    AC_CHECK_PROG(SYSCTL, [sysctl], yes, no)
    if test  "x${SYSCTL}" == xyes ; then
      INFO_CPU=`sysctl -a 2> /dev/null | grep machdep.cpu.features | tr "[[:upper:]]" "[[:lower:]]"`
    fi
  fi
  AC_MSG_CHECKING([which SIMD flags can be recognized easily])
  TMP=${SIMD_FLAGS}
  SIMD_FLAGS=""
  for SET in ${TMP} ; do
    CXXFLAGS="$SAVE_CXXFLAGS -m${SET}" ## name is obligatory
    info=`echo "${INFO_CPU}" | grep " $SET "` 
    if test "x${info}" != x ; then
      AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [SIMD_FLAGS="${SIMD_FLAGS} $SET"], [])
   fi						   
  done
  if test "x${SIMD_FLAGS}" == x ; then
    AC_MSG_RESULT([none])
  else  
    AC_MSG_RESULT([${SIMD_FLAGS}])
    USE_AVX="yes"
  fi
elif test "x${CROSS}" != xnosimd && test "x${CROSS}" != xnoflags ; then
  USE_AVX="yes"
fi

#AC_MSG_NOTICE([simdflags ${SIMD_FLAGS}])


# which counterpart "-mno-xxx" exists?
NOT_EQUAL_OR_HIGHER_FLAG=""
AC_MSG_CHECKING([which downwards controls might be used])
if test "x${CROSS}" == xnoflags ; then
  CXXFLAGS=""
elif test "x${CROSS}" == xarm32 ; then
  CXXFLAGS="-mfpu=neon -funsafe-math-optimizations"
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])],[CROSS_FLAG="${CXXFLAGS}"],[CROSS_FLAG=""])
else
  CXXFLAGS="-mno-sse2"
  AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])],[NOT_SSE2="${CXXFLAGS}"],[CXXFLAGS=""])
fi

if test "x${USE_AVX}" != xyes ; then
  AC_MSG_RESULT([${NOT_SSE2}.])
else 
  if test "x${CROSS}" != xmmx ; then
    DO_SSE2="-msse2"
    CXXFLAGS="-mno-sse3"
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_SSE3="${CXXFLAGS}"],[CXXFLAGS=""])
    if test "x${CROSS}" != xsse2 ; then
      DO_SSE3="-msse3"
      CXXFLAGS="-mno-ssse3"
      AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_SSSE3="${CXXFLAGS}"],[CXXFLAGS=""])
      if test "x${CROSS}" != xsse3 ; then
        DO_SSSE3="-mssse3"
        CXXFLAGS="-mno-sse4.1"
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_SSE41="${CXXFLAGS}"],[CXXFLAGS=""])
        if test "x${CROSS}" != xssse3 ; then
	  DO_SSE41="-msse4.1"
          CXXFLAGS="-mno-avx"
          AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_AVX="${CXXFLAGS}"],[CXXFLAGS=""])
 	  if test "x${CROSS}" != xsse41 ; then
 	    DO_AVX="-mavx"
            CXXFLAGS="-mno-avx2"
            AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_AVX2="${CXXFLAGS}"],[CXXFLAGS=""])
	    if test "x${CROSS}" != xavx ; then
 	      DO_AVX2="-mavx2"
              CXXFLAGS="-mno-avx512f" ## name is obligatory
              AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [NOT_AVX512F="${CXXFLAGS}"],[CXXFLAGS=""])
	      if test "x${CROSS}" != xavx2 ; then
	        DO_AVX512F="-mavx512f"
                if test "x${CROSS_BOOL}" == xyes || test "x${CROSS}" == xavx512f ; then
                  CXXFLAGS=""
                else
                  AC_MSG_ERROR([unrecognized CROSS option '${CROSS}'])
                fi
              fi
	    fi
	  fi
        fi
      fi
    fi
  fi  
  if test "x${CROSS_BOOL}" != xyes ; then ## and USE_AVX=yes
    NOT_EQUAL_OR_HIGHER_FLAG="${CXXFLAGS}"
  fi
  AC_MSG_RESULT([${NOT_AVX512F} ${NOT_AVX2} ${NOT_AVX} ${NOT_SSE41} ${NOT_SSSE3} ${NOT_SSE3} ${NOT_SSE2}.])
  
  for SET in ${SIMD_FLAGS} ; do
    ## set MY_xxx_FLAGS sharp to xxx (and nothing higher,
    ## if supported by compiler)
    CXXFLAGS="$SAVE_CXXFLAGS -m$SET" ## name is obligatory
    flag_test=0
    ## test only necesary if CROSS not in {TRUE, FALSE, nosimd},
    ## but performed also cases except CROSS=nosimd
    AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1], [flag_test=0])
    if test $flag_test == 0 ; then
      AC_MSG_ERROR([compilation failure for '$SAVE_CXXFLAGS -m$SET'])
    else
      if test "${SET}" == sse2 ; then
        SSE2_FLAGS="${DO_SSE2} ${NOT_SSE3}"
      elif test "${SET}" == sse3 ; then
        SSE3_FLAGS="${DO_SSE3} ${NOT_SSSE3}"
      elif test "${SET}" == ssse3 ; then
        SSSE3_FLAGS="${DO_SSSE3} ${NOT_SSE41}"
      elif test "${SET}" == sse41 ; then
        SSE41_FLAGS="${DO_SSE41} ${NOT_AVX}"
      elif test "${SET}" == avx ; then
        AVX_FLAGS="${DO_AVX} ${NOT_AVX2}"
      elif test "${SET}" == avx2 ; then
        AVX2_FLAGS="${DO_AVX2} ${NOT_AVX512F}"
      elif test "${SET}" == avx512f ; then
        AVX512F_FLAGS="${DO_AVX512F} "
      else
        EXOTIC_SIMD_FLAGS="-m${SET} ${EXOTIC_SIMD_FLAGS}" # iteratively called
      fi
    fi
  done
fi
popdef([prog])


## add the list of MY_MAX_xxx files to list of MY_xxx files
if test "x${MY_MAX_AVX512F}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_AVX512F}"
fi  
if test "x${DO_AVX512F}" != x && test "x${MAX_FILES}" != x ; then
   MY_AVX512F="${MAX_FILES} ${MY_AVX512F}"
   MAX_FILES=""
fi

if test "x${MY_MAX_AVX2}" != x ; then
  MAX_FILES="${MY_MAX_AVX2} ${MAX_FILES}"
fi
if test "x${DO_AVX2}" != x && test "x${MAX_FILES}" != x ; then
  MY_AVX2="${MAX_FILES} ${MY_AVX2}"
  MAX_FILES=""
fi

if test "x${MY_MAX_AVX}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_AVX}"
fi
if test "x${DO_AVX}" != x && test "x${MAX_FILES}" != x ; then
  MY_AVX="${MAX_FILES} ${MY_AVX}"
  MAX_FILES=""
fi

if test "x${MY_MAX_SSE41}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_SSE41}"
fi
if test "x${DO_SSE41}" != x && test "x${MAX_FILES}" != x ; then
  MY_SSE41="${MAX_FILES} ${MY_SSE41}"
  MAX_FILES=""
fi

if test "x${MY_MAX_SSSE3}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_SSSE3}"
fi
if test "x${DO_SSSE3}" != x && test "x${MAX_FILES}" != x ; then
  MY_SSSE3="${MAX_FILES} ${MY_SSSE3}"
  MAX_FILES=""
fi

if test "x${MY_MAX_SSE3}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_SSE3}"
fi
if test "x${DO_SSE3}" != x && test "x${MAX_FILES}" != x ; then
  MY_SSE3="${MAX_FILES} ${MY_SSE3}"
  MAX_FILES=""
fi

if test "x${MY_MAX_SSE2}" != x ; then
  MAX_FILES="${MAX_FILES} ${MY_MAX_SSE2}"
fi
if test "x${DO_SSE2}" != x && test "x${MAX_FILES}" != x ; then
  MY_SSE2="${MAX_FILES} ${MY_SSE2}"
  MAX_FILES=""
fi

#AC_MSG_NOTICE([XXXuseavx=${USEAVX} less than ${NOT_EQUAL_OR_HIGHER_FLAG}; server=${CROSS}])


## determine the SIMD upper bound in case of CROSS=TRUE
if test "x${USE_AVX}" != xyes ; then
  if test "x$CROSS" != xFALSE &&  test "x$CROSS" != xnoflags ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_SSE2}"    
  fi
elif test "x$CROSS" == xTRUE ; then
  if test "x${MY_AVX512F}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG=""
  elif test "x${MY_AVX2}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_AVX512F}"
  elif test "x${MY_AVX}" != x ; then
     NOT_EQUAL_OR_HIGHER_FLAG="${NOT_AVX2}"
  elif test "x${MY_SSE41}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_AVX}"
  elif test "x${MY_SSSE3}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_SSE41}"
  elif test "x${MY_SSE3}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_SSSE3}"
  elif test "x${MY_SSE2}" != x ; then
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_SSE3}"
  else  
    NOT_EQUAL_OR_HIGHER_FLAG="${NOT_SSE2}"  
  fi
fi

#AC_MSG_NOTICE([useavx=${USEAVX} less than ${NOT_EQUAL_OR_HIGHER_FLAG}; server=${CROSS}])

## prepare CROSS flags for Makevars.in



if test "x$CROSS" == xTRUE ; then
  CROSS_FLAG="-DREQUIRED_SIMD -DCROSS_CAPACITY=${NOT_EQUAL_OR_HIGHER_FLAG}"
elif test "x$CROSS" == xFALSE ; then
  CROSS_FLAG="-DREQUIRED_SIMD=3"
elif test "x$CROSS" == xNA ; then
  CROSS_FLAG="-DREQUIRED_SIMD=2"
elif test "x$CROSS" == xnoflags ; then
  CROSS_FLAG=""
elif test "x$CROSS" == xarm32 ; then
  CROSS_FLAG="${CROSS_FLAG} -DREQUIRED_SIMD=4" ## higher 3 reservered for ARM
elif test "x$CROSS" != xnosimd ; then
  CROSS_FLAG="-DCROSS_CAPACITY=${CROSS}" 
elif test "x${NOT_SSE2}" == x ; then # && CROSS = nosimd
  CROSS_FLAG="-DREQUIRED_SIMD=0"
else ## CROSS = nosimd, no explicit limitation possible
  CROSS_FLAG="-DREQUIRED_SIMD=1"
fi

OMP="\$(SHLIB_OPENMP_CXXFLAGS)"
MY_PKG_FLAGS="${SAVE_CXXFLAGS} ${CPUID_FLAG} ${GPU_FLAG} ${EXOTIC_SIMD_FLAGS} ${NOT_EQUAL_OR_HIGHER_FLAG} ${CROSS_FLAG} ${MEM_IS_ALIGNED_FLAG} ${USERASKED_FLAG}"
MY_C_FILES="$MY_C_FILES $MY_SSE2 $MY_SSE3 $MY_SSSE3 $MY_SSE41 $MY_AVX $MY_AVX2 $MY_AVX512F $MAX_FILES"
MY_LIB_FLAGS="$LIB_FLAGS ${OMP}"
TMP="PKG_CXXFLAGS = ${MY_PKG_FLAGS} ${OMP} "



#
AC_MSG_NOTICE([default compilation option is ${MY_PKG_FLAGS}])


## prepare MY_xxx for Makevars.in 
if test "x${USE_AVX}" == xyes ; then
  if test "x${MY_SSE2}" != x ; then
    MY_SSE2="${MY_SSE2}: ${TMP} ${SSE2_FLAGS}"
  fi
  if test "x${MY_SSE3}" != x ; then
    MY_SSE3="${MY_SSE3}: ${TMP} ${SSE3_FLAGS}"
  fi
  if test "x${MY_SSSE3}" != x ; then
    MY_SSSE3="${MY_SSSE3}: ${TMP} ${SSSE3_FLAGS}"
  fi
  if test "x${MY_SSE41}" != x ; then
    MY_SSE41="${MY_SSE41}: ${TMP} ${SE41_FLAGS}"
  fi
  if test "x${MY_AVX}" != x ; then
    MY_AVX="${MY_AVX}: ${TMP} ${AVX_FLAGS}"
  fi
  if test "x${MY_AVX2}" != x ; then
    MY_AVX2="${MY_AVX2}: ${TMP} ${AVX2_FLAGS}"
  fi
  if test "x${MY_AVX512F}" != x ; then
    MY_AVX512F="${MY_AVX512F}: ${TMP} ${AVX512F_FLAGS}"
  fi
else
  MY_SSE2=""
  MY_SSE3=""
  MY_SSSE3=""
  MY_SSE41=""
  MY_AVX=""
  MY_AVX2=""
  MY_AVX512F=""
fi


AC_SUBST(MY_PKG_FLAGS) 
AC_SUBST(MY_LIB_FLAGS)
AC_SUBST(MY_CUDA_LIBS)
AC_SUBST(MY_CUDA_HOME)

AC_SUBST(MY_SSE2)
AC_SUBST(MY_SSE3)
AC_SUBST(MY_SSSE3)
AC_SUBST(MY_SSE41)
AC_SUBST(MY_AVX)
AC_SUBST(MY_AVX2)
AC_SUBST(MY_AVX512F)
#AC_SUBST(MY_CUDA)
AC_SUBST(MY_CU_FILES)
AC_SUBST(MY_C_FILES)

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
