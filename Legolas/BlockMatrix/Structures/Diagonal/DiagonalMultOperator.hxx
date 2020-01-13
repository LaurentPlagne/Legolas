#ifndef __LEGOLAS_DIAGONALMULTOPERATOR_HXX__
#define __LEGOLAS_DIAGONALMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualMultOperator.hxx"

namespace Legolas{
  
  class DiagonalMultOperator : public DiagonalVirtualMultOperator{
  public:
    
    std::string name( void ) const {return "DiagonalMultOperator";}

    void addMult(const DiagonalVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const DiagonalVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);
    
    DiagonalMultOperator( void );
    ~DiagonalMultOperator( void );
    DiagonalMultOperator(const DiagonalMultOperator & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new DiagonalMultOperator(*this);
    }
    
  };
}

#endif
    
