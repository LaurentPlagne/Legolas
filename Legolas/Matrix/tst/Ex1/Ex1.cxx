/**
 * project  DESCARTES
 *
 * @file     Ex1.cxx
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

using namespace std; 

int main( int argc,  char *argv[] )
{

  class Comparison{
  public:
    static bool isSmaller(const double & x, const double & y){
      bool result=true;
      if (y<x) result=false;
      return result;
    }
  };
  
  double x=1.0;
  double y=2.0;
  
  cout << "x<y "<< Comparison::isSmaller(x,y) << endl;
  cout << "y<x "<< Comparison::isSmaller(y,x) << endl;

  
  
}
