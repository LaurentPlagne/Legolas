#ifndef __LEGOLAS_SOURCEFUNCTOR_HXX__
#define __LEGOLAS_SOURCEFUNCTOR_HXX__

template <class REAL_TYPE>
class SourceFunctor{
public:
  typedef REAL_TYPE RealType;

  SourceFunctor(const RealType & omega):omega_(omega){};

  RealType operator () (const RealType & x, const RealType & y) const {
    return -2.0*omega_*omega_*cos(omega_*x)*cos(omega_*y);
  }

  RealType exact(const RealType & x, const RealType & y) const {
    return cos(omega_*x)*cos(omega_*y);
  }
  
private:
  RealType omega_;
};

#endif
  
