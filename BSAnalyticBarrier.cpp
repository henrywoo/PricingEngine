#include <ql/time/date.hpp>
#include <ql/instruments/barriertype.hpp>
#include <ql/instruments/barrieroption.hpp>
#include <ql/settings.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/exercise.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/instruments/payoffs.hpp>
#include <ql/processes/blackscholesprocess.hpp>
#include <ql/pricingengines/barrier/analyticbarrierengine.hpp>
#include <iostream>

using namespace QuantLib;
using namespace std;

#define bsptr boost::shared_ptr

int main3(int, char*[])
{
  try{
    Barrier::Type barrierType(Barrier::DownOut);
    Option::Type optionType(Option::Call);

    Date todaysDate(2, Apr, 2010);
    Date settlementDate = todaysDate;
    Date maturityDate = settlementDate;
    maturityDate += 2 * Months;
    maturityDate += 12 * Days;
    cout << maturityDate << endl;

    Settings::instance().evaluationDate() = todaysDate;
    Calendar calendar = TARGET();
    DayCounter dayCounter = Actual365Fixed();


    Real underlyingPrice = 100.0;
    Real strikePrice = 100.0;
    Real barrier = 97.0;
    Real rebate = 0.00;
    Spread dividendYield = 0.00;
    Rate riskFreeRate = 0.10;
    Volatility volatility = 0.30;

    bsptr<Exercise> europeanExercise(new EuropeanExercise(maturityDate));

    Handle<Quote> underlyingPriceQuote(
      bsptr<Quote>(new SimpleQuote(underlyingPrice)));

    Handle<YieldTermStructure> riskFreeRateTermStructure(
      bsptr<YieldTermStructure>(
      new FlatForward(
      settlementDate,
      riskFreeRate,
      dayCounter)));

    Handle<YieldTermStructure> dividendTermStructure(
      bsptr<YieldTermStructure>(
      new FlatForward(
      settlementDate,
      dividendYield,
      dayCounter)));

    Handle<BlackVolTermStructure> volatilityTermStructure(
      bsptr<BlackVolTermStructure>(
      new BlackConstantVol(
      settlementDate,
      calendar,
      volatility,
      dayCounter)));

    bsptr<StrikedTypePayoff> plainVanillaPayoff(
      new PlainVanillaPayoff(
      optionType,
      strikePrice));

    bsptr<BlackScholesMertonProcess> blackScholesMertonProcess(
      new BlackScholesMertonProcess(
      underlyingPriceQuote,
      dividendTermStructure,
      riskFreeRateTermStructure,
      volatilityTermStructure));

    BarrierOption barrierOption(
      barrierType,
      barrier,
      rebate,
      plainVanillaPayoff,
      europeanExercise);

    barrierOption.setPricingEngine(
      bsptr<PricingEngine>(
      new AnalyticBarrierEngine(
      blackScholesMertonProcess)));

    std::cout << "NPV = " << barrierOption.NPV() << std::endl;
    return 0;
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
    return 1;
  }
  catch (...)
  {
    std::cout << "unknown error" << std::endl;
    return 1;
  }
}
