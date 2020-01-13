/**
 * project  DESCARTES
 *
 * @file     ComputeLinearSystemResidual.hxx
 *
 * @author Laurent PLAGNE
 * @date   january 2006
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */

#ifndef __LEGOLAS_COMPUTELINEARSYSTEMRESIDUAL_HXX__
#define __LEGOLAS_COMPUTELINEARSYSTEMRESIDUAL_HXX__


// Compute and fetch ||AX-B|| and ||AX-B||/||B||

namespace Legolas{

  struct ComputeLinearSystemResidual{
  
    template <class MATRIX,class VECTOR_X, class VECTOR_B > 
    static inline void apply(const MATRIX & A, const VECTOR_X & X,const VECTOR_B & B, bool isTransposed=false){

#ifdef GLASS_COMPUTE_RESIDUAL
    
      typedef typename VECTOR_B::RealType RealType;

      VECTOR_B AX(B);
    
      if (!isTransposed){
	AX=A*X;
      }
      else{
	AX=transpose(A)*X;
      }
    
      AX-=B;

      const RealType residual=dot(AX,AX);
      const RealType normB=dot(B,B);
      const RealType epsilon=1.e-10;
      const int sizeB=B.size();
    
      if (normB>epsilon){
	INFOS("sizeB="<<sizeB<<"  ||AX-B||="<<residual<<"  ||AX-B||/||B||="<<residual/normB);
      }
      else{
	INFOS("sizeB="<<sizeB<<"  ||AX-B||="<<residual);
      }

#endif //GLASS_COMPUTE_RESIDUAL
    
    }
    
  };

}

#endif


    
  
