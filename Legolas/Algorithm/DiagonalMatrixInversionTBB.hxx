/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixInversionTBB.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXINVERSIONTBB_HXX__
#define __LEGOLAS_DIAGONALMATRIXINVERSIONTBB_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"
//#include "Legolas/Algorithm/LinearSolverWorkSpace.hxx"
#include "Legolas/Algorithm/LinearSolver.hxx"
#include "my_tbb_parallel_for.hxx"

namespace Legolas{

  class DiagonalMatrixInversionTBB{
  public :
  
    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
      typedef LinearSolver<TA,TX,TB> Base;
    public:
      
      Engine():LinearSolver<TA,TX,TB>(){
	INFOS("ON EST ICIIIIIII");
      }

      Engine(const TA & A, TX & X, const TB & B):LinearSolver<TA,TX,TB>(A,X,B){
	MESSAGE("DiagonalMatrixInversionTBB Ctor");
      }

      Engine(const TA & A, TX & X, const TB & B,const typename Base::VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){
	MESSAGE("DiagonalMatrixInversionTBB Ctor");
      }
      
      inline std::string name( void ) const { return "DiagonalMatrixInversionTBB";}


      class InverseFunctor{
      private:
	LinearSolver<TA,TX,TB> & ls_;
	const TA & A_;
        const TB & B_;
	TX & X_;
      public:
	InverseFunctor(LinearSolver<TA,TX,TB> & ls, 
		       const TA & A, 
		       const TB & B, 
		       TX & X):ls_(ls),A_(A),B_(B),X_(X){}

		
	inline void operator()(const my_tbb::blocked_range<int> & r) const {

	  //	  LinearSolver<TA,TX,TB> ls(ls_);

	  for (int i=r.begin() ; i!=r.end() ; i++){
	    ls_.blockSolve(A_.diagonalGetElement(i),X_[i],B_[i],i);
	  }
	}
      };
	

	
      void solve(const TA & A, TX & X, const TB & B){

	//	INFOS("ICCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");

	my_tbb::parallel_for(my_tbb::blocked_range<int>(0,B.size()),InverseFunctor(*this,A,B,X));
	
	ComputeLinearSystemResidual::apply(A,X,B);

      }

      void transposeSolve(const TA & A, TX & X, const TB & B){
	INFOS("not yet implemented");
	throw std::runtime_error("Not yet implemented");

      }

    };
  };



}

#endif	
