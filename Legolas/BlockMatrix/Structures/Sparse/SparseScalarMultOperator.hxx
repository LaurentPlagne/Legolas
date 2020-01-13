#ifndef __SPARSESCALARMULTOPERATOR_HXX__
#define __SPARSESCALARMULTOPERATOR_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"

namespace Legolas{

  struct SparseScalarMultOperator{
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixMultOperator<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      virtual inline std::string name( void ) const { return "SparseScalarMultOperator" ;}

      virtual VirtualMultOperator * clone( void ) const {
	return new Engine(*this);
      }
      
      //Y+=a*A*X
      void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	for (int i=0 ; i < n ; i++ ){
	  RealType accumulator=0.0;
	  for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	    accumulator+=A.sparseGetElement(i,j)*X[j];
	  }
	  Y[i]+=a*accumulator;
	}
      }

      //Y+=a*A*X
      inline void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	const int n=A.nrows();
	for (int i=0 ; i < n ; i++ ){
	  RealType accumulator=0.0;
	  for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	    accumulator+=A.sparseGetElement(i,j)*X[j];
	  }
	  Y[i]=accumulator;
	}
      }

      //Y+=a*A*X
      void transposeAddMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	INFOS("not yet implemented");
	throw std::runtime_error("Not yet implemented");
      }
      void transposeMult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	INFOS("not yet implemented");
	throw std::runtime_error("Not yet implemented");
      }
      
    };
  };
}

#endif
    
