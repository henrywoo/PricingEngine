#ifndef __DAOC__
#define __DAOC__

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
#include <ql/pricingengines/barrier/mcbarrierengine.hpp>
#include <ql/pricingengines/barrier/analyticbarrierengine.hpp>
#include <ql/pricingengines/barrier/fdblackscholesbarrierengine.hpp>
#include <iostream>
using namespace QuantLib;

Real BSMCBarrier();
Real BSFDBarrier();
Real BSAnalyticBarrier();

#endif
