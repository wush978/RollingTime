#include <memory>
#include <Rcpp.h>
#include "rolling_time.hpp"
using namespace Rcpp;

const int* ensure_iterator(const int* p, std::function<bool(const int*)> window_checker, std::function<bool(const int*)> boundary_checker) {
  while (true) {
    if (boundary_checker(p)) break;
    if (window_checker(p)) break;
    p++;
  }
  return p;
}

//[[Rcpp::export]]
SEXP get_result_int_int(
  const std::string& agg,
  IntegerVector grp,
  IntegerVector order_by,
  IntegerVector value,
  int start,
  int end
) {
  auto aggregation_function = RollingTime::get_aggregation_function(agg, &value[0]);
  IntegerVector starts(grp.attr("starts"));
  const int* pgrp = &grp[0];

  IntegerVector result(order_by.size());
  for(R_len_t i = 0;i < starts.size();i++) {
    const int start_offset = starts[i];
    const int* window_begin = pgrp + (start_offset - 1);
    const int* window_end = window_begin;
    const int end_offset = (i + 1 < starts.size() ? starts[i+1] : grp.size() + 1);
    const int* boundary_pointer = pgrp + (end_offset - 1);
#ifdef ROLLING_TIME_DEBUG
    Rprintf(
      "start_offset: %d end_offset: %d\n",
      start_offset,
      end_offset
    );
#endif // ROLLING_TIME_DEBUG
    for(int j = start_offset - 1; j < end_offset - 1;j++) {
      const auto idx = *(pgrp + j) - 1;
      const auto order_by_value = order_by[idx];
      window_begin = ensure_iterator(
        window_begin,
        [&order_by, &order_by_value, &start](const int* p) {
#ifdef ROLLING_TIME_DEBUG
#endif // ROLLING_TIME_DEBUG
          return order_by[*p - 1] >= order_by_value + start;
        },
        [&boundary_pointer](const int* p) {
#ifdef ROLLING_TIME_DEBUG
          Rprintf("(window_begin) boundary_pointer - p: %d\n", boundary_pointer - p);
#endif // ROLLING_TIME_DEBUG
          return p == boundary_pointer;
        }
      );
      window_end = ensure_iterator(
        window_end,
        [&order_by, &order_by_value, &end](const int* p) {
#ifdef ROLLING_TIME_DEBUG
          Rprintf(
            "(window_end) *p: %d, order_by[*p - 1]: %d,order_by_value + end: %d\n",
            *p,
            order_by[*p - 1],
            order_by_value + end
          );
#endif // ROLLING_TIME_DEBUG
          return order_by[*p - 1] > order_by_value + end;
        },
        [&boundary_pointer](const int* p) {
#ifdef ROLLING_TIME_DEBUG
          Rprintf("(window_end) boundary_pointer - p: %d\n", boundary_pointer - p);
#endif // ROLLING_TIME_DEBUG
          return p == boundary_pointer;
        }
      );
#ifdef ROLLING_TIME_DEBUG
      Rprintf(
        "j: %d, window_begin_offset: %d, window_end_offset: %d \n",
        j, window_begin - pgrp, window_end - pgrp
      );
#endif // ROLLING_TIME_DEBUG
      result[idx] = std::accumulate(window_begin, window_end, 0, aggregation_function);
    }
  }
  return result;
}
