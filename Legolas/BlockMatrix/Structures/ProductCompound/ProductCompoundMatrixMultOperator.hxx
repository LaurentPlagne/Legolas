#ifndef __LEGOLAS_PRODUCTCOMPOUNDMATRIXMULTOPERATOR_HXX__
#define __LEGOLAS_PRODUCTCOMPOUNDMATRIXMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{
  
  //  class ProductCompoundMatrix;
  
  class ProductCompoundMatrixMultOperator : public VirtualMultOperator{
    
    //    VirtualVector * RXPtr_;

  public:
    
    void addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);

    //    ProductCompoundMatrixMultOperator( void ):VirtualMultOperator(),RXPtr_(0){}
    ProductCompoundMatrixMultOperator( void ):VirtualMultOperator(){}

    //    ProductCompoundMatrixMultOperator(const ProductCompoundMatrixMultOperator & other):VirtualMultOperator(*this),RXPtr_(0){}
    
    virtual ~ProductCompoundMatrixMultOperator( void ){}

    //    virtual ~ProductCompoundMatrixMultOperator( void ){ 
    //      if (RXPtr_){
    //	delete RXPtr_; RXPtr_=0;
    //      }
    //    }

    virtual VirtualMultOperator * clone( void ) const {
      return new ProductCompoundMatrixMultOperator(*this);
    }
  
    inline std::string name( void ) const { return "ProductCompoundMultOperator" ;}
  
  };
  
}

#endif
