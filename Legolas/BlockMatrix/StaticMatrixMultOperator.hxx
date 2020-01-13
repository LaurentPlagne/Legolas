#pragma once

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"
#include "Legolas/Vector/Vector.hxx"

namespace Legolas{

  template <class SCALAR_MATRIX,int LEVEL>
  class StaticMatrixMultOperator : public VirtualMultOperator{
    typedef SCALAR_MATRIX ScalarMatrix;
    typedef typename ScalarMatrix::RealType RealType;
    typedef Legolas::MultiVector<RealType,LEVEL> VLD;
  public:
    
    virtual void addMult(const Matrix & Av, const double & a, const VirtualVector & Xv, VirtualVector & Yv){
      //      INFOS("ICI");
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const VLD & X=static_cast<const VLD &>(Xv);
      VLD & Y=static_cast<VLD &>(Yv);
      this->addMult(A,a,X,Y);
    }

    virtual void mult(const Matrix & Av, const VirtualVector & Xv, VirtualVector & Yv){
      //      INFOS("ICI");
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const VLD & X=static_cast<const VLD &>(Xv);
      VLD & Y=static_cast<VLD &>(Yv);
      this->mult(A,X,Y);
    }
    
    virtual void addMult(const ScalarMatrix & A, const double & a, const VLD & X, VLD & Y) =0 ;
    virtual void mult(const ScalarMatrix & A, const VLD & X, VLD & Y) =0 ;
    virtual VirtualMultOperator * clone( void ) const = 0 ;
  };
}
    
    
