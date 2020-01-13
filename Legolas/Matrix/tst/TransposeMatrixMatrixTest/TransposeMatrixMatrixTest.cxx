/**
 * project  DESCARTES
 *
 * @file     DenseMatrixTest.cxx
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

# include "AMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"

#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"

#include "X86Timer.hxx"

#include "BMatrixDefinition.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");

  typedef double RealType;

  // don't change this size ! 
  const int size=4;

  // Vectors X,Y,R
  //  Legolas::Vector<double> X(size),Y(size),R(size);
  Legolas::MultiVector<double,1> X(size),Y(size),R(size);

  Y[0]=34.0;
  Y[1]=21.0;
  Y[2]=33.0;
  Y[3]=48.0;

  // Matrix A

  typedef Actual StorageMode;

  //  typedef InputMatrixOptions<StorageMode> AMatrixOptions ;

  typedef InputMatrixOptions<StorageMode,
                             SparseGaussSeidel,
                             SparseMatrixVectorProduct,
                             STLVectorContainer,
                             DenseF77MatrixContainer,
                             Dense> AMatrixOptions;



  typedef GenericMatrixInterface<AMatrixDefinition<RealType>,AMatrixOptions,RealElementInterface<RealType> > AMatrixInterface;
  
  AMatrixInterface::Matrix A(Legolas::MatrixShape<1>(size,size));


  A(0,0)=5.0;
  A(0,1)=2.0;
  A(0,2)=3.0;
  A(0,3)=4.0;

  A(1,0)=3.0;
  A(1,1)=4.0;
  A(1,2)=2.0;
  A(1,3)=1.0;

  A(2,0)=1.0;
  A(2,1)=2.0;
  A(2,2)=4.0;
  A(2,3)=4.0;

  A(3,0)=2.0;
  A(3,1)=3.0;
  A(3,2)=4.0;
  A(3,3)=7.0;

  X=Y/A;

  INFOS("X"<<X);

  R=A*X;
  R-=Y;

  INFOS("AX-B="<<R);

  {

    INFOS("1 : Transpose simple");
    
    Legolas::transposeSolve(A,R,Y);
    //    R=Y/transpose(A);
    Y-=transpose(A)*R;
    
    INFOS("Y-transpose(A)*R"<<Y);

  }

  // don't change this size ! 
  const int sizeBlock=2;
    

  typedef GenericMatrixInterface<BMatrixDefinition<RealType>,AMatrixOptions,AMatrixInterface > BMatrixInterface;
  BMatrixDefinition<RealType>::Data initB(sizeBlock);
  BMatrixInterface::Matrix B(initB);

  for (int i=0 ; i < B.nrows() ; i++){
    for (int j=0 ; j < B.ncols() ; j++){

      int decalI=B.nrows()*i;
      int decalJ=B.ncols()*j;

      for (int k=0 ; k < B(i,j).nrows() ; k++){
	for (int l=0 ; l < B(i,j).ncols() ; l++){

	  B(i,j)(k,l)=A(decalI+k,decalJ+l);
	}
      }
    }
  }

  for (int i=0 ; i < B.nrows() ; i++){
    for (int j=0 ; j < B.ncols() ; j++){
      for (int k=0 ; k < B(i,j).nrows() ; k++){
	for (int l=0 ; l < B(i,j).ncols() ; l++){

	  INFOS("B("<<i<<","<<j<<")("<<k<<","<<l<<")="<<B(i,j)(k,l));
	}
      }
    }
  }

  Legolas::MultiVector<double,2>::Shape shape(sizeBlock,sizeBlock);

  Legolas::MultiVector<double,2> XX(shape),YY(shape),RR(shape); 
  
//   XX.resize(sizeBlock);
//   YY.resize(sizeBlock);
//   RR.resize(sizeBlock);

//   for (int i=0 ; i<sizeBlock ; i++){
//     XX[i].resize(sizeBlock);
//     YY[i].resize(sizeBlock);
//     RR[i].resize(sizeBlock);
//   }
  
  XX=0.0;

  YY[0][0]=34.0;
  YY[0][1]=21.0;
  YY[1][0]=33.0;
  YY[1][1]=48.0;

  RR=0.0;

  XX=YY/B;

  INFOS("XX"<<XX);      

  RR=B*XX;

  RR-=YY;

  INFOS("RR"<<RR);      

  // Transposed

  //  XX=YY/transpose(B);
  Legolas::transposeSolve(B,XX,YY);

  INFOS("XX"<<XX);      

  RR=transpose(B)*XX;

  RR-=YY;

  INFOS("RR"<<RR);      

  BMatrixInterface::Matrix Bt(B);
  
  for (int i=0 ; i < B.nrows() ; i++){
    for (int j=0 ; j < B.ncols() ; j++){
      for (int k=0 ; k < B(i,j).nrows() ; k++){
	for (int l=0 ; l < B(i,j).ncols() ; l++){

	  Bt(j,i)(l,k)=B(i,j)(k,l);

	}
      }
    }
  }

  // Transposed

  Legolas::solve(Bt,XX,YY);
  //  XX=YY/transpose(Bt);

  INFOS("XX=YY/tranpose(Bt)="<<XX);      

  RR=transpose(Bt)*XX;
  RR-=YY;

  INFOS("RR"<<RR);      

  YY-=transpose(Bt)*XX;
  INFOS("YY"<<YY);      


}
