#ifndef __TRIDIAGONALSCALARMULTOPERATOR_HXX__
#define __TRIDIAGONALSCALARMULTOPERATOR_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"

namespace Legolas{

  struct TriDiagonalScalarMultOperator{
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixMultOperator<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      inline std::string name( void ) const { return "TriDiagonalMultOperator" ;}

      virtual VirtualMultOperator * clone( void ) const {
	return new Engine(*this);
      }
      
      //Y+=a*A*X
      inline void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	RealType factor(a);

	if (n>=1) Y[0]+=factor*A.diagonalGetElement(0)*X[0];
	if (n>=2) Y[0]+=factor*A.upperDiagonalGetElement(0)*X[1];

	//	INFOS("X.shape()"<<X.shape());
	//	INFOS("X="<<X);

	for (int i=1 ; i < n-1 ; i++ ){
	  //	  INFOS("i="<<i);
	  Y[i]+=factor*(A.lowerDiagonalGetElement(i)*X[i-1]+A.diagonalGetElement(i)*X[i]+A.upperDiagonalGetElement(i)*X[i+1]);
	}
	
	if (n>=2) Y[n-1]+=factor*(A.lowerDiagonalGetElement(n-1)*X[1]+A.diagonalGetElement(n-1)*X[n-1]);
	
      }


      //Y=A*X
      inline void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	
	const int n=A.nrows();

	if (n>=1) Y[0]=A.diagonalGetElement(0)*X[0];
	if (n>=2) Y[0]+=A.upperDiagonalGetElement(0)*X[1];

	for (int i=1 ; i < n-1 ; i++ ){
	  Y[i]=A.lowerDiagonalGetElement(i)*X[i-1]+A.diagonalGetElement(i)*X[i]+A.upperDiagonalGetElement(i)*X[i+1];
	}
	
	if (n>=2) Y[n-1]=A.lowerDiagonalGetElement(n-1)*X[1]+A.diagonalGetElement(n-1)*X[n-1];
	
      }
    };
  };
}

#endif
    
