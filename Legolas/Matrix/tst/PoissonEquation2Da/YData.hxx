#ifndef __LEGOLAS_YDATA_HXX__
#define __LEGOLAS_YDATA_HXX__

#include "Legolas/Matrix/MatrixShape.hxx"

class YData : public Legolas::MatrixShape<1> {
public:
  YData(const Legolas::MatrixShape<1> & ms , 
	bool isDiagonal):Legolas::MatrixShape<1>(ms),
			 diagonalElement_(0.0),
			 upperDiagonalElement_(0.0),
			 lsup_(0)
  {
    if (isDiagonal){
      diagonalElement_=-4.0;	
      upperDiagonalElement_=1.0; 
      lsup_=1;
    }
    else{
      diagonalElement_=1.0;
    }
  };

  const double & diagonalElement( void ) const {return diagonalElement_;};
  const double & upperDiagonalElement( void ) const {return upperDiagonalElement_;};
  const int & lsup ( void ) const { return lsup_ ;};
  
private:
  double diagonalElement_;
  double upperDiagonalElement_;
  int lsup_;
};

#endif
  
