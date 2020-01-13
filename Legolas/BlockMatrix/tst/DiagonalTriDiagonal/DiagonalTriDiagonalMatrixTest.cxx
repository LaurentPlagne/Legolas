#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyDiagonalMatrix.hxx" 
#include "MyDiagonalBlockMatrix.hxx" 
#include "RealType.hxx"

#include "Legolas/BlockMatrix/Structures/Sparse/SparseGaussSeidelSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalParallelSolver.hxx"

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "SIMDSolve.hxx"

int main( int argc,  char *argv[] )
{
  
  //Level 2
  {

    INFOS("DEBUT LEVEL 2");

    //    typedef double RealType;
    //    typedef double RealType;
    
    const int s1=10;
    const int s2=5;
    
    Legolas::MultiVector<RealType,2>::Shape  shape(s1,s2);
    Legolas::MultiVector<RealType,2>  Xref(shape);
    Xref=1.0;

    Legolas::MatrixShape<2>::Shape rowShape(s1,s2);
    Legolas::MatrixShape<2> ms2(rowShape,rowShape);
    MyDiagonalBlockMatrix  A(ms2);

    Legolas::MultiVector<RealType,2>  B(Xref.shape());
    B=0.0;
    INFOS("B="<<B);
    INFOS("Xref="<<Xref);
    A.addMult(1.0,Xref,B);
    INFOS("B="<<B);


    solveAndCheckResidual(A,Xref);
    A.displayLatex("A.tex");

    A.setSolverPtr( new Legolas::DiagonalParallelSolver());
    
    //    A.setSolverPtr( new Legolas::SparseGaussSeidelSolver() );
    
    solveAndCheckResidual(A,Xref);

    {
      X86Timer chronos;
      
      const int s1=600;
      const int s2=800;
      const int nbtour=100;

      const double nbops=double(s1)*double(s2)*double(nbtour)*11.;
      
      Legolas::MultiVector<RealType,2>::Shape  shape(s1,s2);
      Legolas::MultiVector<RealType,2>  Xref(shape);
      Xref=1.0;
      
      Legolas::MatrixShape<2>::Shape rowShape(s1,s2);
      Legolas::MatrixShape<2> ms2(rowShape,rowShape);
      MyDiagonalBlockMatrix  A(ms2);
      
      Legolas::MultiVector<RealType,2>  B(shape);
      Legolas::MultiVector<RealType,2>  X(shape);
      X=1.0;

      {

	double duration=0.0;

	for (int i=0 ; i<nbtour ; i++){
	  
	  B=RealType(i);

	  chronos.start();	  
	  A.solve(B,X);
	  chronos.stop();
	  duration+=chronos.get_elapsed_time_in_second();

	  if (i==1) Xref=X;
	  
	}

	std::cout<< "duration="<<duration<<std::endl;
	std::cout<<"GFLOPS="<<nbops/(duration*1.e9)<<std::endl;

      }

      {
	A.setSolverPtr( new Legolas::DiagonalParallelSolver());
	
	double duration=0.0;

	for (int i=0 ; i<nbtour ; i++){
	  
	  B=RealType(i);
	  chronos.start();
	  A.solve(B,X);
	  chronos.stop();
	  duration+=chronos.get_elapsed_time_in_second();

	  if (i==1) INFOS("TBB Legolas::norm2(X-Xref)="<<Legolas::norm2(X-Xref));
	  
	}

	std::cout<< "duration="<<duration<<std::endl;
	std::cout<<"GFLOPS="<<nbops/(duration*1.e9)<<std::endl;

      }

      {//Version sse

	SIMDSolve SSEA(A);

	double duration=0.0;
	
	for (int i=0 ; i<nbtour ; i++){
	  
	  SSEA.updateB(RealType(i));

	  chronos.start();
	  SSEA.solve();
	  chronos.stop();
	  duration+=chronos.get_elapsed_time_in_second();



	  if (i==1){
	    SSEA.updateX(X);
	    INFOS("SSE Legolas::norm2(X-Xref)="<<Legolas::norm2(X-Xref));
	  }
	  
	}

	std::cout<< "duration="<<duration<<std::endl;
	std::cout<<"GFLOPS="<<nbops/(duration*1.e9)<<std::endl;

	  
      }//End sse


      {//Version sse+tbb

	SIMDSolve SSEA(A);

	double duration=0.0;
	
	for (int i=0 ; i<nbtour ; i++){
	  
	  SSEA.updateB(RealType(i));

	  chronos.start();
	  SSEA.parallelSolve();
	  chronos.stop();
	  duration+=chronos.get_elapsed_time_in_second();

	  if (i==1){
	    SSEA.updateX(X);
	    INFOS("SSE + TBB Legolas::norm2(X-Xref)="<<Legolas::norm2(X-Xref));
	  }
	  
	}

	std::cout<< "duration="<<duration<<std::endl;
	std::cout<<"GFLOPS="<<nbops/(duration*1.e9)<<std::endl;

	  
      }//End sse
      
      
    }
  }
}
