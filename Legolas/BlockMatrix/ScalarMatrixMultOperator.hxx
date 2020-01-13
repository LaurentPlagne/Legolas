#ifndef __SCALARMATRIXMULTOPERATOR_HXX__
#define __SCALARMATRIXMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"
#include "Legolas/Vector/Vector.hxx"

namespace Legolas{
  
  template <class SCALAR_MATRIX>
  class ScalarMatrixMultOperator : public VirtualMultOperator{
    typedef SCALAR_MATRIX ScalarMatrix;
    typedef typename ScalarMatrix::RealType RealType;
    typedef Legolas::MultiVector<RealType,1> V1D;
  public:
    
    virtual void addMult(const Matrix & Av, const double & a, const VirtualVector & Xv, VirtualVector & Yv){
      //      INFOS("ICI");
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const V1D & X=static_cast<const V1D &>(Xv);
      V1D & Y=static_cast<V1D &>(Yv);
      this->addMult(A,a,X,Y);
    }

    virtual void mult(const Matrix & Av, const VirtualVector & Xv, VirtualVector & Yv){
      //      INFOS("ICI");
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const V1D & X=static_cast<const V1D &>(Xv);
      V1D & Y=static_cast<V1D &>(Yv);
      this->mult(A,X,Y);
    }
    
    virtual void addMult(const ScalarMatrix & A, const double & a, const V1D & X, V1D & Y) =0 ;
    virtual void mult(const ScalarMatrix & A, const V1D & X, V1D & Y) =0 ;
    virtual VirtualMultOperator * clone( void ) const = 0 ;
  };
}
#endif
    
    
