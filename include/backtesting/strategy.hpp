
#pragma once 
#include "core/types.hpp"
#include "data/bar.hpp"
#include <random>

struct Signal{
  trd::Type type;
};

struct Strategy{
  virtual ~Strategy()=default;
  virtual Signal onBar(const trd::Bar& bar)=0;
};

// -- Strategies

struct UpDownStrategy : Strategy {
 
  bool has_prev = false;
  double prev_close = 0.0;

  Signal onBar(const trd::Bar& bar) override {
    if (!has_prev) { has_prev = true; prev_close = bar.close; return {trd::Type::None}; }
    Signal s = (bar.close > prev_close) ? Signal{trd::Type::Buy} : Signal{trd::Type::Sell};
    prev_close = bar.close;
    return s;
  }

};

struct CoinFlip : Strategy {

  std::mt19937 rng{123};

  Signal onBar(const trd::Bar&) override {
    std::uniform_int_distribution<int> d(0, 2);
    int r = d(rng);
    if (r == 0) return {trd::Type::Buy};
    if (r == 1) return {trd::Type::Sell};
    return {trd::Type::None};
  }

};
