#ifndef __LEGOLAS_NOINVERSIONALGORITHM_HXX__
#define __LEGOLAS_NOINVERSIONALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"
#include "X86Timer.hxx"

#include "Legolas/Algorithm/LinearSolver.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"

namespace Legolas{
  
  class NoInversionAlgorithm{
  public :

    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
    public:

      Engine():LinearSolver<TA,TX,TB>(){}
      
      typedef LinearSolver<TA,TX,TB> Base;

      typedef typename Base::VectorPoolPtr   VectorPoolPtr;
      typedef typename Base::VectorProvider  VectorProvider;
      typedef typename Base::ElementProvider ElementProvider;


      Engine(const TA & A, TX & X, const TB & B ):LinearSolver<TA,TX,TB>(A,X,B){}
      Engine(const TA & A, TX & X, const TB & B, const VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){}

      inline void solve(const TA & A, TX & X, const TB & B){
	INFOS("Entering an Invalid Function !!!");
	throw std::runtime_error("Entering an invalid function !!!");
      }
    

      inline void transposeSolve(const TA & A, TX & X, const TB & B){
	INFOS("Entering an Invalid Function !!!");
	throw std::runtime_error("Entering an invalid function !!!");
      }
      
    };
    
  };

}


#endif		
