#ifndef TRADEDATADOWNLOADTHREAD_H
#define TRADEDATADOWNLOADTHREAD_H

#include <QThread>
#include "./include/istrategyinterface.h"
class TradeDataDownloadThread : public QThread
{
    Q_OBJECT
public:
    TradeDataDownloadThread();
    void init(MQuant::IHtContext *context);
protected:
    virtual void run();

private:
    MQuant::IHtContext *m_context;
    MQUANT_PYMODULE m_pyModule;
};

#endif // TRADEDATADOWNLOADTHREAD_H
