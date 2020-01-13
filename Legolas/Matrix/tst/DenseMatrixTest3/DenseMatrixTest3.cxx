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
# include "Legolas/Vector/Vector.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"

# include "AMatrixDefinition.hxx"
# include <vector>

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"
#include "Legolas/Algorithm/SparseGaussSeidel.hxx"
#include "Legolas/Algorithm/SparseMatrixVectorProduct.hxx"
#include "Legolas/Matrix/GenericVector/STLVectorContainer.hxx"
#include "Legolas/Matrix/GenericVector/CVectorContainer.hxx"
#include "Legolas/Matrix/MatrixStructures/Dense/DenseCMatrixContainer.hxx"

# include "Legolas/Matrix/MatrixVectorOperations.hxx"

#define USE_F77_STORAGE

using namespace std; using namespace Legolas;
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef AMatrixDefinition<double>      MD ;

#ifdef USE_F77_STORAGE

  typedef InputMatrixOptions<Actual,
                             SparseGaussSeidel,
                             SparseMatrixVectorProduct,
                             CVectorContainer,
                             DenseF77MatrixContainer>    MO ;

#endif
#ifndef USE_F77_STORAGE

  typedef InputMatrixOptions<Actual,
                             SparseGaussSeidel,
                             SparseMatrixVectorProduct,
                             STLVectorContainer,
                             DenseCMatrixContainer>    MO ;
#endif



  typedef RealElementInterface<double>   EI ;

  typedef GenericMatrixInterface<MD,MO,EI> AMatrixInterface;
  
  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  A(0,0)=5.0; A(0,1)=2.0; A(0,2)=3.0;
  A(1,0)=3.0; A(1,1)=4.0; A(1,2)=2.0;
  A(2,0)=1.0; A(2,1)=2.0; A(2,2)=4.0;  

  // one can get the raw data for interoperability purpose 


#ifdef USE_F77_STORAGE

  double * AF77=A.getDataReference().matrix().getPointer();

  for (int i=0 ; i< size ; i++){
    cout << "[" ;
    for (int j=0 ; j< size ; j++){
      int index=j+size*i; // ColMajor
      cout << AF77[index] << " " ;
    }
    cout << "]" << endl;
  }
#endif
#ifndef USE_F77_STORAGE

  vector< vector<double > > & AC=A.getDataReference().matrix();

  for (int i=0 ; i< size ; i++){
    cout << "[" ;
    for (int j=0 ; j< size ; j++){
      cout << AC[i][j] << " " ;
    }
    cout << "]" << endl;
  }

#endif    

  // ********* Vector construction : X,B,R ***********
  Legolas::Vector<double> X(size),B(size),R(size);
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  cout << "B      "<< B << endl ;

  // ********* Linear solver test : AX=B ***********

  //  Legolas::solve(A,X,B); // solves AX=B
  Legolas::solve(A,X,B);
  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;
}
