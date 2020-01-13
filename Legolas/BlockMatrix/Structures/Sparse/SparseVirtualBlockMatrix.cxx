#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseVirtualBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseMultOperator.hxx"

namespace Legolas{

  SparseVirtualBlockMatrix::SparseVirtualBlockMatrix(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){
    this->setSolverPtr( new SparseGaussSeidelSolver());
    this->setMultOperatorPtr( new SparseMultOperator());
  }

  SparseVirtualBlockMatrix::~SparseVirtualBlockMatrix( void ){ 
    //  INFOS("SparseVirtualBlockMatrix Dtor");
  }
  
  void SparseVirtualBlockMatrix::setSolverPtr(SparseVirtualSolver * sparseVirtualSolverPtr){
    this->setVirtualSolverPtr(sparseVirtualSolverPtr);
  }

  void SparseVirtualBlockMatrix::setMultOperatorPtr(SparseVirtualMultOperator * sparseVirtualMultOperatorPtr){
    this->setVirtualMultOperatorPtr(sparseVirtualMultOperatorPtr);
  }
  
  void SparseVirtualBlockMatrix::iterateOverElements(Legolas::MatrixStream & matrixStream) const{

    const SparseVirtualBlockMatrix & block=*this;
    
    const int nr=block.nrows();
    const int nc=block.ncols();

    assert(nr!=0);
    assert(nc!=0);

    matrixStream.blockInitialize(nr,nc);

    std::vector<int> cornerFlatRowIndexBlock(nr);
    std::vector<int> blockFlatRowSize(nr);
    //    for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=Legolas::getSubFlatSize(brs,i);
    //    for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=block.getSubFlatRowSize(i);
    for (int i=0 ; i < nr ; i++) blockFlatRowSize[i]=block.getVirtualMatrixShape().getRowShape().subFlatSize(i);
    cornerFlatRowIndexBlock[0]=matrixStream.cornerFlatRowIndex();
    for (int i=1 ; i < nr ; i++) cornerFlatRowIndexBlock[i]=cornerFlatRowIndexBlock[i-1]+blockFlatRowSize[i-1];
    //	for (int i=0 ; i < nr ; i++) INFOS("cornerFlatRowIndexBlock["<<i<<"]="<<cornerFlatRowIndexBlock[i]);
    
    std::vector<int> cornerFlatColIndexBlock(nc);
    std::vector<int> blockFlatColSize(nc);
    //    for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=Legolas::getSubFlatSize(bcs,j);
    //for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=block.getSubFlatColSize(j);
    for (int j=0 ; j < nc ; j++) blockFlatColSize[j]=block.getVirtualMatrixShape().getColShape().subFlatSize(j);
    cornerFlatColIndexBlock[0]=matrixStream.cornerFlatColIndex();
    for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+blockFlatColSize[j-1];
    
    //	for (int j=1 ; j < nc ; j++) cornerFlatColIndexBlock[j]=cornerFlatColIndexBlock[j-1]+Legolas::getSubFlatSize(bcs,j-1);
    
    std::vector<bool> emptycol(nc,true);
    std::vector< std::vector<bool> > emptyness(nr,emptycol);
    
    for (int i=block.beginRow() ; i < block.endRow() ; i++ ){
      for (int j=block.beginColInRow(i) ; j < block.endColInRow(i) ; j=block.nextColInRow(i,j) ){
	emptyness[i][j]=false;
      }
    }
    
    for (int i=0 ; i < nr ; i++){     
      matrixStream.rowInitialize(i);
      for (int j=0 ; j < nc ; j++){
	matrixStream.colInRowInitialize(i,j);
	if (!emptyness[i][j]){
	  matrixStream.createNonEmptyBlock(i,j);
	  
	  MatrixStream * childPtr=matrixStream.child(i,j,
						     cornerFlatRowIndexBlock[i],
						     cornerFlatColIndexBlock[j],
						     blockFlatRowSize[i],
						     blockFlatColSize[j]);
	  
	  block.sparseGetElement(i,j).iterateOverElements((*childPtr));
	  delete childPtr;
	  
	}
	else{
	  matrixStream.createEmptyBlock(i,j);
	}
	matrixStream.colInRowFinalize(i,j);
      }
      matrixStream.rowFinalize(i);
    }
    matrixStream.blockFinalize();
  }
  
  void SparseVirtualBlockMatrix::collectStatistics(Profile & profile) const{
    
    const SparseVirtualBlockMatrix & A=*this;
    
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

    //Loop on children (elements)
    for (int i=0 ; i< A.nrows() ; i++){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	if (A.sparseGetElement(i,j).virtualSolver()->counter()!=0){
	  profile[A.level()][A.name()].contributions[A.sparseGetElement(i,j).name()][A.sparseGetElement(i,j).virtualSolver()->name()].duration+=A.sparseGetElement(i,j).virtualSolver()->duration();
	  profile[A.level()][A.name()].contributions[A.sparseGetElement(i,j).name()][A.sparseGetElement(i,j).virtualSolver()->name()].counter+=A.sparseGetElement(i,j).virtualSolver()->counter();
	}
	if (A.sparseGetElement(i,j).virtualMultOperator()->counter()!=0){
	  profile[A.level()][A.name()].contributions[A.sparseGetElement(i,j).name()][A.sparseGetElement(i,j).virtualMultOperator()->name()].duration+=A.sparseGetElement(i,j).virtualMultOperator()->duration();
	  profile[A.level()][A.name()].contributions[A.sparseGetElement(i,j).name()][A.sparseGetElement(i,j).virtualMultOperator()->name()].counter+=A.sparseGetElement(i,j).virtualMultOperator()->counter();
	}
	A.sparseGetElement(i,j).collectStatistics(profile);
      }
    }
  }
  
  int SparseVirtualBlockMatrix::getPrecisionInBytePerRealElement( void ) const{
    
    int i=0;
    int result=-1;
    bool notFound=true;
    
    while( (i<this->nrows()) && notFound){
      
      int j=this->beginColInRow(i);
      
      if (j< this->endColInRow(i)){
	notFound=false;
	result=this->sparseGetElement(i,j).getPrecisionInBytePerRealElement();
      }

      i++;
    }

    if (notFound){
      INFOS("Empy Matrix !!!");
      throw std::runtime_error("Empty matrix !!!");
    }

    return result;
  }

  std::string SparseVirtualBlockMatrix::name( void ) const {
    return "SparseVirtualBlockMatrix";
  }
      
	
      
    

  
}
