
#include <ql/math/distributions/normaldistribution.hpp>
#include <cmath>
#include "BS.hpp"
using std::sqrt;
using std::exp;
using std::log;


namespace {

	QuantLib::CumulativeNormalDistribution N;
	QuantLib::NormalDistribution n;
}

BSprice BS(double S, double K, double T,double r, double sigma) {

	double d1 = (1 / (sigma * sqrt(T))) * (log(S / K) + (r + sigma * sigma / 2) * T);
	double d2 = d1 - sigma * sqrt(T);

	BSprice val;
	val.analyticPrice = N(d1) * S - N(d2) * K * exp(-r * T);

	return val;
}