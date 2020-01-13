#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyLightDiagonalMatrix.hxx" 
#include "MySparseBlockMatrix.hxx"

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"


int main( int argc,  char *argv[] )
{
  
  {
    INFOS("START LEVEL 1");
    
    typedef double RealType;
    const int s1=10;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+1);
    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MyLightDiagonalMatrix<RealType> A(ms1);
    
    solveAndCheckResidual(A,Xref);

    A.displayLatex("light.tex");

    INFOS("END LEVEL 1");
  }

  //Level 2
  {

    INFOS("DEBUT LEVEL 2");

    typedef double RealType;
    
    const int s1=5;
    const int s2=10;
    
    Legolas::MultiVector<RealType,2>::Shape  shape(s1,s2);
    Legolas::MultiVector<RealType,2>  Xref(shape);
    Xref=1.0;
    
    Legolas::MatrixShape<2>::Shape rowShape(s1,s2);
    Legolas::MatrixShape<2> ms2(rowShape,rowShape);
    MySparseBlockMatrix  A(ms2);

    solveAndCheckResidual(A,Xref);

    A.displayLatex("A2.tex");

    INFOS("END LEVEL 2");
    
  }
  

  
}
