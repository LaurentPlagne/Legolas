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

  // Matrix A
  
  typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;
  Legolas::MatrixShape<1> msA(2,3);
  
  AMatrixInterface::Matrix A(msA);

  A.sparseGetElement(0,1)=1.;
  A.sparseGetElement(0,2)=2.;
  A.sparseGetElement(1,0)=3.;
  A.sparseGetElement(1,2)=4.;

  Legolas::displayLatex(A,"A.tex",0);

  Legolas::MatrixShape<1> msB(3,4);
  AMatrixInterface::Matrix B(msB);

  B.sparseGetElement(0,0)=6.;
  B.sparseGetElement(0,2)=5.;
  B.sparseGetElement(0,3)=4.;
  B.sparseGetElement(1,0)=3.;
  B.sparseGetElement(2,1)=2.;
  B.sparseGetElement(2,3)=1.;

  Legolas::displayLatex(B,"B.tex",0);

  Legolas::MatrixShape<1> msC(2,4);
  AMatrixInterface::Matrix C(msC);

  Legolas::plusEqualProduct(A,B,C);

  Legolas::displayLatex(C,"C.tex",0);
  
  {// level 2
    
    typedef Legolas::MultiLevelSparseMatrixInterface<RealType,2> AAMatrixInterface;
    
    Legolas::MatrixShape<2>::Shape rowShapeA(2,2);
    Legolas::MatrixShape<2>::Shape colShapeA(2,3);
    Legolas::MatrixShape<2> matrixShapeA(rowShapeA,colShapeA);
    AAMatrixInterface::Matrix AA(matrixShapeA);

    Legolas::MatrixShape<2>::Shape rowShapeB(2,3);
    Legolas::MatrixShape<2>::Shape colShapeB(2,4);
    Legolas::MatrixShape<2> matrixShapeB(rowShapeB,colShapeB);
    AAMatrixInterface::Matrix BB(matrixShapeB);

    for (int i=0 ; i<AA.nrows() ; i++){
      for (int j=0 ; j<AA.ncols() ; j++){


	typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;

	if (i==j) AA.sparseGetElement(i,j)=A;
	if (i!=j) BB.sparseGetElement(i,j)=B;
	
      }
    }
  
    Legolas::displayLatex(AA,"AA.tex",0);
    Legolas::displayLatex(BB,"BB.tex",0);

    Legolas::MatrixShape<2>::Shape rowShapeC(2,2);
    Legolas::MatrixShape<2>::Shape colShapeC(2,4);
    Legolas::MatrixShape<2> matrixShapeC(rowShapeC,colShapeC);
    AAMatrixInterface::Matrix CC(matrixShapeC);

    Legolas::plusEqualProduct(AA,BB,CC);

    Legolas::displayLatex(CC,"CC.tex",0);

  }

  

}
  
