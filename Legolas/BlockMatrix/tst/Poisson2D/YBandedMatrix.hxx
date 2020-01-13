#ifndef __LEGOLAS_YBANDEDMATRIX_HXX__
#define __LEGOLAS_YBANDEDMATRIX_HXX__

#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrix.hxx"

#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Banded/BandedScalarMatrixOptions.hxx"

// class YBandedScalarContainer{
// public:
//   template <class SCALAR>
//   class Engine: public Legolas::Matrix{
//   public:
//     typedef SCALAR RealType;
//     typedef Engine<SCALAR> Container;

//     SCALAR diagonalElement_;
//     SCALAR upperDiagonalElement_;
//     int lsup_;
    
//   public:
    
//     //    inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape):Matrix(virtualMatrixShape){}

//     inline Engine(const Container & container):Matrix(container),
// 					       diagonalElement_(container.diagonalElement_),
// 					       upperDiagonalElement_(container.upperDiagonalElement_),
// 					       lsup_(container.lsup_){}
    
    
//     inline Engine(const Legolas::VirtualMatrixShape & virtualMatrixShape,
// 		  const bool & isDiagonal ):Matrix(virtualMatrixShape),
// 					    diagonalElement_(0.0),
// 					    upperDiagonalElement_(0.0),
// 					    lsup_(0)
//     {
//       if (isDiagonal){
// 	diagonalElement_=-4.0;	
// 	upperDiagonalElement_=1.0; 
// 	lsup_=1;
//       }
//       else{
// 	diagonalElement_=1.0;
//       }
//     }

//     inline int lsup( void ) const { return lsup_;}
//     inline int linf( void ) const { return lsup_;}
    
//     inline SCALAR bandedGetElement(int i, int j) const{ 
//       if (i!=j){
// 	return upperDiagonalElement_;
//       }
//       else{
// 	return diagonalElement_;
//       }
//     }
      
//   };
// };

template <class SCALAR>
class YBandedMatrix : public Legolas::BandedScalarMatrix<SCALAR> {
  typedef SCALAR RealType;
  typedef Legolas::BandedScalarMatrix<SCALAR> Base;
  typedef typename Base::Container Container;


  static int selectLsup( const bool & isDiagonal ){ 
    int result=0;
    if (isDiagonal) result=1;
    return result;
  }

public:
  
  std::string name( void ) const{ return "YBandedMatrix";}

  
  YBandedMatrix(const Legolas::MatrixShape<1> & ms, const bool & isDiagonal):Base(ms,selectLsup(isDiagonal),selectLsup(isDiagonal)){
    if (isDiagonal){
      for (int i=0 ; i<this->nrows() ; i++){
	for (int j=std::max(i-1,0) ; j<std::min(i+2,this->nrows()) ; j++){
	  if (i==j  ) this->bandedGetElement(i,j)=-4.0;
	  if (i==(j-1)) this->bandedGetElement(i,j)=1.0;
	  if (i==(j+1)) this->bandedGetElement(i,j)=1.0;
	}
      }
    }
    else{
      for (int i=0 ; i<this->nrows() ; i++){
	this->bandedGetElement(i,i)=1.0;
      }
    }
  }
  
};
  
#endif
    
