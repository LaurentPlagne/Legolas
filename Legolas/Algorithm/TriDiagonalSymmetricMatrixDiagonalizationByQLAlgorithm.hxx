/**
 * project  DESCARTES
 *
 * @file     TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   November 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_TRIDIAGONALSYMMETRICMATRIXDIAGONALIZATIONBYQLALGORITHM_HXX__
#define __LEGOLAS_TRIDIAGONALSYMMETRICMATRIXDIAGONALIZATIONBYQLALGORITHM_HXX__

#include <vector>
#include "Legolas/Algorithm/pythag.hxx"
#include "Legolas/Algorithm/InverseDenseMatrixByGaussJordanAlgorithm.hxx"

namespace Legolas{

  // This algorithm performs the diagonalization of a symetric tridiagonal matrix of float A
  // $A=M^{-1} D M where M and M^{-1} are dense matrices and D is a diagonal matrix
  // The diagonalization use the QL algorithm
  // The algorithm is adapted from Numerical Recipes in C++ Second Edition
  // Chapter 11 page 485 (tqli routine)
  // On input a matrix A is given
  // On output : M, M^{-1} and D.

  using namespace std;

  template<class T>
  inline const T SIGN(const T &a, const T &b)
  {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

  inline float SIGN(const float &a, const double &b)
  {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}

  inline float SIGN(const double &a, const float &b)
  {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}



  class TriDiagonalSymmetricMatrixDiagonalizationByQLAlgorithm{

  public :

    template <class REAL_SYM_TRIDIAG_MATRIX, class REAL_DENSE_MATRIX, class REAL_DIAGONAL_MATRIX>
    static inline void apply(const REAL_SYM_TRIDIAG_MATRIX & A, 
			     REAL_DENSE_MATRIX & M, 
			     REAL_DENSE_MATRIX & Mm1, 
			     REAL_DIAGONAL_MATRIX & D)
    {
    
      typedef REAL_SYM_TRIDIAG_MATRIX     AMatrix;
      typedef REAL_DENSE_MATRIX           MMatrix;
      typedef REAL_DIAGONAL_MATRIX        DMatrix;
    
      typedef typename AMatrix::RealType   RealType;
    
      typedef std::vector<int>            VectorInt;
      typedef std::vector<RealType>       VectorReal;


      VectorReal e(A.nrows());
    
      for (int i=0 ; i < A.nrows() ; i++) D.diagonalGetElement(i)=A.diagonalGetElement(i);
      for (int i=1 ; i < A.nrows() ; i++) e[i-1]=A.lowerDiagonalGetElement(i);
      e[A.nrows()-1]=0.0;

      // z : on input must contain the identity matrix
      //    typename MMatrix::Copy z(M);
    
      MMatrix z(M);

      for (int i=0 ; i < z.nrows() ; i++){
	for (int j=0 ; j < z.ncols() ; j++){
	  z(i,j)=0.0;
	}
	z(i,i)=1.0;
      }


      // original tqli routine

      int m,l,iter,i,k;
      double s,r,p,g,f,dd,c,b;

      int n=D.nrows();
    
      for (l=0;l<n;l++) {
	iter=0;
	do {
	  for (m=l;m<n-1;m++) {
	    dd=fabs(D.diagonalGetElement(m))+fabs(D.diagonalGetElement(m+1));
	    if (fabs(e[m])+dd == dd) break;
	  }
	  if (m != l) {
	    if (iter++ == 30) cout << "Too many iterations in tqli" << endl;
	    g=(D.diagonalGetElement(l+1)-D.diagonalGetElement(l))/(2.0*e[l]);
	    r=pythag(g,1.0);
	    g=D.diagonalGetElement(m)-D.diagonalGetElement(l)+e[l]/(g+SIGN(r,g));
	    s=c=1.0;
	    p=0.0;
	    for (i=m-1;i>=l;i--) {
	      f=s*e[i];
	      b=c*e[i];
	      e[i+1]=(r=pythag(f,g));
	      if (r == 0.0) {
		D.diagonalGetElement(i+1) -= p;
		e[m]=0.0;
		break;
	      }
	      s=f/r;
	      c=g/r;
	      g=D.diagonalGetElement(i+1)-p;
	      r=(D.diagonalGetElement(i)-g)*s+2.0*c*b;
	      D.diagonalGetElement(i+1)=g+(p=s*r);
	      g=c*r-b;
	      // Next loop can be omitted if eigenvectors not wanted
	      for (k=0;k<n;k++) {
		f=z(k,i+1);
		z(k,i+1)=s*z(k,i)+c*f;
		z(k,i)=c*z(k,i)-s*f;
	      }
	    }
	    if (r == 0.0 && i >= l) continue;
	    D.diagonalGetElement(l) -= p;
	    e[l]=g;
	    e[m]=0.0;
	  }
	} while (m != l);
      }

      // end of original tqli routine

      for (int i=0 ; i < M.nrows() ; i++){
	for (int j=0 ; j < M.ncols() ; j++){
	  M(i,j)=z(j,i);
	}
      }

      Mm1=M;

      MESSAGE("QL ALGO -> GAUSS JORDAN");
      InverseDenseMatrixByGaussJordanAlgorithm::apply(Mm1);
      MESSAGE("GAUSS JORDAN OK");
    }
  };

}

#endif	
