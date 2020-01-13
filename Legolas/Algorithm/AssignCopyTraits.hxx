/**
 * project  DESCARTES
 *
 * @file     AssignCopyTraits.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ASSIGNCOPYTRAITS_HXX__
#define __LEGOLAS_ASSIGNCOPYTRAITS_HXX__

#include "UTILITES.hxx"
#include "AssignMode.hxx"

namespace Legolas{

  template <class VECTOR, class ASSIGN_MODE>
  class AssignCopyTraits{
  public:
    typedef VECTOR Type;
  
    template <class VECTOR_LEFT,class VECTOR_RIGHT>
    static inline void assign(VECTOR_LEFT & left,const VECTOR_RIGHT & right){
      INFOS("PROGRAM ERROR!!!!!!!! THIS FUNCTION SHOULD NOT BE CALLED")
	return;
    }
  };

  template <class VECTOR>
  class AssignCopyTraits<VECTOR,EqualAssign>{
  public:
    typedef VECTOR & Type;
  
    template <class VECTOR_LEFT,class VECTOR_RIGHT>
    static inline void assign(VECTOR_LEFT & left,const VECTOR_RIGHT & right){
      return;
    }
  };

  template <class VECTOR>
  class AssignCopyTraits<VECTOR,PlusAssign>{
  public:
    typedef VECTOR Type;
  
    template <class VECTOR_LEFT,class VECTOR_RIGHT>
    static inline void assign(VECTOR_LEFT & left,const VECTOR_RIGHT & right){
      INFOS("ICI");
      left+=right;
      return;
    }
  };


  template <class VECTOR>
  class AssignCopyTraits<VECTOR,MinusAssign>{
  public:
    typedef VECTOR Type;
  
    template <class VECTOR_LEFT,class VECTOR_RIGHT>
    static inline void assign(VECTOR_LEFT & left,const VECTOR_RIGHT & right){
      INFOS("LA");    
      left-=right;
      return;
    }
  };


}

#endif
