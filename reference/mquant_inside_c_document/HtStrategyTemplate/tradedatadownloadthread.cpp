#include "tradedatadownloadthread.h"
#include "./include/istrategyinterface.h"
#include <QDebug>
#include "./include/mquantultis.h"
TradeDataDownloadThread::TradeDataDownloadThread() :QThread(),m_context(NULL),m_pyModule(NULL)
{

}

void TradeDataDownloadThread::init(MQuant::IHtContext *context)
{
    m_context = context;
    if(m_context){
        qDebug() << "register python file" ;
        //此处的文件路径需要修改为用户机器上DownloadTradeData.py所在的文件路径
        m_pyModule = m_context->getPythonExecutor()->registerPythonFile("D:\\MaticMaster\\modules\\M-Quant\\HtProEngine\\HtStragetyRunEnv\\HtStrategyTemplate\\DownloadTradeData.py");
        qDebug() << "register python file return:"  << m_pyModule;
        if(m_pyModule){
            MQuant::HtLog::registerCustomLogFile("D:\\trade.csv", "test");
        }
    }
}

void TradeDataDownloadThread::run()
{
    while (isRunning()) {
        //每个10s查询一次委托、成交，记录在文件中,调用python接口来记录
        if(!m_pyModule){
            break;
        }
        MQuant::HtString ret = m_context->getPythonExecutor()->callPythonFunc(m_pyModule, "download_trade_data", "hello world");
        qDebug() << "call python func  return:" << ret.str();
        sleep(10);
    }
}
