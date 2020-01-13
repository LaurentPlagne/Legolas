#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MySparseMatrix.hxx" 

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundSolver.hxx"

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
    A.displayPNG("A.png");

    INFOS("A.name="<<A.name());
    
    Legolas::SumCompoundMatrix AA(new MySparseMatrix<RealType>(ms1,true),new MySparseMatrix<RealType>(ms1,false));

    INFOS("Xref="<<Xref);

    solveAndCheckResidual(AA,Xref);

    
    Legolas::SumCompoundSolver * scs=new Legolas::SumCompoundSolver();
    INFOS("scs="<<scs);
    //    A.setVirtualSolverPtr(new Legolas::SumCompoundSolver());
    AA.setSolverPtr(scs);

    AA.maxIteration()=100;

    INFOS("Xref="<<Xref);
    solveAndCheckResidual(AA,Xref);


    INFOS("END LEVEL 1");
  }

  
}
