#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/ExpressionTest.hxx"

#include <list>

void display2D(std::vector< std::vector<double> > & X){
  std::cout << "Vector Elements :"<< std::endl;
  for (uint i=0 ; i<X.size() ; i++){
    std::cout <<"X["<<i<<"]={";
    for (uint j=0 ; j<X[i].size() ; j++){
      std::cout << X[i][j];
      if (j!=(X[i].size()-1))
	std::cout <<",";
      else
	std::cout<<"}";
    }
    std::cout <<std::endl;
  }
}


/*! \file Tuto1Test.cxx
*/

/*! \brief Mes gg
 */
int main( int argc,  char *argv[] )
{
  Legolas::MultiVector<double,2> X("(1x2,2x3)");
  X[0]=0.0; X[1]=1.0 ; X[2]=2.0; 
  std::cout << "X=" << X << std::endl;

  Legolas::MultiVector<double,1> & X1D0=X[0];
  X1D0=3.0;
  std::cout << "X=" << X << std::endl;

  //2D STL Vector
  {
    std::vector< std::vector<double> > X(3);
    X[0].resize(2); X[1].resize(3); X[2].resize(3);

    int counter=0;
    for (uint i=0 ; i<X.size() ; i++){
      std::vector<double> & Xi=X[i];
      for (uint j=0 ; j<Xi.size() ; j++){
	Xi[j]=double(counter);
	counter++;
      }
    }
    display2D(X);
  }

  // beginTuto3 : 2D Legolas++ Vector
  {
    Legolas::MultiVector<double,2>::Shape shape;
    shape.appendElement(2);     
    shape.appendNElement(2,3);     

    Legolas::MultiVector<double,2> X(shape);
    
    int counter=0;
    for (int i=0 ; i<X.size() ; i++){
      Legolas::MultiVector<double,1> & Xi=X[i];
      for (int j=0 ; j<Xi.size() ; j++){
	Xi[j]=double(counter);
	counter++;
      }
    }
    std::cout << "X=" << X << std::endl;
  }


}
