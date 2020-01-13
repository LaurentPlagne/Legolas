/**
 * project  DESCARTES
 *
 * @file     DenseDRMatrixInversionByDLAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSEDRMATRIXINVERSIONBYDLALGORITHM_HXX__
#define __LEGOLAS_DENSEDRMATRIXINVERSIONBYDLALGORITHM_HXX__

// Direct Linear Solver AX=B where A=D+R (D = Diagonal Part).
// The R Matrix is Lower Diagonal allowing a simple direct resolution 

namespace Legolas{



  class DenseDRMatrixInversionByDLAlgorithm{

  public :

    template <class ASSIGN_MODE>
    class Engine{
    public:
      template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const DR_MATRIX & A, 
			       VECTOR & B,
			       VECTOR_INOUT & X)
      {
      
	int size=B.size();
      
	typename DR_MATRIX::DMatrixInterface::ConstView D(A.D());
	typename DR_MATRIX::RMatrixInterface::ConstView R(A.R());
      
      
	for (int i=0 ; i < size ; i++ ){
	
	  for (int j=0 ; j < i ; j++ ){
	  
	    B[i]-=R(i,j)*X[j];
	  
	  }
	
	  X[i]=B[i]/D(i,i);
	
	}
      
	return ;
      }
    
    };


    // Transpose Linear Solver :  transpose(A)X=B where A=D+R (D = Diagonal Part).
    // The R Matrix is Lower Diagonal allowing a simple direct resolution 
    // Transpose(R) is Upper Diagonal and the direct solver loop is reversed (size-1 -> 0)

    class Transpose{
    public:
      template <class ASSIGN_MODE>
      class Engine{
      public:
	template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
	static inline void apply(const DR_MATRIX & A, 
				 VECTOR & B,
				 VECTOR_INOUT & X)
	{
	
	  int size=B.size();
      
	  typename DR_MATRIX::DMatrixInterface::ConstView D(A.D());
	  typename DR_MATRIX::RMatrixInterface::ConstView R(A.R());
      
	
	  for (int i=size-1 ; i > -1 ; i-- ){
	
	    for (int j=i ; j < size ; j++ ){
	  
	      B[i]-=R(j,i)*X[j];
	  
	    }
	
	    X[i]=B[i]/D(i,i);
	
	  }
      
	  return ;
	}
    
      };

    };


    //   template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
    //   static inline void apply(const DR_MATRIX & A, 
    // 			   const VECTOR & B,
    // 			   VECTOR_INOUT & X)
    //   {

    //     int size=B.size();

    //     typename DR_MATRIX::DMatrixInterface::ConstView D(A.D());
    //     typename DR_MATRIX::RMatrixInterface::ConstView R(A.R());
    
    //     typedef typename VECTOR::Element VectorElement;

    //     for (int i=0 ; i < size ; i++ ){

    //       VectorElement s=B[i];

    //       for (int j=0 ; j < i ; j++ ){
	  
    // 	s-=R(i,j)*X[j];
	  
    //       }

    //       X[i]=s/D(i,i);

    //     }
    
    //     return ;
    //   }

    //   template <class DR_MATRIX,class VECTOR, class VECTOR_INOUT > 
    //   static inline void applyFast(const DR_MATRIX & A, 
    // 			       VECTOR & B,
    // 			       VECTOR_INOUT & X)
    //   {

    //     int size=B.size();

    //     typename DR_MATRIX::DMatrixInterface::ConstView D(A.D());
    //     typename DR_MATRIX::RMatrixInterface::ConstView R(A.R());

    
    //     for (int i=0 ; i < size ; i++ ){

    //       for (int j=0 ; j < i ; j++ ){
	  
    // 	B[i]-=R(i,j)*X[j];
	  
    //       }

    //       X[i]=B[i]/D(i,i);

    //     }
    
    //     return ;
    //   }



  

  };

}

#endif	
