#ifndef __LEGOLAS_YDATA_HXX__
#define __LEGOLAS_YDATA_HXX__

#include "Legolas/Matrix/Helper/Zero.hxx"

class YData{
public:
  
  YData(int nrows):nrows_(nrows),isNull_(false){};
  YData(int nrows, Zero zero):nrows_(nrows),isNull_(true){};
  
  const int & nrows( void ) const { return nrows_ ;};
  const bool & isNull( void ) const {return isNull_ ;};
  
private:

  int  nrows_;
  bool isNull_;
};

#endif
  
