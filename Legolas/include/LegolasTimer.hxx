#pragma once

#if !defined(_WIN32) && !defined(_MSC_VER)
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/times.h>
#endif
#include "HardwareTimer.hxx"
#include "UTILITES.hxx"
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include "SingletonTimerTable.hxx"
#include "Parallel.hxx"

class LegolasTimer {

public:

  typedef Legolas::tick_count ClickType;
  typedef std::map<ClickType,ClickType> CountedClickType;

  inline void improveAccuracy( void ){
    _residualTime=1.e10;
    for (int i=0 ; i < 100 ; i++){
      this->start();
      this->stop();
      const double this_time=this->get_raw_elapsed_time_in_second();
      if (this_time< _residualTime)  _residualTime=this_time;
    }
  }

  LegolasTimer( void ):_residualTime(0.0)
  {
    MESSAGE("LegolasTimer Default Ctor");
  }

  inline void start( void ){
    _click_start=Legolas::tick_count::now();
  }

  inline void stop( void ){
    _click_stop=Legolas::tick_count::now();
  }

  inline void storeResult(const std::string operation_name){
    _Table.add_measurement(operation_name,get_elapsed_time_in_second());
  }

  inline double get_elapsed_time_in_second( void ){
    return (_click_stop-_click_start).seconds()-_residualTime;
  }

  inline double get_raw_elapsed_time_in_second( void ){
    return (_click_stop-_click_start).seconds();
  }

private:

  double _residualTime;
  ClickType _click_start;
  ClickType _click_stop;
  static SingletonTimerTable _Table;

};
