#ifndef ISTRATEGYINTERFACE_H
#define ISTRATEGYINTERFACE_H
#include "mquantultis.h"

/// 各种系统接口类名称定义，通过getTradeHandler接口传入相应名称获取对应的接口类对象
#define MQUANT_GET_SYS_TRADE_HANDLER "getSysApi"
#define MQUANT_GET_ETF_TRADE_HANDLER "getEtfHandler"
#define MQUANT_GET_MARGIN_TRADE_HANDLER "getMarginHandler"
#define MQUANT_GET_ALGO_TRADE_HANDLER "getAlgoTradeHandler"
#define MQUANT_GET_OPTION_TRADE_HANDLER "getOptionTradeHandler"
#define MQUANT_GET_PYTHON_EXECUTOR "getPythonExecutor"
typedef void* MQUANT_PYMODULE;
namespace MQuant {

///系统API
class IMQuantSysApi {
public:
    ///
    /// \brief runTimelyPeriod 注册周期定时信号
    /// \param period 时间周期，单位为s
    /// \param startTime 开始时间，格式为09:30:00 000
    /// \param endTime 结束时间，格式为14:57:00 000
    /// \param customField 用户自定义字段，回调时会带给客户
    /// \return 0 成功， -1失败
    /// \remark 注册成功后，会通过timerFuncPeriod回调通知给客户
    virtual int runTimelyPeriod(int period, const char* startTime, const char* endTime, const char* customField = 0) = 0;
    ///
    /// \brief runTimelyOneShot 注册单次定时信号
    /// \param time 触发时间，格式为09:30:00 000
    /// \param customField 用户自定义字段，回调时会带给客户
    /// \return 0 成功， -1失败
    /// \remark 注册成功后，会通过timerFuncOneShot回调通知给客户
    virtual int runTimelyOneShot(const char* time, const char* customField = 0) = 0;

    ///
    /// \brief subscribe 订阅
    /// \param symbolList 标的列表，SymbolList类型
    /// \param type 订阅类型，SubscribeType类型
    /// \return 0 成功， -1失败
    ///
    virtual int subscribe(const SymbolList& symbolList, int type = MQuant::SubscribeType::TICK) = 0;

    ///
    /// \brief unsubscribe 退订行情
    /// \param symbolList 标的列表，SymbolList类型
    /// \param type 订阅类型，SubscribeType类型
    /// \return 0 成功， -1失败
    ///
    virtual int unsubscribe(const SymbolList& symbolList, int type = MQuant::SubscribeType::TICK) = 0;

    ///
    /// \brief unsubscribeAll 退订所有消息
    /// \return 0 成功， -1失败
    ///
    virtual int unsubscribeAll() = 0;

    ///
    /// \brief getCurrentTick 获取缓存中最新的tick数据
    /// \param symbol 标的
    /// \return 指定标的在缓存中的tick数据
    /// \remark 必须先订阅，缓存中才会有tick数据，否则会返回一个全空的Tick对象
    ///

    virtual Tick getCurrentTick(const char* symbol) = 0;

    ///
    /// \brief queryKLineData 查询K线
    /// \param req 查询请求
    /// \return k线列表
    ///
    virtual KLineDataList queryKLineData(const KLineQueryReq& req) = 0;

    ///
    /// \brief queryKLineDataFromInitDate 从指定初始日期查询N日的K线数据
    /// \param req 查询请求
    /// \return k线列表
    ///
    virtual KLineDataList queryKLineDataFromInitDate(const KLineQueryFromInitDateReq& req) = 0;

    ///
    /// \brief queryTick 查询历史tick
    /// \param symbol 标的
    /// \param startTime 开始时间，yyyyMMddhhmmss格式，不足14位会在末尾自动补0
    /// \param endTime  结束时间，yyyyMMddhhmmss格式，不足14位会在末尾自动补0
    /// \return 查询到的tick列表
    /// \remark 仅支持level-1数据查询，单次查询时间范围不可跨日
    virtual TickDataList queryTick(const char* symbol, __int64 startTime, __int64 endTime) = 0;
    ///
    /// \brief order 报单（异步），支持股、债、基、期货、期权、信用交易
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 报单请求
    /// \return 成功，返回Order对象，对象中部分字段返回，具体请查看备注；失败返回空的Order对象
    ///
    virtual Order order(int accountType, const OrderRequest& req) = 0;

    ///
    /// \brief batchOrders 批量报单（异步），支持股、债、基、期货、期权、信用交易
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 批量报单请求
    /// \return 返回OrderList列表
    ///
    virtual OrderList batchOrders(int accountType, const BatchOrderReq& req) = 0;

    ///
    /// \brief cancelOrder 单笔撤单(异步）
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param orderId  订单编号
    /// \param batchNo  批次号
    /// \param batchFlag 批次类型，0表示按照orderId撤单，1表示按照batchNo撤单，默认按照orderId撤单，期货暂不支持按批次号撤单
    /// \return 成功，返回Order对象，对象中部分字段返回，具体请查看备注；失败返回空的Order对象
    ///
    virtual Order cancelOrder(int accountType, const char* orderId, int batchNo = 0, int batchFlag = 0) = 0;

    ///
    /// \brief getOrders 获取订单列表
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 查询请求
    /// \return 符合条件的订单列表
    ///
    virtual OrderList getOrders(int accountType, const OrderQueryReq& req) = 0;

    ///
    /// \brief getExecutions 查询成交列表
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 查询请求
    /// \return 符合条件的成交列表
    ///
    virtual ExecutionList getExecutions(int accountType, const ExecutionQueryReq& req) = 0;

    ///
    /// \brief getPositions 获取持仓信息
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param symbol 标的，选填
    /// \return 符合条件的持仓列表
    ///
    virtual PositionList getPositions(int accountType, const char* symbol = "") = 0;

    ///
    /// \brief getFundInfo 获取资金信息
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \return 指定账户的资金信息
    ///
    virtual MQuant::FundInfo getFundInfo(int accountType) = 0;
    ///
    /// \brief subscribeCustomMsg 订阅自定义消息
    /// \param msgType 自定义消息类型，10001-65536之间的自定义值
    /// \return 0 成功， -1失败
    /// \remark 订阅成功后，接收到对应类型的自定义信号时，会通过onCustomMsg回调通知给客户
    ///
    virtual int subscribeCustomMsg(int msgType) = 0;

    ///
    /// \brief unsubscribeCustomMsg 退订自定义消息
    /// \param msgType 自定义消息类型，10001-65536之间的自定义值
    /// \return 0 成功， -1失败
    ///
    virtual int unsubscribeCustomMsg(int msgType) = 0;

    ///
    /// \brief publishCustomMsg 发布自定义消息
    /// \param msgType 自定义消息类型，10001-65536之间的自定义值
    /// \param msg 消息体,不定长度，用户自定义格式
    /// \return 0 成功， -1失败
    /// \remark
    ///
    virtual int publishCustomMsg(int msgType, const char* msg) = 0;

    ///
    /// \brief getSymbolList 获取标的列表
    /// \param exchangeType 市场，在ExchangeType命名空间中定义
    /// \param securityType 证券类型，在SecurityType命名空间中定义
    /// \param securitySubType 证券子类型，在SecuritySubType命名空间中定义
    /// \return 符合条件的标的代码
    /// \remark 如需批量订阅某市场某种类型全部标的的行情，可以先调用此接口获取到标的列表，然后直接将返回值传入subscribe接口订阅
    virtual SymbolList getSymbolList(const char* exchangeType, const char* securityType, const char* securitySubType) = 0;

    ///
    /// \brief getSymbolDetial 获取标的详情
    /// \param symbol 标的代码
    /// \return 标的详情信息，SecurityDetial类型
    ///
    virtual SecurityDetial getSymbolDetial(const char* symbol) = 0;

    ///
    /// \brief getFutureContractInfo 获取合约信息
    /// \param symbol 标的
    /// \param investType 投保标记,在MQuant::InvestType命名空间中定义
    /// \return
    ///
    virtual FutureContractInfo getFutureContractInfo(const char* symbol, int investType) = 0;

    ///
    /// \brief stopStragety 停止策略
    /// \param instId 被停止的实例ID，传入空字符串表示停止当前实例
    /// \return 0 成功， -1 失败
    ///
    virtual int stopStragety(const char* instId = "") = 0;

    ///
    /// \brief startStrategy 启动新的策略实例
    /// \param strategyFile 策略文件，支持.py文件和.dll文件
    /// \param runParams 启动参数，json格式
    /// \param instanceName 实例名称
    /// \param showParams
    /// \return
    ///
    virtual int startStrategy(const char* strategyFile, const StrategyParams& runParams = StrategyParams(), const char* instanceName = "", bool showParams = false) = 0;

    ///
    /// \brief getCurTime 获取当前时间，
    /// \return 实盘模式下返回当前机器时间，回测及行情回放模式下，返回根据行情计算出的当前时间
    ///
    virtual HtString getCurTime() = 0;

    ///
    /// \brief registerBacktestSymbols 注册回测标的
    /// \param symbols 回测标的列表
    /// \return 0 成功， -1 失败
    /// \remark 注册回测标的列表，回测策略脚本必须在初始化阶段调用，实盘脚本调用无影响
    virtual int registerBacktestSymbols(const SymbolList& symbols) = 0;

    ///
    /// \brief getIndexComponents 查询指数成分券
    /// \param indexSymbol 指数代码
    /// \return 指数成分券列表
    ///
    virtual IndexComponentList getIndexComponents(const char* indexSymbol) = 0;

    ///
    /// \brief getUserParams 在matic界面弹框，用户操作后将操作结果传给脚本
    /// \param jsonParams 界面参数，例如{"left_layout":{"name":"操作表格","ui_type":"table","columns":[
    /// {"name":"","ui_type":"check_box"},
    /// {"name":"证券代码","ui_type":"code_edit"},
    /// {"name":"持仓数量","ui_type":"line_edit","editable":"0"},
    /// {"name":"可用数量","ui_type":"line_edit","editable":"0"},
    /// {"name":"持仓均价","ui_type":"line_edit","editable":"0"},
    /// {"name":"止损价","ui_type":"line_edit"}],
    /// "rows":[[0,"601688.SH",5400,1800,29.15,29.15],[0,"000001.SZ",3800,3000,18.15,18.15]]}}
    /// 表示一个显示持仓的表格，策略调用此接口可以在matic界面展示这个表格，交易员可以修改止损价点击确定回传给策略
    /// \param wndTitle 弹出窗口标题
    /// \param sync 是否同步，默认同步阻塞，待用户操作完成后再返回，但是如果在策略消息回调中调用，同步会导致策略在阻塞期间不能接收到任何消息，同时消息会一直积压在策略端，此时可以根据自身需求确定是否需要设置为异步，如果设置为异步，将会在
    /// \return
    ///
    virtual HtString getUserParams(const char* jsonParams, const char* wndTitle, bool sync = true) = 0;

    ///
    /// \brief registerFastParamsWnd 注册快速输入参数的窗口
    /// \param wndTitle 窗口标题
    ///
    virtual void registerFastParamsWnd(const char* wndTitle) = 0;

    ///
    /// \brief playSound 播放声音
    /// \param wavFile
    ///
    virtual HtString playSound(const char* wavFile) = 0;

    ///
    /// \brief stopSound 停止播放声音
    /// \param wavFile
    ///
    virtual void stopSound(const char* soundId) = 0;

    ///
    /// \brief setInstanceRemark 设置实例备注
    /// \param instRemark
    ///
    virtual void setInstanceRemark(const char* instRemark) = 0;

    ///
    /// \brief messageBox 弹出提示框
    /// \param title 弹框标题
    /// \param msg 弹框消息
    /// \param sync 是否同步
    /// \param width 宽度
    /// \param height 高度
    /// \return 异步，直接返回false， 同步：如果用户点击弹框的确定按钮，返回true，否则返回false
    ///
    virtual bool messageBox(const char* title, const char* msg, bool sync = false, int width = 200, int height = 200) = 0;
    ///
    /// \brief bondConvertToStock 债转股接口
    /// \param convertSymbol
    /// \param qty
    /// \param accountType
    /// \return
    ///
    virtual Order bondConvertToStock(const char* convertSymbol, int qty, int accountType = AccountType::STOCK) = 0;

    ///
    /// \brief getSymbolDetialEx 获取标的详情(扩展字段，包含可转债字段）
    /// \param symbol 标的
    /// \return
    ///
    virtual MQuant::SecurityDetialEx getSymbolDetialEx(const char* symbol) = 0;

    ///
    /// \brief enableOrderReply 打开/关闭接收报撤单异步响应的全局开关
    /// \param enable true 允许接收 false 不允许接收
    /// \return 0 成功 -1 失败
    ///
    virtual int enableOrderReply(bool enable) = 0;

    ///
    /// \brief getCurrentTickEx 获取缓存中最新的tick数据(包含扩展字段）
    /// \param symbol
    /// \return
    ///
    virtual TickEx getCurrentTickEx(const char* symbol) = 0;

    ///
    /// \brief getPositions 获取持仓信息
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param symbol 标的，选填
    /// \return 符合条件的持仓列表
    /// 直查柜台，option_hold_type不提供
    virtual PositionList getPositionFromCounter(int accountType, const char* symbol = "") = 0;

    ///
    /// \brief order 报单（同order），新增支持自定义订单号、ETF申赎
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 报单请求
    /// \return 成功，返回Order对象，对象中部分字段返回，具体请查看备注；失败返回空的Order对象
    ///
    virtual Order orderEx(int accountType, const OrderRequestEx& req) = 0;

    ///
    /// \brief batchOrdersEx（同batchOrders），新增支持自定义订单号
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param req 批量报单请求
    /// \return 返回OrderList列表
    ///
    virtual OrderList batchOrdersEx(int accountType, const BatchOrderReqEx& req) = 0;
};

class IEtfHandler {
public:
    ///
    /// \brief getEtfInfo 获取ETF信息
    /// \param etfFundSymbol ETF基金代码
    /// \return ETF信息，EtfInfo类型
    ///
    virtual EtfInfo getEtfInfo(const char* etfFundSymbol) = 0;

    ///
    /// \brief getEtfConstituentList
    /// \param etfFundSymbol
    /// \return
    ///
    virtual EtfConstituentList getEtfConstituentList(const char* etfFundSymbol) = 0;

    ///
    /// \brief etfPurchase ETF申购
    /// \param etfFundSymbol ETF基金代码
    /// \param amount 申购份数
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param channelType 通道标志，仅对深市跨市ETF申赎有效，1为现金申赎，2为实物申赎，目前仅支持现金申赎，不支持实物申赎，接口预留方便后续扩展
    /// \return 订单对象
    ///
    virtual Order etfPurchase(const char* etfFundSymbol, int amount, int accountType = MQuant::AccountType::STOCK, int channelType = 1) = 0;

    ///
    /// \brief etfRedemption ETF赎回
    /// \param etfFundSymbol ETF基金代码
    /// \param amount 赎回份数
    /// \param accountType 账号类型，MQuant::AccountType命名空间中定义
    /// \param channelType 通道标志，仅对深市跨市ETF申赎有效，1为现金申赎，2为实物申赎，目前仅支持现金申赎，不支持实物申赎，接口预留方便后续扩展
    /// \return 订单对象
    ///
    virtual Order etfRedemption(const char* etfFundSymbol, int amount, int accountType = MQuant::AccountType::STOCK, int channelType = 1) = 0;
};

class IMarginHandler {
public:
    ///
    /// \brief marginCashDirectRefund 直接还款
    /// \param cash 金额
    /// \param contractNo 合约编号，不填按默认顺序归还
    /// \return 0 成功， -1 失败
    ///
    virtual int marginCashDirectRefund(double cash, const char* contractNo = "") = 0;

    ///
    /// \brief getMarginCashStocks 获取融资标的列表
    /// \return
    ///
    virtual SymbolList getMarginCashStocks() = 0;

    ///
    /// \brief getMarginSecStocks 获取融券标的列表
    /// \return
    ///
    virtual SymbolList getMarginSecStocks() = 0;

    ///
    /// \brief getMarginContract 获取合约信息
    /// \param contractNo 合约编号，为空表示获取所有合约
    /// \return 合约列表
    ///
    virtual MarginContractList getMarginContract(const char* contractNo = "") = 0;

    ///
    /// \brief getMarginAsset 获取信用资产
    /// \return 信用资产
    ///
    virtual MarginAsset getMarginAsset() = 0;

    ///
    /// \brief getMarginAssureSecurityList 获取担保券列表
    /// \return 担保券列表
    ///
    virtual MarginAssureSecurityList getMarginAssureSecurityList() = 0;

    ///
    /// \brief getMarginAccountSecurityList 获取账户的可融券信息
    /// \return 可融券列表
    ///
    virtual MarginAccountSecurityList getMarginAccountSecurityList() = 0;

    ///
    /// \brief getAvaliableMarginQty 获取大约可交易数量
    /// \param req 请求
    /// \return  数量
    /// \remark 根据客户输入的交易参数，返回大约可交易数量
    virtual __int64 getAvaliableMarginQty(const MarginAvaliableQtyQueryReq& req) = 0;
    ///
    /// \brief getMarginAssetFromCounter 从柜台查询信用资产
    /// \return MarginAssetRsp 对象
    /// \remark 目前限频10s一次，超过频率将会查询失败
    virtual MarginAssetRsp getMarginAssetFromCounter() = 0;
    ///
    /// \brief getMarginSecurityList
    /// \param symbol 标的
    /// \param posType 头寸性质，目前只支持专项头寸，传入普通头寸将会返回空列表
    /// \return
    ///
    virtual MarginAccountSecurityList getMarginSecurityList(const char* symbol = "", int posType = PositionType::VIP) = 0;
};

class IAlgoTradeHandler {
public:
};

class IOptionTradeHandler {
public:
    ///
    /// \brief optionExercise 期权行权
    /// \param symbol
    /// \param amount
    /// \return
    ///
    virtual Order optionExercise(const char* symbol, int amount) = 0;

    ///
    /// \brief underlyingSecurityFreeze 标的券锁定/解锁
    /// \param symbol 标的ETF证券
    /// \param amount 数量
    /// \param lock true 表示锁定， false 表示解锁
    /// \return
    /// \remark 上交所期权需要显式调用标的券锁定/解锁接口冻结标的券，才能进行备兑交易。备兑开仓前，应该先锁定标准券，备兑平仓后，应该在当日调用接口解锁标准券
    virtual Order underlyingSecurityFreeze(const char* symbol, int amount, bool lock) = 0;
};

class IPythonExecutor {
public:
    ///
    /// \brief execPy 执行外部python脚本
    /// \param pyFilePath python文件路径，建议全路径
    /// \param params 脚本执行参数
    /// \param pythonInterpreterPath python解释器路径，要带上python.exe，空字符串表示mquant内置解释器，也可以用用户本地安装的解释器
    /// \return 外部脚本运行输出
    /// \remark 同步执行外部python脚本,支持使用用户本地的python环境，典型的应用场景是通过第三方财经信息库获取数据并写入文件，在MQuant策略脚本中调用此接口执行外部脚本后，立即读取外部脚本落在本地的文件，仅支持初始化阶段调用
    virtual HtString execPy(const char* pyFilePath, const char* params = "", const char* pythonInterpreterPath = "") = 0;

    ///
    /// \brief registerPythonFile 注册后续要调用的python文件
    /// \param pyFilePath python文件路径
    /// \return python模块对象，用户需要记录此对象，后续调用python模块中的函数时需要传入
    /// \remark 必须在初始化函数中调用
    virtual MQUANT_PYMODULE registerPythonFile(const char* pyFilePath) = 0;

    ///
    /// \brief callPythonFunc 调用python模块中的函数
    /// \param module registerPythonFile返回的module
    /// \param funcName 函数名
    /// \param params 参数，仅允许传入字符串类型参数
    /// \return 返回值，为字符串类型
    ///
    virtual MQuant::HtString callPythonFunc(MQUANT_PYMODULE module, const char* funcName, const char* params) = 0;
};

class IHtContext {
public:
    virtual IMQuantSysApi* getSysApi() const = 0;
    virtual IEtfHandler* getEtfHandler() const = 0;
    virtual IMarginHandler* getMarginHandler() const = 0;
    virtual IAlgoTradeHandler* getAlgoTradeHandler() const = 0;
    virtual IOptionTradeHandler* getOptionTradeHandler() const = 0;
    virtual IPythonExecutor* getPythonExecutor() const = 0;
    ///
    /// \brief getTradeHandler 通用的获取接口类对象的函数
    /// \param getterName 获取接口类对象的函数名称，如传入MQUANT_SYS_TRADE_HANDLER与getSysApi函数返回值相同
    /// \return
    ///

    virtual void* getTradeHandler(const char* getterName) const = 0;
    ///
    /// \brief runParams 获取策略运行参数
    /// \return 策略运行参数，MQuant::StrategyParams类型
    ///
    virtual MQuant::StrategyParams& runParams() = 0;
    ///
    /// \brief getFundAccount 根据账号类型获取资金账号
    /// \param accountType 账号类型，在MQuant::AccountType命名空间中定义
    /// \return 资金账号，获取失败返回空字符串
    ///
    virtual HtString getFundAccount(int accountType = MQuant::AccountType::STOCK) = 0;
    ///
    /// \brief getBatchIndex 批量固化的策略获取批次序号
    /// \return
    ///
    virtual int getBatchIndex() = 0;

    ///
    /// \brief getStrategyPluginPath 获取插件的路径
    /// \return
    ///
    virtual HtString getStrategyPluginPath() = 0;
};

///策略事件处理函数
class IStrategyInterface {
public:
    ///
    /// \brief getStrategyParams 获取策略参数模板
    /// \return 策略参数，json格式{"key1":{"value":value1, "desc":"description","type":0},"key2":{"value":"value2","type":3},"key3":{"value":value3}}，支持int(0),float(1),bool(2),string(3),list(4),table(5)
    ///
    virtual StrategyParams getStrategyParams() {
        return StrategyParams();
    }
    ///
    /// \brief initialize 策略初始化
    /// \param context 上下文变量
    ///
    virtual void initialize(IHtContext* context) = 0;
    ///
    /// \brief onStrategyStart 策略开始
    /// \param context  上下文变量
    /// \remark 初始化完成后，立即调用此回调函数，在此回调函数中可以报单，不能读取外部文件
    virtual void onStrategyStart(IHtContext* /*context*/) {
    }
    ///
    /// \brief handleTick tick行情接收回调
    /// \param context 上下文变量
    /// \param tick tick数据，MQuant::Tick类型, 可以强转为MQuant::TickEx以获取扩展字段
    ///
    virtual void handleTick(IHtContext* /*context*/, const Tick* /*tick*/) {
    }
    ///
    /// \brief handleData k线行情接收回调
    /// \param context 上下文变量
    /// \param kline k线数据，MQuant::KLineData类型
    ///
    virtual void handleData(IHtContext* /*context*/, const KLineDataPush* /*kline*/) {
    }
    ///
    /// \brief handleOrderRecord 逐笔委托接收回调
    /// \param context 上下文变量
    /// \param recordOrder 逐笔委托，MQuant::RecordOrder类型
    /// \remark 仅深市标的有逐笔委托数据。由于逐笔数据量很大，订阅大量标的的逐笔数据时，请与华泰技术支持人员联系，获取解决方案
    ///        增加原始订单号、逐笔数据序号字段，可通过强转为RecordOrder591获取
    virtual void handleOrderRecord(IHtContext* /*context*/, const RecordOrder* /*recordOrder*/) {
    }
    ///
    /// \brief handleOrderTransaction 逐笔成交接收回调
    /// \param context 上下文变量
    /// \param recordTrans 逐笔成交，MQuant::RecordTransaction类型
    /// \remark 由于逐笔数据量很大，订阅大量标的的逐笔数据时，请与华泰技术支持人员联系，获取解决方案
    ///         增加成交编号字段，可通过强转为RecordTransaction591获取
    virtual void handleOrderTransaction(IHtContext* /*context*/, const RecordTransaction* /*recordTrans*/) {
    }
    ///
    /// \brief handleOrderReport 订单回报推送
    /// \param context 上下文变量
    /// \param order 订单
    ///
    virtual void handleOrderReport(IHtContext* /*context*/, const Order* /*order*/) {
    }
    ///
    /// \brief handleExecutionReport 成交回报推送
    /// \param context 上下文变量
    /// \param execution 成交
    ///
    virtual void handleExecutionReport(IHtContext* /*context*/, const Execution* /*execution*/) {
    }

    ///
    /// \brief handleEtfEstimateInfo ETF预估信息推送回调
    /// \param context 上下文变量
    /// \param estimateInfo 实时ETF预估信息
    /// \remark 需要先将订阅的ETF在ETF套利界面上添加到常用ETF列表中才可使用
    virtual void handleEtfEstimateInfo(IHtContext* /*context*/, const EtfEstimateInfo* /*estimateInfo*/) {
    }
    ///
    /// \brief onStrategyParamsChange 策略参数变更推送
    /// \param context 上下文变量
    /// \param params 参数
    /// \param filePath 参数文件路径，仅通过导入文件修改参数时有效
    ///
    virtual void onStrategyParamsChange(IHtContext* /*context*/, const char* /*params*/, const char* /*filePath*/) {
    }
    ///
    /// \brief marketOpen 开盘信号
    /// \param context 上下文变量
    /// \param tradeDate 交易日，yyyyMMdd格式
    /// \remark 仅回测模式下有效
    virtual void marketOpen(IHtContext* /*context*/, int /*tradeDate*/) {
    }
    ///
    /// \brief marketClose 收盘信号
    /// \param context  上下文变量
    /// \param tradeDate    交易日，yyyyMMdd格式
    /// \remark 仅回测模式下有效
    virtual void marketClose(IHtContext* /*context*/, int /*tradeDate*/) {
    }
    ///
    /// \brief onReadyStopStrategy 策略准备停止回调
    /// \param context 上下文变量
    /// \remark 策略停止时，首先调用此回调，然后再调用onStrategyEnd。此回调中允许交易，不允许和外部系统交互，可以在此回调中撤掉在途订单
    virtual void onReadyStopStrategy(IHtContext* /*context*/) {
    }
    ///
    /// \brief onStrategyEnd 策略停止回调
    /// \param context 上下文变量
    ///
    virtual void onStrategyEnd(IHtContext* /*context*/) {
    }
    ///
    /// \brief timerFuncPeriod 周期定时信号回调
    /// \param context 上下文变量
    /// \param interval 时间间隔
    /// \param startTime 开始时间 MQUANT_TIME_TYPE格式
    /// \param endTime 结束时间 MQUANT_TIME_TYPE格式
    /// \param customField 用户自定义数据
    ///
    virtual void timerFuncPeriod(IHtContext* /*context*/, int /*interval*/, const char* /*startTime*/, const char* /*endTime*/, const char* /*customField*/) {
    }
    ///
    /// \brief timerFuncOneShot 单次定时信号
    /// \param context 上下文变量
    /// \param time 触发时间 MQUANT_TIME_TYPE格式
    /// \param customField 用户自定义数据
    ///
    virtual void timerFuncOneShot(IHtContext* /*context*/, const char* /*time*/, const char* /*customField*/) {
    }
    ///
    /// \brief onCustomMsg 自定义消息接收回调
    /// \param context 上下文变量
    /// \param msgType 自定义消息类型，10001-65536之间的自定义值
    /// \param msg 消息体
    ///
    virtual void onCustomMsg(IHtContext* /*context*/, int /*msgType*/, const char* /*msg*/) {
    }

    ///
    /// \brief onRspUserParams 请求用户参数的异步响应，下个版本支持
    /// \param context 上下文
    /// \param jsonParams 客户修改后的参数，json格式
    /// \param wndTitle 窗口名称
    ///
    virtual void onRspUserParams(IHtContext* /*context*/, const char* /*jsonParams*/, const char* /*wndTitle*/) {
    }

    ///
    /// \brief onRequestUserParamsTemplate 点击实例监控界面上的手工干预按钮，调用此接口，下个版本支持
    /// \param params 点击时的用户传参
    ///
    virtual void onRequestUserParamsTemplate(IHtContext* /*context*/, const char* /*params*/) {
    }

    ///
    /// \brief onFastParams 接收快速参数数据， 暂未实现
    /// \param context
    /// \param params 参数
    ///
    virtual void onFastParams(IHtContext* /*context*/, const char* /*params*/) {
    }

    ///
    /// \brief onFundUpdate 资金更新
    /// \param context
    /// \param fundInfo
    ///
    virtual void onFundUpdate(IHtContext* /*context*/, const FundInfo* /*fundInfo*/) {
    }

    ///
    /// \brief onPositionUpdate 持仓更新
    /// \param context
    /// \param posInfo
    ///
    virtual void onPositionUpdate(IHtContext* /*context*/, const Position* /*posInfo*/) {
    }

    ///
    /// \brief handleFundFlow 实时资金流向回调
    /// \param context
    /// \param fundFlow 实时资金流向数据
    ///
    virtual void handleFundFlow(IHtContext* /*context*/, const FundFlow* /*fundFlow*/) {
    }

    ///
    /// \brief onRecvOrderReply 报撤单异步响应
    /// \param context
    /// \param orderReply 异步响应数据
    ///
    virtual void onRecvOrderReply(IHtContext* /*context*/, const OrderReply& /*orderReply*/) {
    }
};

}  // namespace MQuant

#define MQLogDebug MQuant::HtLog(__FILE__, __LINE__, __FUNCTION__).debug()
#define MQLogInfo MQuant::HtLog(__FILE__, __LINE__, __FUNCTION__).info()
#define MQLogWarn MQuant::HtLog(__FILE__, __LINE__, __FUNCTION__).warn()
#define MQLogError MQuant::HtLog(__FILE__, __LINE__, __FUNCTION__).error()
#define MQUserLog MQuant::HtLog(__FILE__, __LINE__, __FUNCTION__).userLog

#endif  // ISTRATEGYINTERFACE_H
