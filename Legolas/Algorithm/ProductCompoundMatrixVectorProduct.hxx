/**
 * project  DESCARTES
 *
 * @file     ProductCompoundMatrixVectorProduct.hxx
 *
 * @author Laurent PLAGNE
 * @date   sept 2009
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_PRODUCTCOMPOUNDMATRIXVECTORPRODUCT__HXX__ 
#define __LEGOLAS_PRODUCTCOMPOUNDMATRIXVECTORPRODUCT__HXX__

namespace Legolas{

  // A=L*R => Product :Y=A*X=L*(R*X)

  class ProductCompoundMatrixVectorProduct{
  public :

    template <class ASSIGN_MODE>
    class Engine{
    public: 
      template <class PC_MATRIX,class VECTOR, class VECTOR_INOUT > 
      static inline void apply(const PC_MATRIX & A, 
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
      {
	typedef typename PC_MATRIX::LMatrixInterface LMatrixInterface;
	typedef typename PC_MATRIX::RMatrixInterface RMatrixInterface;
      
	typename LMatrixInterface::ConstView L(A.L());
	typename RMatrixInterface::ConstView R(A.R());

	
	X86Timer chronos;
	chronos.start();
	VECTOR_INOUT RX(R.getRowShape());
	chronos.stop();
	chronos.storeResult("ProductCompound MVP part0");
	RX=0.0;

	chronos.start();
	RX+=R*X;
	chronos.stop();
	chronos.storeResult("ProductCompound MVP part1");
	
	
	chronos.start();
	ASSIGN_MODE::apply(Y,L*RX);
	
	chronos.stop();
	chronos.storeResult("ProductCompound MVP part2");

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
			       const VECTOR & X,
			       VECTOR_INOUT & Y)
	{

	  typedef typename PC_MATRIX::LMatrixInterface LMatrixInterface;
	  typedef typename PC_MATRIX::RMatrixInterface RMatrixInterface;
      
	  typename LMatrixInterface::ConstView L(A.L());
	  typename RMatrixInterface::ConstView R(A.R());

	  X86Timer chronos;

	  chronos.start();
	  VECTOR_INOUT LTX(L.getColShape());
	  LTX=0.0;

	  LTX+=transpose(L)*X;
	  chronos.stop();
	  chronos.storeResult("ProductCompound MVP part1 (transposed)");

	  chronos.start();
	  ASSIGN_MODE::apply(Y,transpose(R)*LTX);
	  chronos.stop();
	  chronos.storeResult("ProductCompound MVP part2 (transposed)");
	  
	  return ;
	  
	}
      };
    };

  };
} 
#endif		/* #if ! defined( __ProductCompoundMatrixVectorProduct__HXX__ ) */
