#ifndef __POLYMORPHICSOLVER_HXX__
#define __POLYMORPHICSOLVER_HXX__

#include <memory>
#include "Legolas/Algorithm/AnySolver.hxx"
#include "Legolas/Matrix/PolymorphicMatrix.hxx"
#include "Legolas/Vector/VectorPool.hxx"

namespace Legolas{

  class PolymorphicSolver: public Legolas::AnySolver {
  public:
    PolymorphicSolver(void):solverPtr_(){}
    PolymorphicSolver(std::shared_ptr<Legolas::AnySolver> solverPtr):solverPtr_(solverPtr){}
//     PolymorphicSolver(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B):solverPtr_(A.getSolverPtr(X,B)){
//     }
    
    PolymorphicSolver(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B,void * avp):solverPtr_(A.getSolverPtr(X,B,avp)){
    }

    
    inline void solve(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B) {
      const PolymorphicMatrix * APtr=static_cast<const PolymorphicMatrix *>(&A);
      const PolymorphicMatrix & polyA(*APtr);
      solverPtr_->solve((*polyA.getMatrixPtr()),X,B);
    }

    inline std::string name( void ) const { 
      std::string result("PolymorphicSolver(");
      result+= solverPtr_->name();
      result+=")";
      return result;
    }

    inline void transposeSolve(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B) {
      const PolymorphicMatrix * APtr=static_cast<const PolymorphicMatrix *>(&A);
      const PolymorphicMatrix & polyA(*APtr);
      //      INFOS("ICI");
      solverPtr_->transposeSolve((*polyA.getMatrixPtr()),X,B);
    }
    
  private:
    std::shared_ptr<Legolas::AnySolver>  solverPtr_;
  };
    
}
#endif
