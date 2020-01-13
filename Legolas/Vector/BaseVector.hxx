#ifndef __LEGOLAS_BASEVECTOR_HXX__
#define __LEGOLAS_BASEVECTOR_HXX__

namespace Legolas{

  struct AnyVector{};

  template <class VECTOR>
  struct BaseVector : public AnyVector {
    
    typedef VECTOR             VectorSkin;
    typedef VectorSkin &       VR;
    typedef const VectorSkin & CVR;
    
    VR getVR( void ) {
      return static_cast<VR>(*this);
    }

    CVR getCVR( void ) const {
      return static_cast<CVR>(*this);
    }
  };

}

#endif
