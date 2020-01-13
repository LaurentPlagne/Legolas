#ifndef __LEGOLAS_NOINVERSE_HXX__
#define __LEGOLAS_NOINVERSE_HXX__

#include "UTILITES.hxx"
#include "Legolas/Algorithm/LinearSolver.hxx"

namespace Legolas{

  class NoInverse{
  public :

    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
      typedef LinearSolver<TA,TX,TB> Base;
    public:
      
      Engine():LinearSolver<TA,TX,TB>(){
      }

      Engine(const TA & A, TX & X, const TB & B):LinearSolver<TA,TX,TB>(A,X,B){
	MESSAGE("NoInverse::Engine Ctor");
      }

      Engine(const TA & A, TX & X, const TB & B,const typename Base::VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){
	MESSAGE("NoInverse::Engine Ctor");
      }

      
      void solve(const TA & A, TX & X, const TB & B){
	INFOS("Not Implemented");
	throw std::runtime_error("Not implemented");
      }
      void transposeSolve(const TA & A, TX & X, const TB & B){
 	INFOS("not yet implemented");
 	throw std::runtime_error("Not implemented");
      }
    };
  };

}

#endif
