/**
 * project  DESCARTES
 *
 * @file     SparseMatrixTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
# include <cstdlib>
# include <cmath>
# include <iostream>
# include "UTILITES.hxx" 

# include "Legolas/Matrix/MatrixStructures/Sparse/MultiLevelSparseMatrixInterface.hxx"
# include "Legolas/Vector/Vector.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // don't change this size ! 
  const int size=4;

  // Matrix A
  
  typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;
  Legolas::MatrixShape<1> ms(size,size);
  
  AMatrixInterface::Matrix A(ms);

  INFOS("A.nrows()="<<A.nrows());
  INFOS("A.ncols()="<<A.ncols());

  //  A.resize(size,size);
  
  A.sparseGetElement(0,0)=1.;
  A.sparseGetElement(0,1)=2.;
  
  A.sparseGetElement(1,0)=3.;
  A.sparseGetElement(1,1)=4.;

  A.sparseGetElement(2,2)=5.;
  A.sparseGetElement(2,3)=6.;

  A.sparseGetElement(3,2)=7.;
  A.sparseGetElement(3,3)=8.;
  
  
  Legolas::displayLatex(A,"A.tex",0);


  AMatrixInterface::Matrix B(ms);

  INFOS("B.nrows()="<<B.nrows());
  INFOS("B.ncols()="<<B.ncols());

  //  B.resize(size,size);
  
  B.sparseGetElement(0,1)=1.;
  B.sparseGetElement(0,3)=2.;
  
  B.sparseGetElement(1,0)=3.;
  B.sparseGetElement(1,3)=4.;

  B.sparseGetElement(2,0)=5.;
  B.sparseGetElement(2,2)=6.;

  B.sparseGetElement(3,0)=7.;
  B.sparseGetElement(3,3)=8.;
  
  Legolas::displayLatex(B,"B.tex",0);
  
  Legolas::plusEqual(B,A);

  Legolas::displayLatex(A,"ApB.tex",0);


  {// level 2
    
    typedef Legolas::MultiLevelSparseMatrixInterface<RealType,2> AAMatrixInterface;
    
    Legolas::MatrixShape<2>::Shape rowShape(3,2);
    Legolas::MatrixShape<2>::Shape colShape(2,3);
    
    Legolas::MatrixShape<2> matrixShape(rowShape,colShape);
    
    AAMatrixInterface::Matrix AA(matrixShape);
    AAMatrixInterface::Matrix BB(matrixShape);
    
    
    for (int i=0 ; i<AA.nrows() ; i++){
      for (int j=0 ; j<AA.ncols() ; j++){


	typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;

	Legolas::MatrixShape<1> ms(2,3);
	AMatrixInterface::Matrix A(ms);
	A.sparseGetElement(0,0)=1.0;
	A.sparseGetElement(1,1)=2.0;

	AMatrixInterface::Matrix B(ms);
	B.sparseGetElement(0,0)=1.0;
	B.sparseGetElement(1,0)=2.0;
	B.sparseGetElement(0,1)=3.0;

	if ( ((i+j)%2==0) ){
	  INFOS("i="<<i<<" j="<<j);
	  AA.sparseGetElement(i,j)=A;
	}

	if ( ((i*j)%2==0) ){
	  INFOS("i="<<i<<" j="<<j);
	  BB.sparseGetElement(i,j)=B;
	}
      }
    }
  
    Legolas::displayLatex(AA,"AA.tex",0);
    Legolas::displayLatex(BB,"BB.tex",0);

    Legolas::plusEqual(BB,AA);
    Legolas::displayLatex(AA,"AApBB.tex",0);

  }

  

}
  
