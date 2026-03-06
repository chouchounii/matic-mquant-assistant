#ifndef TRADEDATADOWNLOADTHREAD_H
#define TRADEDATADOWNLOADTHREAD_H

#include <windows.h>
#include "./include/istrategyinterface.h"
class TradeDataDownloadThread 
{
	//友元函数,可以在类的外部访问类的私有成员
	friend DWORD WINAPI ThreadProFuncForTradeDataDownloadThread(LPVOID lpParam);

public:
    TradeDataDownloadThread();
    void init(MQuant::IHtContext *context);
	void start();
	void quit();

	
protected:
    virtual void run();
	bool isRunning();


private:
    MQuant::IHtContext *m_context;
    MQUANT_PYMODULE m_pyModule;

	bool	m_bRunning;
	HANDLE	m_hThread;
	DWORD	m_dwThreadId;
};

#endif // TRADEDATADOWNLOADTHREAD_H
