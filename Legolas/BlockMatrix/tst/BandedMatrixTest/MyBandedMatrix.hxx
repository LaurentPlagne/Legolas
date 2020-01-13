#ifndef __LEGOLAS_MYBANDEDMATRIX_HXX__
#define __LEGOLAS_MYBANDEDMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrix.hxx"

template <class SCALAR>
class MyBandedMatrix : public Legolas::BandedScalarMatrix<SCALAR> {
  typedef SCALAR RealType;
  typedef Legolas::BandedScalarMatrix<SCALAR> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
public:
  
  //  std::string name( void ) const { return "MyBandedMatrix" ;}

  MyBandedMatrix(const Legolas::MatrixShape<1> & ms):Base(ms,1,1){
    for (int i=0 ; i< this->nrows() ; i++){
      for (int j=std::max(0,i-1) ; j< std::min(this->nrows(),i+2) ; j++){
	if(i==j) (*this).bandedGetElement(i,j)=4.0;
	if(i==j-1) (*this).bandedGetElement(i,j)=-1.0;
	if(i==j+1) (*this).bandedGetElement(i,j)=-1.0;
      }
    }
  }
  
  
};
  
#endif
    
