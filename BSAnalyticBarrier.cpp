#include "DAOC.h"

using namespace std;

#define bsptr boost::shared_ptr


Real BSAnalyticBarrier(){
  try{
    Barrier::Type barrierType(Barrier::DownOut);
    Option::Type optionType(Option::Call);

    Date todaysDate(2, Apr, 2010);
    Date settlementDate = todaysDate;
    Date maturityDate = settlementDate;
    maturityDate += 2 * Months;
    maturityDate += 12 * Days;

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

    Real r = barrierOption.NPV();
    std::cout << __FUNCTION__ << ", NPV = " << r << std::endl;
    return r;
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
