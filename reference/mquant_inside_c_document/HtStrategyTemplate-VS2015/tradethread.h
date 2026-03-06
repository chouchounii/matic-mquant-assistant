#ifndef TRADETHREAD_H
#define TRADETHREAD_H
#include <list>
#include <windows.h>
#include "./include/istrategyinterface.h"
class TradeThread 
{
	//友元函数,可以在类的外部访问类的私有成员
	friend DWORD WINAPI ThreadProFuncForTradeThread(LPVOID lpParam);

public:
    TradeThread();
    void setContext(MQuant::IHtContext *context);
    void setSymbolList(const std::list<std::string> &symbolList);
	void start();
	void quit();
protected:
    virtual void run();
	bool isRunning();
private:
    MQuant::IHtContext *m_context;
    std::list<std::string> m_symbolList;

	bool	m_bRunning;
	HANDLE	m_hThread;
	DWORD	m_dwThreadId;
};

#endif // TRADETHREAD_H
