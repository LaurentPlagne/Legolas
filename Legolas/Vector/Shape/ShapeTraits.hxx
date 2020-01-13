#ifndef __LEGOLAS_SHAPETRAITS_HXX__
#define __LEGOLAS_SHAPETRAITS_HXX__

//#include "Legolas/Vector/Shape/CompressedVector.hxx"
#include "Legolas/Vector/Shape/COWCompressedVector.hxx"
#include "Legolas/Common/BasicTypes.hxx"
#include "Legolas/Vector/Shape/L1Shape.hxx"

namespace Legolas{

  template <int LEVEL>
  struct ShapeTraits{
    typedef typename ShapeTraits<LEVEL-1>::Type Element;
    typedef COWCompressedVector< Element > Type;
  };
  
  template <>
  struct ShapeTraits<1>{
    typedef L1Shape  Type;
  };

  // template <>
  // struct ShapeTraits<0>{
  //   typedef std::string  Type;
  // };

  // template <>
  // struct ShapeTraits<1>{
  //   typedef SizeType  Type;
  // };

  // template <>
  // struct ShapeTraits<0>{
  //   typedef SizeType  Type;
  // };


//   template <int LEVEL>
//   struct ShapeTraits{
//     typedef typename RecursiveShapeTraits<LEVEL>::Type Type;
//   };

//   template <>
//   struct ShapeTraits<1>{
//     typedef SizeType Type;
//     //    typedef long Type; //LP : WorkAround avoiding to overload the SizeType Ctor... 
//   };


}

#endif

  



