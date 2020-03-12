
#include <ql/qldefines.hpp>
#ifdef BOOST_MSVC
#  include <ql/auto_link.hpp>
#endif
#include "binomialengine.hpp"
#include <ql/instruments/vanillaoption.hpp>

#include <ql/pricingengines/vanilla/binomialengine.hpp>


#include <ql/pricingengines/vanilla/analyticeuropeanengine.hpp>
#include <ql/pricingengines/vanilla/analytichestonengine.hpp>
#include <ql/pricingengines/vanilla/baroneadesiwhaleyengine.hpp>
#include <ql/pricingengines/vanilla/bjerksundstenslandengine.hpp>
#include <ql/pricingengines/vanilla/batesengine.hpp>
#include <ql/pricingengines/vanilla/integralengine.hpp>
#include <ql/pricingengines/vanilla/fdblackscholesvanillaengine.hpp>
#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>
#include <ql/pricingengines/vanilla/mcamericanengine.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/utilities/dataformatters.hpp>



#include <iostream>
#include <iomanip>



#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

	Integer sessionId() { return 0; }

}
#endif


using namespace QuantLib;


int main() {

	try {

		std::cout << std::endl;

		// set up dates
		Calendar calendar = TARGET();
		Date todaysDate(8, March, 2020);
		Date settlementDate(10, May, 2020);
		Settings::instance().evaluationDate() = todaysDate;

		// our options
		Option::Type type(Option::Call);
		Real underlying = 100;
		Real strike =95;
		Spread dividendYield = 0.00;
		Rate riskFreeRate = 0.06;
		Volatility volatility = 0.20;
		Date maturity(10, March, 2021);

		DayCounter dayCounter = Actual365Fixed();

		std::cout << "Option type = " << type << std::endl;
		std::cout << "Maturity = " << maturity << std::endl;
		std::cout << "Underlying price = " << underlying << std::endl;
		std::cout << "Strike = " << strike << std::endl;
		std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate)
			<< std::endl;
		std::cout << "Dividend yield = " << io::rate(dividendYield)
			<< std::endl;
		std::cout << "Volatility = " << io::volatility(volatility)
			<< std::endl;
		std::cout << std::endl;
		std::string method;
		std::cout << std::endl;

		/////////////////////// Exercise

		ext::shared_ptr<Exercise> europeanExercise(
			new EuropeanExercise(maturity));


		////////////////////	
		Handle<Quote> underlyingH(
			ext::shared_ptr<Quote>(new SimpleQuote(underlying)));
		/////////////////////////////
		Handle<YieldTermStructure> flatTermStructure(
			ext::shared_ptr<YieldTermStructure>(
				new FlatForward(settlementDate, riskFreeRate, dayCounter)));
		Handle<YieldTermStructure> flatDividendTS(
			ext::shared_ptr<YieldTermStructure>(
				new FlatForward(settlementDate, dividendYield, dayCounter)));
		Handle<BlackVolTermStructure> flatVolTS(
			ext::shared_ptr<BlackVolTermStructure>(
				new BlackConstantVol(settlementDate, calendar, volatility,
					dayCounter)));

		ext::shared_ptr<StrikedTypePayoff> payoff(
			new PlainVanillaPayoff(type, strike));

		ext::shared_ptr<BlackScholesMertonProcess> bsmProcess(
			new BlackScholesMertonProcess(underlyingH, flatDividendTS,
				flatTermStructure, flatVolTS));

		//////////////////////////////
		// options
		VanillaOption europeanOption(payoff, europeanExercise);

		// Binomial method: Jarrow-Rudd
		//Size timeSteps = 801;
		/*//method = "Binomial Jarrow-Rudd";
		europeanOption.setPricingEngine(ext::shared_ptr<PricingEngine>(
			new BinomialVanillaEngine<JarrowRudd>(bsmProcess, timeSteps)));
		std::cout << europeanOption.NPV()
			<< std::endl;
		*/
		bool oscillation = true;
		std::cout << "Pricing with oscillation" << std::endl;
		
		for (Size timeSteps = 500; timeSteps < 512; ++timeSteps) {
			europeanOption.setPricingEngine(ext::make_shared<BinomialVanillaEngine_2<CoxRossRubinstein> >(bsmProcess, timeSteps,oscillation));

			std::cout << timeSteps << "\t" << europeanOption.NPV() << std::endl;
			

		}

		std::cout << std::endl;


	    oscillation = false;
		std::cout << "Pricing without oscillation" << std::endl;
		
		for (Size timeSteps = 500; timeSteps < 512; ++timeSteps) {
			europeanOption.setPricingEngine(ext::make_shared<BinomialVanillaEngine_2<CoxRossRubinstein> >(bsmProcess, timeSteps, oscillation));

			std::cout << timeSteps << "\t" << europeanOption.NPV() << std::endl;


		}














		std::cout << std::setprecision(10);





		return 0;

	}
	catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "unknown error" << std::endl;
		return 1;
	}
}

