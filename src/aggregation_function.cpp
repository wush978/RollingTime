#include "aggregation_function.hpp"

namespace RollingTime {

class Sum : std::function<int(int, const int)> {

  const int* _value;

public:

  Sum(const int* value) : _value(value) { }

  int operator()(int a, const int b) {
    return a + _value[b - 1];
  }

};

std::function<int(int, const int)> get_aggregation_function(
  const std::string& agg,
  const int* value
) {
  if (agg.compare("sum") == 0) return Sum(value=value);
  throw std::invalid_argument("invalid agg: " + agg);
}

}
