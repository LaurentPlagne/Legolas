#ifndef __LEGOLAS_RYDATA_HXX__
#define __LEGOLAS_RYDATA_HXX__

class RYData : public Legolas::MatrixShape<1> {
public:
  RYData(const Legolas::MatrixShape<1> & ms , 
	 const bool & isNull):Legolas::MatrixShape<1>(ms),
			      diagonalElement_(1.0){
    if (isNull) diagonalElement_=0.0; 
  };
  const double & diagonalElement( void ) const {return diagonalElement_;};
private:
  double diagonalElement_;
};

#endif
  
