#ifndef __LEGOLAS_SOURCEFUNCTOR_HXX__
#define __LEGOLAS_SOURCEFUNCTOR_HXX__

template <class REAL_TYPE>
class SourceFunctor{
public:
  typedef REAL_TYPE RealType;

  SourceFunctor(const RealType & omegaX,
		const RealType & omegaY,
		const RealType & omegaZ):omegaX_(omegaX),
					 omegaY_(omegaY),
					 omegaZ_(omegaZ){
  };

  RealType operator () (const RealType & x, const RealType & y, const RealType & z) const {
    const double cx=cos(omegaX_*x);
    const double cy=cos(omegaY_*y);
    const double cz=cos(omegaZ_*z);
    const double cxyz=cx*cy*cz;
    
    return -1.0*(omegaX_*omegaX_+omegaY_*omegaY_+omegaZ_*omegaZ_)*cxyz;
  }

  RealType exact(const RealType & x, const RealType & y, const RealType & z) const {
    return cos(omegaX_*x)*cos(omegaY_*y)*cos(omegaZ_*z);
  }
  
private:
  RealType omegaX_;
  RealType omegaY_;
  RealType omegaZ_;
};

#endif
  
