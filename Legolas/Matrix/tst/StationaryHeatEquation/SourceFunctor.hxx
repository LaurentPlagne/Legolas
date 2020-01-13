#ifndef __LEGOLAS_SOURCEFUNCTOR_HXX__
#define __LEGOLAS_SOURCEFUNCTOR_HXX__

#include "BoundaryConditions.hxx"
#include "lambda.hxx"






template <class REAL_TYPE>
class SourceFunctor{
public:
  typedef REAL_TYPE RealType;
  
  RealType dirichletBoundaryForV(const RealType & x, const RealType & y) const {
    const RealType a=BoundaryConditions::a();
    const RealType b=BoundaryConditions::b();
    return (a*x+b*y)*lambda(x,y);
  }

  RealType dirichletBoundaryForT(const RealType & x, const RealType & y) const {
    const RealType a=BoundaryConditions::a();
    const RealType b=BoundaryConditions::b();
    return (a*x+b*y);
  }



};

#endif
  
