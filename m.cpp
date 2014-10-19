#include <ql/quantlib.hpp>
#include <ql/math/distributions/normaldistribution.hpp>
#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <ta_libc.h>

using namespace QuantLib;
using namespace std;

CumulativeNormalDistribution phai;

Real dplus(Real x, Real K, Real r, Real sigma, Real T){
  return (log(x / K) + (r + sigma*sigma / 2)*T) / (sigma*sqrt(T));
}

Real dminus(Real x, Real K, Real r, Real sigma, Real T){
  return (log(x / K) + (r - sigma*sigma / 2)*T) / (sigma*sqrt(T));
}

Real BSC(Real x, Real K, Real r, Real sigma, Real T){
  return x*phai(dplus(x, K, r, sigma, T)) - K*phai(dminus(x, K, r, sigma, T))*exp(-r*T);
}

Real DAOC_Continuous(Real x, Real K, Real r, Real sigma, Real T, Real H){
  return BSC(x, K, r, sigma, T) - 
    pow(H / x, (2 * r / (sigma*sigma) - 1)) * BSC(H*H / x, K, r, sigma, T);
}

vector<Real> simulateZI(uint16_t len){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<> d;
  vector<Real> zi(len);
  for (uint16_t i = 0; i < len; ++i){
    zi[i] = d(gen);
  }
  return zi;
}

vector<Real> simulateGBM(Real s0, Real mu, Real sigma, Real T, uint16_t step){
  vector<Real> zi = simulateZI(step);
  vector<Real> rs(step+1);
  Real dt = T / step;
  rs[0] = s0;
  for (size_t i = 1; i < step+1; ++i){
    rs[i] = rs[i - 1] * exp((mu-sigma*sigma/2)*dt+sigma*sqrt(dt)*zi[i-1]);
  }
  return rs;
}

pair<Real,Real> DAOC_Discrete(Real x, Real K, Real r, Real sigma, Real T, Real H, uint64_t step, uint64_t simnum){
  vector<Real> ps;
  for (uint16_t i = 0; i < simnum; ++i){
    // one sample path
    vector<Real> vr1 = simulateGBM(x, r, sigma, T, step);
    bool isknockedout = false;
    for (Real s : vr1){
      if (s < H){
        isknockedout = true;
        break;
      }
    }
    if (isknockedout){
      ps.push_back(0);
      continue;
    }
    Real tmp = vr1.back() - K;
    ps.push_back(tmp > 0 ? tmp : 0);
  }
  Real cumP = accumulate(ps.begin(), ps.end(), 0.0);
  int ob, n, sz = ps.size();
  double stddev=0.0;
  TA_STDDEV(0, sz - 1, &ps[0], sz, 1, &ob, &n, &stddev);
  return pair<Real, Real>(cumP/simnum, stddev);
}


int main2(int aggc, char* argv[])
{
  Real x=100, K=100, r=0.1, sigma=0.3, T=0.2, H=97;
  
  Real result= DAOC_Continuous(x, K, r, sigma, T, H);
  cout << "Continuous DAOC price:" << result << endl;

  uint16_t step = 5;
  pair<Real, Real> result2 = DAOC_Discrete(x, K, r, sigma, T, H, step, 10000);
  cout << "Discrete DAOC price[step=5]:" << result2.first 
    << "\tStandard Deviation:" << result2.second << endl;

  step = 50;
  result2 = DAOC_Discrete(x, K, r, sigma, T, H, step, 10000);
  cout << "Discrete DAOC price[step=50]:" << result2.first
    << "\tStandard Deviation:" << result2.second << endl;

  /*for (uint16_t i = 100; i < 1000; i +=100){
    step = i;
    result2 = DAOC2(x, K, r, sigma, T, H, step, 10000);
    cout << result2.first << "\t" << result2.second << endl;
  }*/
  return 0;
}