#ifndef __LEGOLAS_ANYSOLVER_HXX__
#define __LEGOLAS_ANYSOLVER_HXX__


namespace Legolas{

  struct AnyMatrix;
  struct AnyVector;

  
  struct AnySolver{
    
    virtual ~AnySolver( void ){} ;

    inline std::string name( void ) const { return "AnySolver";}
    
    inline virtual void solve(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B) {
      INFOS("AnySolver::solve not yet implemented");
      throw std::runtime_error("AnySolver::solve not yet implemented");
    }


    inline virtual void transposeSolve(const Legolas::AnyMatrix & A, Legolas::AnyVector & X, const Legolas::AnyVector & B) {
      INFOS("this->name()="<<this->name());
      INFOS("AnySolver::transposeSolve not yet implemented");
      throw std::runtime_error("AnySolver::transposeSolve not yet implemented");      
    }
  };

}

#endif
    
