#ifndef __AGGREGATION_FUNCTION_HPP__
#define __AGGREGATION_FUNCTION_HPP__

#include <functional>

namespace RollingTime {

template<typename ValueType>
class Sum : std::function<ValueType(ValueType, const int)> {

  const ValueType* _value;

public:

  Sum(const ValueType* value) : _value(value) { }

  ValueType operator()(ValueType a, const int b) {
    return a + _value[b - 1];
  }

};


template<typename ValueType>
std::function<ValueType(ValueType, const int)> get_aggregation_function(const std::string& agg, const ValueType* value) {
  if (agg.compare("sum") == 0) return Sum<ValueType>(value=value);
  throw std::invalid_argument("invalid agg: " + agg);
}

}
#endif // __AGGREGATION_FUNCTION_HPP__
