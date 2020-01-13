#ifndef __PRODUCTCOMPOUNDMATRIX_HXX__
#define __PRODUCTCOMPOUNDMATRIX_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{

  class ProductCompoundMatrix : public Matrix {
  private:
    Matrix * leftMatrixPtr_;
    Matrix * rightMatrixPtr_;
  public:

    ProductCompoundMatrix( void );
    ProductCompoundMatrix(Matrix * leftMatrixPtr, Matrix * rightMatrixPtr);
    ~ProductCompoundMatrix( void );

    const Matrix & leftMatrix( void ) const ;
    const Matrix & rightMatrix( void ) const ;
    virtual std::string name( void ) const;
       
    // //Y<- Y + a*A*X
    // //Y<- Y + a*L*R*X
    // virtual void addMult(const double & a, const VirtualVector & X, VirtualVector & Y) const;
    // //Y<- A*X
    // //Y<- L*R*X
    // virtual void mult(const VirtualVector & X, VirtualVector & Y) const;

    virtual void setMultOperatorPtr(VirtualMultOperator * virtualMultOperatorPtr);

    void collectStatistics(Profile & profile) const ;

    virtual int getPrecisionInBytePerRealElement( void ) const;

    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const ;
    
  };
  
}

#endif

    
      
      
    
  
  
  
