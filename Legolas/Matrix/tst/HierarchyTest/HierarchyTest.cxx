/**
 * project  DESCARTES
 *
 * @file     HierarchyTest.cxx
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
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "AMatrixDefinition.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef AMatrixDefinition<double>     MD ;
  typedef InputMatrixOptions<Actual>    MO ;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<MD,MO,EI> AMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);
  AMatrixInterface::Matrix A(ms);

  // Matrix subtype

  typedef AMatrixInterface::Matrix MyMatrix;

  MyMatrix::RealType reel=2.0;
  reel+=2.0; // avoid unused reel warning.

  // Matrix methods
  
  //  Legolas::displayFlatMatrix(A);
  A.setMaxIteration()=10;
  cout << "Max iteration = "<< A.getMaxIteration() << endl;
  A.setEpsilon()=1.e-4;
  cout << "Epsilon = "<< A.getEpsilon() << endl;

  MyMatrix::Data & dataRef =A.getDataReference();
  vector<double> & diagVector =dataRef.diagonal();
  cout << "diagVector size="<<diagVector.size() << endl;

//   // Dense API
  
//   cout << "*** Dense API ***" << endl ;
//   for (int i=0 ; i< A.nrows() ; i++){
//     cout << "[" ;
//     for (int j=0 ; j< A.ncols() ; j++){
//       cout << A(i,j) << " " ;
//     }
//     cout << "]" << endl;
//  }

  // Banded API

cout << "*** Banded API ***" << endl ;
  for (int i=0 ; i< A.nrows() ; i++){
    int jmax=min(i+A.lsup()+1,size);
    int jmin=max(i-A.linf(),0);
    for (int j=jmin ; j < jmax ; j++){
      A.bandedGetElement(i,j)+=1.0;
      cout << "A.bandedGetElement(" << i <<","<< j << ")="<< A.bandedGetElement(i,j) << " " ;
    }
    cout << endl;
  }

  // Diagonal API

  cout << "*** Banded API ***" << endl ;
  for (int i=0 ; i< A.nrows() ; i++){
    A.diagonalGetElement(i)-=1.0;
    cout << "A.diagonalGetElement(" << i << ")="<< A.diagonalGetElement(i) << endl;
  }

  // Sparse API
  
  cout << "*** Sparse API ***" << endl ;
  MyMatrix::FilledRows rows(A);
  
  for (int i=rows.begin() ; i < rows.end() ; rows.next(i) ){
    
    MyMatrix::FilledRows::FilledCols cols=rows.filledColsInRow(i);

    for (int j=cols.begin() ; j < cols.end() ; cols.next(j) ){
      
      A.sparseGetElement(i,j)+=1.0;
      cout << "A.sparseGetElement(" << i <<","<< j << ")="<< A.sparseGetElement(i,j) << " " ;

    }
    cout << endl;
  }


  // ********* Vector construction : X,B,R ***********
  
  Vector<double> X,B,R; 
  
  X.resize(size); B.resize(size); R.resize(size);
  
  X=0.0;   R=0.0;
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  cout << "B      "<< B << endl ;

  // Linear API

  // ********* Linear solver test : AX=B ***********

  //  Legolas::solve(A,X,B); // solves AX=B

  Legolas::solve(A,X,B);

  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;
}
