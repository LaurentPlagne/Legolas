#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrixMultOperator.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"

#include "Legolas/BlockMatrix/Structures/MultOnly/MultOnlyIterateOverElements.hxx"

namespace Legolas{


  SumCompoundMatrix::SumCompoundMatrix( void ):Matrix(),
					       leftMatrixPtr_(0),
					       rightMatrixPtr_(0)
  {
    this->setSolverPtr(new Legolas::ConjugateGradientSolver());
    this->setMultOperatorPtr(new Legolas::SumCompoundMatrixMultOperator());
  }
    
  SumCompoundMatrix::SumCompoundMatrix(Matrix * leftMatrixPtr, Matrix * rightMatrixPtr):Matrix(),
											leftMatrixPtr_(leftMatrixPtr),
											rightMatrixPtr_(rightMatrixPtr)
  {
    //    INFOS("SumCompoundMatrix Ctor");
    
    const VirtualMatrixShape & lms=leftMatrixPtr->getVirtualMatrixShape();
    const VirtualMatrixShape & rms=rightMatrixPtr->getVirtualMatrixShape();
    VirtualMatrixShape * sumCompoundShapePtr=lms.newVirtualMatrixShape(lms.getRowShape(),rms.getColShape());
    
    this->setVirtualMatrixShapePtr(sumCompoundShapePtr);
    this->setSolverPtr(new Legolas::ConjugateGradientSolver());
    this->setMultOperatorPtr(new Legolas::SumCompoundMatrixMultOperator());
  };
    
  SumCompoundMatrix::~SumCompoundMatrix( void ){
    if (leftMatrixPtr_){ delete leftMatrixPtr_; leftMatrixPtr_=0;}
    if (rightMatrixPtr_){ delete rightMatrixPtr_; rightMatrixPtr_=0;}
  }

  const Matrix &  SumCompoundMatrix::leftMatrix( void ) const {
    assert(leftMatrixPtr_);
    return *leftMatrixPtr_;
  }
  
  const Matrix &  SumCompoundMatrix::rightMatrix( void ) const {
    assert(rightMatrixPtr_);
    return *rightMatrixPtr_;
  }
  
  
  std::string SumCompoundMatrix::name( void ) const { 
    std::string result("(");
    result+=leftMatrixPtr_->name();
    result+=std::string("+");
    result+=rightMatrixPtr_->name();
    result+=std::string(")");
    return result;
  }
  

  void SumCompoundMatrix::setMultOperatorPtr(VirtualMultOperator * virtualMultOperatorPtr){
    this->setVirtualMultOperatorPtr(virtualMultOperatorPtr);
  }
  
  void SumCompoundMatrix::collectStatistics(Profile & profile) const{
    
    const SumCompoundMatrix & A=*this;
    
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
  
  
  int SumCompoundMatrix::getPrecisionInBytePerRealElement( void ) const{
    assert(leftMatrixPtr_->getPrecisionInBytePerRealElement()==rightMatrixPtr_->getPrecisionInBytePerRealElement());
    return leftMatrixPtr_->getPrecisionInBytePerRealElement();
  }
  

  inline void SumCompoundMatrix::iterateOverElements(Legolas::MatrixStream & matrixStream) const{
     Legolas::MultOnlyIterateOverElements::apply(this,matrixStream);
   }

    
  

    
}

    
      
      
    
  
  
  
