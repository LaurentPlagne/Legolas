/**
 * project  DESCARTES
 *
 * @file     RealDataDriver.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_REALDATADRIVER_HXX__
#define __LEGOLAS_REALDATADRIVER_HXX__

#include "Legolas/Matrix/StorageModeTraits/StorageMode.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class RealDataDriver {
  public:
  
    typedef REAL_TYPE RealType;
    typedef REAL_TYPE Data;
    typedef Actual    StorageMode;

    static const int level=0;

    static inline void copy( const Data & input, Data & data){
      data=input;
    }
    //   static inline REAL_TYPE dot( const REAL_TYPE & left, const REAL_TYPE & right){
    //     return left*right;
    //   }

 //    static inline REAL_TYPE dot(const REAL_TYPE & left, const REAL_TYPE & right){
//       REAL_TYPE result=left*right;
//       return result;
//     }

//     template <class DATA>
//     static inline REAL_TYPE zero(const DATA & data){ return 0.0;}




  };
}

#endif
  
