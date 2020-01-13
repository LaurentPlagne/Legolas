#ifndef __LEGOLAS_SCALARTRAITS_HXX__
#define __LEGOLAS_SCALARTRAITS_HXX__

namespace Legolas{

  template <class REAL_TYPE, class VECTOR_ELEMENT>
  struct ScalarTraits{
    typedef REAL_TYPE Type;
  };

  template <class REAL_TYPE>
  struct ScalarTraits<REAL_TYPE,double>{
    typedef double Type;
  };

  template <class REAL_TYPE>
  struct ScalarTraits<REAL_TYPE,float>{
    typedef float Type;
  };

}

#endif

    




    
    
