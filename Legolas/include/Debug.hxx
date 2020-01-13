#ifndef __LEGOLAS_DEBUG_HXX__
#define __LEGOLAS_DEBUG_HXX__

#include "UTILITES.hxx"
#include <iostream>
#include <sstream>


#define DK_DEBUG( level, message ) \
	{ std::ostringstream str;  \
	  str << message << std::ends ; \
	  DK::Debug m( level, __FILE__, __LINE__ );  \
	  m << str.str() ; }



namespace DK{
  
  struct Debug{
    
    template <class INTEGER>
    Debug( INTEGER level, const char *file, long line, std::ostream &os = std::cerr ){
      
    }
    
    static int GetLevel( void ){ return 1; }
    
  };



}
#endif
