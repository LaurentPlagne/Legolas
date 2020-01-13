/**
 * project  DESCARTES
 *
 * @file     ProductCompoundMatrixInversion.hxx
 *
 * @author Laurent PLAGNE
 * @date   Sept 2009
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D 2009 
 */
#ifndef __LEGOLAS_PRODUCTCOMPOUNDMATRIXINVERSION_HXX__
#define __LEGOLAS_PRODUCTCOMPOUNDMATRIXINVERSION_HXX__

namespace Legolas{

  // A=L*R => Inverse : solve (L*R) X = B => solve Y=B/L then solve X=Y/R (typically usefull for LU Factorization)
  
  class ProductCompoundMatrixInversion{
  public :
  
    template <class PC_MATRIX, class VECTOR_INOUT, class VECTOR>
    class Engine : public LinearSolver<PC_MATRIX,VECTOR_INOUT,VECTOR> {
    private:
      typedef LinearSolver<PC_MATRIX,VECTOR_INOUT,VECTOR> Base;
    public:

      Engine(const PC_MATRIX & A, 
 	     VECTOR_INOUT & X, 
 	     const VECTOR & B):Base(){}

      Engine(const PC_MATRIX & A, 
 	     VECTOR_INOUT & X, 
 	     const VECTOR & B,
	     const typename Base::VectorPoolPtr & vectorPoolPtr):Base(){}
      

      //      Engine(const PC_MATRIX & A, 
// 	     VECTOR_INOUT & X, 
// 	     const VECTOR & B):LinearSolver<PC_MATRIX,VECTOR_INOUT,VECTOR>(A,X,B){}
      
      void transposeSolve(const PC_MATRIX & A, VECTOR_INOUT & X, const VECTOR & B){
	INFOS("not yet implemented");
    throw std::runtime_error("Not yet implemented");
      }

      
      void solve(const PC_MATRIX & A, VECTOR_INOUT & X, const VECTOR & B){
	
	typedef typename PC_MATRIX::LMatrixInterface LMatrixInterface;
	typedef typename PC_MATRIX::RMatrixInterface RMatrixInterface;
	typedef typename RMatrixInterface::Matrix::FilledRows FilledRows;
	
	const typename LMatrixInterface::ConstView L(A.L());
	const typename RMatrixInterface::ConstView R(A.R());
	
	VECTOR_INOUT Y(L.getColShape());
	Legolas::solve(L,Y,B);
	Legolas::solve(R,X,Y);
	//	Y=B/L;
	//	X=Y/R;
	// could probably be replaced by ;
	// X=B/L; X=X/R;

	ComputeLinearSystemResidual::apply(A,X,B);
	
	return ;
      }
    };
    
    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:

	template <class PC_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const PC_MATRIX & A, 
				 const VECTOR & B,
				 VECTOR_INOUT & X)
	{

	  typedef typename PC_MATRIX::LMatrixInterface LMatrixInterface;
	  typedef typename PC_MATRIX::RMatrixInterface RMatrixInterface;
	  typedef typename RMatrixInterface::Matrix::FilledCols FilledCols;
	
	  const typename LMatrixInterface::ConstView L(A.L());
	  const typename RMatrixInterface::ConstView R(A.R());
	  
	  INFOS("not yet implemented !");
	  throw std::runtime_error("Not yet implemented");

	}
      };
    };

  };

}

#endif
