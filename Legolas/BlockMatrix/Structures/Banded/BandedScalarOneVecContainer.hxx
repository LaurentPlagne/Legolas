#ifndef __LEGOLAS_BANDEDSCALARONEVECCONTAINER_HXX__
#define __LEGOLAS_BANDEDSCALARONEVECCONTAINER_HXX__

#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  

  class BandedScalarOneVecContainer{
  public:
    template <class SCALAR>
    class Engine: public Matrix{
    public:
      typedef SCALAR RealType;
      typedef Engine<SCALAR> Container;
    private:
      typedef Legolas::MultiVector<SCALAR,1>  BandedData;
      
      int linf_;
      int lsup_;
      int width_;
      BandedData bandedData_;
      
      inline int getDataIndex(int i, int j) const {
	assert(i-j<=linf_);
	assert(j-i<=lsup_);
	assert(i<this->nrows());
	assert(j<this->nrows());
	const int result=j+i*(width_-1)+lsup_;
	//    INFOS("i="<<i<<" j="<<j<<" result="<<result<<" width_*nrows="<<width_*this->nrows());
	return result;
      }
      
    public:
      
      inline Engine(const VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape),linf_(0),lsup_(0),width_(0),bandedData_(){}
      

      inline Engine(const Container & container):Matrix(container),
						 bandedData_(container.bandedData_){}
      inline void resize(int linf, int lsup){
	linf_=linf;
	lsup_=lsup;
	width_=linf+lsup+1;
	bandedData_.resize((linf+lsup+1)*this->nrows());
      }
	
      inline const SCALAR & bandedGetElement(int i, int j) const{
	return bandedData_[this->getDataIndex(i,j)];
      }

      inline SCALAR & bandedGetElement(int i, int j){
	return bandedData_[this->getDataIndex(i,j)];
      }

      inline const int & linf( void ) const { return linf_;}
      inline const int & lsup( void ) const { return lsup_;}
      
    };

  };

}

#endif
    
    
    
    
