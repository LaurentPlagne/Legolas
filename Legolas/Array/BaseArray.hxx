#pragma once

namespace Legolas{

  template <class DERIVED_ARRAY>
  struct BaseArray {
    
    DERIVED_ARRAY & getArrayRef( void ) {
      return static_cast<DERIVED_ARRAY &>(*this);
    }

    const DERIVED_ARRAY & getArrayRef( void ) const {
      return static_cast<const DERIVED_ARRAY &>(*this);
    }

    const DERIVED_ARRAY & getConstArrayRef( void ) const {
      return static_cast<const DERIVED_ARRAY &>(*this);
    }
    

  };


  // template <class DERIVED_MULTI_ARRAY>
  // struct BaseMultiArray {
    
  //   DERIVED_MULTI_ARRAY & getArrayRef( void ) {
  //     return static_cast<DERIVED_MULTI_ARRAY &>(*this);
  //   }

  //   const DERIVED_MULTI_ARRAY & getArrayRef( void ) const {
  //     return static_cast<const DERIVED_MULTI_ARRAY &>(*this);
  //   }

  //   const DERIVED_MULTI_ARRAY & getConstArrayRef( void ) const {
  //     return static_cast<const DERIVED_MULTI_ARRAY &>(*this);
  //   }
    

  // };

}
