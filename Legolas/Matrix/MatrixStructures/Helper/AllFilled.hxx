/**
 * project  DESCARTES
 *
 * @file     AllFilled.hxx
 *
 * @author Angelique PONCOT Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ALLFILLED_HXX__
#define __LEGOLAS_ALLFILLED_HXX__

namespace Legolas{

  class AllFilled{
  public:

    AllFilled( int size ):size_(size)
    {
    }
    inline int begin( void ) const {
      return 0;
    }

    inline int end( void ) const {
      return size_;
    }

    inline void next( int & i ) const {
      i++;
      return;
    }

    inline int size( void ) const {
      return size_;
    }


  private:
  
    int size_;
  };
  
}

#endif
    
