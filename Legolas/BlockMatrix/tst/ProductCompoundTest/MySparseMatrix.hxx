#ifndef __LEGOLAS_MYSPARSEMATRIX_HXX__
#define __LEGOLAS_MYSPARSEMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrix.hxx"

template <class SCALAR>
class MySparseMatrix : public Legolas::SparseScalarMatrix<SCALAR> {
  typedef SCALAR RealType;
  typedef Legolas::SparseScalarMatrix<SCALAR> Base;
  typedef Legolas::MultiVector<RealType,1> V1D;
public:

  std::string name( void ) const { return "MySparseMatrix" ;}
  
  MySparseMatrix(const Legolas::MatrixShape<1> & ms):Base(ms){
    for (int i=0 ; i< this->nrows() ; i++){
      for (int j=std::max(0,i-1) ; j< std::min(this->nrows(),i+2) ; j++){
	if(i==j) (*this).setSparseElement(i,j,4.0);
	if(i==j-1) (*this).setSparseElement(i,j,-1.0);
	if(i==j+1) (*this).setSparseElement(i,j,-1.0);
      }
    }
  }
  
  
};
  
#endif
    
