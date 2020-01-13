#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include "UTILITES.hxx"
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include <hptt.h>

int main(int argc, char *argv[]) {
    typedef double RealType;


    INFOS("TensorTransposeTest");

    const int dim = 3;

    const int nx = 100;
    const int ny = 100;
    const int nz = 100;
    //  const int nx=200;
    //  const int ny=800;
    //  const int nz=800;

    typedef Legolas::Array<RealType, 3> A3D;

    A3D A(nz,ny,nx);
    A3D B(nz,ny, nx);
    
    INFOS(" A%32="<<((uintptr_t)(A.realDataPtr()) % 32));
    INFOS(" B%32="<<((uintptr_t)(A.realDataPtr()) % 32));
    

    for (int k=0 ; k<nz ; k++){
          for (int j=0 ; j<ny ; j++){
	        for (int i=0 ; i<nx ; i++){
		  A[k][j][i]=RealType(i+nx*j+ny*nx*k);
		}
	  }
    }

    INFOS("A[0][0]="<<A[0][0]);
    
    //    A.fill(1.0);
    B.fill(0.0);
    
    // specify permutation and size

    int perm[dim];
    perm[0]=2; perm[1]=1; perm[2]=0; 
    int size[dim];
    size[0]=nz; size[1]=ny; size[2]=nx;
    //    = {ny, nx};
    
    RealType alpha=1.0;
    RealType beta=0.0;

    const int numThreads=4;
    //B <- alphaA+ beta B
    // create a plan (shared_ptr)
    auto start_time = std::chrono::high_resolution_clock::now();
    auto plan = hptt::create_plan(perm, dim,
            alpha, A.realDataPtr(), size, NULL,
            beta, B.realDataPtr(), NULL,
            hptt::ESTIMATE, numThreads);
    //    hptt::ESTIMATE, numThreads);
    //    hptt::ESTIMATE, numThreads);
    auto end_time = std::chrono::high_resolution_clock::now();
    double microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    INFOS(" transposition plan duration:"<<(microsec/1000000.)<<"(s)");
    // execute the transposition
    start_time = std::chrono::high_resolution_clock::now();
    const int nloop=3100;
    const bool useStream=false;
    const bool useThreads=false;
    const bool betaIsNull=true;
    for (int i=0 ; i<nloop ; i++){
      //plan->execute();
      plan->execute_expert<useStream,useThreads,betaIsNull>();
    }
    end_time = std::chrono::high_resolution_clock::now();
    microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    INFOS(" transposition execution:"<<(microsec/1000000.)<<"(s)");
    double GBs=double(nloop)*double(nx*ny*nz)*sizeof(RealType)/(microsec*1000.);
    INFOS("GBs="<<GBs);
    
    INFOS("B[0][0]="<<B[0][0]);
    
    start_time = std::chrono::high_resolution_clock::now();
    for (int i=0 ; i<nloop ; i++){
      B=alpha*A+beta*B || Legolas::par;
    }
    end_time = std::chrono::high_resolution_clock::now();
    microsec=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    INFOS(" copy execution:"<<(microsec/1000000.)<<"(s)");
    GBs=double(nloop)*double(nx*ny*nz)*sizeof(RealType)/(microsec*1000.);
    INFOS("GBs="<<GBs);
    INFOS("B[0][0]="<<B[0][0]);
}




