#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedBlockMatrix.hxx"

namespace Legolas{
  
  BandedBlockMatrix::BandedBlockMatrix(const VirtualMatrixShape & virtualMatrixShape, 
				       int linf, int lsup):BandedVirtualBlockMatrix(virtualMatrixShape),
							   bandedData_(),
							   linf_(linf),
							   lsup_(lsup),
							   width_(linf+lsup+1)
  {
    assert(lsup>=0);
    assert(linf>=0);
    assert(lsup<this->nrows());
    assert(linf<this->nrows());
    bandedData_.resize(width_*this->nrows());
  }
  inline int BandedBlockMatrix::getDataIndex(int i, int j) const {
    assert(i-j<=linf_);
    assert(j-i<=lsup_);
    assert(i<this->nrows());
    assert(j<this->nrows());
    const int result=j+i*(width_-1)+lsup_;
    //    INFOS("i="<<i<<" j="<<j<<" result="<<result<<" width_*nrows="<<width_*this->nrows());
    return result;
  }

  BandedBlockMatrix::~BandedBlockMatrix( void ){
    for (int i=0 ; i< this->nrows() ; i++){
      for (int j=std::max(i-this->linf(),0);j!=std::min(i+this->lsup()+1,this->nrows());j++){
	if (bandedData_[this->getDataIndex(i,j)]) delete bandedData_[this->getDataIndex(i,j)] ;
	bandedData_[this->getDataIndex(i,j)]=0;
      }
    }
  }
  

  void BandedBlockMatrix::setBandedElementPtr(int i, int j, Matrix * matrixPtr){
    bandedData_[this->getDataIndex(i,j)]=matrixPtr;
  }
  
  const Matrix & BandedBlockMatrix::bandedGetElement( int i, int j ) const {
    assert(bandedData_[this->getDataIndex(i,j)]);
    return *bandedData_[this->getDataIndex(i,j)];
  }

  Matrix & BandedBlockMatrix::bandedGetElement( int i, int j ){
    return *bandedData_[this->getDataIndex(i,j)];
  }

  int BandedBlockMatrix::linf( void ) const { return linf_;}
  int BandedBlockMatrix::lsup( void ) const { return lsup_;}
  

}

