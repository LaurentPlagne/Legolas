#ifndef __LEGOLAS_VIRTUALSOLVER_HXX__
#define __LEGOLAS_VIRTUALSOLVER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/MatrixOperator.hxx"
#include "Legolas/BlockMatrix/IterationControler.hxx"

namespace Legolas{
  
  class Matrix;
  class VirtualSolver: public MatrixOperator{
    Legolas::IterationControler iterationControler_;
  public:
    
    VirtualSolver( void );
    virtual void solve(const Matrix & A, const VirtualVector & B, VirtualVector & X) = 0 ;
    virtual void transposeSolve(const Matrix & A, const VirtualVector & B, VirtualVector & X){
      INFOS("Not yet Implemented");
      throw std::runtime_error("Not yet implemented");
    }
    virtual inline ~VirtualSolver( void ){};
    
    virtual VirtualSolver * clone( void ) const = 0 ;
    virtual inline IterationControler & iterationControler( void ) { return iterationControler_;}
    inline int iterationNumber( void ) const { return iterationControler_.iterationNumber() ; }
    inline double relativeDifference( void ) const { return iterationControler_.relativeDifference() ; }
    
    inline const int & maxIteration( void ) const {return iterationControler_.maxIteration();}
    inline int & maxIteration( void ) {return iterationControler_.maxIteration();}
    inline const double & epsilon( void ) const {return iterationControler_.epsilon();}
    inline double & epsilon( void ) {return iterationControler_.epsilon();}
    inline void modifyEpsilon( double epsilon ) const { iterationControler_.modifyEpsilon(epsilon) ; }
    inline const bool & fixedIterationNumber( void ) const {return iterationControler_.fixedIterationNumber();}
    inline bool & fixedIterationNumber( void ) {return iterationControler_.fixedIterationNumber();}
    
  };
}

#endif
    
