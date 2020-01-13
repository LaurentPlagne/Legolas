#ifndef __MYTBBPARALLELFOR_HXX__
#define __MYTBBPARALLELFOR_HXX__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
//#include <stdio.h>
//#include <stdlib.h>
#include "dkconfig.hxx"	/* pour récupérer la valeur de la macro USING_TBB utilisée plus bas */
#include "UTILITES.hxx"
#include <chrono>

//#define USING_TBB 0

inline int spn_thread_number( void ){
  /*
   * la fonction spn_thread_number récupère dans l'environnement la valeur
   * positionnant le paramètre shell SPN_THREAD_NUMBER (s'il a été positionné)
   * pour configurer l'utilisation des TBB.
   */
  int result=-1; /* valeur par défuat si SPN_THREAD_NUMBER n'est positionné */
  char * pSTN=0;
  pSTN = getenv ("SPN_THREAD_NUMBER");
  if (pSTN!=NULL){
    //      printf ("The current path is: %s",pPath);
    result=atoi(pSTN);
    MESSAGE("SPN_THREAD_NUMBER="<<result);
  }
  else{
    MESSAGE("SPN_THREAD_NUMBER est non positionné (-> nthreads==nprocs).");
  }

  return result;
}



namespace no_tbb{


  struct task_scheduler_init{
    int i_;
    task_scheduler_init(int i):i_(i){}

    static int default_num_threads() { return 1 ; }
    inline void terminate( void ) const {}

  };

  template <class Range, class Functor>
  void parallel_for(const Range & range, const Functor & functor,int part=0){
    functor(range);
  }

  template <class Index, class Functor>
  void parallel_for(Index first, Index last, const Functor & functor,int part=0){
    for (Index i=first ; i<last ; i++){
      functor(i);
    }
  }

  template <class Range, class Functor>
  void parallel_reduce(const Range & range, Functor & functor,int part=0){
    functor(range);
  }

  typedef int affinity_partitioner;
  typedef int auto_partitioner;
  typedef int simple_partitioner;
  typedef int static_partitioner;

  struct split{};


  template <class T>
  struct blocked_range{
    T begin_;
    T end_;
    int grainSize_;
    blocked_range(T begin, T end, int grainSize=1):
            begin_(begin),
            end_(end),
            grainSize_(grainSize)
    {
    }

    const T & begin( void ) const { return begin_;}
    const T & end( void ) const { return end_;}
  };


  template <class T>
  struct parallel_do_feeder: public std::list<T>{
    parallel_do_feeder(){};
    void add(const T & item){
      this->push_back(item);
    }
  };



  template<typename Iterator, typename Body, typename Item>
  inline void select_parallel_do( Iterator first, Iterator last, const Body& body, void (Body::*)(Item) const){
    std::for_each(first, last, body) ;
  }

  template<typename Iterator, typename Body, typename Item, typename _Item>
  inline void select_parallel_do( Iterator first, Iterator last, const Body& body, void (Body::*)(Item, parallel_do_feeder<_Item>&) const){
    //    typedef typename Body::argument_type Item;
    parallel_do_feeder<_Item> feeder;

    while (first!=last){
      body(*first,feeder);
      first++;
    }

    typename parallel_do_feeder<Item>::iterator b=feeder.begin();
    typename parallel_do_feeder<Item>::iterator e=feeder.end();
    while (b!=e){
      body(*b,feeder);
      b++;
      e=feeder.end();
    }
  }


  template <class FORWARD_ITERATOR, class Body>
  inline void parallel_do(FORWARD_ITERATOR first, const FORWARD_ITERATOR & last, Body body) {
    if (first!=last){
      select_parallel_do(first,last,body,&Body::operator());
    }
  }

  template <class T>
  class atomic{
    T data_;
  public:

    T & data(){ return data_;}
    const T & data() const { return data_;}

    atomic(const T & t):data_(t){};
    atomic( void ):data_(){};
    atomic(const atomic<T> & t):data_(t){};

    atomic<T> operator + (const atomic<T> & right){ return atomic<T>(this->data_+right.data_); }
    atomic<T> operator - (const atomic<T> & right){ return atomic<T>(this->data_-right.data_); }
    atomic<T> operator + (const T & right){ return atomic<T>(this->data_+right); }
    atomic<T> operator - (const T & right){ return atomic<T>(this->data_-right); }

    bool operator ==(const atomic<T> & right) const { return (this->data_==right.data_); }
    bool operator !=(const atomic<T> & right) const { return (this->data_!=right.data_); }
    bool operator >(const atomic<T> & right) const { return (this->data_>right.data_); }
    bool operator <(const atomic<T> & right) const { return (this->data_<right.data_); }

    bool operator ==(const T & right) const { return (this->data_==right); }
    bool operator !=(const T & right) const { return (this->data_!=right); }
    bool operator >(const T & right) const { return (this->data_>right); }
    bool operator <(const T & right) const { return (this->data_<right); }

    atomic<T> & operator= (const T & right){data_=right ; return *this ; }
    atomic<T> & operator= (const atomic<T> & right){data_=right.data_ ; return *this ; }

    atomic<T> & operator+= (const T & right){data_+=right ; return *this ; }
    atomic<T> & operator+= (const atomic<T> & right){data_+=right.data_ ; return *this ; }

    atomic<T> & operator-= (const T & right){data_-=right ; return *this ; }
    atomic<T> & operator-= (const atomic<T> & right){data_-=right.data_ ; return *this ; }

    atomic<T> & operator++ ( void ){data_++ ; return *this ; }
    atomic<T> & operator++ ( int ){T ans=data_ ; ++data_ ; return atomic<T>(ans) ; }

    atomic<T> & operator-- ( void ){data_-- ; return *this ; }
    atomic<T> & operator-- ( int ){T ans=data_ ; --data_ ; return atomic<T>(ans) ; }
  };

  template <class T> bool operator == (const T & a,  const atomic<T> & b){ return (a==b.data());}
  template <class T> bool operator != (const T & a,  const atomic<T> & b){ return (a!=b.data());}
  template <class T> bool operator > (const T & a,  const atomic<T> & b){ return (a>b.data());}
  template <class T> bool operator < (const T & a,  const atomic<T> & b){ return (a<b.data());}





  template <class T>
  class concurrent_vector: public std::vector<T>{
  public:
    concurrent_vector( void ){};
  };

  template <class TC>
  class tick_count_interval{
  public:
    TC begin_;
    TC end_;
    tick_count_interval(const TC & begin, const TC & end):begin_(begin),end_(end){}
    double seconds( void ){
      auto nanoseconds=std::chrono::duration_cast<std::chrono::nanoseconds>(end_.getBase()-begin_.getBase()).count() ;
      return double(nanoseconds)/1.e9;
    }

  };


//typedef std::chrono::time_point<std::chrono::system_clock> BaseTimer;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<Clock> BaseTimer;

class tick_count : public BaseTimer{
  public:
  tick_count( const BaseTimer & other):BaseTimer(other){}
  tick_count( const tick_count & other):BaseTimer(other){}

  const BaseTimer & getBase( void ){ return static_cast<const BaseTimer &>(*this);}

  tick_count( void ):BaseTimer(){}

    static inline tick_count now( void ){ return Clock::now() ;}
  inline tick_count_interval<tick_count> operator - (const tick_count & right){
    return tick_count_interval<tick_count>(right,*this);
    }
  };

  template <class T>
  struct enumerable_thread_specific{
    typedef T & reference;
    T data_;
    reference local( void ){ return data_;}
  };

}//my_tbb



#if ! defined (USING_TBB)	/* macro définie dans dkconfig.hxx */
#error C macro USING_TBB is not defined
#endif	/* #if ! defined (USING_TBB) */


#if USING_TBB == 1		/* macro définie dans dkconfig.hxx */

#define TBB_PREVIEW_STATIC_PARTITIONER 1
#include "tbb/tbb.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "tbb/partitioner.h"

namespace my_tbb=tbb;

#elif USING_TBB == 0

#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
#warning  USING_TBB est positionnÃ©e a 0
namespace my_tbb=no_tbb;

#else	/* #if USING_TBB == 1 */

#error invalid C macro USING_TBB value

#endif	/* #if USING_TBB == 1 */
#endif
