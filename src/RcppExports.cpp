// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// time_interval_sum
SEXP time_interval_sum(const DatetimeVector& result_ts, const DatetimeVector& value_ts, const NumericVector& value, double interval_size);
RcppExport SEXP _RollingTime_time_interval_sum(SEXP result_tsSEXP, SEXP value_tsSEXP, SEXP valueSEXP, SEXP interval_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const DatetimeVector& >::type result_ts(result_tsSEXP);
    Rcpp::traits::input_parameter< const DatetimeVector& >::type value_ts(value_tsSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type value(valueSEXP);
    Rcpp::traits::input_parameter< double >::type interval_size(interval_sizeSEXP);
    rcpp_result_gen = Rcpp::wrap(time_interval_sum(result_ts, value_ts, value, interval_size));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_RollingTime_time_interval_sum", (DL_FUNC) &_RollingTime_time_interval_sum, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_RollingTime(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}