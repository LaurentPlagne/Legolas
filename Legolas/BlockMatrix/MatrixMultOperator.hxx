#ifndef __LEGOLAS_MATRIXMULTOPERATOR_HXX__
#define __LEGOLAS_MATRIXMULTOPERATOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{
  
  class Matrix;
  class MatrixMultOperator: public VirtualMultOperator{
  public:
    
    MatrixMultOperator(){}
    virtual void addMult(const Matrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y) = 0 ;
    virtual void mult(const Matrix & A, const VirtualVector & X, VirtualVector & Y) = 0 ;
    //    virtual void transposeAddMult(const Matrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y){
    //   INFOS("not yet implemented");
    //   throw std::runtime_error("Not yet implemented");
    // }
    //    virtual void transposeMult(const Matrix & A, const VirtualVector & X, VirtualVector & Y){
    //   INFOS("not yet implemented");
    //   throw std::runtime_error("Not yet implemented");
    // };
    virtual ~MatrixMultOperator( void ){};

    virtual MatrixMultOperator * clone( void ) const = 0 ;
  };
}

#endif
    
