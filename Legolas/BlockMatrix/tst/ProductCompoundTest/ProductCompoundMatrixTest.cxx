#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MySparseMatrix.hxx" 

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "Legolas/BlockMatrix/Structures/ProductCompound/ProductCompoundMatrix.hxx"

int main( int argc,  char *argv[] )
{
  
  {
    INFOS("START LEVEL 1");
    
    typedef double RealType;
    const int s1=4;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+1);
    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MySparseMatrix<RealType> A(ms1);
    
    solveAndCheckResidual(A,Xref);

    A.displayLatex("A.tex");
    A.displaySVG("A.svg");

    INFOS("A.name="<<A.name());

    Legolas::ProductCompoundMatrix AA(new MySparseMatrix<RealType>(ms1),new MySparseMatrix<RealType>(ms1));

    solveAndCheckResidual(AA,Xref);


    AA.displayLatex("AA.tex");
    AA.displaySVG("AA.svg");

    INFOS("AA.name="<<AA.name());


    INFOS("END LEVEL 1");
  }

  
}
