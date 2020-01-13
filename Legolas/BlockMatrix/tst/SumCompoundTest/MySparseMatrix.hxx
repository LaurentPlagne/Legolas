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
  
  MySparseMatrix(const Legolas::MatrixShape<1> & ms, const bool isDiag=true):Base(ms){
    for (int i=0 ; i< this->nrows() ; i++){
      if (isDiag){
	(*this).setSparseElement(i,i,4.0);
      }
      else{
	for (int j=std::max(0,i-1) ; j< std::min(this->nrows(),i+2) ; j++){
	  if(i==j-1) (*this).setSparseElement(i,j,-0.10);
	  if(i==j+1) (*this).setSparseElement(i,j,-0.10);
	}
      }
    }
  }
  
  
};
  
#endif
    
