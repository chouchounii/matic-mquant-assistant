#ifndef MQUANTSTRUCT_H
#define MQUANTSTRUCT_H
#define MQUANT_SYMBLE_LEN 14
#define MQUANT_DATETIME_LEN 24
#define MQUANT_TIME_LEN 12
#define MQUANT_ORDER_SIDE_LEN 8

#define MQUANT_CUSTOM_MSG_TYPE_BEGIN 10000  //用户自定义信号的起始值

typedef char MQUANT_DATETIME_TYPE[MQUANT_DATETIME_LEN];  //日期时间，格式为yyyyMMdd hh:mm:ss zzz
typedef char MQUANT_TIME_TYPE[MQUANT_TIME_LEN];  //时间，格式为hh:mm:ss zzz
typedef char MQUANT_SYMBOL_TYPE[MQUANT_SYMBLE_LEN];
typedef char MQUANT_ORDER_ID_TYPE[64];
typedef char MQUANT_KLINE_TYPE[12];

#ifdef HTCXXADAPTER_LIBRARY
#define MQUANT_EXPORT __declspec(dllexport)
#else
#define MQUANT_EXPORT __declspec(dllimport)
#endif

namespace MQuant {

class MQUANT_EXPORT HtString {
public:
    //默认构造函数
    HtString();
    //带参构造函数
    HtString(const char* str);
    //析构函数
    ~HtString();
    //拷贝构造
    HtString(const HtString& obj);
    //复制初始化
    HtString& operator=(const HtString& obj);

public:
    //把后面字符串加在*this上
    HtString& operator+=(const HtString& obj);
    //把后面字符串加在前面
    HtString operator+(const HtString& obj);
    //==判断字符串是否相等
    bool operator==(const HtString& str) const;
    //!=判断字符串是否相等
    bool operator!=(const HtString& str) const;
    //[]下标运算符重载
    char operator[](int index);
    //返回字符串长度
    int length() const;
    //取从position所指位置连续取len个字符组成子串返回
    const char* str() const;

private:
    char* _str;
    int _len;
};
////K线类型
namespace KLineDataType {
    const HtString KLINEDATA_1M = "kline_1m";  // 1分钟k线
    const HtString KLINEDATA_5M = "kline_5m";  // 5分钟线
    const HtString KLINEDATA_15M = "kline_15m";  // 15分钟线
    const HtString KLINEDATA_30M = "kline_30m";  // 30分钟线
    const HtString KLINEDATA_60M = "kline_60m";  // 60分钟线
    const HtString KLINEDATA_1D = "kline_1d";  // 日k线
}  // namespace KLineDataType

namespace SubscribeType {
    const int TICK = 1;  // tick行情
    const int KLINE_1M = 2;  // 1分钟k线
    const int RECORD_ORDER = 3;  // 逐笔委托
    const int RECORD_TRANSACTION = 4;  // 逐笔成交
    const int ETF_ESTIMATE_INFO = 5;  // ETF预估信息
    const int FUND_FLOW = 6;  // 实时资金流向
}  // namespace SubscribeType

namespace ExRightsType {
    const int DEFAULT = 0;  //默认，前复权
    const int NO_EXRIGHTS = 10;  //不复权
    const int FORWARD_EXRIGHTS = 11;  //向前复权
    const int BACKWARD_EXRIGHTS = 12;  //向后复权
}  // namespace ExRightsType
////交易方向
namespace OrderSide {
    const int UNKNOWN = 0;
    const int BUY = 1;  // 买入
    const int SELL = 2;  // 卖出
}  // namespace OrderSide

/// 信用合约类型
namespace MarginContractType {
    const int UNKNOWN = 0;  //未知类型
    const int FINACING = 1;  // 融资合约
    const int SECURITY = 2;  // 融券合约
}  // namespace MarginContractType

////日期类型
namespace DateType {
    const int NORMAL = 0;  // 自然日
    const int TRADE = 1;  // 交易日
}  // namespace DateType

////仓位属性
namespace PositionProp {
    const int SHORT_FOR_SHORT = 0;  // 备兑持仓
    const int LONG_POSITION = 1;  // 多仓
    const int SHORT_POSITION = 2;  // 空仓
    const int UNKNOWN = -1;  // 未知
}  // namespace PositionProp

////订单行为
namespace OrderAction {
    const int UNKNOWN = 0;
    const int OPEN = 1;  // 开
    const int CLOSE = 2;  // 平
    const int EXERCISE = 3;  // 行权
    const int AUTO_EXERCISE = 4;  // 自动行权
}  // namespace OrderAction
////期货投保标记
namespace InvestType {
    const int UNKNOWN = 0;  // 未知
    const int SPECULATION = 1;  // 投机
    const int HEDGING = 2;  // 套保
    const int ARBITRAGE = 3;  // 套利
}  // namespace InvestType

////平仓类型
namespace CloseDirection {
    const int DEFAULT = 0;  // 默认
    const int CLOSE_TODAY = 1;  // 平今仓
    const int CLOSE_OLD = 2;  // 平老仓
    const int FORCE_CLOSE = 3;  // 强平
    const int FORCE_OFF = 4;  // 强减
    const int LOCAL_FORCE_CLOSE = 5;  // 本地强平
}  // namespace CloseDirection

namespace OrderStatus {
    const int NEW = 1;  ///待报
    const int OPEN = 2;  ///已报交易所
    const int PENDING_CANCEL = 3;  ///已报待撤
    const int PARTIALLY_FILLED_FOR_CANCEL = 4;  ///部分成交待撤
    const int PARTIALLY_CANCEL = 5;  ///部分撤销
    const int CANCELLED = 6;  ///已撤
    const int PARTIALLY_FILLED = 7;  ///部分成交
    const int FILLED = 8;  ///全部成交
    const int REJECTED = 9;  ///废单
}  // namespace OrderStatus

namespace PriceType {
    const HtString LIMIT = "1";  ///限价
    const HtString BEST5_OR_CANCEL = "a";  ///五档即成剩撤
    const HtString BEST5_OR_LIMIT = "b";  ///上交所五档即成剩转限价
    const HtString BEST_OR_CANCEL = "c";  ///深交所即成剩撤
    const HtString REVERSE_BEST_LIMIT = "d";  ///深交所对手方最优（科创板对手方最优）
    const HtString FORWARD_BEST = "e";  ///深交所本方最优（科创板本方最优）
    const HtString ALL_OR_CANCEL = "f";  ///深交所全额成交或撤
    const HtString BEST5_OR_LIMIT_CF = "l";  ///中金所五档即成剩撤
    const HtString AFTER_MARKET_FIX_PRICE = "PFP";  ///盘后固定价（科创板）
    const HtString HK_IN_TRADE = "0";  ///增强限价盘
    const HtString HK_BEFORE_TRADE = "2";  ///竞价限价盘
}  // namespace PriceType
/// 委托属性
namespace EntrustProp {
    const HtString TRADE = "0";  // 买卖
    const HtString REDEMPTION = "N";  // ETF申赎
    const HtString PLEDGE = "f";  // 债券质押
    const HtString COVERED_TRANSFER = "9";  //  备兑划转
    const HtString OPTION_EXERCISE = "a";  // 期权行权
    const HtString CONVERSION = "7";  //转股
}  // namespace EntrustProp
///委托类型，成交类型
namespace EntrustType {
    const int ENTRUST = 0;  // 委托
    const int CANCEL_ORDER = 2;  // 撤单
    const int CREDIT_FINANCING = 6;  // 信用融资    卖券还款、融资买入
    const int CREDIT_MARGIN = 7;  // 信用融券      融券卖出、买券还券
    const int CREDIT_OPEN = 8;  // 信用平仓//
    const int CREDIT_TRANSACTIONS = 9;  // 信用交易    担保品买卖
    const int RETURN_SEC_DIRECT = 10;  //直接还券
}  // namespace EntrustType

/// 期权备兑信息
namespace OptionCoveredFlag {
    const int UNKNOWN = 0;  // 未知
    const int UNCOVERED = 1;  // 非备兑
    const int COVERED = 2;  // 备兑
}  // namespace OptionCoveredFlag
/// 期权持仓类型
namespace OptionHoldType {

    const int DEFAULT = 0;
    const int RIGHT = 1;  // 权利仓
    const int COMPULSORY = 2;  // 义务仓
    const int COVEREDE = 3;  // 备兑仓
}  // namespace OptionHoldType
/// 账号类型
namespace AccountType {
    const int UNKNOWN = 0;
    const int STOCK = 1;  // A股账号
    const int MARGIN = 2;  // 信用账号
    const int FUTURES = 3;  // 期货
    const int OPEN_FUND = 4;  // 场外基金，暂不支持
    const int OPTION = 5;  // 期权
}  // namespace AccountType
/// 头寸类型
namespace PositionType {
    const int DEFAULT = 0;  // 默认，等价于普通头寸
    const int NORMAL = 1;  // 普通头寸
    const int VIP = 2;  // 专项头寸
}  // namespace PositionType

/// 交易所定义
namespace ExchangeType {
    const HtString SH = "SH";  //上海

    const HtString SZ = "SZ";  //深圳

    const HtString SHF = "SHF";  //上期

    const HtString CFFEX = "CF";  //中金

    const HtString ZCE = "ZCE";  //郑商

    const HtString DCE = "DCE";  //大商

    const HtString HK = "HK";  //港交所

    const HtString SI = "SI";  //申万

    const HtString BJ = "BJ";  // 北交所
}  // namespace ExchangeType

/// 证券交易市场
namespace SecurityExchangeType {
    const HtString UNKNOWN = "-1";  //未知

    const HtString SH = "1";  //#上交所

    const HtString SZ = "2";  //深交所

    const HtString TZA = "9";  //特转A

    const HtString TZB = "A";  //特转B

    const HtString YHJ = "B";  //银行间

    const HtString JJ = "C";  //基金

    const HtString SHB = "D";  //上海B

    const HtString HHK = "G";  //沪HK（沪港通南向）

    const HtString SZB = "H";  //深圳B

    const HtString HK = "HK";  //香港

    const HtString CFFEX = "M";  //中金所

    const HtString SHF = "N";  //上期所

    const HtString CZCE = "O";  //郑商所

    const HtString DCE = "P";  //大商所

    const HtString SZHK = "S";  //深HK（深港通南向）

    const HtString SSC = "SSC";  //沪股通（沪港通北向）

    const HtString SZC = "SZC";  //深股通（深港通北向）
}  // namespace SecurityExchangeType

/// 标的类型定义
namespace SecurityType {
    const HtString All = "";  //所有标的

    const HtString IndexType = "1";  //指数

    const HtString StockType = "2";  //股票

    const HtString FundType = "3";  //基金

    const HtString BondType = "4";  //债券

    const HtString RepoType = "5";  //回购

    const HtString WarrantType = "6";  //权证，暂不支持

    const HtString OptionType = "7";  //期权

    const HtString FuturesType = "8";  //期货

    const HtString ForexType = "9";  //外汇，暂不支持

    const HtString RateType = "10";  //利率，暂不支持

    const HtString NmetalType = "11";  //贵金属，暂不支持
}  // namespace SecurityType
/// 标的子类型
namespace SecuritySubType {

    const HtString AsiaIndex = "01002";  //亚洲指数

    const HtString InternationalIndex = "01003";  //国际指数

    const HtString Systemclassificationindex = "01004";  //系统分类指数

    const HtString Userclassificationindex = "01005";  //用户分类指数

    const HtString Futuresindex = "01006";  //期货指数

    const HtString Indexspot = "01007";  //指数现货

    const HtString Ashares = "02001";  // A股（主板）

    const HtString ScienTechInnovateBoard = "02200";  //科创板

    const HtString Smallandmediumstock = "02002";  //中小板股

    const HtString Gemstock = "02003";  //创业板股

    const HtString Strategicemergingboard = "02006";  //战略新兴板

    const HtString Newthreeboard = "02007";  //新三板

    const HtString MainboardofHongKongshares = "02008";  //港股主板

    const HtString HongKongEquitygem = "02009";  //港股创业板

    const HtString HongkonglistedNASDstock = "02010";  //香港上市NASD股票

    const HtString Hongkongextendedplatestock = "02011";  //香港扩展板块股票

    const HtString BjSelectedStock = "02022";  // 北交所精选层

    const HtString Preferredstock = "02100";  //优先股

    const HtString Fund = "03001";  //基金（封闭式）

    const HtString ListedopenfundLOF = "03003";  //上市开放基金LOF

    const HtString TradingopenindexfundETF = "03004";  //交易型开放式指数基金ETF

    const HtString Classificationsubfund = "03005";  //分级子基金

    const HtString Extendedplatefund = "03006";  //扩展板块基金(港)

    const HtString Redemptionfundonly = "03007";  //仅申赎基金

    const HtString Governmentbonds = "04001";  //政府债券（国债）

    const HtString Corporatebond = "04002";  //企业债券

    const HtString Financialbond = "04003";  //金融债券

    const HtString Corporatedebt = "04004";  //公司债

    const HtString Convertiblebond = "04005";  //可转债券

    const HtString Privatedebt = "04006";  //私募债

    const HtString Exchangeableprivatedebt = "04007";  //可交换私募债

    const HtString Securitiescompanysubordinateddebt = "04008";  //证券公司次级债

    const HtString Securitiescompanyshorttermdebt = "04009";  //证券公司短期债

    const HtString Exchangeablecorporatedebt = "04010";  //可交换公司债

    const HtString Bondpreissue = "04011";  //债券预发行

    const HtString Pledgetypetreasurybondrepurchase = "05001";  //质押式国债回购

    const HtString Thecorporatebondpledgedrepo = "05002";  //质押式企债回购

    const HtString Buybackofbuyoutbond = "05003";  //买断式债券回购

    const HtString Bidrepurchase = "05004";  //报价回购

    const HtString Stockoption = "07001";  //个股期权

    const HtString ETFoption = "07002";  // ETF期权

    const HtString Indexfutures = "08001";  //指数期货

    const HtString Commodityfutures = "08002";  //商品期货

    const HtString Stockfutures = "08003";  //股票期货

    const HtString Bondfutures = "08004";  //债券期货

    const HtString Interbankinterestratefutures = "08005";  //同业拆借利率期货

    const HtString ExchangeFundNoteFuturesExchangeFundpaperfutures = "08006";  //外汇基金票据期货

    const HtString ExchangeForPhysicalsfuturesturntospot = "08007";  //期货转现货
}  // namespace SecuritySubType

/// 股票的ST标识
namespace STStatus {
    const int NOT_ST = 0;  // 非ST标的
    const int ST = 1;  // ST
    const int STAR_ST = 2;  // *ST
}  // namespace STStatus

namespace TradingPhaseCode {
    const int StartBeforeOpen = 0;  //开盘前，启动
    const int OpenAggregateAuction = 1;  //开盘集合竞价
    const int AfterAggregateAuction = 2;  //开盘集合竞价阶段结束到连续竞价阶段开始之前
    const int ContinuousAuction = 3;  //连续竞价
    const int CloseAtNoon = 4;  //中午休市
    const int CloseAggregateAuction = 5;  //收盘集合竞价
    const int Closed = 6;  //已闭市
    const int PostTrading = 7;  //盘后交易
    const int TemporarilySuspended = 8;  //临时停牌
    const int VolatilityInterrupted = 9;  //波动性中断
}  // namespace TradingPhaseCode
// ETF基金类型
namespace EtfType {
    const int singleMarket = 1;  //# 单市场ETF
    const int crossBorder = 2;  //  # 跨境ETF
    const int crossMarket = 3;  //  # 跨市ETF
    const int currency = 4;  // # 货币ETF
    const int bond = 5;  // # 实物债券ETF
    const int commodity = 6;  //  # 商品ETF
    const int cashBond = 7;  // # 现金债券ETF
    const int unknown = -1;  //  # 未知
}  // namespace EtfType

// ETF基金状态
namespace EtfStatus {
    const int forbidden = 0;  //  # 全部禁止
    const int allow = 1;  //# 全部允许
    const int onlyAllowApplyBuy = 2;  //  # 仅允许申购
    const int onlyAllowRedemption = 3;  //  # 仅允许赎回
    const int unknown = -1;  //  # 未知
}  // namespace EtfStatus

//单条行情数据结构
struct Tick {
    MQUANT_SYMBOL_TYPE symbol;  //交易所内证券代码
    int time;  //时间(HHMMSSmmm)
    int date;  //日期(yyyyMMdd)
    double pre_close;  //昨收价格
    double open;  //开盘价格
    double high;  //最高价格
    double low;  //最低价格
    double current;  //最新价格
    unsigned __int64 num_trades;  //成交笔数,股票、基金、期权有，指数、期货没有
    unsigned __int64 volume;  //总成交量[股]，期货为手
    double money;  //总成交金额[元]
    unsigned __int64 bid_volume;  //当前时刻委买挂单总量[股]
    unsigned __int64 ask_volume;  //当前时刻委卖挂单总量[股]
    double w_bid_price;  //加权平均委买价格
    double w_ask_price;  //加权平均委卖价格
    double bid_price[10];  //申买价格
    double ask_price[10];  //申卖价格
    int bid_vol[10];  //申买量
    int ask_vol[10];  //申卖量
    double high_limit;  //涨停价格
    double low_limit;  //跌停价格
    double close;  //今日收盘
    //以下为期货期权黄金现货专用
    unsigned __int64 pre_open_interest;  //昨持仓
    unsigned __int64 open_interest;  //持仓总量
    double pre_settle_price;  //昨结算价
    double settle_price;  //今结算价
    double pre_delta;  //昨虚实度
    double curr_delta;  //今虚实度
    //新增字段
    char trading_phase_code[4];  //行情状态
    int b1_amount_list[50];  //买一委托数量列表
    int a1_amount_list[50];  //卖一委托数量列表
    unsigned __int64 total_bid_number;  // int64	买入总笔数
    unsigned __int64 total_ask_number;  // int64	卖出总笔数
    //
    int bid_nums[10];  //申买委托笔数
    int ask_nums[10];  //申卖委托笔数
    int withdraw_buy_num;  //买入撤单笔数
    unsigned __int64 withdraw_buy_amount;  //买入撤单数量
    double withdraw_buy_money;  //买入撤单金额
    int withdraw_sell_num;  //卖出撤单笔数
    unsigned __int64 withdraw_sell_amount;  //卖出撤单数量
    double withdraw_sell_money;  //卖出撤单金额
    unsigned __int64 bid_trade_max_duration;  //买入委托成交最大等待时间
    unsigned __int64 ask_trade_max_duration;  //卖出委托成交最大等待时间
    int num_bid_orders;  //买方委托价位数
    int num_ask_orders;  //卖方委托价位数
    double IOPV;  // ETF的IOPV，交易所原始数据
    //    char                reserved[256];          //保留字段，用于后续扩展
    int ClientTime;  //客户端接收tick时间(HHmmssmmm)
    //适配深交所新债券交易系统的数据变化
    int LastPxType;  // LastPx对应的成交方式，取值：1-匹配成交；2-协商成交；3-点击成交；4-询价成交；5-竞买成交
    double AuctionLastPx;  //匹配成交最新成交价
    unsigned __int64 AuctionVolumeTrade;  //匹配成交成交量
    unsigned __int64 AuctionValueTrade;  //匹配成交成交金额
};

struct KLineData {
    MQUANT_DATETIME_TYPE datetime;  // 时间，yyyyMMdd hh:mm:ss zzz格式
    double open;  // k线周期内的开盘价
    double close;  // k线周期内的收盘价
    double high;  // k线周期内的最高价
    double low;  // k线周期内的最低价
    __int64 total_volume;  // k线周期内的成交总量
    double total_money;  // k线周期内的总成交金额
    __int64 num_trades;  // k线周期内的总成交笔数
    MQUANT_SYMBOL_TYPE symbol;  // 证券代码
    __int64 open_interest;  //期权期货持仓总量
    double settle_price;  //期权期货结算价
};

struct KLineDataPush : public KLineData {
    MQUANT_KLINE_TYPE data_type;  // k线数据类型，例如KLineDataType::KLINEData_1M;
};

struct RecordOrder {
    MQUANT_SYMBOL_TYPE symbol;  // 标的,MQuant格式
    MQUANT_DATETIME_TYPE datetime;  // 行情时间
    int side;  // 买卖方向,OrderSide命名空间中定义，如OrderSide::BUY
    int order_type;  // -1 无效，1 市价 2 限价 3 本方最优
    double price;  // 委托价格
    int amount;  // 委托数量
    __int64 order_index;  //委托编号

    __int64 order_no;  //原始订单号（上交所在新增、删除订单时用以标识订单的唯一编号）
    __int64 appl_seq_num;  //逐笔数据序号
    __int64 channel_no;  //交易所原始频道代码
};

struct RecordTransaction {
    MQUANT_SYMBOL_TYPE symbol;  // 标的,MQuant格式
    MQUANT_DATETIME_TYPE datetime;  // 行情时间
    int trade_type;  // 成交类别，
                     // 0	交易业务成交记录
                     // 1 交易业务撤单回报记录
                     // 2	即时成交剩余撤销委托”未能成交部分或其他原因的自动撤单回报记录
                     // 3 ETF基金申购/赎回成功回报记录或ETF基金申购/赎回成功证券给付明细回报记录 4	ETF基金申购/赎回撤单报记录
                     // 5 最优五档即时成交剩余撤销委托”未能成交部分的自动撤单或其他原因的自动撤单回报记录
                     // 6 全额成交或撤销委托”未能成交时的自动撤单或其他原因自动撤单回报记
                     // 7 本方最优价格委托的撤单回报记录
                     // 8	对手方最优价格委托的撤单回报记录
                     // 9	ETF基金申购/赎回成功允许/必须现金替代明细回报记录
    int side;  // 买卖方向,OrderSide命名空间中定义，如OrderSide::BUY
    double price;  // 成交价格
    int amount;  // 成交数量
    double business_balance;  // 成交金额
    __int64 trade_buy_no;  //买方委托序号
    __int64 trade_sell_no;  //卖方委托序号

    __int64 trade_index;  //成交编号
    __int64 appl_seq_num;  //逐笔数据序号
    __int64 channel_no;  //交易所原始频道代码
};

struct Order {
    int status{0};  // 状态, 一个OrderStatus命名空间中定义，随着订单执行状态改变，包含在订单执行回报中
    MQUANT_DATETIME_TYPE add_time{""};  // 订单创建时间, datetime.datetime对象，为后台时间，包含在订单执行回报中
    int amount{0};  // 下单数量, 不管是买还是卖, 都是正数，下单立即返回，包含在订单执行回报中
    int filled{0};  // 已经成交的股票数量, 正数，包含在订单执行回报中
    MQUANT_SYMBOL_TYPE symbol{""};  // 股票代码，同下单请求的security参数，下单立即返回，包含在订单执行回报中
    MQUANT_ORDER_ID_TYPE order_id{""};  // 订单ID，为M-Quant系统内部生成的订单id，非委托编号，下单/撤单立即返回，包含在订单执行回报中
    double price{0.0};  //委托价格，包含在订单执行回报中
    double avg_cost{0.0};  // 平均成交价格, 已经成交的股票的平均成交价格(一个订单可能分多次成交)，包含在订单执行回报中
    int side{0};  // 买卖方向,下单立即返回，包含在订单执行回报中,OrderSide命名空间中定义，如OrderSide::BUY
    int action{0};  // 开平行为，OrderAction命名空间中定义，期货有效
    int invest_type{0};  // 投资类型，InvestType命名空间中定义，报单立即返回，期货有效
    int close_direction{0};  // 平仓类型，CloseDirection命名空间中定义，报单立即返回，期货有效

    int batch_no{0};  // 批次号，包含在订单执行回报中
    MQUANT_ORDER_ID_TYPE orig_order_id{""};  // 原始订单号，撤单订单有效，目前保留字段，撤单接口的返回值中该字段有效
    char price_type[4]{""};  // 价格类型，限价单或各种类型市价单等
    char cancel_info[256]{""};  // 废单原因，废单有效
    int withdraw_amount{0};  // 撤单数量，包含在订单执行回报中
    double business_balance{0.0};  // 已成交金额，包含在订单执行回报中
    double clear_balance{0.0};  // 成交费用，目前保留字段
    char entrust_prop[4]{""};  // 委托属性，包含在订单执行回报中
                               // "0"// 买卖
                               // "N"// ETF申赎
                               // "f": // /债券质押
    int entrust_type{0};  // 委托类型，包含在订单执行回报中，在EntrustType命名空间中定义
    char algo_inst_id[64]{""};  // 算法实例ID，由MQuant启动的算法实例相关订单中有此字段，包含在订单执行回报中
    char error_code[8]{""};  // 错误码
    char inst_id[64]{""};  // Mquant实例ID，仅查询订单返回
    char trader_name[32]{""};  // 交易员名
    char entrust_no[64]{""};  // 柜台委托编号，2019.11.19添加
    int covered_flag{0};  // 期权备兑标志，OptionCoveredFlag命名空间中定义，2019.11.19添加，期权有效
    int last_amount{0};  //本次成交数量，仅推送有效，2020.3.2添加
    double last_price{0.0};  //本次成交价格，仅推送有效，2020.3.2添加
    char fund_account[20]{""};
    char security_exchange[4]{""};  // 市场,SecurityExchangeType命名空间中定义
};

struct Execution {
    MQUANT_DATETIME_TYPE time{""};  // 成交时间，查询及推送返回
    int amount{0};  // 成交数量，查询及推送返回
    double price{0.0};  // 成交价格，查询及推送返回
    char trade_id[64]{""};  // 柜台成交编号，2019.11.19新增，查询及推送返回
    MQUANT_ORDER_ID_TYPE order_id{""};  // 订单id，查询及推送返回
    MQUANT_SYMBOL_TYPE symbol{""};  // 股票代码，MQuant格式，下单立即返回，查询及推送返回
    int entrust_type{0};  // 委托类型，EntrustType命名空间中定义，查询及推送返回
    double business_balance{0.0};  // 成交金额，查询及推送返回
    double cost_balance{0.0};  // 成交费用，目前保留字段
    MQUANT_ORDER_ID_TYPE orig_order_id{""};  // 原始订单号，撤单订单有效，目前保留字段
    int side{0};  // 买/卖方向,OrderSide命名空间中定义，查询及推送返回
    char algo_inst_id[64]{""};  // 算法实例ID，由MQuant启动的算法实例相关订单中有此字段，包含在订单执行回报中
    char inst_id[64]{""};  // Mquant实例ID，仅查询成交返回
    char trader_name[32]{""};  // 交易员，查询及推送返回
    int action{0};  // 开平行为，OrderAction命名空间中定义，期货有效
    int invest_type{0};  // 投资类型，InvestType命名空间中定义，报单立即返回，期货有效
    int close_direction{0};  // 平仓类型，CloseDirection命名空间中定义，报单立即返回，期货有效
    char entrust_no[64]{""};  // 柜台委托编号， 2019.11.19新增
    char priceType[4]{""};  // 价格类型，限价单或各种类型市价单等
    char fund_account[20]{""};
    char security_exchange[4]{""};  // 市场,SecurityExchangeType命名空间中定义
};
///单只标的的持仓信息
struct Position {
    MQUANT_SYMBOL_TYPE symbol{""};  // 标的代码
    double price{0.0};  // 最新行情价格
    double hold_cost{0.0};  // 持仓成本价
    MQUANT_DATETIME_TYPE init_time{""};  // 建仓时间，格式为 datetime.datetime,该字段目前保留
    MQUANT_DATETIME_TYPE transact_time{""};  // 最后交易时间，格式为 datetime.datetime,该字段目前保留
    __int64 total_amount{0};  // 总仓位
    __int64 closeable_amount{0};  // 可卖出的仓位
    __int64 locked_amount{0};  // 冻结仓位，期货、期权中包含多头冻结+空头冻结
    double value{0.0};  // 标的价值，计算方法是: price * (total_amount + locked_amount) ，仅支持A股
    __int64 redemption_num{0};  // 可申赎数
    int position_prop{0};  // 仓位类型，PositionProp命名空间中定义，股、债、基为多仓，期货分多仓和空仓
    __int64 init_amount{0};  //期初数量，当日不变
    //////以下字段期货专用
    double open_cost{0.0};  // 开仓均价，股票和hold_cost相同，期货=开仓成本/（总持仓*合约乘数）
    __int64 today_amount{0};  // 今天开的仓位
    __int64 old_amount{0};  // 昨持仓
    double occupy_margin{0.0};  // 占用保证金
    // 开仓：交易前已占用保证金+开仓价*开仓数量*合约乘数*保证金比例；
    // 平仓：交易前已占用保证金—开仓价*平今仓数量*合约乘数*保证金比例—昨结算价*平昨仓数量*合约乘数*保证金比例
    double close_pos_profit{0.0};  // 平仓盈亏（盯市），只有平仓的合约才会计入平仓盈亏,老仓采用昨结算价计算
    double close_profit_by_trade{0.0};  // 平仓盈亏（逐笔）,只有平仓的合约才会计入平仓盈亏,老仓采用开仓价计算
    double commission{0.0};  // 手续费
    int contract_multiplier{0};  // 合约乘数
    int invest_type{0};  //投资类型，InvestType命名空间中定义，报单立即返回，期货有效
    int option_hold_type{0};  // 期权持仓类型,OptionHoldType命名空间中定义
    char fund_account[20]{""};  //资金账号
    char stock_account[20]{""};  //股东账号
    char security_exchange[4]{""};  // 市场,SecurityExchangeType命名空间中定义
};

struct FundInfo {
    double available_cash;  //可用资金, 可用来购买证券的资金
    double locked_cash;  //挂单锁住资金，冻结资金
    double total_value;  //总资产, 包括现金, 保证金, 仓位的总价值, 可用来计算收益
    double starting_cash;  //初始资金, 暂时不可用
    double positions_value;  //持仓价值, 股票基金才有持仓价值, 期货为0,暂时不可用
    double settled_cash;  //新增字段，期初资金
    double hold_cash;  //当前资金余额
    double market_value;  //证券市值
    double transferable_cash;  //可取资金余额

    //以下字段为期货专用
    double available_margin;  //可用保证金,期货可用
    double occupied_margin;  //占用保证金，期货可用
    double pre_balance;  //期初权益，期货可用
    double current_balance;  //客户权益，期货可用
    double risk_level;  //占用保证金/客户权益
    double holding_profit;  //盯市盈亏，以持仓成本计算的浮动盈亏
    double close_pos_profit;  //平仓盈亏（盯市），只有平仓的合约才会计入平仓盈亏,老仓采用昨结算价计算
    double close_profit_by_trade;  //平仓盈亏（逐笔）,只有平仓的合约才会计入平仓盈亏,老仓采用开仓价计算
    double commission;  //手续费
    double frozen_commission;  //冻结手续费
    double freezed_deposit;  //委托冻结保证金
    char fund_account[20];  //资金账号
    double hk_available_cash;  //港股通可用资金
};

/// 证券详情
struct SecurityDetial {
    MQUANT_SYMBOL_TYPE symbol;
    char display_name[32];  //中文名,暂不提供
    char spell_name[16];  //拼音简称
    int start_date;  //上市日期, 格式为yyyyMMdd，暂不提供
    int end_date;  //退市日期， 格式为yyyyMMdd, 如果没有退市则为2200-01-01，暂不提供
    char security_type[8];  //股票、基金、金融期货、期货、债券基金、股票基金、QDII 基金、货币基金、混合基金、场外基金，SecurityType命名空间中定义
    char security_sub_type[8];  //子类型，在SecuritySubType命名空间中定义
    int st_flag;  // ST标志，STStatus命名空间中定义
    int trading_phase_code;  //股票状态，TradingPhaseCode命名空间中定义
    int round_lot = 100;  //单手股数，暂不可用
    __int64 local_total_share;  //本市总股本
    __int64 local_listed_share;  //本市流通股本
    double tick_size;  //价格精度，暂不可用
    double high_limit_px;  //涨停价
    double low_limit_px;  //跌停价
    double pre_close_px;  //昨收盘价
    int margin_trading_flag;  //融资融券标识，1表示支持融资融券，0表示不支持融资融券，暂不提供
    // 以下字段期权专用
    double strike_price;  //行权价格，期权有效
    int strike_date;  //行权日期， 期权有效，yyyyMMdd格式
    char put_or_call[4];  //认购("C")/认沽("")
    MQUANT_SYMBOL_TYPE underlying_symbol;  //标的证券代码

    MQUANT_SYMBOL_TYPE convert_symbol;
    double convert_price;
    int convert_start_date;
    int convert_end_date;
};

/// 期货合约信息
struct FutureContractInfo {
    MQUANT_SYMBOL_TYPE symbol;
    double price_step;  // 最小价格变动单位
    int contract_multiplier;  // 合约乘数
    int start_deliv_date;  //开始交割日,yyyyMMdd格式
    int end_deliv_date;  // 结束交割日,yyyyMMdd格式
    double long_margin_ratio;  //多头保证金率（按金额）
    double short_margin_ratio;  // 空头保证金率（按金额）
    double long_margin_ratio_amount;  // 多头保证金率（按手数）
    double short_margin_ratio_amount;  // 空头保证金率（按手数）
    int margin_ratio_flag;  // 1 按金额   2 按手数
    __int64 market_max_buy_qty;  // 市价最大买入数量
    __int64 market_max_sell_qty;  // 市价最大卖出数量
    int market_min_buy_qty;  //市价最小买入数量
    int market_min_sell_qty;  // 市价最小卖出数量
    __int64 max_buy_qty;  // 限价最大买入数量
    __int64 max_sell_qty;  // 限价最大卖出数量
    int min_buy_qty;  //限价最小买入数量
    int min_sell_qty;  // 限价最小卖出数量
};

/// 报单接口
struct OrderRequest {
    MQUANT_SYMBOL_TYPE symbol;  //标的，必填
    int amount;  //数量，必须为正整数，必填
    int side;  //买卖方向，OrderSide命名空间中定义，必填
    double price;  //价格，市价单填1.0，直接还券直接用市价单，必填
    char price_type[4];  //价格类型，PriceType命名空间中定义，必填
    int entrust_type;  //委托类型，EntrustType命名空间中定义，必填
    int action;  //开平方向,OrderAction命名空间中定义，期货、期权必填，其他不用填写
    int invest_type;  //投资类型，期货必填，其他不用填写，InvestType命名空间中定义
    int close_direction;  //平仓类型，期货填写，其他不填，CloseDirection命名空间中定义
    int covered_flag;  //备兑类型，期权填写，其他不填，OptionCoveredFlag命名空间中定义
    int position_type;  //融券卖出、买券还券、直接还券必填，其他不用填写，PositionType命名空间中定义
    char contract_no[32];  //合约编号，信用交易归还合约时选填，不填按默认顺序归还
    int channel_type;  // ETF申赎通道标志，深市跨市ETF申赎有效，1：现金申赎，2：实物申赎，目前仅支持现金申赎，暂时保留该字段
    char securityExchange[4];  // 市场，沪港通、深港通有效，其余无效,SecurityExchangeType命名空间中定义
};
struct OrderRequestEx : OrderRequest {
    char entrust_prop[4];  //委托属性，EntrustProp命名空间中定义，当前支持ETF申赎(填REDEMPTION，申购side填BUY,赎回side填SELL)，其余可不填
    char clOrderID[64];  // 订单ID，无需填写，仅有特殊指定需求时使用，需注意避免重复，建议UUID
};

struct BatchOrderReq {
    OrderRequest* req_list;
    int count;
    int batch_no;
    int last_batch_flag;  //当前委托是否为batch_no对应的最后一笔委托, 0:不是, 1:是，目前保留不用
};

struct BatchOrderReqEx {
    OrderRequestEx* req_list;
    int count;
    int batch_no;
    int last_batch_flag;  //当前委托是否为batch_no对应的最后一笔委托, 0:不是, 1:是，目前保留不用
};

struct OrderQueryReq {
    MQUANT_SYMBOL_TYPE symbol;  //标的代码
    MQUANT_ORDER_ID_TYPE order_id;  //订单编号
    int page_no;  //分页查询，页码,从1开始
    int page_size;  //每页数量，-1表示查询符合条件的全部订单
    bool only_this_inst;  //是否只查询当前实例的订单
    char inst_id[64];  //如果only_this_inst=False，可以查询指定inst_id的订单信息，only_this_inst=True时无效
    bool only_open_orders;  //是否仅查询可撤订单
};

struct ExecutionQueryReq {
    MQUANT_SYMBOL_TYPE symbol;  //标的代码
    MQUANT_ORDER_ID_TYPE order_id;  //订单编号
    int page_no;  //分页查询，页码,从1开始
    int page_size;  //每页数量，-1表示查询符合条件的全部订单
    bool only_this_inst;  //是否只查询当前实例的订单
    char inst_id[64];  //如果only_this_inst=False，可以查询指定inst_id的订单信息，only_this_inst=True时无效
    bool only_rejected_orders;  //是否查询废单成交，true，表示仅查询废单成交，false，表示仅查询非废单成交
};

struct KLineQueryReq {
    MQUANT_SYMBOL_TYPE symbol;
    __int64 start_date;  //开始日期，yyyyMMddhhmmss格式，前向兼容，不足12位尾部会补0
    __int64 end_date;  //结束日期，yyyyMMddhhmmss格式，前向兼容，不足12位尾部会补0
    MQUANT_KLINE_TYPE kline_type;  // k线类型，KLineDataType命名空间中定义
    int exrights_type;  //复权类型,ExRightsType命名空间中定义
    bool include_end_date;  //是否包含结束日期,废弃，值始终为true
};

struct KLineQueryFromInitDateReq {
    MQUANT_SYMBOL_TYPE symbol;
    __int64 init_date;  //开始日期,yyyyMMddhhmmss格式，前向兼容，不足12位尾部会补0
    int days;  //从开始日期向后取的天数，days<0表示向前取
    MQUANT_KLINE_TYPE kline_type;  // k线类型，KLineDataType命名空间中定义
    int exrights_type;  //复权类型,ExRightsType命名空间中定义
    bool include_init_date;  //是否包含起始日期,废弃，值始终为true
    int date_type;  //日期类型,DateType命名空间中定义
    char security_exchange_type[8];  //代码交易市场，主要是区分港股代码，需要用户传入区分是港股市场还是沪深港通,SecurityExchangeType命名空间中定义，如果传入SecurityExchangeType::UNKNOWN，取默认市场中定义
};
/// ETF信息
struct EtfInfo {
    MQUANT_SYMBOL_TYPE etf_fund_symbol;  // etf基金代码
    MQUANT_SYMBOL_TYPE etf_symbol;  // etf申赎代码
    int report_unit;  // 一个申赎单位的基金份额
    double cash_balance;  // 预估现金差额
    double max_cash_ratio;  // 最大现金替代比例
    double pre_cash_componet;  // T-1日申购基准单位现金余额
    double nav_percu;  // T-1日申购基准单位净值
    double nav_pre;  // T-1日基金单位净值
    __int64 allot_max;  // 申购份额上限
    __int64 redeem_max;  // 赎回份额上限
    int etf_type;  // etf类型，在EtfType中定义
    int etf_status;  // etf状态，在EtfStatus中定义
};

/// ETF成分券信息
struct EtfConstituentInfo {
    MQUANT_SYMBOL_TYPE symbol;  // 成分券代码，MQuant格式
    int sample_size;  // 样本数量
    int cash_replace_flag;  // 现金替代标志，
                            // 0禁止替代
                            // 1允许替代
                            // 2必须替代
                            // 3 非沪市退补现金替代
                            // 4 非沪市必须现金替代
                            // 5 非沪深退补现金替代
                            // 6 非沪深必须现金替代
    double deposit_ratio;  // 保证金率（ 溢价比率），允许现金替代标的此字段有效
    double replace_balance;  // 替代金额,必须现金替代标的此字段有效
    double pre_close_px;  // 昨收价格
    double high_limit_px;  // 涨停
    double low_limit_px;  // 跌停价
    bool suspend_flag;  // 停牌标志
};

/// 信用交易合约,包含融资负债和融券负债
struct MarginContract {
    int contract_type;  // 负债类型，MarginContractType命名空间中定义
    int open_date;  // 开仓日期，yyyyMMdd格式
    MQUANT_SYMBOL_TYPE symbol;  // 标的
    int position_type;  // 头寸类型（融券合约专用）,0-普通，1-专项
    double unpaid_amount;  // 未还金额
    __int64 unpaid_qty;  // 未还数量
    double interest;  // 利息
    double cost;  // 费用
    int deadline;  // 归还截止日期,yyyyMMdd格式
    char contract_no[32];  // 合同编号
};

/// 信用资产信息, 后台系统定时从柜台同步，不保证实时性
struct MarginAsset {
    double cash_asset;  // 现金资产
    double security_market_value;  // 证券市值
    double assure_asset;  // 担保资产
    double total_liability;  // 总负债
    double maintain_value;  // 个人维持担保比例
    double available_margin;  // 可用保证金
    double occupy_margin;  // 占用保证金
    double collateral_available_money;  // 买担保品可用资金
    double finance_available_money;  // 买融资标的可用资金
    double security_available_fund;  // 买券还券可用资金
    double cash_available_money;  // 现金还款可用资金
    double finance_quota_capacity;  // 融资额度上限
    double finance_available_quota;  // 融资可用额度
    double finance_occupy_quota;  // 融资占用额度
    double finance_occupy_margin;  // 融资占用保证金
    double finance_compact_quota;  // 融资合约金额
    double finance_compact_commission;  // 融资合约费用
    double finance_compact_interest;  // 融资合约利息
    double finance_market_value;  // 融资市值,暂不提供
    double finance_compact_revenue;  // 融资合约收益
    double security_loan_quota_capacity;  // 融券额度上限
    double security_loan_available_quota;  // 融券可用额度
    double security_loan_occupy_quota;  // 融券占用额度
    double security_loan_occupy_margin;  // 融券占用保证金
    double security_loan_compact_quota;  // 融券合约金额
    double security_loan_compact_commission;  // 融券合约费用
    double security_loan_compact_interest;  // 融券合约利息
    double security_loan_market_value;  // 融券市值，暂不提供
    double security_loan_compact_revenue;  // 融券合约收益
    double transfer_asset;  // 可转出资产
    double compact_total_interest;  // 合约总利息
    double net_asset;  // 净资产
    double withdraw_quota;  // 可取金额
    //    double other_fare = 0                       #其他费用
    double security_loan_sell_balance;  // 融资卖出所得金
};
/// 可融券信息
struct MarginSecurityInfo {
    __int64 loan_available_qty;  //融券可用额度
    double loan_ratio;  // 融券保证金比例
    int position_type;  // 头寸性质,PositionType命名空间中定义
    MQUANT_SYMBOL_TYPE symbol;  // 标的
};

struct MarginAvaliableQtyQueryReq {
    MQUANT_SYMBOL_TYPE symbol;  //标的，必填
    int side;  //买卖方向，OrderSide命名空间中定义，必填
    double price;  //价格，市价单填1.0，直接还券直接用市价单，必填
    char price_type[4];  //价格类型，PriceType命名空间中定义，必填
    int entrust_type;  //委托类型，EntrustType命名空间中定义，必填
    int position_type;  //头寸类型，PositionType命名空间中定义
};

/// ETF预估信息
struct EtfEstimateInfo {
    MQUANT_SYMBOL_TYPE symbol;  // 标的的代码，实时传入,MQuant格式
    MQUANT_DATETIME_TYPE datetime;  // ETF时间，实时传入
    double IOPV;  // 最新价计算的IOPV
    double DIOPVS1;  // 卖一价计算的IOPV
    double DIOPVB1;  // 买一价计算的IOPV
    double premium;  // 溢价套利预估利润
    double discount;  // 折价套利预估利润
};

struct IndexComponent {
    MQUANT_SYMBOL_TYPE symbol;  //成分券代码
    double weight;  //权重，所有成分券权重相加为100,暂不提供
    int includeDate;  //进入日期
    int excludeDate;  //剔除日期
    int status;  //状态                //样本数量
};

namespace RequestType {
    const HtString newOrder = "0";
    const HtString cancelOrder = "2";
}  // namespace RequestType

struct MQuantRsp {
    int err_code;
    HtString err_info;
};

struct MarginAssetRsp : MQuantRsp {
    MarginAsset margin_asset;
};

struct FundFlowDetial {
    double in_flow_value;  // 流入金额
    __int64 in_flow_qty;  // 流入股数
    double out_flow_value;  // 流出金额
    __int64 out_flow_qty;  // 流出股数
};

struct FundFlow {
    MQUANT_SYMBOL_TYPE symbol;
    MQUANT_DATETIME_TYPE datetime;
    FundFlowDetial super_large_order;
    FundFlowDetial large_order;
    FundFlowDetial middle_order;
    FundFlowDetial small_order;
    FundFlowDetial main_order;
};

struct OrderReply {
    char req_type[4];  // 类型,RequestType中定义
    char error_code[8];  // 错误码
    char fund_account[20];  // 资金账号
    MQUANT_ORDER_ID_TYPE order_id;  // 订单编号
    MQUANT_ORDER_ID_TYPE origin_order_id;  // 原订单编号
    int batch_no;  // 批次号
    char algo_inst_id[64];  // 算法实例ID注
    char inst_id[64];  // 实例ID
    char error_info[256];  // 错误信息
};

struct TickEx : Tick {
    //当前版本，全部拉平至Tick中，此结构用于兼容
    char reserved[256];  //保留字段，用于后续扩展
};

/// 可转债信息
struct SecurityDetialEx : SecurityDetial {
    //当前版本，全部拉平至SecurityDetial中，此结构用于兼容
    char reserved[256];  //保留字段，留待后续扩展
};

struct RecordOrder591 : RecordOrder {
    //当前版本，全部拉平至RecordOrder中，此结构用于兼容
};

struct RecordTransaction591 : RecordTransaction {
    //当前版本，全部拉平至RecordTransaction中，此结构用于兼容
};
}  // namespace MQuant

#endif  // MQUANTSTRUCT_H
