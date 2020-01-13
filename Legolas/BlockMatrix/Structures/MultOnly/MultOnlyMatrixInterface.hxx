#pragma once

#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/VirtualSolver.hxx"
#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/BlockProfile.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrix.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"
#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyIterateOverElements.hxx"

namespace Legolas{
  
  template <class SCALAR, int LEVEL>
  class MultOnlyMatrixInterface: public Matrix{
  public:
    typedef SCALAR RealType;
  private:
    typedef Legolas::MultiVector<RealType,LEVEL> VLD;
    
  public:

    typedef Legolas::MatrixShape<LEVEL> Shape;

    inline void initializeOperatorPtr( void ){
      this->setVirtualSolverPtr(new ConjugateGradientSolver());
    }      
    
    inline MultOnlyMatrixInterface(const MultOnlyMatrixInterface & source):Matrix(source){
      this->setVirtualSolverPtr(source.virtualSolver()->clone());
      this->setVirtualSolverPtr(source.virtualMultOperatorPtr()->clone());
    }
    inline MultOnlyMatrixInterface(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){
      this->initializeOperatorPtr();
    }
    
    
    void iterateOverElements(Legolas::MatrixStream & matrixStream) const{
      Legolas::MultOnlyIterateOverElements::apply(this,matrixStream);
    }

    
    void collectStatistics(Profile & profile) const{
      INFOS("collectStatistics");
    }
  
    int getPrecisionInBytePerRealElement( void ) const{
      return sizeof(RealType);
    }


  };
  
}

    
    
    
    

  
