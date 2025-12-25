
#pragma once 
#include "core/types.hpp"
#include "strategy.hpp"
#include <vector>

struct Trade{
    trd::timestamp epoch;
    trd::Type type;
    trd::quantity qty;
    double px;
    double fee;
};

struct Result{
    std::vector<double> equity; 
    std::vector<Trade> trades;
    double final_equity{};
    double maxdd{};
    double netFees{};
    double netBuys{};
    double netSells{};
};

class Backtest{

    public:
    Result run(const std::vector<trd::Bar>& bars, Strategy& strat);

};