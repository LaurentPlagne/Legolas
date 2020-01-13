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

  // Vectors X,B,R
  Legolas::Vector<RealType> X(size),B(size),R(size);

  B[0]=0.814179;
  B[1]=1.089563;
  B[2]=1.11942;
  B[3]=1.58127;

  // Matrix A
  
  typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;
  Legolas::MatrixShape<1> ms(size,size);
  
  AMatrixInterface::Matrix A(ms);

  INFOS("A.nrows()="<<A.nrows());
  INFOS("A.ncols()="<<A.ncols());


  //  A.resize(size,size);
  
  RealType a=11.1148;
  RealType b=-3.64756;
  RealType c=-3.81972;
  
  A.sparseGetElement(0,0)=a;
  A.sparseGetElement(0,1)=b;
  A.sparseGetElement(1,0)=b;
  A.sparseGetElement(1,1)=a;

  A.sparseGetElement(2,2)=a;
  A.sparseGetElement(2,3)=b;
  A.sparseGetElement(3,2)=b;
  A.sparseGetElement(3,3)=a;
  
  A.sparseGetElement(2,0)=c;
  A.sparseGetElement(3,1)=c;
  
  Legolas::displayLatex(A,"A.tex",0);



  INFOS("A.getMaxIteration()="<<A.getMaxIteration());

  A.setMaxIteration()=100;
  INFOS("A.setMaxIteration()="<<A.setMaxIteration());


  INFOS("A.getEpsilon()="<<A.getEpsilon());

  A.setEpsilon()=1.e-8;
  INFOS("A.getEpsilon()="<<A.getEpsilon());

  Legolas::solve(A,X,B);
  
  //  Legolas::solve(A,X,B);

  INFOS("X"<<X);
  
  R=A*X;

  INFOS("A*X"<<R);

  R-=B;

  INFOS("A*X-B"<<R);

  {// level 2

    typedef Legolas::MultiLevelSparseMatrixInterface<RealType,2> AAMatrixInterface;

    Legolas::MatrixShape<2>::Shape rowShape(3,2);
    Legolas::MatrixShape<2>::Shape colShape(2,3);
    
    Legolas::MatrixShape<2> matrixShape(rowShape,colShape);
  
    AAMatrixInterface::Matrix AA(matrixShape);

    INFOS("A.nrows()="<<A.nrows());
    INFOS("A.ncols()="<<A.ncols());
    

    for (int i=0 ; i<AA.nrows() ; i++){
      for (int j=0 ; j<AA.ncols() ; j++){


	typedef Legolas::MultiLevelSparseMatrixInterface<RealType,1> AMatrixInterface;

	Legolas::MatrixShape<1> ms(2,3);
	AMatrixInterface::Matrix A(ms);
	A.sparseGetElement(0,0)=1.0;
	A.sparseGetElement(1,1)=2.0;
	
	if (!( (i==2) && (j==1)) ){
	  INFOS("i="<<i<<" j="<<j);
	  AA.sparseGetElement(i,j)=A;
	}
      }
    }
  
    Legolas::displayLatex(AA,"AA.tex",0);

    // level 3

    typedef Legolas::MultiLevelSparseMatrixInterface<RealType,3> AAAMatrixInterface;

    Legolas::MatrixShape<3>::Shape rowShape3(2,rowShape);
    Legolas::MatrixShape<3>::Shape colShape3(2,colShape);
    
    Legolas::MatrixShape<3> matrixShape3(rowShape3,colShape3);
  
    AAAMatrixInterface::Matrix AAA(matrixShape3);


    AAA.sparseGetElement(0,0)=AA;
    AAA.sparseGetElement(1,1)=AA;

    Legolas::displayLatex(AAA,"AAA.tex",0);


  }


}
