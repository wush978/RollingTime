#include <Rcpp.h>
using namespace Rcpp;

void ensure_iterator(
  const DatetimeVector& value_ts,
  std::vector<std::size_t>::const_iterator& it,
  std::function<bool(DatetimeVector::value_type)> condition_checker,
  std::function<bool(std::vector<std::size_t>::const_iterator&)> boundary_checker
) {
  while (true) {
    if (boundary_checker(it)) break;
    double it_ts = value_ts[*it];
    if (condition_checker(it_ts)) break;
    it++;
  }
}


//'@export
//'@result_ts The sorted POSIXct vector of result timestamp.
//'@value_ts The POSIXct vector of timestamp of data.
//'@value The numeric vector of data.
//'@interval_size The length of the interval in second.
//'@return A numeric vector. The \code{i}-th element is the sum of \code{value} whose corresponding timestamp is
//  between \code{(result_ts[i] - interval_size, result_ts[i]]}.
//[[Rcpp::export]]
SEXP time_interval_sum(
  const DatetimeVector& result_ts,
  const DatetimeVector& value_ts,
  const NumericVector& value,
  double interval_size
) {
  if (result_ts.size() == 0) throw std::invalid_argument("Empty target_ts");
  if (value_ts.size() != value.size()) throw std::invalid_argument("Inconsistent value_ts and value");
  if (interval_size < 1) throw std::invalid_argument("Non positive interval_size");

  // sort the index of value_ts by value_ts
  std::vector<std::size_t> value_idx(value_ts.size());
  for(std::size_t i = 0;i < value_idx.size();i++) {
    value_idx[i] = i;
  }
  std::sort(value_idx.begin(), value_idx.end(), [&value_ts](std::size_t i, std::size_t j) {
    return value_ts[i] < value_ts[j];
  });

  std::vector<std::size_t>::const_iterator interval_begin = value_idx.begin();
  std::vector<std::size_t>::const_iterator interval_end = value_idx.begin();

  NumericVector result(result_ts.size());
  for(std::size_t i = 0;i < result_ts.size();i++) {
    ensure_iterator(
      value_ts,
      interval_begin,
      [&result_ts, &i, &interval_size](const DatetimeVector::value_type& a) {
        return a > result_ts[i] - interval_size;
      },
      [&value_idx](std::vector<std::size_t>::const_iterator it) {
        return it == value_idx.end();
      }
    );
    ensure_iterator(
      value_ts,
      interval_end,
      [&result_ts, &i](const DatetimeVector::value_type& a) {
        return a > result_ts[i];
      },
      [&value_idx](std::vector<std::size_t>::const_iterator it) {
        return it == value_idx.end();
      }
    );
    if (interval_begin != value_idx.end()) {
      result[i] = std::accumulate(interval_begin, interval_end, 0.0, [&value](double a, std::size_t b) {
        return a + value[b];
      });
    } else {
      result[i] = 0;
    }
  }
  return result;
}
