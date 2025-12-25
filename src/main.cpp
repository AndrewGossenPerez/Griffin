
#include "data/csv_reader.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <chrono>
#include "backtesting/strategy.hpp"
#include "backtesting/backtesting.hpp"

int main(){

    using clock = std::chrono::steady_clock;

    std::cout << "Creating bars" << std::endl;
    auto t0 = clock::now();
    auto bars=trd::csvReader::loadBars("samples/aapl.csv");
    auto t1 = clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << " Bars created: " << bars.size() << 
    " Total Time to create : " << elapsed_ms << "ms" << std::endl;

    // Begin backtesting
    Backtest backtest;
    CoinFlip strat;
    
    Result r = backtest.run(bars, strat);

    std::cout << " \n -- Backtest results --- \n ";
    std::cout << "Final equity : " << r.final_equity << std::endl;
    std::cout <<  "Max Draw Down : " << r.maxdd << std::endl;
    std::cout <<  "Total fees paid : " << r.netFees << std::endl;
    std::cout <<  "Net trades : " << r.trades.size() << std::endl;
  
    std::cout << std::endl;

    return 0;

}