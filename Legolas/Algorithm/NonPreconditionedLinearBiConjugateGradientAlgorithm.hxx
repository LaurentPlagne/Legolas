/**
 * project  DESCARTES
 *
 * @file     NonPreconditionedLinearBiConjugateGradientAlgorithm.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_NONPRECONDITIONEDLINEARBICONJUGATEGRADIENTALGORITHM_HXX__
#define __LEGOLAS_NONPRECONDITIONEDLINEARBICONJUGATEGRADIENTALGORITHM_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
//#include "Legolas/Algorithm/LinearSolverWorkSpace.hxx"
#include "Legolas/Algorithm/LinearSolver.hxx"

namespace Legolas{

  class NonPreconditionedLinearBiConjugateGradientAlgorithm{
  public :

    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
//       TB rk_;
//       TB rtk_;
//       TB pk_;
//       TB ptk_;
//       TX qk_;
//       TX qtk_;
      
//       double rhoKm1_;
//       bool firstCall_;
      
    public:

      typedef typename LinearSolver<TA,TX,TB>::VectorPoolPtr VectorPoolPtr;
      
      //      Engine():LinearSolver<TA,TX,TB>(),rk_(),rtk_(),pk_(),ptk_(),qk_(),qtk_(),rhoKm1_(0.0),firstCall_(true){}
      Engine():LinearSolver<TA,TX,TB>(){}
      Engine(const TA & A, TX & X, const TB & B):LinearSolver<TA,TX,TB>(A,X,B){}
      Engine(const TA & A, TX & X, const TB & B,const VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){}

//       Engine(const TA & A, TX & X, const TB & B):LinearSolver<TA,TX,TB>(A,X,B),rk_(B),rtk_(B),pk_(B),ptk_(B),qk_(X),qtk_(X),rhoKm1_(0.0),firstCall_(true){
// 	rk_-=A*X;
// 	rtk_=rk_;
// 	pk_=rk_;
// 	ptk_=rtk_;
// 	MESSAGE("NonPreconditionedLinearBiConjugateGradientAlgorithm Ctor");
//       }

      //#define NEW_SOLVE
// #ifdef NEW_SOLVE
      
//       void solve(const TA & A, TX & X, const TB & B){
	

// 	typedef typename TA::RealType RealType;
// 	typedef NoLinearSystemAcceleration<TA,TB,TX> Acceleration;
// 	typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;
// 	IterationControl iter(A,B,X);
	
// 	iter.maxIteration()=10;

// 	bool ended=false;

// 	do {
// 	  const double rhoK=Legolas::dot(rk_,rtk_);

// 	  if (!firstCall_){
// 	    const double betaK=rhoK/rhoKm1_;

// 	    pk_*=betaK;
// 	    pk_+=rk_;
// 	    ptk_*=betaK;
// 	    ptk_+=rtk_;
// 	  }
// 	  firstCall_=false;

// 	  qk_=A*pk_;
// 	  qtk_=transpose(A)*ptk_;

// 	  const double alpha=rhoK/Legolas::dot(ptk_,qk_);

// 	  X+=alpha*pk_;

// 	  rk_-=alpha*qk_;
// 	  rtk_-=alpha*qtk_;
// 	  rhoKm1_=rhoK;
// 	  ended=iter.end(X);
	  
	  
// 	}while(!ended);
      
// 	INFOS("Linear BiConjugate Gradient Algoritm has converged within "<<iter.getCurrentIteration()<<" iterations");
//       }

// #else

      void solve(const TA & A, TX & X, const TB & B){
	
	typedef typename TA::RealType RealType;
	typedef NoLinearSystemAcceleration<TA,TB,TX> Acceleration;
	typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;
	IterationControl iter(A,B,X);
	
	//	iter.maxIteration()=20;

	typedef typename LinearSolver<TA,TX,TB>::VectorProvider  VectorProvider;
	VectorProvider vp=this->getVectorProvider();
	
	typename VectorProvider::ReturnType rk=vp.getItem(Legolas::getShape(B));
	typename VectorProvider::ReturnType rtk=vp.getItem(Legolas::getShape(B));
	typename VectorProvider::ReturnType pk=vp.getItem(Legolas::getShape(B));
	typename VectorProvider::ReturnType ptk=vp.getItem(Legolas::getShape(B));
	typename VectorProvider::ReturnType qk=vp.getItem(Legolas::getShape(X));
	typename VectorProvider::ReturnType qtk=vp.getItem(Legolas::getShape(X));
	
	INFOS("Legolas::getShape(B)="<<Legolas::getShape(B));


	//	rk=B-A*X;
	rk=B;

	rk-=A*X;
	rtk=rk;
	
	pk=rk;
	ptk=rtk;

	qk=X;
	qtk=X;

// 	INFOS("rk="<<rk);
// 	INFOS("rtk="<<rtk);

// 	INFOS("pk="<<pk);
// 	INFOS("ptk="<<ptk);

// 	INFOS("qk="<<qk);
// 	INFOS("qtk="<<qtk);


	double rhoKm1=0.0;

	bool ended=false;

	do {

	  //	  zk=rk;
	  //	  ztk=rtk;

	  //	  const double rhoK=Legolas::dot(zk,rtk);
	  const double rhoK=Legolas::dot(rk,rtk);

	  if (iter.getCurrentIteration()!=0){
	    const double betaK=rhoK/rhoKm1;

	    pk*=betaK;
	    //	    pk+=zk;
	    pk+=rk;

	    ptk*=betaK;
	    //	    ptk+=ztk;
	    ptk+=rtk;
	  }

	  qk=A*pk;
	  qtk=transpose(A)*ptk;

	  const double alpha=rhoK/Legolas::dot(ptk,qk);

	  X+=alpha*pk;

	  //	  INFOS("ICI");
	  rk-=alpha*qk;
	  //	  INFOS("ICI");
	  rtk-=alpha*qtk;
	  //	  INFOS("ICI");

	  rhoKm1=rhoK;

	  //	  INFOS("ICI");
	  ended=iter.end(X);
	  //	  INFOS("ICI");
	  
	}while(!ended);

      
	INFOS("Linear BiConjugate Gradient Algoritm has converged within "<<iter.getCurrentIteration()<<" iterations");
      
      }

      //#endif


//       void transposeSolve(const TA & A, TX & X, const TB & B){
// 	INFOS("not yet implemented");
// 	throw std::runtime_error("not yet implemented");
//       }
      

      void transposeSolve(const TA & A, TX & X, const TB & B){
	

	typedef typename TA::RealType RealType;
	typedef NoLinearSystemAcceleration<TA,TB,TX> Acceleration;
	typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;
	IterationControl iter(A,B,X);

	TX rk(B);
	rk-=transpose(A)*X;


	TX rtk(rk);

	//	TX zk(X);
	//	TX ztk(X);

	TX pk(rk);
	TX ptk(rtk);

	TX qk(X);
	TX qtk(X);

	double rhoKm1=0.0;

	do {

	  //	  zk=rk;
	  //	  ztk=rtk;

	  //	  const double rhoK=Legolas::dot(zk,rtk);
	  const double rhoK=Legolas::dot(rk,rtk);

	  if (iter.getCurrentIteration()!=0){
	    const double betaK=rhoK/rhoKm1;

	    pk*=betaK;
	    //	    pk+=zk;
	    pk+=rk;

	    ptk*=betaK;
	    //	    ptk+=ztk;
	    ptk+=rtk;
	  }

	  qk=transpose(A)*pk;
	  qtk=A*ptk;

	  const double alpha=rhoK/Legolas::dot(ptk,qk);

	  X+=alpha*pk;

	  rk-=alpha*qk;
	  rtk-=alpha*qtk;

	  rhoKm1=rhoK;
	  
	}while(!iter.end(X));

      
	INFOS("Transposed Linear BiConjugate Gradient Algoritm has converged within "<<iter.getCurrentIteration()<<" iterations");
      
      }
    
    };

  };

  //   template <class ASSIGN_MODE>
//     class Engine{
//     public:

//       template <class TA,class TX, class TB>
//       static inline void apply(const TA & A, 
// 			       const TX & B,
// 			       TB & X)
//       {
// 	LinearSolverWorkSpace<TX> ws(B);
// 	apply(A,B,X,ws);
//       }

//       template <class TA,class TX, class TB>
//       static inline void apply(const TA & A, 
// 			       const TX & B,
// 			       TB & X,
// 			       LinearSolverWorkSpace<TX> & ws)
//       {

// 	typedef typename TA::RealType RealType;
// 	typedef NoLinearSystemAcceleration<TA,TX,TB> Acceleration;
// 	typedef DefaultLinearSystemIterationControl<Acceleration> IterationControl;
// 	IterationControl iter(A,B,X);

// 	TX rk(B);
// 	rk-=A*X;


// 	TX rtk(rk);

// 	//	TX zk(X);
// 	//	TX ztk(X);

// 	TX pk(rk);
// 	TX ptk(rtk);

// 	TX qk(X);
// 	TX qtk(X);

// 	double rhoKm1=0.0;

// 	do {

// 	  //	  zk=rk;
// 	  //	  ztk=rtk;

// 	  //	  const double rhoK=Legolas::dot(zk,rtk);
// 	  const double rhoK=Legolas::dot(rk,rtk);

// 	  if (iter.getCurrentIteration()!=0){
// 	    const double betaK=rhoK/rhoKm1;

// 	    pk*=betaK;
// 	    //	    pk+=zk;
// 	    pk+=rk;

// 	    ptk*=betaK;
// 	    //	    ptk+=ztk;
// 	    ptk+=rtk;
// 	  }

// 	  qk=A*pk;
// 	  qtk=transpose(A)*ptk;

// 	  const double alpha=rhoK/Legolas::dot(ptk,qk);

// 	  X+=alpha*pk;

// 	  rk-=alpha*qk;
// 	  rtk-=alpha*qtk;

// 	  rhoKm1=rhoK;
	  
// 	}while(!iter.end(X));

      
// 	INFOS("Linear BiConjugate Gradient Algoritm has converged within "<<iter.getCurrentIteration()<<" iterations");
      
//       }
    
//     };

//   };




}

#endif
