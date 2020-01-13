#ifndef __LEGOLAS_WRAPPER_HXX__
#define __LEGOLAS_WRAPPER_HXX__

#include "Legolas/Matrix/OwnerShipPolicy.hxx"

namespace Legolas{

  template <class DATA, template <class> class OP=Owner>
  class Wrapper : public OP<DATA>
  {
  public:
  
    typedef DATA                                   Data;
    typedef OP<Data>                               OwnerShipPolicy;
    typedef typename OwnerShipPolicy::CtorType     CtorType;
  
    Wrapper( CtorType init ):OwnerShipPolicy(init){}

    typedef Wrapper                                Copy;
    typedef Wrapper<Data,NotOwner>                 View;
    typedef Wrapper<Data,ConstNotOwner>            ConstView;

    void display( void ){
      INFOS(" display method ");
      std::cout << this->getRef() << std::endl;
    }
  
  };

}

#endif

