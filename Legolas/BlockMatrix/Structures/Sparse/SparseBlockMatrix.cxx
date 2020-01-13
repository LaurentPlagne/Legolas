#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"

namespace Legolas{

  SparseBlockMatrix::SparseBlockMatrix(const VirtualMatrixShape & virtualMatrixShape):SparseVirtualBlockMatrix(virtualMatrixShape),
										      sparseData_(virtualMatrixShape.nrows())
  {
    //    INFOS("LA : "<<this->level()<<"this="<<this);
  }

  SparseBlockMatrix::~SparseBlockMatrix( void ){
    //    INFOS("ICI"<<this->level()<<"this="<<this);
    for (int i=0 ; i< this->nrows() ; i++){
      //      INFOS("sparseData_[i].size()="<<sparseData_[i].size());
      for (int j=this->beginColInRow(i) ; j < this->endColInRow(i) ; j=this->nextColInRow(i,j) ){
	//	INFOS("sparseData_[i][j]="<<sparseData_[i][j]);
	delete sparseData_[i][j];
	sparseData_[i][j]=0;
      }
    }
  }
  

  void SparseBlockMatrix::setSparseElementPtr(int i, int j, Matrix * matrixPtr){
    //LP Check if a Matrix has benn already place at that point (Could lead to a memomy leak...)
    const Row & rowI=sparseData_[i];
    Row::const_iterator it=rowI.find(j);
    if (it!=rowI.end()){
      INFOS("Element does exist !!");
      delete sparseData_[i][j];
    }
    
    sparseData_[i][j]=matrixPtr;
  }
  
  const Matrix & SparseBlockMatrix::sparseGetElement( int i, int j ) const {
    const Row & rowI=sparseData_[i];
    
    Row::const_iterator it=rowI.find(j);      

    if (it==rowI.end()){
      INFOS("Element does not exist");
      INFOS("i="<<i);
      INFOS("j="<<j);
      throw std::runtime_error("Element does not exist");
    }
    return (*(*it).second);
  }
  
  Matrix & SparseBlockMatrix::sparseGetElement( int i, int j ) {
    Row & rowI=sparseData_[i];
    Row::iterator it=rowI.find(j);      

    if (it==rowI.end()){
      INFOS("Element does not exist");
      INFOS("i="<<i);
      INFOS("j="<<j);
      throw std::runtime_error("Element does not exist");
    }
    return (*(*it).second);
    //    return *(data_[i][j]);
  }

  int SparseBlockMatrix::beginRow( void ) const { return 0;}
  int SparseBlockMatrix::endRow( void ) const { return sparseData_.size();}
  int SparseBlockMatrix::beginColInRow( int i ) const { 
    int result=0;
    const Row & rowI=sparseData_[i];
    if (rowI.begin()!=sparseData_[i].end()) result=(*(rowI.begin())).first;
    return result;
  }
  int SparseBlockMatrix::endColInRow( int i ) const {
    int result=0;
    const Row & rowI=sparseData_[i];
    if (rowI.begin()!=rowI.end()){
      result=(*(--rowI.end())).first+1;
    }
    return result;
  }

  int SparseBlockMatrix::nextColInRow(int i, int j) const {
    const Row & rowI=sparseData_[i];
    Row::const_iterator it=rowI.find(j);
    if (it!=rowI.end()) it++;
    
    int result=j+1;
    
    if (it!=rowI.end()){
      result=(*it).first;	  
    }

    return result;
  }
  

}

