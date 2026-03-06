#ifndef TRADETHREAD_H
#define TRADETHREAD_H
#include <QThread>
#include <QList>
#include "./include/istrategyinterface.h"
class TradeThread : public QThread
{
    Q_OBJECT
public:
    TradeThread();
    void setContext(MQuant::IHtContext *context);
    void setSymbolList(const QList<QString> &symbolList);
protected:
    virtual void run();
private:
    MQuant::IHtContext *m_context;
    QList<QString> m_symbolList;
};

#endif // TRADETHREAD_H
