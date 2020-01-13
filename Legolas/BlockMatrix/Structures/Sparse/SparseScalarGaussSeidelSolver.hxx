#ifndef __SPARSESCALARGAUSSSEIDELSOLVER_HXX__
#define __SPARSESCALARGAUSSSEIDELSOLVER_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixSolver.hxx"

namespace Legolas{
  
  class SparseScalarGaussSeidelSolver{
  public:
    template <class SCALAR_MATRIX>
    class Engine: public ScalarMatrixSolver<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:

      virtual std::string name( void ) const { return "SparseScalarGaussSeidelSolver" ;}
      
      virtual VirtualSolver * clone( void ) const {
	return new Engine(*this);
      }
      
      void solve(const SCALAR_MATRIX & A, const V1D & B, V1D & X){

	this->iterationControler().initialize(A,X);
	
	do {
	  
	  RealType accumulator=0.0;
	  const int n=A.nrows();
	  
	  for (int i=0 ; i < n ; i++ ){
	    
	    accumulator=B[i];
	    
	    for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	      
	      if (i!=j) accumulator-=A.sparseGetElement(i,j)*X[j];
	    }
	    
	    X[i]=accumulator/A.sparseGetElement(i,i);
	    
	  }
	  
	}while(!this->iterationControler().end(X));
      }
    };
  };
}

#endif
    
