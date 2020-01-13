#ifndef __STRINGCONVERT_HXX__
#define __STRINGCONVERT_HXX__ 

#include "UTILITES.hxx"
#include <iostream>
#include <sstream>
#include <string>

// utilities converting "convertible instance of class T into a string (e.g. double, int, long,.

template <class STRING_CONVERTIBLE>
std::string stringConvert(const STRING_CONVERTIBLE & value){

  std::ostringstream s;
  
  if (s << value) {// conversion worked
    
    MESSAGE("conversion OK");
  }
  else{
    INFOS("Conversion failed....");
    exit(0);
  }

  return s.str();
  
}

#endif
