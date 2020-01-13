#pragma once

#include "Legolas/Vector/Shape/VirtualVectorShape.hxx"

namespace Legolas{

  class VirtualMatrixShape{
  public:
    virtual int nrows() const =0 ; 
    virtual int ncols() const =0 ;
    
    virtual VirtualMatrixShape * getSubMatrixShapePtr(int i , int j) const =0;
    virtual VirtualMatrixShape * clone( void ) const =0;
    virtual VirtualMatrixShape * newVirtualMatrixShape(const VirtualVectorShape & l,  const VirtualVectorShape & r) const =0;
    virtual const VirtualVectorShape & getRowShape( void ) const = 0 ;
    virtual const VirtualVectorShape & getColShape( void ) const = 0 ;
  
    virtual ~VirtualMatrixShape( void ) {}; 

    virtual int getLevel( void ) const =0;

    // virtual VirtualMatrixShape * getProductMatrixShape(const VirtualMatrixShape & right) const = 0 ;
      
  };

}
