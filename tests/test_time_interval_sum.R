library(RollingTime)

now <- Sys.time()
check <- function(value, value_ts, interval_size) {
  result_ts <- seq(now - 3600 * 100, by = 20, length.out = 64)[c(1, 2, 4, 8, 16, 32, 64)]
  print(list(
    data.frame(result_ts = result_ts),
    data.frame(value_ts = value_ts)
  ))
  result <- time_interval_sum(
    result_ts = result_ts,
    value_ts = value_ts,
    value = value,
    interval_size = interval_size
  )
  print(result)
  stopifnot(length(result) == length(result_ts))
  for(i in seq_along(result)) {
    stopifnot(result[i] == sum(value[(value_ts > result_ts[i] - interval_size) & (value_ts <= result_ts[i])]))
  }
}

check(
  value = rnorm(20),
  value_ts = seq(now - 3600 * 100, by = 60, length.out = 20),
  interval_size = 10
)

check(
  value = rnorm(20),
  value_ts = seq(now - 3600 * 100, by = 60, length.out = 20),
  interval_size = 100
)

check(
  value = rnorm(20),
  value_ts = seq(now - 3600 * 100, by = 60, length.out = 20) - 86400,
  interval_size = 10
)

check(
  value = rnorm(20),
  value_ts = seq(now - 3600 * 100, by = 60, length.out = 20) + 86400,
  interval_size = 10
)

check(
  value = rnorm(20),
  value_ts = seq(now - 3600 * 100 - 36000, by = 6000, length.out = 20),
  interval_size = 100
)
