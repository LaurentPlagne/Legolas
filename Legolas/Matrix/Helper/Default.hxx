/**
 * project  DESCARTES
 *
 * @file     Default.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DEFAULT_HXX__
#define __LEGOLAS_DEFAULT_HXX__

namespace Legolas{

  class Default{};

  template <class DEFAULT, class PROVIDED>
  struct DefaultTraits{
    typedef PROVIDED Type;
  };


  template <class DEFAULT>
  struct DefaultTraits<DEFAULT,Default>{
    typedef DEFAULT Type;
  };

}

#endif
