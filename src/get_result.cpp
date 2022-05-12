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

template<typename OrderByType, typename ValueType, int OrderByRType, int ValueRType>
SEXP get_result_template(
  const std::string& agg,
  IntegerVector grp,
  Vector<OrderByRType> order_by,
  Vector<ValueRType> value,
  ValueType start,
  ValueType end
) {
  auto aggregation_function = RollingTime::get_aggregation_function<ValueType>(agg, &value[0]);
  IntegerVector starts(grp.attr("starts"));
  const int* pgrp = &grp[0];

  Vector<ValueRType> result(order_by.size());
  for (R_len_t i = 0;i < starts.size();i++) {
    const int start_offset = starts[i];
    const int* window_begin = pgrp + (start_offset - 1);
    const int* window_end = window_begin;
    const int end_offset = (i + 1 < starts.size() ? starts[i+1] : grp.size() + 1);
    const int* boundary_pointer = pgrp + (end_offset - 1);
    for(int j = start_offset - 1; j < end_offset - 1;j++) {
      const auto idx = *(pgrp + j) - 1;
      const auto order_by_value = order_by[idx];
      window_begin = ensure_iterator(
        window_begin,
        [&order_by, &order_by_value, &start](const int* p) {
          return order_by[*p - 1] >= order_by_value + start;
        },
        [&boundary_pointer](const int* p) {
          return p == boundary_pointer;
        }
      );
      window_end = ensure_iterator(
        window_end,
        [&order_by, &order_by_value, &end](const int* p) {
          return order_by[*p - 1] > order_by_value + end;
        },
        [&boundary_pointer](const int* p) {
          return p == boundary_pointer;
        }
      );
      result[idx] = std::accumulate(window_begin, window_end, static_cast<ValueType>(0), aggregation_function);
    }
  }
  return result;
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
  return get_result_template<int, int, INTSXP, INTSXP>(
    agg, grp, order_by, value, start, end
  );
}

//[[Rcpp::export]]
SEXP get_result_int_num(
    const std::string& agg,
    IntegerVector grp,
    IntegerVector order_by,
    NumericVector value,
    int start,
    int end
) {
  return get_result_template<int, double, INTSXP, REALSXP>(
      agg, grp, order_by, value, start, end
  );
}

//[[Rcpp::export]]
SEXP get_result_num_int(
    const std::string& agg,
    IntegerVector grp,
    NumericVector order_by,
    IntegerVector value,
    double start,
    double end
) {
  return get_result_template<double, int, REALSXP, INTSXP>(
      agg, grp, order_by, value, start, end
  );
}

//[[Rcpp::export]]
SEXP get_result_num_num(
    const std::string& agg,
    IntegerVector grp,
    NumericVector order_by,
    NumericVector value,
    double start,
    double end
) {
  return get_result_template<double, double, REALSXP, REALSXP>(
      agg, grp, order_by, value, start, end
  );
}
