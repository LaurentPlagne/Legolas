#ifndef __MATRIXOPERATOR_HXX__
#define __MATRIXOPERATOR_HXX__

namespace Legolas{

  class OperatorStatistics{
    double duration_;
    size_t counter_;
  public:
    
    inline OperatorStatistics( void ):duration_(0.0),counter_(0){}
    
    inline double & duration( void ){ return duration_;}
    inline const double & duration( void )const{ return duration_;}
    inline size_t & counter( void ){ return counter_;}
    inline const size_t & counter( void )const{ return counter_;}
  };
  
  class MatrixOperator{
    Legolas::OperatorStatistics operatorStatistics_;
  public:
    
    virtual inline std::string name( void ) const { return "Anonymous" ;}
    
    inline MatrixOperator( void ):operatorStatistics_(){};
    
    inline const double & duration( void ) const { return operatorStatistics_.duration();}
    inline const size_t & counter( void ) const { return operatorStatistics_.counter();}
    

    inline void storeDuration(const double & time){
      operatorStatistics_.duration()+=time;
      operatorStatistics_.counter()++;
    }

    virtual inline ~MatrixOperator( void ) {} ;

    
  };
  
}

#endif
    
    
    
    

