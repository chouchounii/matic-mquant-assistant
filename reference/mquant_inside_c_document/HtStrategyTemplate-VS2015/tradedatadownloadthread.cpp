#include "tradedatadownloadthread.h"
#include "./include/istrategyinterface.h"
#include "./include/mquantultis.h"
#include <iostream>

//线程入口函数，实际此处不处理实际逻辑，在run中处理
DWORD WINAPI ThreadProFuncForTradeDataDownloadThread(LPVOID lpParam)
{
	TradeDataDownloadThread* p = (TradeDataDownloadThread*)lpParam;
	if (NULL != p)
	{
		p->run();
	}
	return 0;
}


TradeDataDownloadThread::TradeDataDownloadThread() :m_context(0),m_pyModule(0)
{
	m_bRunning = false;
	m_hThread = NULL;
	m_dwThreadId = 0;
}

void TradeDataDownloadThread::init(MQuant::IHtContext *context)
{
    m_context = context;
    if(m_context)
	{
        std::cout << "register python file" << std::endl ;
        //此处的文件路径需要修改为用户机器上DownloadTradeData.py所在的文件路径
        m_pyModule = m_context->getPythonExecutor()->registerPythonFile("D:\\MaticMaster\\modules\\M-Quant\\HtProEngine\\HtStragetyRunEnv\\HtStrategyTemplate\\DownloadTradeData.py");
		std::cout << "register python file return:"  << m_pyModule << std::endl;
        if(m_pyModule)
		{
            MQuant::HtLog::registerCustomLogFile("D:\\trade.csv", "test");
        }
    }
}

void TradeDataDownloadThread::start()
{
	if (NULL == m_hThread)
	{
		// 默认安全属性、默认堆栈大小、线程入口地址、传递给线程函数的参数、指定线程立即运行、线程ID号
		m_hThread = CreateThread(NULL, NULL, ThreadProFuncForTradeDataDownloadThread, this, 0, &m_dwThreadId);
		m_bRunning = true;
	}
}

void TradeDataDownloadThread::quit()
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

//此函数内添加需要处理事务的代码
void TradeDataDownloadThread::run()
{
    while (isRunning()) 
	{
		//TODO：在此循环内添加处理代码

        //每个10s查询一次委托、成交，记录在文件中,调用python接口来记录
        if(!m_pyModule)
		{
            break;
        }
        MQuant::HtString ret = m_context->getPythonExecutor()->callPythonFunc(m_pyModule, "download_trade_data", "hello world");
        std::cout << "call python func  return:" << ret.str() << std::endl;
        Sleep(10000);
    }
}

bool TradeDataDownloadThread::isRunning()
{
	return m_bRunning;
}
