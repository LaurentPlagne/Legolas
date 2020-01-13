#ifndef __LEGOLAS_VIRTUALMULTOPERATOR_HXX__
#define __LEGOLAS_VIRTUALMULTOPERATOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/MatrixOperator.hxx"

namespace Legolas{

  class Matrix;
  
  class VirtualMultOperator: public MatrixOperator{
  public:
    
    VirtualMultOperator(){}
    virtual void addMult(const Matrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y) = 0 ;
    virtual void mult(const Matrix & A, const VirtualVector & X, VirtualVector & Y) = 0 ;
    virtual void transposeAddMult(const Matrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }
    
    virtual void transposeMult(const Matrix & A, const VirtualVector & X, VirtualVector & Y){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }
      
    virtual ~VirtualMultOperator( void ){};
    
    virtual VirtualMultOperator * clone( void ) const = 0 ;
  };
}

#endif

