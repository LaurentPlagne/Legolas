#ifndef __TRIDIAGONALSCALARDIRECTSOLVER_HXX__
#define __TRIDIAGONALSCALARDIRECTSOLVER_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixSolver.hxx"

namespace Legolas{

  class TriDiagonalScalarDirectSolver{
  public:
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixSolver<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
      V1D S_;
    public:
      
      virtual inline std::string name( void ) const { return "TriDiagonalDirectScalarSolver" ;}
      
      virtual VirtualSolver * clone( void ) const {
	return new Engine(*this);
      }
      
      inline void solve(const SCALAR_MATRIX & A, const V1D & B, V1D & X){

	const int n=A.nrows();
	
	if (n==1){
	  X[0]=B[0]/A.diagonalGetElement(0);
	}
	else{

	  if (S_.size()!=n) S_.resize(n);
	  
	  RealType s=A.diagonalGetElement(0);

	  X[0]=B[0]/s;

	  for (int i=1 ; i < n ; i++ ){
	    
	    S_[i]=A.upperDiagonalGetElement(i-1)/s;
	    //	    s=A.diagonalGetElement(i)-A.lowerDiagonalGetElement(i)*S_[i];
	    s=A.diagonalGetElement(i)-A.lowerDiagonalGetElement(i)*S_[i];
	    X[i]=(B[i]-A.lowerDiagonalGetElement(i)*X[i-1])/s;
	    
	  }
	  
	  for (int i=(n-2) ; i >= 0 ; i-- ){
	    
	    X[i]-=S_[i+1]*X[i+1];
	    
	  }
	}
	
      }
    };
  };
}

#endif
