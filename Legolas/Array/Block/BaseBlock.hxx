#pragma once

namespace Legolas {


    template <class DERIVED_BLOCK>
    struct BaseBlock{

        DERIVED_BLOCK & getBlockRef( void ) {
            return static_cast<DERIVED_BLOCK &>(*this);
        }

        const DERIVED_BLOCK & getBlockRef( void ) const {
            return static_cast<const DERIVED_BLOCK &>(*this);
        }

        const DERIVED_BLOCK & getConstBlockRef( void ) const {
            return static_cast<const DERIVED_BLOCK &>(*this);
        }


    };

}
