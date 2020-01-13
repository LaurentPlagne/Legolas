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

# include "SparseMatrixDefinition.hxx"
# include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"

# include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/MatrixStructures/MatrixContainers.hxx"
#include "Legolas/Matrix/MatrixVectorOperations.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixInterface.hxx"

#include "Legolas/Algorithm/SparseGaussSeidel.hxx"



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
  
  typedef Actual StorageMode;
  
  //  typedef Legolas::EmptySparseMatrixDefinition< Legolas::RealDataDriver<RealType> > SMD;

  typedef SparseMatrixDefinition<RealType>   SMD;
  typedef InputMatrixOptions<StorageMode,Legolas::SparseGaussSeidel>    SMO;
  typedef RealElementInterface<RealType>     MEI;
  
  //  typedef GenericMatrixInterface<SMD,SMO,MEI> 
  typedef Legolas::EmptySparseMatrixInterface<MEI> AMatrixInterface;
  
  
  SMD::Data init(size,size);
  AMatrixInterface::Matrix A(init);

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

  //  typedef Legolas::SparseGaussSeidel::Engine< AMatrixInterface::Matrix,Legolas::Vector<RealType>,Legolas::Vector<RealType> > Solver;
  //  Solver solver(A,X,B);
  //  solver.solve(A,X,B);

  Legolas::solve(A,X,B);

  
  //  Legolas::solve(A,X,B);

  INFOS("X"<<X);
  
  R=A*X;

  INFOS("A*X"<<R);

  R-=B;

  INFOS("A*X-B"<<R);

  if (Legolas::norm2(R)>1.e-5) throw std::runtime_error("Legolas::norm2(R)>1.e-5");





}
