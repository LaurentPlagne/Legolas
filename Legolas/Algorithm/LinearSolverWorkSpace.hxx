#ifndef __LEGOLAS_LINEARSOLVERWORKSPACE_HXX__
#define __LEGOLAS_LINEARSOLVERWORKSPACE_HXX__

namespace Legolas{

  class AnyWorkSpace{};

  template <class VECTOR>
  class LinearSolverWorkSpace : public AnyWorkSpace{
  public:

    typedef typename VECTOR::Element VectorElement;
    
    LinearSolverWorkSpace(const VECTOR & B):s_(B[0]),
					    subLinearSolverWorkSpace_(B[0])
    {
      INFOS("ICI LEVEL="<<B.level());
    }
    
    VectorElement & getVectorElement( void ){ return s_;}
    LinearSolverWorkSpace<VectorElement> & getSubLinearSolverWorkSpace( void ){ return subLinearSolverWorkSpace_;}
    
  private:
    
    VectorElement s_;
    LinearSolverWorkSpace<VectorElement> subLinearSolverWorkSpace_;
  };

  template <>
  class LinearSolverWorkSpace<double>: public AnyWorkSpace{
  public:
    LinearSolverWorkSpace(const double & value){};
  };

  template <>
  class LinearSolverWorkSpace<float>: public AnyWorkSpace{
  public:
    LinearSolverWorkSpace(const float & value){};
  };
}

#endif



