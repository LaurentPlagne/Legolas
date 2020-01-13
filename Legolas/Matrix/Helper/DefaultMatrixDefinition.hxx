#ifndef __LEGOLAS_DEFAULTMATRIXDEFINITION_HXX__
#define __LEGOLAS_DEFAULTMATRIXDEFINITION_HXX__

//
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
//#include "Legolas/Vector/Shape/ShapeTraits.hxx"

//template <class T>
//struct DefaultPrecision{};


namespace Legolas{

  template <class REAL_TYPE, int LEVEL=1>
  class DefaultMatrixDefinition : public DefaultPrecision<REAL_TYPE>
  {
  public:
    //     static const int level=LEVEL;
//     typedef typename ShapeTraits<level>::Type Shape;
    
//     template <class DATA>
//     static inline Shape getRowShape(const DATA & data){
//       INFOS("The getRowShape static function is missing in the matrix definition");
//       //      throw std::runtime_error("The getRowShape static function is missing in the matrix definition");
//       return Shape();
//     }
  };
}

#endif
