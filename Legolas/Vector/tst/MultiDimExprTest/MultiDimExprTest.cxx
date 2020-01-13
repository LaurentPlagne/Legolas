#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include <iostream>
#include <cstdio>

using Legolas::SizeType;

int main( int argc,  char *argv[] )
{

  Legolas::Vector<double> v(10);
  INFOS("v="<<v);
  v=1.0;
  Legolas::Vector<double> v2(v);
  INFOS("v2="<<v2);
  v2+=v;
  INFOS("v2="<<v2);

  
}




    
