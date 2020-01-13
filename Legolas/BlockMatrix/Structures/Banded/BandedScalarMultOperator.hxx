#ifndef __BANDEDSCALARMULTOPERATOR_HXX__
#define __BANDEDSCALARMULTOPERATOR_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/BlockMatrix/ScalarMatrixMultOperator.hxx"


namespace Legolas{

  struct BandedScalarMultOperator{
    template <class SCALAR_MATRIX>
    class Engine : public ScalarMatrixMultOperator<SCALAR_MATRIX>{
      typedef typename SCALAR_MATRIX::RealType RealType;
      typedef Legolas::MultiVector<RealType,1> V1D;
    public:
      
      inline std::string name( void ) const { return "BandedScalarMultOperator" ;}

      virtual VirtualMultOperator * clone( void ) const {
	return new Engine(*this);
      }
      
      //Y+=a*A*X
      void addMult(const SCALAR_MATRIX & A, const double & a, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	for (int i=0 ; i < n ; i++ ){
	  RealType accumulator=0.0;
	  for (int j=std::max(i-A.linf(),0) ; j < std::min(i+A.lsup()+1,n) ; j++ ){
	    accumulator+=A.bandedGetElement(i,j)*X[j];
	  }
	  Y[i]+=a*accumulator;
	}
      }

      //Y=A*X
      void mult(const SCALAR_MATRIX & A, const V1D & X, V1D & Y){
	
	const int n=A.nrows();
	for (int i=0 ; i < n ; i++ ){
	  RealType accumulator=0.0;
	  for (int j=std::max(i-A.linf(),0) ; j < std::min(i+A.lsup()+1,n) ; j++ ){
	    accumulator+=A.bandedGetElement(i,j)*X[j];
	  }
	  Y[i]=accumulator;
	}
      }
    };
  };
}

#endif
    
