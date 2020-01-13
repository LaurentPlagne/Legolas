#ifndef __LEGOLAS_BLOCKPROFILE__HXX__
#define __LEGOLAS_BLOCKPROFILE__HXX__

#ifdef LEGOLAS_PROFILE

#include "X86Timer.hxx"
#include "stringConvert.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"

namespace Legolas{
  
  struct BlockProfile{
    X86Timer chronos_;
    const int flag_;
    const Legolas::Matrix & matrix_;

    
    inline BlockProfile(const Matrix & matrix, const int & flag):chronos_(),
								 flag_(flag),
								 matrix_(matrix){
      chronos_.start();
    }
    
    
    ~BlockProfile( void ){
      chronos_.stop();
      if (flag_==1) matrix_.virtualSolver()->storeDuration(chronos_.get_elapsed_time_in_second());
      if (flag_==2) matrix_.virtualMultOperator()->storeDuration(chronos_.get_elapsed_time_in_second());
    }
  };

}

#else // LEGOLAS_PROFILE

namespace Legolas{

  struct BlockProfile{
    template <class MATRIX>
    inline BlockProfile(const MATRIX & matrix, const int & flag){}
  };
}
  
#endif // LEGOLAS_PROFILE 

#endif


  
  
