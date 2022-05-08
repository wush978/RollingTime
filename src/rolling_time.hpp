#ifndef __ROLLING_TIME_HPP__
#define __ROLLING_TIME_HPP__

#include <RcppCommon.h>
#include "aggregation_function.hpp"

template <typename T, typename InputIt>
void move_iterator(
  const T* value_array,
  InputIt& it,
  std::function<bool(T)> condition_checker,
  std::function<bool(InputIt&)> boundary_checker
) {
  while (true) {
    if (boundary_checker(it)) break;
    const T value = value_array[*it];
    if (condition_checker(value)) break;
    it++;
  }
}


#endif // __ROLLING_TIME_HPP__
