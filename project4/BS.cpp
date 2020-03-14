
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

double BS(double S, double K, double T,double r, double sigma, bool CallOption, double dividend) {

	double d1 = (1 / (sigma * sqrt(T))) * (log(S / K) + (r + sigma * sigma / 2) * T);
	double d2 = d1 - sigma * sqrt(T);

	double analyticPrice;
	 
	if (CallOption) {

	       analyticPrice = N(d1) *	 S * exp(-dividend * T) - N(d2) * K * exp(-r * T);
	}

	else {

		  analyticPrice = -N(-d1) * S * exp(-dividend * T) + N(-d2) * K * exp(-r * T);
	}

	return analyticPrice;
}