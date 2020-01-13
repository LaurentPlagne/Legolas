/**
 * project  DESCARTES
 *
 * @file     EigenDenseMatrixMatrixProduct.hxx
 *
 * @author Laurent PLAGNE
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D 2001-2012
 */
#ifndef __LEGOLAS_EIGENDENSEMATRIXMATRIXPRODUCT_HXX__
#define __LEGOLAS_EIGENDENSEMATRIXMATRIXPRODUCT_HXX__
#include  <vector>

#include "UTILITES.hxx"
#include <Eigen/Core>


namespace Legolas{

  // C=A*B


  class EigenDenseMatrixMatrixProduct{
  public:

    template <class MATRIX>
    static inline void apply(const MATRIX & A , 
			     const MATRIX & B ,
			     MATRIX & C)
    {
      //C mn = Amk * Bkn

      typedef typename MATRIX::RealType RealType;
      
      int M=A.nrows();
      int K=A.ncols();
      int N=B.ncols();

      assert(A.ncols()==B.nrows());
      assert(A.nrows()==C.nrows());
      assert(B.ncols()==C.ncols());
      
      typedef Eigen::Matrix<RealType, Eigen::Dynamic, Eigen::Dynamic > eigen_matrix;

      eigen_matrix Ae,Be,Ce;
      Ae.resize(M,K);
      for (int i=0 ; i<M ; i++)
	for (int j=0 ; j<K ; j++)	
	  Ae(i,j)=A.sparseGetElement(i,j);

      Be.resize(K,N);
      for (int i=0 ; i<K ; i++)
	for (int j=0 ; j<N ; j++)	
	  Be(i,j)=B.sparseGetElement(i,j);

      Ce.resize(M,N);
      Ce=Ae*Be;

      for (int i=0 ; i<M ; i++)
	for (int j=0 ; j<N ; j++)	
	  C.sparseGetElement(i,j)=Ce(i,j);

      
    }
  
  };

}

#endif
