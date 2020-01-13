#ifndef __VIRTUALVECTORSHAPE_HXX__
#define __VIRTUALVECTORSHAPE_HXX__

namespace Legolas{

  class VirtualVectorShape{
  public:
    virtual int level( void ) const =0;
    virtual size_t subFlatSize( int i ) const =0; 
    virtual size_t flatSize( void ) const =0; 

    virtual ~VirtualVectorShape(){}
  };
}

#endif
