library(magrittr)
library(iterators)
library(RollingTime)
options(warn=2)

dt <- data.table(data.frame(
  a = as.integer(c(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
  b = rep(1:3, 4) %>% head(10),
  c = as.integer(2^(1:10))
))
print(dt)
print(data.table:::forderv(dt, by = "b", retGrp = TRUE))

expand.grid(
  start = as.integer(-10:10),
  end = as.integer(-10:10)
) %>%
  data.table() %>%
  .[start <= end] %>%
  iterators::iter(by = "row") %>%
  lapply(function(iter) {
    if (!interactive()) result <- RollingTime:::get_result_int_int(
      agg = "sum",
      grp = data.table:::forderv(dt, by = "b", retGrp = TRUE),
      order_by = dt$a,
      value = dt$c,
      start = iter$start,
      end = iter$end
    )

    result_ref <- sapply(nrow(dt) %>% seq_len, function(i) {
      dt[b == dt[i, b]] %>%
        .[a >= dt[i, a] + iter$start] %>%
        .[a <= dt[i, a] + iter$end] %>%
        .[,sum(c)]
    })

    if (any(result != result_ref)) {
      stop(sprintf("Check failed at start:%d end:%d", iter$start, iter$end))
    }
  }) %>%
  invisible()
