#ifndef __DIAGONALSCALARSOLVER_HXX__
#define __DIAGONALSCALARSOLVER_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixSolver.hxx"

namespace Legolas{

  class DiagonalScalarSolver{
  public:
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixSolver<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      virtual inline std::string name( void ) const { return "DiagonalScalarSolver" ;}
      
      virtual VirtualSolver * clone( void ) const {
	return new Engine(*this);
      }

      void solve(const SCALAR_MATRIX & A, const V1D & B, V1D & X){

	const int n=A.nrows();

	for (int i=0 ; i < n ; i++ ){

	  X[i]=B[i]/A.diagonalGetElement(i);
	}
      }
    };
  };
}

#endif
