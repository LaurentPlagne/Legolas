#pragma once

#include "Legolas/BlockMatrix/VirtualSolver.hxx"
#include "Legolas/Vector/Vector.hxx"

namespace Legolas{

  template <class SCALAR_MATRIX,int LEVEL>
  class StaticMatrixSolver : public VirtualSolver{
    typedef SCALAR_MATRIX ScalarMatrix;
    typedef typename ScalarMatrix::RealType RealType;
    typedef Legolas::MultiVector<RealType,LEVEL> VLD;
  public:
    
    virtual void solve(const Matrix & Av, const VirtualVector & Xv, VirtualVector & Yv){
      //      INFOS("ICI");
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const VLD & X=static_cast<const VLD &>(Xv);
      VLD & Y=static_cast<VLD &>(Yv);
      this->solve(A,X,Y);
    }

    virtual void solve(const ScalarMatrix & A, const VLD & X, VLD & Y) =0 ;

    virtual VirtualSolver * clone( void ) const = 0 ;
  };
}
    
    
