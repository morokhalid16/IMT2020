#pragma once
#ifndef BS_hpp
#define BS_hpp

struct BSprice{

	double analyticPrice;

};

BSprice BS(double underlying, double strike, double maturity,
	double riskFreeRate, double volatility);

#endif