/**
 * project  DESCARTES
 *
 * @file     OwnerShipPolicy.cxx
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
# include <vector>

#include "Wrapper.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  typedef double Data;

  Data a=1;

  {
    Wrapper<Data> w(a);
    w.getRef()+=1.0;
    w.display();
    INFOS("a="<<a);
  }

  {
    Wrapper<Data>::Copy w(a);
    w.getRef()+=1.0;
    w.display();
    INFOS("a="<<a);
  }

  {
    Wrapper<Data>::View w(a);
    w.getRef()+=1.0;
    w.display();
    INFOS("a="<<a);
  }

  {
    Wrapper<Data>::ConstView w(a);
    //    w.getRef()+=1.0;
    w.display();
    INFOS("a="<<a);
  }

}
