#ifndef __LEGOLAS_DYDATA_HXX__
#define __LEGOLAS_DYDATA_HXX__

class DYData : public Legolas::MatrixShape<1> {
public:
  DYData(const Legolas::MatrixShape<1> & ms , 
	 const bool & isNull):Legolas::MatrixShape<1>(ms),
			      diagonalElement_(-4.0),
			      upperDiagonalElement_(1.0){
    if (isNull) diagonalElement_=upperDiagonalElement_=0.0; 
  };
  const double & diagonalElement( void ) const {return diagonalElement_;};
  const double & upperDiagonalElement( void ) const {return upperDiagonalElement_;};
private:
  double diagonalElement_;
  double upperDiagonalElement_;
};

#endif
  
