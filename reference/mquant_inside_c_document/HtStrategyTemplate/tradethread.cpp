#include "tradethread.h"
using namespace MQuant;
#define UTF8STR(str) QStringLiteral(str).toUtf8().constData()
TradeThread::TradeThread() :QThread()
{
    m_context = NULL;
}

void TradeThread::setContext(MQuant::IHtContext *context)
{
    m_context = context;
}

void TradeThread::setSymbolList(const QList<QString> &symbolList)
{
    m_symbolList = symbolList;
}

void TradeThread::run()
{
    while (this->isRunning()) {
        foreach (const QString &symbol, m_symbolList) {
            if(!this->isRunning())
                break;
            Tick tick = m_context->getSysApi()->getCurrentTick(symbol.toUtf8().constData());
            if(symbol != tick.symbol)
            {
//                MQLogError << UTF8STR("获取tick数据失败，symbol:") << symbol.toUtf8().constData();
                continue;
            }

            if((tick.current - tick.pre_close)/tick.pre_close > 0.01)
            {
//                涨1%买入
                OrderRequest orderReq;
                memset(&orderReq, 0, sizeof(OrderRequest));
                strcpy(orderReq.symbol, tick.symbol);
                orderReq.amount = 1000;
                orderReq.side = OrderSide::BUY;
                orderReq.price = tick.ask_price[0];
                strcpy(orderReq.price_type ,PriceType::LIMIT.str());
                orderReq.entrust_type = EntrustType::ENTRUST;       //可不赋值，memset初始化时会默认设置为普通委托
                MQLogDebug << UTF8STR("满足买入条件，开始报单:") << orderReq.symbol << "," << orderReq.amount <<"," << orderReq.side << "," << tick.current << "," << tick.pre_close;
                m_context->getSysApi()->order(AccountType::STOCK,orderReq);
            }
            else if((tick.pre_close - tick.current)/tick.pre_close > 0.01){
                //判断是否有可卖出的持仓，如果有，则卖出
                PositionList posList = m_context->getSysApi()->getPositions(AccountType::STOCK, tick.symbol);
                if(posList.getSize() > 0){
                    if(posList.get(0).closeable_amount > 100){
                        OrderRequest orderReq;
                        memset(&orderReq, 0, sizeof(OrderRequest));
                        strcpy(orderReq.symbol, tick.symbol);
                        orderReq.amount = 100;
                        orderReq.side = OrderSide::SELL;
                        orderReq.price = tick.bid_price[0];
                        strcpy(orderReq.price_type ,PriceType::LIMIT.str());
                        orderReq.entrust_type = EntrustType::ENTRUST;       //可不赋值，memset初始化时会默认设置为普通委托
                        MQLogDebug << UTF8STR("满足卖出条件，开始报单:") << orderReq.symbol << "," << orderReq.amount <<"," << orderReq.side << "," << tick.current << "," << tick.pre_close;
                        m_context->getSysApi()->order(AccountType::STOCK,orderReq);
                    }
                }
            }
        }
        for(int i = 0; i < 10000; i++){
            if(this->isRunning())
                msleep(1);
            else
                break;
        }
    }

}
