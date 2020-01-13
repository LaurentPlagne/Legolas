/**
 * project  DESCARTES
 *
 * @file     OwnerShipPolicy.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_OWNERSHIPPOLICY_HXX__
#define __LEGOLAS_OWNERSHIPPOLICY_HXX__

#include "Legolas/Matrix/GenericVector/ReferenceFromTempoIsForbidden.hxx"

namespace Legolas {

  template <class T>
  class Owner{

  public:

    //   template <class U>
    //   Owner( const T & u):data_(){ 
    // 	  cerr << "Owner( const T & u)" << endl;
    //     MESSAGE("Owner U Ctor");
    //   };

    Owner(const T & init)
      :data_(init)
    { //T Copy Ctor
      MESSAGE("Owner Ctor");
    };

    inline const T & getRef( void ) const {
      MESSAGE("Owner getRef ");
      return data_;
    }

    inline T & getRef( void ) {
      MESSAGE("Owner getRef ");
      return data_;
    }

    typedef const T & CtorType ;

    typedef T InitRef ;

    static std::string name( void ) { return "Owner" ;}

    static inline int code( void ){return 1;}

  
  private:
  
    T data_;


  };

  template <class T>
  class NotOwner{
  public:
    
    NotOwner(const T & init):dataRef_(const_cast<T &> (init)){ // Normal Ctor
      MESSAGE("NotOwner  Ctor");
    };

    inline const T & getRef( void ) const {
      MESSAGE("const NotOwner getRef ");
      return dataRef_;
    }

    inline T & getRef( void ) {
      MESSAGE("const NotOwner getRef ");
      return dataRef_;
    }

    typedef const T & CtorType ;

    typedef ReferenceFromTempoIsForbidden InitRef ;

    static std::string name( void ) { return "NotOwner" ;}

    static std::string shortName( const std::string innerName ) {
      return innerName;
    }

    static inline int code( void ){return 2;}

    //   static std::string shortName( const std::string innerName ) {
    //     std::string result="View("+innerName+")";
    //     return result;
    //   }



  private:

    T & dataRef_;
  };


  template <class T>
  class ConstNotOwner{
  
  public:

    ConstNotOwner(const T & init):constDataRef_(init){
      MESSAGE("ConstNotOwner  Ctor");
    };

    inline const T & getRef( void ) const {
      MESSAGE("const constNotOwner getRef ");
      return constDataRef_;
    }


    typedef const T & CtorType ;

    typedef ReferenceFromTempoIsForbidden InitRef ;

    static std::string name( void ) { return "ConstNotOwner" ;}
    static std::string shortName( const std::string innerName ) {
      return innerName;
    }

    static inline int code( void ){return 3; }

    //   static std::string shortName( const std::string innerName ) {
    //     std::string result="ConstView("+innerName+")";
    //     return result;
    //   }

  private:

    const T & constDataRef_;
  };


}

#endif
