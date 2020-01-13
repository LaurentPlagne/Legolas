#ifndef __DIAGONALSCALARMULTOPERATOR_HXX__
#define __DIAGONALSCALARMULTOPERATOR_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"

namespace Legolas{

  struct DiagonalScalarMultOperator{
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixMultOperator<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      inline std::string name( void ) const { return "DiagonalScalarMultOperator" ;}

      virtual VirtualMultOperator * clone( void ) const {
	return new Engine(*this);
      }
      
      //Y+=a*A*X
      void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	RealType factor(a);
	for (int i=0 ; i < n ; i++ ){
	  Y[i]+=factor*A.diagonalGetElement(i)*X[i];
	}
      }

      //Y=A*X
      void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	for (int i=0 ; i < n ; i++ ){
	  Y[i]=A.diagonalGetElement(i)*X[i];
	}
      }
    };
  };
}

#endif
    
