#ifndef __LEGOLAS_DIAGONALPARALLELMULTOPERATOR_HXX__
#define __LEGOLAS_DIAGONALPARALLELMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalVirtualMultOperator.hxx"

namespace Legolas{
  
  class DiagonalParallelMultOperator : public DiagonalVirtualMultOperator{
  public:
    
    std::string name( void ) const {return "DiagonalParallelMultOperator";}

    void addMult(const DiagonalVirtualBlockMatrix & A, const double & a ,const VirtualVector & X, VirtualVector & Y);
    void mult(const DiagonalVirtualBlockMatrix & A,const VirtualVector & X, VirtualVector & Y);
    
    DiagonalParallelMultOperator( void );
    ~DiagonalParallelMultOperator( void );
    DiagonalParallelMultOperator(const DiagonalParallelMultOperator & source) ; 

    virtual VirtualMultOperator * clone( void ) const {
      return new DiagonalParallelMultOperator(*this);
    }
    
  };
}

#endif
    
