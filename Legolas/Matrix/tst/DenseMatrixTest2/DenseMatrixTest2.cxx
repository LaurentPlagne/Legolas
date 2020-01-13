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
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "AMatrixDefinition.hxx"
# include <vector>


using namespace std; using namespace Legolas;
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef AMatrixDefinition<double>      MD ;
  typedef InputMatrixOptions<Virtual>    MO;
  typedef RealElementInterface<double>   EI ;

  typedef GenericMatrixInterface<MD,MO,EI> AMatrixInterface;

  // mardi 18 mai 2010 : pour compatibilité avec g++ 4.4, 
  // AY a remplacé la ligne suivante
  //vector< vector<double > > Ad(size,size); // eh eh

  // par le bloc commencant ci-dessous
  vector< vector<double > > Ad(size);
  vector< vector<double > >::iterator it( Ad.begin() ); 
  for( ; it!=Ad.end(); ++it ) (*it).resize( size );
  // se terminant ici

  Ad[0][0]=5.0; Ad[0][1]=2.0; Ad[0][2]=3.0;
  Ad[1][0]=3.0; Ad[1][1]=4.0; Ad[1][2]=2.0;
  Ad[2][0]=1.0; Ad[2][1]=2.0; Ad[2][2]=4.0;  

  AMatrixInterface::Matrix A(Ad);

  for (int i=0 ; i< A.nrows() ; i++){
    cout << "[" ;
    for (int j=0 ; j< A.ncols() ; j++){
      cout << A(i,j) << " " ;
    }
    cout << "]" << endl;
  }

  // ********* Vector construction : X,B,R ***********
  Legolas::Vector<double> X(size),B(size),R(size);
  
  B[0]=18.0;
  B[1]=17.0;
  B[2]=17.0;

  cout << "B      "<< B << endl ;

  // ********* Linear solver test : AX=B ***********

  Legolas::solve(A,X,B); // solves AX=B
  cout << "Legolas::solve(A,X,B)  "<< X << endl ;
  
  R=A*X;
  cout << "R=A*X  "<< R << endl ;

  R-=B;
  cout << "R=A*X-B"<< R << endl ;

  exit(0);
}
