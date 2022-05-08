#ifndef __AGGREGATION_FUNCTION_HPP__
#define __AGGREGATION_FUNCTION_HPP__

#include <functional>

namespace RollingTime {

std::function<int(int, const int)> get_aggregation_function(const std::string&, const int*);

}
#endif // __AGGREGATION_FUNCTION_HPP__
