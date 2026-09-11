#pragma once

#include "dkconfig.hxx"
#include "SingletonTimerTable.hxx"

#if defined( USING_X86_TIMER ) && USING_X86_TIMER == 1

#include "LegolasTimer.hxx"
#define X86Timer LegolasTimer

#else

#include <string>

class X86Timer {
public :
  inline X86Timer( void ) {}
  inline void start( void ) {}
  inline void stop( void ) {}
  inline void storeResult(const std::string &) {}
  inline double get_elapsed_time_in_second( void ){ return -1.0; }
  inline void find_frequency( void ) {}
  inline double frequency( void ){ return -1.0; }
  inline void add_get_click( void ) {}
  inline void dump_statistics(std::string) {}
  inline void dump_history(std::string) {}

private :
  static SingletonTimerTable _Table;
};

#endif
