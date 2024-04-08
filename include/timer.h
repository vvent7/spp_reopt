#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class timer {
public:
  timer();
  timer(double *total);
  ~timer();
  void restart();
  double elapsed();
private:
  double *total;
  std::chrono::_V2::system_clock::time_point begin;
};

#endif