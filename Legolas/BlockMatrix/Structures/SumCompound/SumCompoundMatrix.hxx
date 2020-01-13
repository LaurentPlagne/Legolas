#ifndef __SUMCOMPOUNDMATRIX_HXX__
#define __SUMCOMPOUNDMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{

  class SumCompoundMatrix : public Matrix {
  private:
    Matrix * leftMatrixPtr_;
    Matrix * rightMatrixPtr_;
  public:

    SumCompoundMatrix( void );
    SumCompoundMatrix(Matrix * leftMatrixPtr, Matrix * rightMatrixPtr);
    ~SumCompoundMatrix( void );

    const Matrix & leftMatrix( void ) const ;
    const Matrix & rightMatrix( void ) const ;
    virtual std::string name( void ) const;
       
    virtual void setMultOperatorPtr(VirtualMultOperator * virtualMultOperatorPtr);

    void collectStatistics(Profile & profile) const ;

    virtual int getPrecisionInBytePerRealElement( void ) const;
    
    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const ;

    
  };
  
}

#endif

    
      
      
    
  
  
  
