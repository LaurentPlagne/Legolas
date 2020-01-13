#ifndef __LEGOLAS_MULTONLYSCALARMATRIXINTERFACE_HXX__
#define __LEGOLAS_MULTONLYSCALARMATRIXINTERFACE_HXX__

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
  
  template <class SCALAR>
  class MultOnlyScalarMatrixInterface: public Matrix{
  public:
    typedef SCALAR RealType;
  private:
    typedef Legolas::MultiVector<RealType,1> V1D;
    
  public:

    typedef Legolas::MatrixShape<1> Shape;

    inline void initializeOperatorPtr( void ){
      //      typedef typename MULT_OPERATOR::template Engine<MultOnlyScalarMatrixInterface> MultOperator;
      //      typedef typename MATRIX_OPTIONS::Solver::template Engine<MultOnlyScalarMatrixInterface> Solver;
      //      this->setVirtualSolverPtr(new Solver());
      //      typedef typename MATRIX_OPTIONS::MultOperator:: template Engine<MultOnlyScalarMatrixInterface> MultOperator;
      this->setVirtualSolverPtr(new ConjugateGradientSolver());
      //      this->setVirtualMultOperatorPtr(new MultOperator());
    }      
    
    inline MultOnlyScalarMatrixInterface(const MultOnlyScalarMatrixInterface & source):Matrix(source){
      this->setVirtualSolverPtr(source.virtualSolver()->clone());
      this->setVirtualSolverPtr(source.virtualMultOperatorPtr()->clone());
    }
    inline MultOnlyScalarMatrixInterface(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){
      //      INFOS("ICI");
      this->initializeOperatorPtr();
    }
    
    
    void iterateOverElements(Legolas::MatrixStream & matrixStream) const{

      INFOS("*************************************************************************");
      Legolas::MultOnlyIterateOverElements::apply(this,matrixStream);
      



      // Legolas::SparseScalarMatrix<SCALAR> sparseMatrix(this->getVirtualMatrixShape());
      // V1D ej(this->ncols());
      // V1D Y(this->nrows());
      
      // const RealType tinyValue=1.e-10;
      
      // for (int j=0 ; j<this->ncols();j++){
      // 	ej[j]=1.0;
      // 	this->mult(ej,Y);
      // 	for (int i=0 ; i< this->nrows() ; i++){
      // 	  if (std::abs(Y[i])>tinyValue) sparseMatrix.setSparseElement(i,j,Y[i]);
      // 	}
      // 	ej[j]=0.0;
      // }

      // sparseMatrix.iterateOverElements(matrixStream);
      
      // INFOS("iterateOverElements");
    }
    

    
    void collectStatistics(Profile & profile) const{
      INFOS("collectStatistics");
    }

    int getPrecisionInBytePerRealElement( void ) const{
      return sizeof(RealType);
    }
  


  };
  
}

#endif

    
    
    
    

  
