/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixInversion.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXINVERSION_HXX__
#define __LEGOLAS_DIAGONALMATRIXINVERSION_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"
//#include "Legolas/Algorithm/LinearSolverWorkSpace.hxx"
#include "Legolas/Algorithm/LinearSolver.hxx"

namespace Legolas{

  class DiagonalMatrixInversion{
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
	MESSAGE("NonPreconditionedLinearBiConjugateGradientAlgorithm Ctor");
      }

      Engine(const TA & A, TX & X, const TB & B, const typename Base::VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){
	MESSAGE("NonPreconditionedLinearBiConjugateGradientAlgorithm Ctor");
      }
      
      inline std::string name( void ) const { return "DiagonalMatrixInversion";}

      void solve(const TA & A, TX & X, const TB & B){
	
    	int size=B.size();
	for (int i=0 ; i < size ; i++ ){
	  this->blockSolve(A.diagonalGetElement(i),X[i],B[i],i);
	}

	ComputeLinearSystemResidual::apply(A,X,B);

      }

      void transposeSolve(const TA & A, TX & X, const TB & B){
	
    	int size=B.size();
	for (int i=0 ; i < size ; i++ ){
	  this->blockTransposeSolve(A.diagonalGetElement(i),X[i],B[i],i);
	}
	
	ComputeLinearSystemResidual::apply(A,X,B,true);

      }

    };
  };

 //    template <class ASSIGN_MODE>
//     class Engine{
//     public:

//       template <class SPARSE_MATRIX,class VECTOR, class VECTOR_INOUT>
//       static inline void apply(const SPARSE_MATRIX & A, 
// 			       const VECTOR & B,
// 			       VECTOR_INOUT & X)
//       {
// 	LinearSolverWorkSpace<VECTOR> ws(B);
// 	apply(A,B,X,ws);
//       }

//       template <class DIAGONAL_MATRIX,class VECTOR, class VECTOR_INOUT>
//       static inline void apply(const DIAGONAL_MATRIX & A, 
// 			       const VECTOR & B,
// 			       VECTOR_INOUT & X,
// 			       LinearSolverWorkSpace<VECTOR> & ws)
//       {
    
// 	int size=B.size();
// 	for (int i=0 ; i < size ; i++ ){
// 	  ASSIGN_MODE::apply(X[i],B[i]/A.diagonalGetElement(i));
// 	}

// 	ComputeLinearSystemResidual::apply(A,X,B);

//       }

//     };
//   };


}

#endif	
