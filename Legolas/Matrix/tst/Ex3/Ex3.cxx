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

#include "Comparison.hxx"
using namespace std; 
int main( int argc,  char *argv[] )
{
  {
    double x=1.0;
    double y=2.0;
  
    cout << "x<y "<< Comparison<double>::isSmaller(x,y) << endl;
    cout << "y<x "<< Comparison<double>::isSmaller(y,x) << endl;
  }
  {
    int x=1;
    int y=2;
  
    cout << "x<y "<< Comparison<int>::isSmaller(x,y) << endl;
    cout << "y<x "<< Comparison<int>::isSmaller(y,x) << endl;
  }
}
