/**
 * project  DESCARTES
 *
 * @file     DenseMatrixTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2010
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

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
#include "Legolas/Matrix/MatrixOptions/DefaultMatrixOptions.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"
#include "Legolas/Matrix/MatrixVectorOperations.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"

#include <assert.h>


#include "NullMatrixInterface.hxx"
#include "NullMatrixDefinition.hxx"

int main( int argc,  char *argv[] )
{

  typedef Legolas::Virtual StorageMode;
  
  typedef Legolas::InputMatrixOptions<>             IMO;
  typedef double RealType;
  typedef Legolas::RealElementInterface<RealType>   EMI;
  typedef Legolas::NullMatrixDefinition<RealType,1> NMD;

  const int nrows1=2;
  const int ncols1=3;

  Legolas::MatrixShape<1> ms1(nrows1,ncols1);

  typedef Legolas::GenericMatrixInterface<NMD,IMO,EMI> GMI;
  
  GMI::Matrix n1(ms1);

  INFOS("ms1"<<ms1);

  
  Legolas::Vector<RealType> X(nrows1),B(ncols1),R(ncols1);
  
  B[0]=1.0;
  B[1]=2.0;
  B[2]=3.0;

  X=1.0;

  INFOS("X="<<X);

  X+=n1*B;

  INFOS("X+=n1*B"<<X);

  X=n1*B;

  INFOS("X=n1*B"<<X);

  Legolas::displayPNG(n1,"n1.png");

  
  Legolas::MatrixShape<2>::Shape rowShape2(3,nrows1);
  Legolas::MatrixShape<2>::Shape colShape2(2,ncols1);
  
  Legolas::MatrixShape<2> ms2(rowShape2,colShape2);

  typedef Legolas::InputMatrixOptions<>             IMO2;
  typedef Legolas::NullMatrixDefinition<RealType,2> NMD2;
  

  typedef Legolas::GenericMatrixInterface<NMD2,IMO2,GMI> GMI2;

  GMI2::Matrix n2(ms2);
  
  INFOS("ms2"<<ms2);

  Legolas::displayPNG(n2,"n2.png");

  Legolas::MultiVector<RealType,2> X2(rowShape2),B2(colShape2),R2(colShape2);
  
  B2[0]=1.0;
  B2[1]=2.0;
  
  X2=1.0;

  INFOS("X2="<<X2);

  X2+=n2*B2; 

  INFOS("X2+=n2*B2"<<X2);

  X2-=n2*B2; 

  INFOS("X2-=n2*B2"<<X2);

  X2=n2*B2;

  INFOS("X2=n2B2"<<X2);

  typedef Legolas::NullMatrixInterface<float,3> GMI3;

  Legolas::MatrixShape<3>::Shape rowShape3(4,rowShape2);
  Legolas::MatrixShape<3>::Shape colShape3(3,colShape2);
  Legolas::MatrixShape<3> ms3(rowShape3,colShape3);
  
  GMI3::Matrix n3(ms3);

  Legolas::MultiVector<RealType,3> X3(rowShape3),B3(colShape3),R3(colShape3);
  
  B3[0]=1.0;
  B3[1]=3.0;
  
  X3=1.0;

  INFOS("X3="<<X3);

  X3+=n3*B3; 

  INFOS("X3+=n3*B3"<<X3);

  X3-=n3*B3; 

  INFOS("X3-=n3*B3"<<X3);

  X3=n3*B3;

  INFOS("X3=n3B3"<<X3);

  Legolas::displayPNG(n3,"n3.png");

}


