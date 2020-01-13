#include "Legolas/BlockMatrix/Structures/ProductCompound/ProductCompoundMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/ProductCompound/ProductCompoundMatrixMultOperator.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"

#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyIterateOverElements.hxx"

namespace Legolas{


  ProductCompoundMatrix::ProductCompoundMatrix( void ):Matrix(),
						       leftMatrixPtr_(0),
						       rightMatrixPtr_(0)
  {
    this->setSolverPtr(new Legolas::ConjugateGradientSolver());
    this->setMultOperatorPtr(new Legolas::ProductCompoundMatrixMultOperator());
  }
    
  ProductCompoundMatrix::ProductCompoundMatrix(Matrix * leftMatrixPtr, Matrix * rightMatrixPtr):Matrix(),
												leftMatrixPtr_(leftMatrixPtr),
												rightMatrixPtr_(rightMatrixPtr)
  {
    INFOS("ProductCompoundMatrix Ctor");
    
    const VirtualMatrixShape & lms=leftMatrixPtr->getVirtualMatrixShape();
    const VirtualMatrixShape & rms=rightMatrixPtr->getVirtualMatrixShape();
    VirtualMatrixShape * productCompoundShapePtr=lms.newVirtualMatrixShape(lms.getRowShape(),rms.getColShape());
    
    this->setVirtualMatrixShapePtr(productCompoundShapePtr);
    this->setSolverPtr(new Legolas::ConjugateGradientSolver());
    this->setMultOperatorPtr(new Legolas::ProductCompoundMatrixMultOperator());
  };
    
  ProductCompoundMatrix::~ProductCompoundMatrix( void ){
    if (leftMatrixPtr_){ delete leftMatrixPtr_; leftMatrixPtr_=0;}
    if (rightMatrixPtr_){ delete rightMatrixPtr_; rightMatrixPtr_=0;}
  }

  const Matrix &  ProductCompoundMatrix::leftMatrix( void ) const {
    assert(leftMatrixPtr_);
    return *leftMatrixPtr_;
  }
  
  const Matrix &  ProductCompoundMatrix::rightMatrix( void ) const {
    assert(rightMatrixPtr_);
    return *rightMatrixPtr_;
  }
  
  // //solve AX=B where A=L*R 
  // // (L*R) X=B;
  // // first solve LY=B;
  // // then solve RX=Y;
  // virtual inline void LRsolve(const VirtualVector & B, VirtualVector & X) const{ 
  //   assert(leftMatrixPtr_);
  //   assert(rightMatrixPtr_);
  //   const Matrix & L=*leftMatrixPtr_;
  //   const Matrix & R=*rightMatrixPtr_;
    
  //   VirtualVector * YPtr=X.clone();
  //   VirtualVector & Y=*YPtr;
    
  //   L.solve(B,Y);
  //   R.solve(Y,X);
  // }

  std::string ProductCompoundMatrix::name( void ) const { 
    std::string result("(");
    result+=leftMatrixPtr_->name();
    result+=std::string("*");
    result+=rightMatrixPtr_->name();
    result+=std::string(")");
    return result;
  }
  

  void ProductCompoundMatrix::setMultOperatorPtr(VirtualMultOperator * virtualMultOperatorPtr){
      this->setVirtualMultOperatorPtr(virtualMultOperatorPtr);
  }
  
  void ProductCompoundMatrix::collectStatistics(Profile & profile) const{
    
    const ProductCompoundMatrix & A=*this;
    
    if (A.virtualMultOperator()){
      if (A.virtualMultOperator()->counter()>0){
	profile[A.level()][A.name()].operationProfiles[A.virtualMultOperator()->name()].duration+=A.virtualMultOperator()->duration();
	profile[A.level()][A.name()].operationProfiles[A.virtualMultOperator()->name()].counter+=A.virtualMultOperator()->counter();
      }
    }
    
    if (A.virtualSolver()){
      if (A.virtualSolver()->counter()>0){
	profile[A.level()][A.name()].operationProfiles[A.virtualSolver()->name()].duration+=A.virtualSolver()->duration();
	profile[A.level()][A.name()].operationProfiles[A.virtualSolver()->name()].counter+=A.virtualSolver()->counter();
      }
    }
  }
  
  int ProductCompoundMatrix::getPrecisionInBytePerRealElement( void ) const{
    assert(leftMatrixPtr_->getPrecisionInBytePerRealElement()==rightMatrixPtr_->getPrecisionInBytePerRealElement());
    return leftMatrixPtr_->getPrecisionInBytePerRealElement();
  }
  

  inline void ProductCompoundMatrix::iterateOverElements(Legolas::MatrixStream & matrixStream) const{
     Legolas::MultOnlyIterateOverElements::apply(this,matrixStream);
   }

    
}

    
      
      
    
  
  
  
