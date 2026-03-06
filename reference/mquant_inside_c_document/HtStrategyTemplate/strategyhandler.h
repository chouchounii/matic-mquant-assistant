#ifndef STRATEGYHANDLER_H
#define STRATEGYHANDLER_H
#include "./include/istrategyinterface.h"
#include <QHash>
#include "tradethread.h"
#include "tradedatadownloadthread.h"
using namespace MQuant;
class StrategyHandler : public IStrategyInterface
{
public:
    StrategyHandler();
    static StrategyHandler &instance();
public:
    ///
    /// \brief getStrategyParams 获取策略参数模板
    /// \return 策略参数，json格式{"key1":{"value":value1, "desc":"description","type":0},"key2":{"value":"value2","type":3},"key3":{"value":value3}}，支持int(0),float(1),bool(2),string(3),list(4),table(5)
    ///
    virtual StrategyParams getStrategyParams();
    ///
    /// \brief initialize 策略初始化
    /// \param context 上下文变量
    ///
    virtual void initialize(IHtContext *context) ;
    ///
    /// \brief onStrategyStart 策略开始
    /// \param context  上下文变量
    /// \remark 初始化完成后，立即调用此回调函数，在此回调函数中可以报单，不能读取外部文件
    virtual void onStrategyStart(IHtContext *context);
    ///
    /// \brief handleTick tick行情接收回调
    /// \param context 上下文变量
    /// \param tick tick数据，MQuant::Tick类型
    ///
    virtual void handleTick(IHtContext *context, const Tick* tick);
    ///
    /// \brief handleData k线行情接收回调
    /// \param context 上下文变量
    /// \param kline k线数据，MQuant::KLineData类型
    ///
    virtual void handleData(IHtContext *context, const KLineDataPush *kline);
    ///
    /// \brief handleOrderRecord 逐笔委托接收回调
    /// \param context 上下文变量
    /// \param recordOrder 逐笔委托，MQuant::RecordOrder类型
    /// \remark 仅深市标的有逐笔委托数据。由于逐笔数据量很大，订阅大量标的的逐笔数据时，请与华泰技术支持人员联系，获取解决方案
    virtual void handleOrderRecord(IHtContext *context, const RecordOrder *recordOrder);
    ///
    /// \brief handleOrderTransaction 逐笔成交接收回调
    /// \param context 上下文变量
    /// \param recordTrans 逐笔成交，MQuant::RecordTransaction类型
    /// \remark 由于逐笔数据量很大，订阅大量标的的逐笔数据时，请与华泰技术支持人员联系，获取解决方案
    virtual void handleOrderTransaction(IHtContext *context, const RecordTransaction *recordTrans);
    ///
    /// \brief handleOrderReport 订单回报推送
    /// \param context 上下文变量
    /// \param order 订单
    ///
    virtual void handleOrderReport(IHtContext *context, const Order* order);
    ///
    /// \brief handleExecutionReport 成交回报推送
    /// \param context 上下文变量
    /// \param execution 成交
    ///
    virtual void handleExecutionReport(IHtContext *context, const Execution *execution);
    ///
    /// \brief handleEtfEstimateInfo ETF预估信息推送回调
    /// \param context 上下文变量
    /// \param estimateInfo 实时ETF预估信息
    /// \remark 需要先将订阅的ETF在ETF套利界面上添加到常用ETF列表中才可使用
    virtual void handleEtfEstimateInfo(IHtContext *context, const EtfEstimateInfo *estimateInfo);
    ///
    /// \brief onStrategyParamsChange 策略参数变更推送
    /// \param context 上下文变量
    /// \param params 参数
    /// \param filePath 参数文件路径，仅通过导入文件修改参数时有效
    ///
    virtual void onStrategyParamsChange(IHtContext *context, const char *params, const char *filePath);
    ///
    /// \brief marketOpen 开盘信号
    /// \param context 上下文变量
    /// \param tradeDate 交易日，yyyyMMdd格式
    /// \remark 仅回测模式下有效
    virtual void marketOpen(IHtContext *context, int tradeDate);
    ///
    /// \brief marketClose 收盘信号
    /// \param context  上下文变量
    /// \param tradeDate    交易日，yyyyMMdd格式
    /// \remark 仅回测模式下有效
    virtual void marketClose(IHtContext *context, int tradeDate);
    ///
    /// \brief onStrategyEnd 策略停止回调
    /// \param context 上下文变量
    ///
    virtual void onStrategyEnd(IHtContext *context);
    ///
    /// \brief timerFuncPeriod 周期定时信号回调
    /// \param context 上下文变量
    /// \param interval 时间间隔
    /// \param startTime 开始时间
    /// \param endTime 结束时间
    /// \param customField 用户自定义数据
    ///
    virtual void timerFuncPeriod(IHtContext *context, int interval, const char*  startTime, const char*  endTime, const char *customField);
    ///
    /// \brief timerFuncOneShot 单次定时信号
    /// \param context 上下文变量
    /// \param time 触发时间
    /// \param customField 用户自定义数据
    ///
    virtual void timerFuncOneShot(IHtContext *context, const char*  time, const char *customField);
    ///
    /// \brief onCustomMsg 自定义消息接收回调
    /// \param context 上下文变量
    /// \param msgType 自定义消息类型，10001-65536之间的自定义值
    /// \param msg 消息体
    ///
    virtual void onCustomMsg(IHtContext *context, int msgType, const char *msg);

protected:
    virtual void testMargin(IHtContext *context);
    void testEtf(IHtContext *context);
    void testMultiThread(IHtContext *context);
    void setLogOption();

private:
    bool m_batchOrderFlag{false};      //批量报单标志，如果已经批量报单，则置为true
    bool m_etfPurchaseFlag{false};
    bool m_etfRedemptionFlag{false};
    bool m_testMarginFlag{false};
    bool m_registerCustomFileFlag{false};

    QList<QString> m_thread1Symbols;
    QList<QString> m_thread2Symbols;
    QList<QString> m_thread3Symbols;
    QList<QString> m_thread4Symbols;
    QList<QString> m_thread5Symbols;

    TradeThread m_thread1;
    TradeThread m_thread2;
    TradeThread m_thread3;
    TradeThread m_thread4;
    TradeThread m_thread5;

    TradeDataDownloadThread m_downloadThread;

};

#ifdef __cplusplus
extern "C"
{
#endif
    __declspec(dllexport) MQuant::IStrategyInterface *getStrategyHandler();
#ifdef __cplusplus
}
#endif


#endif // STRATEGYHANDLER_H
