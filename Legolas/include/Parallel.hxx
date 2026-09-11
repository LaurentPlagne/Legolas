#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include "UTILITES.hxx"
#include "StaticScheduler.hxx"
#include "WorkStealing.hxx"

inline int legolas_thread_number( void ){
  /*
   * Récupère dans l'environnement le nombre de threads souhaité
   * via LEGOLAS_NUM_THREADS (ou OMP_NUM_THREADS en repli standard).
   */
  int result=-1;
  const char * pSTN = std::getenv("LEGOLAS_NUM_THREADS");
  if (pSTN == nullptr) {
    pSTN = std::getenv("OMP_NUM_THREADS");
  }
  if (pSTN != nullptr){
    result = std::atoi(pSTN);
    MESSAGE("LEGOLAS_NUM_THREADS=" << result);
  }
  else{
    MESSAGE("LEGOLAS_NUM_THREADS non positionné (-> nthreads==nprocs).");
  }

  return result;
}

namespace Legolas{

  using StaticScheduler::task_scheduler_init;
  using StaticScheduler::parallel_for;
  using StaticScheduler::blocked_range;
  using StaticScheduler::auto_partitioner;
  using StaticScheduler::simple_partitioner;
  using StaticScheduler::static_partitioner;
  using StaticScheduler::split;

  template <class Range, class Functor>
  inline void parallel_reduce(const Range & range, Functor & functor, int part=0){
    functor(range);
  }

  typedef int affinity_partitioner;

  template <class T>
  struct parallel_do_feeder: public std::list<T>{
    parallel_do_feeder(){};
    void add(const T & item){
      this->push_back(item);
    }
  };

  template<typename Iterator, typename Body, typename Item>
  inline void select_parallel_do( Iterator first, Iterator last, const Body& body, void (Body::*)(Item) const){
    std::for_each(first, last, body);
  }

  template<typename Iterator, typename Body, typename Item, typename _Item>
  inline void select_parallel_do( Iterator first, Iterator last, const Body& body, void (Body::*)(Item, parallel_do_feeder<_Item>&) const){
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

} // namespace Legolas

// Official concise project namespace alias:
namespace LGS = Legolas;
