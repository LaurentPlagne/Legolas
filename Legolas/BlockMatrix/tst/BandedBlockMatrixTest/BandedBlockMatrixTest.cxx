#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyDiagonalMatrix.hxx" 
#include "MyBandedBlockMatrix.hxx" 
#include "MyBandedBlockMatrixLevel3.hxx" 

#include "Legolas/BlockMatrix/Structures/Sparse/SparseGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalSolver.hxx"

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"

int main( int argc,  char *argv[] )
{
  
 //  //Level 1
  {

    INFOS("START LEVEL 1");
    
    typedef double RealType;
    
    const int s1=10;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+1);

    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MyDiagonalMatrix<RealType> A(ms1,2.0);

    solveAndCheckResidual(A,Xref);

    INFOS("END LEVEL 1");

  }

  //Level 2


  {

    INFOS("START LEVEL 2");

    typedef double RealType;
    
    const int s1=5;
    const int s2=2;
    
    Legolas::MultiVector<RealType,2>::Shape  shape(s2,s1);
    Legolas::MultiVector<RealType,2>  Xref(shape);
    
    Xref=1.0;

    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<2> ms2(shape,shape);
    MyBandedBlockMatrix  A(ms2);

    solveAndCheckResidual(A,Xref);
    

    A.setSolverPtr( new Legolas::SparseGaussSeidelSolver() );
    //Le choix suivant est interdit statiquement car une matrice
    // bande n'est pas une matrice diagonale...
    //        A.setSolverPtr( new Legolas::DiagonalSolver() );

    solveAndCheckResidual(A,Xref);

    INFOS("END LEVEL 2");

  }

  {

    INFOS("START LEVEL 3");

    typedef double RealType;
    
    const int s1=100;
    const int s2=3;
    const int s3=4;
    
    Legolas::MultiVector<RealType,2>::Shape  shape2(s2,s1);
    Legolas::MultiVector<RealType,3>::Shape  shape(s3,shape2);
    Legolas::MultiVector<RealType,3>  Xref(shape);
    
    Xref=1.0;

    INFOS("Xref="<<Xref);
    
    Legolas::MatrixShape<3> matrixShape(shape,shape);
    MyBandedBlockMatrixLevel3  A(matrixShape);
    
    solveAndCheckResidual(A,Xref);

    INFOS("END LEVEL 3");

  }
  
  
}
