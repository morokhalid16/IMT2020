#pragma once
#ifndef BS_hpp
#define BS_hpp


double BS(double underlying, double strike, double maturity,
	double riskFreeRate, double volatility, bool CallOption, double dividend);

#endif