#ifndef __LEGOLAS_LINEARSOLVE_HXX__
#define __LEGOLAS_LINEARSOLVE_HXX__

#include "Legolas/Algorithm/LinearSolverWorkSpace.hxx"
#include "Legolas/Vector/Assign/AssignMode.hxx"

namespace Legolas{
  
  template <class SPARSE_MATRIX,class VECTOR, class VECTOR_INOUT>
  inline void linearSolve(const SPARSE_MATRIX & A, 
			  const VECTOR & B,
			  VECTOR_INOUT & X,
			  LinearSolverWorkSpace<VECTOR> & ws){
    A.matrixVectorInverse(B,X,ws);
    


    //    typedef typename SPARSE_MATRIX::MatrixVectorInverse::template Engine< EqualAssign > Operator;
    //    Operator::apply(A,B,X);
  }

  inline void linearSolve(const double & A, 
			  const double & B,
			  double & X,
			  LinearSolverWorkSpace<double> & ws){
    X=B/A;
  }
  inline void linearSolve(const float & A, 
			  const float & B,
			  float & X,
			  LinearSolverWorkSpace<float> & ws){
    X=B/A;
  }
  
}

#endif
