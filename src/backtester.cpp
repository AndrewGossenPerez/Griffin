
#include "core/types.hpp"
#include "backtesting/excecution.hpp"
#include "backtesting/portfolio.hpp"
#include "backtesting/strategy.hpp"
#include "backtesting/backtesting.hpp"
#include <vector>

Result Backtest::run(const std::vector<trd::Bar>& bars, Strategy& strat){

    Portfolio pf;
    Excecution ex;
    Result r;
    r.equity.reserve(bars.size());
    r.trades.reserve(bars.size());

    double peak=pf.equity(bars.front().close);

    for (size_t i=0;i+1<bars.size();++i){

        const auto&b=bars[i];
        const auto& next=bars[i+1];

        Signal s=strat.onBar(b);

        double px=ex.slip(next.open,s.type);

        if (s.type==trd::Type::Buy && pf.pos==0 ) { 
            // Buy with currently zero assets 
            pf.buy(1,px,ex.fee);
            r.netFees+=ex.fee;
            r.netBuys+=1;

            r.trades.push_back(
                Trade{
                    next.epoch,
                    s.type,
                    1,
                    px,
                    ex.fee
                }
            );

        } else if(s.type==trd::Type::Sell && pf.pos>0){

            int qty=pf.pos;
            
            // Sell an asset
            pf.sell(pf.pos,px,ex.fee);
            r.netFees+=ex.fee;
            r.netSells+=1;

            r.trades.push_back(
                Trade{
                    next.epoch,
                    s.type,
                    qty,
                    px,
                    ex.fee
                }
            );

        }

        double eq=pf.equity(b.close);
        r.equity.push_back(eq);
        peak=std::max(peak, eq);
        r.maxdd=std::max(r.maxdd,(peak - eq)/peak);
        
    }

    r.final_equity = r.equity.empty() ? pf.equity(bars.back().close) : r.equity.back();
    
    return r;

}