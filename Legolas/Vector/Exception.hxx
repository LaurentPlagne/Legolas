#ifndef __LEGOLAS_EXCEPTION_HXX
#define __LEGOLAS_EXCEPTION_HXX

#include "UTILITES.hxx"
#include <iostream>
#include <sstream>
#include <string>
#include <exception>

//#define HERE    {  __FILE__ << " [" << __LINE__ << "] : "  }
//#define THROW(chain) { HEREWEARE ; std::cerr << std::setprecision(10) << chain << std::endl ;}

#define HERE    {  __FILE__ << " [" << __LINE__ << "] : "  }
#define LEGOLAS_THROW(chain) { std::ostringstream s ; s << __FILE__ << " [" << __LINE__ << "] : "; s << chain ; std::string result(s.str()) ; throw std::domain_error(result.c_str()) ;}


namespace Legolas{

  class Exception: public std::exception
  {
  public:
    Exception(std::ostringstream s) DK_NOEXCEPT_TRUE :m_phrase(s.str())
    {}
    
    virtual const char* what() const DK_NOEXCEPT_TRUE
    {
      return m_phrase.c_str();
    }
    
    
    virtual ~Exception() DK_NOEXCEPT_TRUE
    {}
    
  private:
    std::string m_phrase;       // Description de l'erreur.
  };
}
#endif

