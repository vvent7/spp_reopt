#include <chrono>
#include "timer.h"

timer::timer() : timer(nullptr) { }
timer::timer(double *total) :
  total(total),
  begin(std::chrono::high_resolution_clock::now()){ }
timer::~timer(){
  if(total) *total = elapsed();
}
void timer::restart() {
  begin = std::chrono::high_resolution_clock::now();
}
double timer::elapsed() {
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / double(1000); //milliseconds
}