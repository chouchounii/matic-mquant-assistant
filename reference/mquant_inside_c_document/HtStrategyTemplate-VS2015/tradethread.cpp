#include "tradethread.h"
#include <tchar.h>
#include <Windows.h>
using namespace MQuant;
#include "CommonDef.h"
//#define UTF8STR(str) _T(str) 

//线程入口函数，实际此处不处理实际逻辑，在run中处理
DWORD WINAPI ThreadProFuncForTradeThread(LPVOID lpParam)
{
	TradeThread* p = (TradeThread*)lpParam;
	if (NULL != p)
	{
		p->run();
	}
	return 0;
}

TradeThread::TradeThread()
{
    m_context = NULL;
	m_bRunning = false;
	m_hThread = NULL;
	m_dwThreadId = 0;
}

void TradeThread::setContext(MQuant::IHtContext *context)
{
    m_context = context;
}

void TradeThread::setSymbolList(const std::list<std::string> &symbolList)
{
    m_symbolList = symbolList;
}

void TradeThread::start()
{
	if (NULL == m_hThread)
	{
		// 默认安全属性、默认堆栈大小、线程入口地址、传递给线程函数的参数、指定线程立即运行、线程ID号
		m_hThread = CreateThread(NULL, NULL, ThreadProFuncForTradeThread, this, 0, &m_dwThreadId);
		m_bRunning = true;
	}
}

void TradeThread::quit()
{
	if (NULL != m_hThread)
	{
		m_bRunning = false;
		CloseHandle(m_hThread);
		delete m_hThread;
		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

bool TradeThread::isRunning()
{
	return m_bRunning;
}

void TradeThread::run()
{
    while (this->isRunning()) {
		std::list<std::string>::iterator it = m_symbolList.begin();
		for (it; it != m_symbolList.end(); it++) {
            if(!this->isRunning())
                break;
			std::string symbol = *it;
            Tick tick = m_context->getSysApi()->getCurrentTick(symbol.c_str());
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
                strcpy_s(orderReq.symbol, tick.symbol);
                orderReq.amount = 1000;
                orderReq.side = OrderSide::BUY;
                orderReq.price = tick.ask_price[0];
                strcpy_s(orderReq.price_type ,PriceType::LIMIT.str());
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
                        strcpy_s(orderReq.symbol, tick.symbol);
                        orderReq.amount = 100;
                        orderReq.side = OrderSide::SELL;
                        orderReq.price = tick.bid_price[0];
                        strcpy_s(orderReq.price_type ,PriceType::LIMIT.str());
                        orderReq.entrust_type = EntrustType::ENTRUST;       //可不赋值，memset初始化时会默认设置为普通委托
                        MQLogDebug << UTF8STR("满足卖出条件，开始报单:") << orderReq.symbol << "," << orderReq.amount <<"," << orderReq.side << "," << tick.current << "," << tick.pre_close;
                        m_context->getSysApi()->order(AccountType::STOCK,orderReq);
                    }
                }
            }
        }
        for(int i = 0; i < 10000; i++){
            if(this->isRunning())
                Sleep(1);
            else
                break;
        }
    }

}
