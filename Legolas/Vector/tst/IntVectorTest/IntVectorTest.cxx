#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"


using Legolas::SizeType;

int main( int argc,  char *argv[] )
{

  SizeType s=10;

  Legolas::Vector<double> vi(s);

  INFOS("vi="<<vi);

  vi[1]=2;

  INFOS("vi="<<vi);

}




    
