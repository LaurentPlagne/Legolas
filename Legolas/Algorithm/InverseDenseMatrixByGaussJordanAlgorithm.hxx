/**
 * project  DESCARTES
 *
 * @file     InverseDenseMatrixByGaussJordanAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   November 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_INVERSEDENSEMATRIXBYGAUSSJORDANALGORITHM_HXX__
#define __LEGOLAS_INVERSEDENSEMATRIXBYGAUSSJORDANALGORITHM_HXX__

namespace Legolas{

  template<class T>
  inline void SWAP(T &a, T &b)
  {T dum=a; a=b; b=dum;}


  // This algorithm computes the inverse matrix Am1 of a dense square matrix A of float
  // The algorithm is adapted from Numerical Recipes in C++ Second Edition
  // Chapter 2 page 42.
  // On input a matrix A is given
  // On output A contains the inverse matrix

#include <vector>

  class InverseDenseMatrixByGaussJordanAlgorithm{

  public :

    template <class DENSE_SQUARE_MATRIX>
    static inline void apply(DENSE_SQUARE_MATRIX & A)
    {
    
      typedef DENSE_SQUARE_MATRIX         Matrix;
      typedef typename Matrix::RealType   RealType;
      typedef std::vector<int>            VectorInt;
    
      typename Matrix::Copy B(A); // workspace
    
      int i,icol,irow,j,k,l,ll;
      RealType big,dum,pivinv;

      icol=0 ; irow=0;
      int n=A.nrows();
      int m=B.ncols();

      vector<int> indxc(n),indxr(n),ipiv(n);

      for (j=0;j<n;j++) ipiv[j]=0;
      for (i=0;i<n;i++) {
	big=0.0;
	for (j=0;j<n;j++)
	  if (ipiv[j] != 1)
	    for (k=0;k<n;k++) {
	      if (ipiv[k] == 0) {
		if (fabs(A(j,k)) >= big) {
		  big=fabs(A(j,k));
		  irow=j;
		  icol=k;
		}
	      }
	    }
	++(ipiv[icol]);
	if (irow != icol) {
	  for (l=0;l<n;l++) SWAP(A(irow,l),A(icol,l));
	  for (l=0;l<m;l++) SWAP(B(irow,l),B(icol,l));
	}
	indxr[i]=irow;
	indxc[i]=icol;
	if (A(icol,icol) == 0.0) cout << "gaussj: Singular Matrix" << endl;
	pivinv=1.0/A(icol,icol);
	A(icol,icol)=1.0;
	for (l=0;l<n;l++) A(icol,l) *= pivinv;
	for (l=0;l<m;l++) B(icol,l) *= pivinv;
	for (ll=0;ll<n;ll++)
	  if (ll != icol) {
	    dum=A(ll,icol);
	    A(ll,icol)=0.0;
	    for (l=0;l<n;l++) A(ll,l) -= A(icol,l)*dum;
	    for (l=0;l<m;l++) B(ll,l) -= B(icol,l)*dum;
	  }
      }
      for (l=n-1;l>=0;l--) {
	if (indxr[l] != indxc[l])
	  for (k=0;k<n;k++)
	    SWAP(A(k,indxr[l]),A(k,indxc[l]));
      }
    }
  
  };



}

#endif	
