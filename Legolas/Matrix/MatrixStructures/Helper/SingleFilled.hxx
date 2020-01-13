/**
 * project  DESCARTES
 *
 * @file     SingleFilled.hxx
 *
 * @author Angelique PONCOT Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SINGLEFILLED_HXX__
#define __LEGOLAS_SINGLEFILLED_HXX__

namespace Legolas{

  class SingleFilled{
  public:

    SingleFilled(int index ):index_(index),indexP1_(index+1)
    {
    }

    SingleFilled(int index, int ncols ):index_(index),indexP1_(std::min(index+1,ncols))
    {
    }

    inline int begin( void ) const {
      return index_;
    }

    inline int end( void ) const {
      return indexP1_;
    }

    inline void next( int & i ) const {
      i++;
      return;
    }

  private:
    int index_;
    int indexP1_;
  };
  
}

#endif
    
