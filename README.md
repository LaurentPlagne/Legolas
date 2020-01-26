# Legolas++

*Building Blocks for Linear Algebra Solvers*

##What is it ?
Legolas++ helps to create an manipulate linear algebra problems involving multi-level block structured matrices.
Those matrices operate on N-Dimensional arrays (tensors) operands.

Legolas++ allows for automatic vectorization and parallelization of algorithms involving ND Legolas++ arrays.

 

## Compile on linux
The array part of Legolas++ is an header only library. 
The following instructions should allow for the execution of the examples of the publication : 

<a id="1">[1]</a> 
PLAGNE, Laurent et BOJNOURDI, Kavoos. Portable vectorization and parallelization of c++ multi-dimensional array computations. In : Proceedings of the 4th ACM SIGPLAN International Workshop on Libraries, Languages, and Compilers for Array Programming. 2017. p. 33-39.


For more details, the corresponding slides are located in this directory: Legolas/presentation.pdf.

### Using CLion
Th CLion configuration is added to this directory. You can edit and adapt the CLion settings (compilers toolkits and 
position of the Eigen3 and Intel TBB libraries) from the CLion interface.
### From a terminal
- install Eigen3 (see <http://eigen.tuxfamily.org>)
- install Intel TBB (<https://www.threadingbuildingblocks.org>)
- Considering that a C++ compiler is installed on you system :
    
    - LEGOLAS_ROOT is the directory containing this README.md file
    
    ```bash

    $ cd LEGOLAS_ROOT
    $ mkdir my_build
    $ cd my_build
    ```
  
     - MY_EIGEN_DIR: Eigen3 instal dir containing "Eigen/Core" file
     - MY_TBB_DIR: TBB install dir containing "include/tbb/tbb.h"
     - Note: you may change the CMAKE_BUILD_TYPE=Release to CMAKE_BUILD_TYPE=Debug
     
    ```bash
    $ cmake .. -DEIGEN3_INCLUDE_DIR=MY_EIGEN_DIR  -DCMAKE_PREFIX_PATH=MY_TBB_DIR -DCMAKE_BUILD_TYPE=Release
    ```
  
  - Compile the MultiThomas benchmark example
  
    ```bash
    $ make MultiThomas
    ```
   - Launch the MultiThomas benchmarks (it takes a few minutes)
   
    ```bash
    $ ./tst/MultiThomas/MultiThomas 
    ```
    
  - This should produce the corresponding performance measurements files :
  
    ```bash
    $ cd ./tst/MultiThomas ; ls
    $ Thomas_1_parallel.dat    Thomas_4_parallel.dat    Thomas_8_parallel.dat
    $ Thomas_1_sequential.dat  Thomas_4_sequential.dat  Thomas_8_sequential.dat

    ```
    
    Provided you have python, python-numpy, python-matplotlib and texlive-full packages installed on your
    system you can obtain this kind of curves (here on a 4cores/4GHz Intel 6700 K) :
    
    ```bash
    $ python  ../../../tst/MultiThomas/plotPerf.py    
    ```
    
    ![](Thomas6.png)
    
    Note that you can also obtain, curves for different thread numbers with :
    
    ```bash
    $ python  ../../../tst/MultiThomas/plotSpeddUp.py    
    ```
    
    ![](Thomas_SpUp8.png)
    
    # Test of Legolas++ Static Arrays
    
    - Compile the StaticArrays benchmark example
    
    ```bash
    $ cd LEGOLAS_ROOT
    $ make StaticArrays
    ```
    
   - Launch the StaticArray benchmarks
   
    ```bash
    $ ./tst/StaticArrays/StaticArrays 
    $   a+=b+c S=16 : GFlops=20.48 (residual=0)
    $    a+=b+c S=16 : GFlops=20.48 (residual=0)
    $    a+=b+c S=16 : GFlops=20.48 (residual=0)
    $    0 3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 
    $    Y+=X*a S=16 : GFlops=25.6 (residual=0)
    $    Y+=X*a S=16 : GFlops=25.6 (residual=0)
    $    Y+=X*a S=16 : GFlops=25.6 (residual=0)
    $    0 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 
    $    Y+=a*X S=16 : GFlops=25.6 (residual=0)
    $    Y+=a*X S=16 : GFlops=25.6 (residual=0)
    $    Y+=a*X S=16 : GFlops=25.6 (residual=0)
    $    0 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60
    ```
  
    
  
  
   Note that you can use the Legolas++ StaticArray in place of Eigen3 for the standard Legolas++ arrays used in the MultiThomas benchmark (see previous section) by commenting the line 293 in Legolas/Array/Array.hxx
    
   ```cpp
    #define USE_EIGEN //Line 293
    #ifdef USE_EIGEN
        typedef Eigen::Array<double,PACK_SIZE,1> PackedDoubleType;
        typedef Eigen::Array<SCALAR_TYPE,PACK_SIZE,1> PackedRealType;
    #else
        typedef StaticArray<double,PACK_SIZE> PackedDoubleType;
        typedef StaticArray<SCALAR_TYPE,PACK_SIZE> PackedRealType;
    #endif
   ```
  
   In this case, one could remove the Eigen3 Legolas's dependency but, up to now, the performances are not satisfactory. Analysing the produced native codes, the vectorization is not done correctly while it is OK with the StaticArray tests...
    

 