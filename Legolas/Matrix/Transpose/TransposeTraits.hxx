/**
 * project  DESCARTES
 *
 * @file     TransposeTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TRANSPOSETRAITS_HXX__
#define __LEGOLAS_TRANSPOSETRAITS_HXX__

namespace Legolas{

  template <bool IsTransposed>
  struct TransposeTraits{
  
    static inline const int & getI(const int & i, const int & j){
      return i;
    }

    static inline const int & getJ(const int & i, const int & j){
      return j;
    }
  };

  template <>
  struct TransposeTraits<false>{
  
    static inline const int & getI(const int & i, const int & j){
      return i;
    }

    static inline const int & getJ(const int & i, const int & j){
      return j;
    }
  };

  template <>
  struct TransposeTraits<true>{
  
    static inline const int & getI(const int & i, const int & j){
      return j;
    }

    static inline const int & getJ(const int & i, const int & j){
      return i;
    }
  };

}

#endif
