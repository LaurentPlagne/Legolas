#pragma once

#include "Legolas/BlockMatrix/Structures/MultOnly/CreateSparseBlockMatrix.hxx"


namespace Legolas{
  
  struct MultOnlyIterateOverElements{
    
    static inline void apply(const Legolas::Matrix * matrixPtr, Legolas::MatrixStream & matrixStream){

      //      INFOS("*********LAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

      const int sizeOfFloat=matrixPtr->getPrecisionInBytePerRealElement();

      if (sizeOfFloat==4){
      	MultOnlyIterateOverElements::applyForType<float>(matrixPtr,matrixStream);
      }
      else{
      	if (sizeOfFloat==8){
      	  MultOnlyIterateOverElements::applyForType<double>(matrixPtr,matrixStream);
      	}
      	else{
      	  INFOS("Unknown Matrix Scalar Type");
      	  throw std::runtime_error("Unknown Matrix Scalar Type");
      	}
      }
    }

    template <class SCALAR_TYPE>
    static inline void applyForType(const Legolas::Matrix * matrixPtr, Legolas::MatrixStream & matrixStream){
      
      const int matrixLevel=matrixPtr->level();

      switch (matrixLevel) {
      case 1 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,1>(matrixPtr,matrixStream);
	break;
      case 2 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,2>(matrixPtr,matrixStream);
	break;
      case 3 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,3>(matrixPtr,matrixStream);
	break;
      case 4 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,4>(matrixPtr,matrixStream);
	break;
      case 5 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,5>(matrixPtr,matrixStream);
	break;
      case 6 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,6>(matrixPtr,matrixStream);
	break;
      case 7 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,7>(matrixPtr,matrixStream);
	break;
      case 8 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,8>(matrixPtr,matrixStream);
	break;
      case 9 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,9>(matrixPtr,matrixStream);
	break;
      case 10 :
	MultOnlyIterateOverElements::applyForTypeForLevel<SCALAR_TYPE,10>(matrixPtr,matrixStream);
	break;
      default:
	INFOS("This Matrix Level is not Implemented");
	throw std::runtime_error("This Matrix Level is not Implemented");
	break;
      }
    }


    template <class SCALAR_TYPE, int LEVEL>
    static inline void applyForTypeForLevel(const Legolas::Matrix * matrixPtr, Legolas::MatrixStream & matrixStream){
      //      INFOS("ICI");
      //      exit(-1);

      typedef SCALAR_TYPE RealType;

      //SparseScalarMatrix Construction:
      // 1D Shape creation

      const Legolas::MatrixShape<LEVEL> & msl=static_cast<const Legolas::MatrixShape<LEVEL> &>(matrixPtr->getVirtualMatrixShape());
      
      //      const Legolas::VirtualMatrixShape & msl=matrixPtr->getVirtualMatrixShape();
	
      typename Legolas::MultiVector<RealType,1>::Shape rowShapeL1(msl.getRowShape().flatSize());
      typename Legolas::MultiVector<RealType,1>::Shape colShapeL1(msl.getColShape().flatSize());

      Legolas::MatrixShape<1> ms1(rowShapeL1,colShapeL1);
	
      Legolas::SparseScalarMatrix<RealType> sparseMatrix(ms1);
	
      Legolas::MultiVector<RealType,LEVEL> elj(msl.getColShape());
      Legolas::MultiVector<RealType,LEVEL> Yl(msl.getRowShape());
	
      Legolas::MultiVector<RealType,1> ej(colShapeL1,elj.realDataPtr());
      Legolas::MultiVector<RealType,1> Y(rowShapeL1,Yl.realDataPtr());
	
      const RealType tinyValue=1.e-10;

      ej=0.0;
	
      for (size_t j=0 ; j<ej.size();j++){
	ej[j]=1.0;
	matrixPtr->mult(elj,Yl);
	for (size_t i=0 ; i< Y.size() ; i++){
	  if (std::abs(Y[i])>tinyValue){
	    sparseMatrix.setSparseElement(i,j,Y[i]);
	    //	    INFOS("s("<<i<<","<<j<<")="<<Y[i]);
	  }
	    
	}
	ej[j]=0.0;
      }

      if (LEVEL==1){

	sparseMatrix.iterateOverElements(matrixStream);
      }
      else{
	
	Legolas::Matrix * matrixPtr=Legolas::CreateSparseBlockMatrix<RealType,LEVEL>::apply(0,0,sparseMatrix,msl);
	
	matrixPtr->iterateOverElements(matrixStream);
	
	delete matrixPtr;
      }
	
    }
  };
}
      




      
