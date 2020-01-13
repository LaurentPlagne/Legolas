#ifndef __TRIDIAGONALSCALARGAUSSSEIDELSOLVER_HXX__
#define __TRIDIAGONALSCALARGAUSSSEIDELSOLVER_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixSolver.hxx"

namespace Legolas{

  class TriDiagonalScalarGaussSeidelSolver{
  public:
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixSolver<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      virtual inline std::string name( void ) const { return "TriDiagonalGaussSeidelScalarSolver" ;}
      
      virtual VirtualSolver * clone( void ) const {
	return new Engine(*this);
      }
      
      inline void solve(const SCALAR_MATRIX & A, const V1D & B, V1D & X){

	
	const int n=A.nrows();

	if (n==1){
	  X[0]=B[0]/A.diagonalGetElement(0);
	}
	else{
	  if (n>1){

	    this->iterationControler().initialize(A,X);
	    
	    do {

	      X[0]=(B[0]-A.upperDiagonalGetElement(1)*X[1])/A.diagonalGetElement(0);
	      for (int i=1 ; i < n-1 ; i++ ){
		X[i]=(B[i]-A.lowerDiagonalGetElement(i)*X[i-1]-A.upperDiagonalGetElement(i)*X[i+1])/A.diagonalGetElement(i);
	      }
	      X[n-1]=(B[n-1]-A.lowerDiagonalGetElement(n-1)*X[n-2])/A.diagonalGetElement(n-1);

	    }while(!this->iterationControler().end(X));

	  }
	}
      }
    };
  };
}

#endif
