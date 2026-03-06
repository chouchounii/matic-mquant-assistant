#include "strategyhandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#define UTF8STR(str) QStringLiteral(str).toUtf8().constData()
StrategyHandler::StrategyHandler()
{

}

StrategyHandler &StrategyHandler::instance()
{
    static StrategyHandler s_inst;
    return s_inst;
}

StrategyParams StrategyHandler::getStrategyParams()
{
    qDebug() << __FUNCDNAME__ << UTF8STR("开始获取参数");
    StrategyParams params;
    params.addIntParam(UTF8STR("整型测试参数"), 10);
    params.addBoolParam(UTF8STR("Bool测试参数"), true);
    params.addFloatParam(UTF8STR("浮点型测试参数"), 15.66);
    params.addStrParam(UTF8STR("字符串测试参数"),"hello");
    params.addListParam(UTF8STR("列表型测试参数"),UTF8STR("列表项1,列表项2,列表项3"));
    params.addTableParam(UTF8STR("表格测试参数"),"D:/TestData/testdata.csv");

    HtString str = params.getStrategyParams();
    qDebug() << __FUNCDNAME__ << str.str();
    return params;
}

void StrategyHandler::initialize(IHtContext *context)
{
    setLogOption();
    //测试打印运行参数
    m_downloadThread.init(context);

    MQLogDebug << __FUNCDNAME__
             << context->runParams().getIntValue(UTF8STR("整型测试参数"))
             << context->runParams().getBoolValue(UTF8STR("Bool测试参数"))
             << context->runParams().getFloatValue(UTF8STR("浮点型测试参数"))
             << context->runParams().getStrValue(UTF8STR("字符串测试参数")).str()
             << context->runParams().getStrValue(UTF8STR("列表型测试参数")).str()
             << context->runParams().getStrValue(UTF8STR("表格测试参数")).str();

    //订阅沪深A股行情
    SymbolList symbolList = context->getSysApi()->getSymbolList(
                MQuant::ExchangeType::SH.str(),MQuant::SecurityType::StockType.str(),MQuant::SecuritySubType::Ashares.str());
    symbolList += context->getSysApi()->getSymbolList(
                MQuant::ExchangeType::SZ.str(),MQuant::SecurityType::StockType.str(),MQuant::SecuritySubType::Ashares.str());

    //将所有标的分到5个线程中监控
    int perThreadSymbolNum = symbolList.getSize() / 5;
    for(int i = 0; i < symbolList.getSize(); i++){
        if(i < perThreadSymbolNum)
            m_thread1Symbols.append(symbolList.get(i).str());
        else if(i < 2 * perThreadSymbolNum)
            m_thread2Symbols.append(symbolList.get(i).str());
        else if(i < 3 * perThreadSymbolNum)
            m_thread3Symbols.append(symbolList.get(i).str());
        else if(i < 4 * perThreadSymbolNum)
            m_thread4Symbols.append(symbolList.get(i).str());
        else
            m_thread5Symbols.append(symbolList.get(i).str());
    }

    //将数据
    //打印第一个标的的详情
    if(symbolList.getSize() > 0){
        MQLogDebug << UTF8STR("第一只标的:") << symbolList.get(0).str() << ":" << context->getSysApi()->getSymbolDetial(symbolList.get(0).str());
        context->getSysApi()->subscribe(symbolList);            //订阅tick，在handleTick中接收
        context->getSysApi()->subscribe(symbolList, MQuant::SubscribeType::RECORD_ORDER);   //订阅逐笔委托，在handleOrderRecord中接收
        context->getSysApi()->subscribe(symbolList, MQuant::SubscribeType::RECORD_TRANSACTION); //订阅逐笔成交，在handleOrderTransaction中接收
        context->getSysApi()->subscribe(symbolList, MQuant::SubscribeType::KLINE_1M);   //订阅分钟k，在handleData中接收
    }
    //注册单次定时信号，当前时间5s后执行，在timerFuncOneShot中接收定时信号
    QTime curTime = QDateTime::currentDateTime().time();
    context->getSysApi()->runTimelyOneShot(curTime.addSecs(1).toString("hh:mm:ss zzz").toUtf8().constData(), "one shot");
    //注册3s执行一次的周期定时信号，3s后开始，2分钟后结束，在timerFuncPeriod中接收定时信号
    context->getSysApi()->runTimelyPeriod(1, curTime.addSecs(1).toString("hh:mm:ss zzz").toUtf8().constData(),
                                          curTime.addSecs(120).toString("hh:mm:ss zzz").toUtf8().constData(),"period");

//    订阅ETF预估信息
    SymbolList etfSymbols;
    etfSymbols.put("510050.SH");
    etfSymbols.put("159919.SZ");
    context->getSysApi()->subscribe(etfSymbols, MQuant::SubscribeType::ETF_ESTIMATE_INFO);

//    //订阅自定义信号，用于策略间通信，示例中演示在同一个策略中收发，在周期定时信号中发，在onCustomMsg回调中接收
    //python和C++策略的自定义信号是互通的
    context->getSysApi()->subscribeCustomMsg(MQUANT_CUSTOM_MSG_TYPE_BEGIN + 1);
    //尝试报单，初始化函数中不允许报单，因此会废单
    OrderRequest orderReq;
    memset(&orderReq, 0, sizeof(OrderRequest));
    strcpy(orderReq.symbol, "601688.SH");
    orderReq.amount = 1000;
    orderReq.side = OrderSide::BUY;
    orderReq.price = 18.95;
    strcpy(orderReq.price_type ,PriceType::LIMIT.str());
    orderReq.entrust_type = EntrustType::ENTRUST;       //可不赋值，memset初始化时会默认设置为普通委托
    context->getSysApi()->order(MQuant::AccountType::STOCK, orderReq);

    //初始化函数中允许读文件，可以使用任何读文件的方式，MQuant不提供封装好的API，在此不再举例
}

void StrategyHandler::onStrategyStart(IHtContext *context)
{
    testMultiThread(context);
    m_downloadThread.start();
    //此处可以报单，不可进行文件读写以及其他方式与外部通信
    OrderRequest orderReq;
    memset(&orderReq, 0, sizeof(OrderRequest));
    strcpy(orderReq.symbol, "601688.SH");
    orderReq.amount = 10000;
    orderReq.side = OrderSide::BUY;
    orderReq.price = 18.95;
    strcpy(orderReq.price_type ,PriceType::LIMIT.str());
    orderReq.entrust_type = EntrustType::ENTRUST;       //可不赋值，memset初始化时会默认设置为普通委托
    MQLogDebug << UTF8STR("策略正式进入运行阶段，开始报单:") << orderReq.symbol << "," << orderReq.amount <<"," << orderReq.side;
    context->getSysApi()->order(MQuant::AccountType::STOCK, orderReq);
}

void StrategyHandler::handleTick(IHtContext *context, const Tick *tick)
{
    //补充完全字段
//    MQLogDebug << UTF8STR("接收到实时行情：") << *tick  ;
//    MQLogDebug << UTF8STR("从缓存中查询最新行情数据：") << context->getSysApi()->getCurrentTick(tick->symbol);
}

void StrategyHandler::handleData(IHtContext *context, const KLineDataPush *kline)
{
    MQLogDebug << UTF8STR("接收到分钟K行情：") << *kline ;
}

void StrategyHandler::handleOrderRecord(IHtContext *context, const RecordOrder *recordOrder)
{
    MQLogDebug << UTF8STR("接收到逐笔委托：" )<< *recordOrder;
}

void StrategyHandler::handleOrderTransaction(IHtContext *context, const RecordTransaction *recordTrans)
{
    MQLogDebug << UTF8STR("接收到逐笔成交：") << *recordTrans;
}

void StrategyHandler::handleOrderReport(IHtContext *context, const Order *order)
{
    MQLogDebug << UTF8STR("接收到订单回报：") << *order;
}

void StrategyHandler::handleExecutionReport(IHtContext *context, const Execution *execution)
{
    MQLogDebug << UTF8STR("接收到成交回报：") << *execution;
}

void StrategyHandler::handleEtfEstimateInfo(IHtContext *context, const EtfEstimateInfo *estimateInfo)
{
    MQLogDebug << UTF8STR("接收到ETF预估信息：") << *estimateInfo;
}

void StrategyHandler::onStrategyParamsChange(IHtContext *context, const char *params, const char *filePath)
{
    MQLogDebug << "运行参数修改：" << params << filePath << context->runParams();
}

void StrategyHandler::marketOpen(IHtContext *context, int tradeDate)
{
    MQLogDebug << UTF8STR("接收到开盘信号：") << tradeDate;
}

void StrategyHandler::marketClose(IHtContext *context, int tradeDate)
{
    MQLogDebug << UTF8STR("接收到收盘信号：") << tradeDate;
}

void StrategyHandler::onStrategyEnd(IHtContext *context)
{
    MQLogDebug << UTF8STR("策略结束");
    context->getSysApi()->unsubscribeAll();

    m_thread1.quit();
    m_thread1.wait(1);
    m_thread2.quit();
    m_thread2.wait(1);
    m_thread3.quit();
    m_thread3.wait(1);
    m_thread4.quit();
    m_thread4.wait(1);
    m_thread5.quit();
    m_thread5.wait(1);

    m_downloadThread.quit();
    m_downloadThread.wait(1);
}

void StrategyHandler::timerFuncPeriod(IHtContext *context, int interval, const char *startTime,
                                      const char *endTime, const char *customField)
{
    MQLogDebug << UTF8STR("接收到周期定时信号：")<< interval << "," << startTime << "," << endTime << "," << customField;
    //查询委托、成交、资金、持仓,发布自定义信号

    context->getSysApi()->publishCustomMsg(MQUANT_CUSTOM_MSG_TYPE_BEGIN + 1, "hello world");
    //查询未成委托,仅查询当前实例的委托
    OrderQueryReq orderQryReq;
    memset(&orderQryReq, 0, sizeof(OrderQueryReq));
    orderQryReq.only_open_orders = true;
    orderQryReq.only_this_inst = true;
    orderQryReq.page_size = -1;     //查询所有可撤订单
    OrderList openOrdList = context->getSysApi()->getOrders(AccountType::STOCK, orderQryReq);
    MQLogDebug << UTF8STR("查询获取到当前实例的未成订单数量：") << openOrdList.getSize();

    MQLogDebug << UTF8STR("对未成订单进行撤单");
    for(int i =0 ; i < openOrdList.getSize(); i++){
        context->getSysApi()->cancelOrder(AccountType::STOCK, openOrdList.get(i).order_id);
    }
    MQLogDebug << UTF8STR("撤单结束");


    //查询当前账号的全部订单
    orderQryReq.only_open_orders = false;
    orderQryReq.only_this_inst = false;
    OrderList ordList = context->getSysApi()->getOrders(AccountType::STOCK, orderQryReq);
    MQLogDebug << UTF8STR("查询获取到A股账户的所有订单数量：") << ordList.getSize();

    //查询成交
    ExecutionQueryReq execQryReq;
    memset(&execQryReq, 0, sizeof(ExecutionQueryReq));
    execQryReq.only_this_inst = true;
    execQryReq.page_size = -1;
    ExecutionList execList = context->getSysApi()->getExecutions(AccountType::STOCK, execQryReq);
    MQLogDebug << UTF8STR("查询当前实例的成交数量：") << execList.getSize();

    execQryReq.only_this_inst = false;
    execQryReq.page_size = -1;
    execList = context->getSysApi()->getExecutions(AccountType::STOCK, execQryReq);
    MQLogDebug << UTF8STR("查询当前账号的成交数量：") << execList.getSize();

    execQryReq.only_rejected_orders = true;
    execList = context->getSysApi()->getExecutions(AccountType::STOCK, execQryReq);
    MQLogDebug << UTF8STR("查询当前账号的废单成交数量：") << execList.getSize();

//    查询持仓
    PositionList posList = context->getSysApi()->getPositions(AccountType::STOCK);
    MQLogDebug << UTF8STR("查询当前账号的持仓数量：") << posList.getSize();
    for(int i = 0; i < posList.getSize(); i++){
        MQLogDebug << "symbol:" << posList.get(i).symbol << posList.get(i);
    }

    testMargin(context);
    testEtf(context);
//    查询资金
    FundInfo fundInfo = context->getSysApi()->getFundInfo(AccountType::STOCK);
    MQLogDebug << UTF8STR("查询当前账号的资金返回：") << fundInfo;

    if(!m_batchOrderFlag){
        //报组合单，获取中证500ETF成分股,融资买入
        EtfConstituentList etfConstituents = context->getEtfHandler()->getEtfConstituentList("512500.SH");
        //查询融资标的
        SymbolList marginCashStocks = context->getMarginHandler()->getMarginCashStocks();

        MQLogDebug << UTF8STR("开始买入中证500ETF成分券");
        BatchOrderReq batchOrderReq;
        memset(&batchOrderReq, 0, sizeof(BatchOrderReq));
        batchOrderReq.batch_no = 1000;
        batchOrderReq.count = etfConstituents.getSize();
        batchOrderReq.req_list = new OrderRequest[batchOrderReq.count];
        memset(batchOrderReq.req_list, 0, sizeof(OrderRequest) * batchOrderReq.count);
        int j = 0;
        for(int i = 0;  i < etfConstituents.getSize(); i++){
            if(marginCashStocks.index(etfConstituents.get(i).symbol) < 0)
                continue;
            strcpy(batchOrderReq.req_list[j].symbol, etfConstituents.get(i).symbol);
            batchOrderReq.req_list[j].amount = etfConstituents.get(i).sample_size - etfConstituents.get(i).sample_size % 100;
            strcpy(batchOrderReq.req_list[j].price_type, PriceType::LIMIT.str());
            batchOrderReq.req_list[j].price= 5.0;
            batchOrderReq.req_list[j].entrust_type = EntrustType::CREDIT_FINANCING;
            batchOrderReq.req_list[j].side = OrderSide::BUY;
            j++;
        }
        batchOrderReq.count = j;
        MQLogDebug << UTF8STR("订单组装完成");
        context->getSysApi()->batchOrders(AccountType::MARGIN, batchOrderReq);
        m_batchOrderFlag = true;
        delete []batchOrderReq.req_list;
        MQLogDebug << UTF8STR("结束买入中证500ETF成分券");
    }
    else
    {
        //按批次号撤单
        MQLogDebug << UTF8STR("开始批量撤单，批次号：") << 1000;
        context->getSysApi()->cancelOrder(AccountType::MARGIN,NULL, 1000, 1);
        MQLogDebug << UTF8STR("结束批量撤单，批次号：") << 1000;
    }
}

void StrategyHandler::timerFuncOneShot(IHtContext *context, const char *time, const char *customField)
{
    MQLogDebug << UTF8STR("接收到单次定时信号:") << time << customField;
    //查询K线并打印
    KLineQueryReq req;
    memset(&req, 0, sizeof(KLineQueryReq));
    strcpy(req.symbol, "601688.SH");
    strcpy(req.kline_type , KLineDataType::KLINEDATA_1M.str());
    req.start_date = 20200301;
    req.end_date = 20200308;
    req.exrights_type = ExRightsType::FORWARD_EXRIGHTS;
    KLineDataList klineList = context->getSysApi()->queryKLineData(req);
    //打印第一条，同时打印总数
    if(klineList.getSize() > 0)
        MQLogDebug << UTF8STR("查询601688.SH K线返回:") << klineList.getSize() << "," << klineList.get(0);
    else
        MQLogWarn << UTF8STR("查询K线失败：") << "601688.SH";

    KLineQueryFromInitDateReq reqFromInitDate;
    memset(&reqFromInitDate, 0, sizeof(KLineQueryFromInitDateReq));
    strcpy(reqFromInitDate.symbol, "601688.SH");
    strcpy(reqFromInitDate.kline_type , KLineDataType::KLINEDATA_1D.str());
    reqFromInitDate.date_type = DateType::TRADE;
    reqFromInitDate.days = -30;
    reqFromInitDate.init_date = QDate::currentDate().toString("yyyyMMdd").toInt();
//    reqFromInitDate.exrights_type = ExRightsType::FORWARD_EXRIGHTS;
    reqFromInitDate.include_init_date = false;
    KLineDataList klineListFromInitDate = context->getSysApi()->queryKLineDataFromInitDate(reqFromInitDate);

//    //打印第一条，同时打印总数
    if(klineListFromInitDate.getSize() > 0)
        MQLogDebug << UTF8STR("查询601688.SH 日K线返回:") << klineListFromInitDate.getSize() << "," << klineListFromInitDate.get(0);
    else
        MQLogWarn << UTF8STR("查询K线失败：") << "601688.SH";
}

void StrategyHandler::onCustomMsg(IHtContext *context, int msgType, const char *msg)
{
    //接收自定义信号
    MQLogDebug << UTF8STR("接收到自定义信号：")<< msgType << msg;

}

void StrategyHandler::testMargin(IHtContext *context)
{
    //查询融资标的
//    if(m_testMarginFlag)
//        return ;
//    SymbolList marginCashStocks = context->getMarginHandler()->getMarginCashStocks();
//    SymbolList marginSecStocks = context->getMarginHandler()->getMarginSecStocks();
//    MarginContractList marginContractList = context->getMarginHandler()->getMarginContract();
//    MarginAsset marginAsset = context->getMarginHandler()->getMarginAsset();
//    MarginAssureSecurityList marginAssureSecList = context->getMarginHandler()->getMarginAssureSecurityList();
//    MarginAccountSecurityList marginAccountSecList = context->getMarginHandler()->getMarginAccountSecurityList();

//    MarginAvaliableQtyQueryReq req;
//    memset(&req, 0, sizeof(MarginAvaliableQtyQueryReq));
//    strcpy(req.symbol, "601688.SH");
//    req.side=OrderSide::BUY;
//    req.entrust_type = EntrustType::CREDIT_FINANCING;
//    req.position_type = PositionType::NORMAL;
//    strcpy(req.price_type , PriceType::BEST5_OR_CANCEL.str());
//    SecurityDetial detial = context->getSysApi()->getSymbolDetial("601688.SH");

//    req.price = detial.high_limit_px;
//    __int64 availableQty = context->getMarginHandler()->getAvaliableMarginQty(req);

//    MQLogDebug << UTF8STR("查询融资标的列表返回：") << marginCashStocks;
//    MQLogDebug << UTF8STR("查询融券标的列表返回：") << marginSecStocks;
//    MQLogDebug << UTF8STR("查询信用合约返回数量：") << marginContractList.getSize() ;
//    if(marginContractList.getSize() > 0){

//        MQLogDebug << UTF8STR("查询信用合约第一条合约：") << marginContractList.get(0) ;
//    }
//    MQLogDebug << UTF8STR("查询信用资产返回：") << marginAsset;
//    MQLogDebug << UTF8STR("查询担保券返回数量：") << marginAssureSecList.getSize() ;
//    if(marginAssureSecList.getSize() > 0){

//        MQLogDebug << UTF8STR("查询担保券第一条数据：") << marginAssureSecList.getSymbol(0) <<  marginAssureSecList.discountRatio(0);
//    }
//    MQLogDebug << UTF8STR("查询账户可融券列表返回数量：") << marginAccountSecList.getSize() ;
//    if(marginAccountSecList.getSize() > 0){

//        MQLogDebug << UTF8STR("查询账户可融券第一条数据：") << marginAccountSecList.getSymbol(0) << marginAccountSecList.marginSecurityInfo(0) ;
//    }
//    MQLogDebug << UTF8STR("查询大约可交易数量返回：") << availableQty ;
//    m_testMarginFlag = true;
}


void StrategyHandler::testEtf(IHtContext *context){

    //测试ETF申赎
    if(!m_etfPurchaseFlag)
    {
        context->getEtfHandler()->etfPurchase("510050.SH", 1);
        m_etfPurchaseFlag = true;
    }
    if(!m_etfRedemptionFlag)
    {
        EtfInfo etfInfo = context->getEtfHandler()->getEtfInfo("159919.SZ");
        MQLogDebug << UTF8STR("获取ETF信息返回：") << etfInfo;

        EtfConstituentList etfConstituentList = context->getEtfHandler()->getEtfConstituentList("159919.SZ");
        MQLogDebug << UTF8STR("获取ETF成分券返回数量：") << etfConstituentList.getSize() ;
        if(etfConstituentList.getSize() > 0){
            MQLogDebug << UTF8STR("获取ETF成分券返回第一条信息：") << etfConstituentList.get(0);
        }

        //判断ETF基金的可赎回数量是否大于或等于ETF基金份额
        PositionList posList = context->getSysApi()->getPositions(AccountType::STOCK, "159919.SZ");
        if(posList.getSize() > 0){
            MQLogDebug << UTF8STR("ETF基金可申赎数量：") << posList.get(0).redemption_num << UTF8STR(",一个申赎单位的ETF基金份额：") << etfInfo.report_unit;
            if(posList.get(0).redemption_num >= etfInfo.report_unit){
                //开始赎回
                m_etfRedemptionFlag = true;
                context->getEtfHandler()->etfRedemption("159919.SZ",1);
            }
        }
    }
}

void StrategyHandler::testMultiThread(IHtContext *context)
{
    //开启5个线程，分别监控5个列表中的标的
    m_thread1.setContext(context);
    m_thread1.setSymbolList(m_thread1Symbols);


    m_thread2.setContext(context);
    m_thread2.setSymbolList(m_thread2Symbols);


    m_thread3.setContext(context);
    m_thread3.setSymbolList(m_thread3Symbols);


    m_thread4.setContext(context);
    m_thread4.setSymbolList(m_thread4Symbols);

    m_thread5.setContext(context);
    m_thread5.setSymbolList(m_thread5Symbols);

    m_thread1.start();
    m_thread2.start();
    m_thread3.start();
    m_thread4.start();
    m_thread5.start();

}

void StrategyHandler::setLogOption()
{
//    HtLog::setConsoleLogLevel(LOG_MAX_LEVEL);   //向控制台输出文本非常影响性能，建议实盘运行的时候调用此命令关闭，测试时可用注释掉此命令
//    HtLog::setDispLogLevel(LOG_MAX_LEVEL);        //大于等于INFO类日志才推送到界面展示
//    HtLog::setLogLevel(LOG_INFO);

    //注册自定义日志，存储订单数据
    QString dataPath ="D:\\MQuantData";
    QDir mquantDataDir(dataPath);
    if(!mquantDataDir.exists())
        mquantDataDir.mkpath(dataPath);
    QString currentDir = QDir::currentPath();
    QDir::setCurrent(dataPath);

    QFile entrustFile("entrust.csv");
    if(!entrustFile.exists()){
        if(!entrustFile.open(QIODevice::WriteOnly|QIODevice::Text))
            qWarning() << UTF8STR("打开entrust.csv文件失败");
        else
        {
            entrustFile.close();
            HtLog::registerCustomLogFile("D:\\MQuantData\\entrust.csv", "entrust");
            m_registerCustomFileFlag = true;
        }
    }
    QDir::setCurrent(currentDir);
}

IStrategyInterface *getStrategyHandler()
{
    return &StrategyHandler::instance();
}
