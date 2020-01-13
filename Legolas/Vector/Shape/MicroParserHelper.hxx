#ifndef __LEGOLAS_MICROPARSERHELPER_HXX__
#define __LEGOLAS_MICROPARSERHELPER_HXX__

#include "UTILITES.hxx"
#include <cstdlib>
#include "L1Shape.hxx"

namespace Legolas{

  template <class STRING, class ELEMENT>
  struct StringToElementCtorTraits{
    static const STRING apply(const STRING & aString){
      return aString;
    }
  };
  
  template <class STRING>
  struct StringToElementCtorTraits<STRING,unsigned int>{
    static unsigned int apply(const STRING & aString){
      return atoi(aString.c_str());
    }
  };
  
  template <class STRING>
  struct StringToElementCtorTraits<STRING,int>{
    static int apply(const STRING & aString){
      return atoi(aString.c_str());
    }
  };

  template <class STRING>
  struct StringToElementCtorTraits<STRING,L1Shape>{
    static  L1Shape apply(const STRING & aString){
      return atoi(aString.c_str());
    }
  };

  
  template <class STRING>
  struct MicroParserHelper{

    typedef STRING String;

    static inline unsigned int stringToUI(const String & aString){
      unsigned int result=atoi(aString.c_str());
      return result;
    }

    template <class ELEMENT>
    struct StringToElementCtor{
      static ELEMENT apply(const String & aString){
	return StringToElementCtorTraits<String,ELEMENT>::apply(aString);
      }
    };

    static bool doesBeginWith(const String & s,
		       const String & b){
      return (s.find(b)==0);
    }

    static bool doesEndWith(const String & s,
		     const String & e){
      return (s.rfind(e)==(s.size()-e.size()));
    }

    static void eraseBeginning(String & s,
			const String & b){
      ASSERT(doesBeginWith(s,b));
      s.erase(0,b.size());
    }

    static void eraseEnding(String & s,
		     const String & e){
      ASSERT(doesEndWith(s,e));
      s.erase(s.size()-e.size(),e.size());
    }
    
    static String extractFirstList(String & chaine){

      ASSERT(chaine.size()>1);
      ASSERT(chaine[0]=='(');
      
      unsigned int i=1;
      unsigned int openingBracket=1;

      while ( (openingBracket >0 ) && (i < chaine.size()))
	{
	  if ( chaine[i]==')')  openingBracket--;
	  if ( chaine[i]=='(')  openingBracket++;
	  i++;
	}

      ASSERT(chaine[i-1]==')');

      String result;
    
      result.insert(0,chaine,0,i);
      chaine.erase(0,i);
    
      return result;
    }


    static String extractFirstUnsignedInteger(String & chaine){

      ASSERT(chaine.size()>0);
      int lastDigitIndex=chaine.size()-1;

      int firstNonDigitIndex=chaine.find_first_not_of("0123456789");
    
      if ( ( firstNonDigitIndex >=0) &&  firstNonDigitIndex < int(chaine.size()) ){
	lastDigitIndex=firstNonDigitIndex-1;
      }


      String result;

      result.insert(0,chaine,0,lastDigitIndex+1);
      chaine.erase(0,lastDigitIndex+1);
    
      return result;
    }
  };

}
#endif

  
  
