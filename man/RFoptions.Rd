\name{RFoptions}
\alias{RFoptions}
\title{Setting control arguments}
\description{
 \command{\link{RFoptions}} sets and returns control arguments for the analysis
 and the simulation of random fields
}
\usage{
 RFoptions(..., no.readonly = TRUE)
}
\arguments{
 \item{...}{arguments in \code{tag = value} form, or a list of tagged
 values.}
 \item{no.readonly}{If \command{\link{RFoptions}} is called without
   argument then all arguments are returned in a list. If
   \code{no.readonly=TRUE} then only rewritable arguments are returned.
   Currenlty all arguments are rewritable. So the list is empty.
 }
}
\details{
  The subsections below comment on\cr
  \bold{1. \code{general}: General options}\cr
  \bold{2. \code{solve}: Options for solving linear systems}\cr

 
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% \bold{16. Options for RFloglikelihood}\cr
% 
% "auto", "full", "composite", "selection"




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\bold{1. General options}
\describe{
  \item{asList}{logical. Lists of arguments are treated slightly
    different from non-lists. If \code{asList=FALSE} they are treated the
    same way as non-lists. This options being set to \code{FALSE} after
    calling \command{RFoptions} it should be set as first element of a list.
    
    Default: \code{TRUE}
  }

  \item{\code{cores}}{
    Number of cores for multicore algorithms;
    currently only used for the Cholesky decomposition.

    Default : 1
  }

  \item{\code{cPrintlevel}}{
    \code{cPrintlevel} is automatically set to \code{printlevel}
    when \code{printlevel} is changed.
    Standard users will never use a value higher than 3.
    
    0 : no messages\cr
    1 : messages and warnings when the user's input looks odd\cr
    2 : messages (and internal errors) documenting the choice of the
    simulation method\cr
    3 : further user relevant informations\cr
    4 : information on recursive function calls\cr
    5 : function flow information of central functions \cr
    6 : errors that are internally treated\cr
    7 : details on building up the covariance structure\cr
    8 : details on taking the square root of the covariance matrix\cr
    9 : details on intermediate calculations\cr
    10 : further details on intermediate calculations\cr
    
    Note that \code{printlevel} works
    on the R level whereas \code{cPrintlevel} works on the C level.
    
    Default: 1 \cr % [also  do].\cr
  }

  \item{\code{printlevel}}{If \code{printlevel}\eqn{\le0}{<=0}
    there is not any output on the screen. The
    higher the number the more tracing information is given.
    Standard users will never use a value higher than 3.
   
    0 : no messages\cr
    1 : important (error) messages and warnings\cr
    2 : less important messages\cr
    3 : details, but still for the user\cr
    4 : recursive call tracing\cr
    5 : function flow information of large functions\cr
    6 : errors that are internally treated\cr
    7 : details on intermediate calculations\cr
    8 : further details on intermediate calculations\cr
    
    Default: 1 %[also do].\cr
  }
     
  \item{seed}{integer (currently only used by the package RandomFields).
    If \code{NULL} or \code{NA}
    \command{\link{set.seed}} is \bold{not} called.
    Otherwise, \code{\link[base]{set.seed}(seed)} is set
    before any simulations are performed.
    
    If the argument is set locally, i.e., within a function,
    it has the usual local effect. If it is set globally, i.e. by
    \command{RFoptions} the \code{seed} is fixed
    for \bold{all subsequent} calls. 
    
    If the number of simulations \code{n} is greater than one
    and if \code{RFoptions(seed=seed)} is set, the \eqn{i}th
    simulation is started with the seed \sQuote{\code{seed}\eqn{+i-1}}.
    %   The function \code{set.seed} should not be used in case \code{n}
    %    is greater than 1.
    %
    %Vgle!   
    %set.seed(5)
    %RFsimulate(RPschlather(RMmatern(nu=2), xi=1, mu=1, s=1), x, grid=F, n=5)@data
    %set.seed(5)
    %RFsimulate(RPschlather(RMmatern(nu=2.01), xi=1, mu=1, s=1), x,grid=F,n=5)@data
    %RFoptions(cPr=3, seed=5)
    %RFsimulate(RPschlather(RMmatern(nu=2), xi=1, mu=1, s=1), x, grid=F, n=5)@data
    %RFsimulate(RPschlather(RMmatern(nu=2.01), xi=1, mu=1, s=1), x, grid=F,n=5)@data

  }
  
  \item{\code{skipchecks}}{logical.
    If \code{TRUE}, several checks whether the given parameter values
    and the dimension are within the allowed range is skipped.
    Do not change the value of this variable except you really
    know what you do.
    
    Default: \code{FALSE} $ %[also do]. 
  }

}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\bold{2. \code{solve}: Options for solving linear systems}


\describe{ % to do
  \item{\code{max_chol}}{integer. Maximum number of rows of a matrix in
  a Cholesky decomposition

  Default: \eqn{8192}
  }
  \item{\code{max_svn}}{integer. Maximum number of rows of a matrix in
  a svd decomposition

    Default: \eqn{6555}
  }
 
  \item{\code{solve_method}}{
    vector of at most 3 integers that gives the sequence of methods
    in order to inverse a matrix or to calculate its square root:
    \code{"Cholesky"}, \code{"SVD"}, \code{"Sparse"},
    \code{"method undefined"}. In the latter case, the algorithm decides
    which method might suit best.

    Note that if \code{use_spam} is not \code{false} the algorithm
    checks whether a sparse matrix algorithm should be used and which is
    then tried first.

    Default: \code{"method undefined"}.
  }
  \item{\code{spam_factor}}{
    integer. See argument \code{spam_sample_n}.

    Default: 4294967
  }

  \item{\code{spam_min_n}}{
    integer. Has the matrix 
    
    Default: 400
  }
  \item{\code{spam_min_p}}{
    number in \eqn{(0,1)} giving the proportion of
    zero about which an sparse matrix algorithm is used.

    Default: \code{0.8}
  }
  \item{\code{spam_pivot}}{
    integer. Pivoting algorithm for sparse matrices:
    0:none; 1:MMD, 2:RCM

    See package \code{spam} for details.

    Default: 1
  }
  \item{\code{spam_sample_n}}{
    Whether a matrix is sparse or not is tested by a
    \sQuote{random} sample of size \code{spam_sample_n};
    The selection of the sample is iteratively
    obtained by multiplying the index by \code{spam_factor}
    modulo the size of the matrix.

    Default: 500. 
  }
  \item{\code{spam_tol}}{
    largest absolute value being considered as zero.
    Default: \code{DBL_EPSILON}
  }
  \item{\code{svdtol}}{
    When the svd decomposition is used for calculating the square root of
    a matrix then the absolute componentwise difference between
    this matrix and the square of the square root must be less
    than \code{svdtol}. No check is performed if
    \code{svdtol} is negative.
 
    When the svd decomposition is used for calculating the inverse of
    a matrix then a diagonal value is set to zero if it is less than
    \code{svdtol}.
    
    Default: \code{1e-8}
  }
  \item{\code{use_spam}}{
    Should the package \code{spam} (sparse matrices)
    be used for matrix calculations?
    If \code{TRUE} \pkg{spam} is always used. If \code{FALSE},
    it is never used. If \code{NA} its use is determined by
    the size and the sparsity of the matrix.
    
    Default: \code{NA}. 
  }
}


}

 
 
\value{
 \code{NULL} if any argument is given, and the full list of
 arguments, otherwise.
}

\references{
  \itemize{
    \item General
    \itemize{
      \item
      Schlather, M. (1999) \emph{An introduction to positive definite
	functions and to unconditional simulation of random fields.}
      Technical report ST 99-10, Dept. of Maths and Statistics,
      Lancaster University.

      \item Schlather, M. (2011) Construction of covariance functions and
      unconditional simulation of random fields. In Porcu, E., Montero, J.M.
      and Schlather, M., \emph{Space-Time Processes and Challenges Related
	to Environmental Problems.} New York: Springer.
      % \item Schlather, M. (2002) Models for stationary max-stable
    }

    \item rectangular distribution; \code{eps_zhou}
    \itemize{
      \item
      Oesting, M., Schlather, M. and Zhou, C. (2013) On the Normalized
      Spectral Representation of Max-Stable Processes on a compact set. 
      \emph{arXiv},  \bold{1310.1813}
    }

    \item \code{shape_power}
    \itemize{
      \item
      Ballani, F. and Schlather, M. (2015) In preparation.
    }
  }
}
\author{Martin Schlather, \email{schlather@math.uni-mannheim.de}
 \url{http://ms.math.uni-mannheim.de/de/publications/software}}


\examples{
  n <- 500
  M <- matrix(rnorm(n * n), nc=n)
  M <- M \%*\% t(M)
  unix.time(chol(M))
  unix.time(cholPosDef(M))
  RFoptions(cores = 2)
  unix.time(cholPosDef(M))
}

\keyword{spatial}
