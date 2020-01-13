#ifndef __SCALARMATRIXSOLVER_HXX__
#define __SCALARMATRIXSOLVER_HXX__

#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/Vector/Vector.hxx"

namespace Legolas{
  
  template <class SCALAR_MATRIX>
  class ScalarMatrixSolver : public MatrixSolver{
    typedef SCALAR_MATRIX ScalarMatrix;
    typedef typename ScalarMatrix::RealType RealType;
    typedef Legolas::MultiVector<RealType,1> V1D;
  public:
    
    virtual void solve(const Matrix & Av, const VirtualVector & Bv, VirtualVector & Xv){
      const ScalarMatrix & A=static_cast<const SCALAR_MATRIX &>(Av);
      const V1D & B=static_cast<const V1D &>(Bv);
      V1D & X=static_cast<V1D &>(Xv);
      this->solve(A,B,X);
    }
    
    virtual void solve(const ScalarMatrix & A, const V1D & B, V1D & X) =0 ;
    virtual VirtualSolver * clone( void ) const = 0 ;
  };
}
#endif
    
    
