#ifndef MQUANTULTIS_H
#define MQUANTULTIS_H
#include "mquantstruct.h"

namespace MQuant {
class IMQuantSysApi;
class IEtfHandler;
class IMarginHandler;
class IAlgoTradeHandler;
class IOptionTradeHandler;
class StrategyParamsImpl;
class SymbolListImpl;
class HtLogImpl;
class MarginAssureSecurityListImpl;
class PositionListImpl;
class OrderListImpl;
class ExecutionListImpl;
class KLineDataListImpl;
class EtfConstituentListImpl;
class MarginContractList;
class MarginAccountSecurityListImpl;
class MarginContractListImpl;
class IndexComponentListImpl;
class TickDataListImpl;
enum LOG_LEVEL
{
    LOG_DEBUG = 0,     //调试类
    LOG_INFO,  //信息类
    LOG_WARNING,   //告警类
    LOG_ERROR,     //错误类
    LOG_MAX_LEVEL,
};



/// 运行参数，嵌套的参数，如表格类型参数，请使用getStrValue获取到参数值，然后重新声明一个StrategyParams对象，调用setParams将返回值设置进去，然后再解析。多层嵌套每层都需要这样操作

class MQUANT_EXPORT StrategyParams
{
public:

    enum ParamType
    {
        TYPE_INT = 0,       //int
        TYPE_FLOAT,         //float
        TYPE_BOOL,          //bool
        TYPE_STRING,        //string
        TYPE_LIST,          //列表
        TYPE_TABLE,         //表格
    };

    StrategyParams();
    StrategyParams(StrategyParams &obj);
    StrategyParams(StrategyParams &&obj);
    StrategyParams &operator=(StrategyParams &obj);
    ~StrategyParams();

    int getIntValue(const char *key);
    double getFloatValue(const char *key);
    HtString getStrValue(const char *key);      //table类型的参数也以string格式返回，返回值可以直接再解析为json obj，格式为{column:[column1,clounm2,clounm3,clounm4], data:[[1,2,3,4],[4,5,6,7]], file:file_path}
    bool getBoolValue(const char *key);

    void addIntParam(const char *key, int value, const char *desc = 0);
    void addFloatParam(const char *key, double value, const char *desc = 0);
    void addBoolParam(const char *key, bool value, const char *desc = 0);
    void addStrParam(const char *key, const char* value, const char *desc = 0);
    void addListParam(const char *key, const char* value, int defaultIndex = 0,const char *desc = 0);     //下拉列表参数，value以英文逗号分隔，如"最新价,对手方一档",调用startStrategy启动策略传入的参数会直接取默认值
    void addTableParam(const char *key, const char *filePath, const char *desc = 0); //表格类型参数，仅支持传入表格文件名，界面会展示表格中的内容。
                                                                                        //用户在界面修改后启动策略时会以{column:[column1,clounm2,clounm3,clounm4], data:[[1,2,3,4],[4,5,6,7]], file:file_path}的格式传入
                                                                                        //请使用getStrValue获取值，然后用json解析
                                                                                        //startStrategy不支持传入table类型的参数

    //以下接口内部使用
    void preHandle() const;
    //设置参数，json object格式的字符串
    void setParams(const HtString &jsonParams);
    HtString getStrategyParams() const;
private:
    StrategyParamsImpl *__impl;

};
///////////////////////////////// 注意，以下类非线程安全，请不要多线程操作同一个类对象////////////////////////////////////
class MQUANT_EXPORT SymbolList
{
public:
    SymbolList();
    ~SymbolList();
    SymbolList(SymbolList &obj);
    SymbolList(SymbolList &&obj);
    SymbolList& operator=(SymbolList &obj);
    void put(const char *symbol);
    const HtString get(int index) const;
    int getSize() const ;
    SymbolList &operator+=(const SymbolList &obj);
    SymbolList operator+(const SymbolList &obj);
    int index(const char *symbol);
private:
    SymbolListImpl *__impl;
};

class MQUANT_EXPORT HtLog
{
public:
    virtual ~HtLog();
    HtLog(const char *file, int line, const char *function);
    HtLog &operator<<(char t);
    HtLog &operator<<(bool t);
    HtLog &operator<<(signed short t);
    HtLog &operator<<(unsigned short t);

    HtLog &operator<<(signed int t);
    HtLog &operator<<(unsigned int t) ;
    HtLog &operator<<(signed long t);
    HtLog &operator<<(unsigned long t);
    HtLog &operator<<(__int64 t);

    HtLog &operator<<(float t) ;
    HtLog &operator<<(double t);
    HtLog &operator<<(const char* t);
//    HtLog &operator<<(const void * t);
    HtLog &operator<<(HtString & t);
    HtLog &operator<<(const Tick & t);
    HtLog &operator<<(const RecordOrder & t);
    HtLog &operator<<(const RecordTransaction & t);
    HtLog &operator<<(const Order & t);
    HtLog &operator<<(const Execution & t);
    HtLog &operator<<(const KLineData & t);
    HtLog &operator<<(const KLineDataPush & t);
    HtLog &operator<<(const EtfEstimateInfo & t);
    HtLog &operator<<(StrategyParams & t);
    HtLog &operator <<(const Position &t);
    HtLog &operator <<(const FundInfo &t);
    HtLog &operator <<(const SecurityDetial &t);
    HtLog &operator <<(const SymbolList &t);
    HtLog &operator <<(const MarginAsset &t);
    HtLog &operator <<(const MarginContract &t);
    HtLog &operator <<(const MarginSecurityInfo &t);
    HtLog &operator <<(const EtfConstituentInfo &t);
    HtLog &operator <<(const EtfInfo &t);


//    HtLog &operator<<(int t[], int size);
//    HtLog &operator<<(double t[], int size);

//    HtLog &HtLog::operator<<(int t[], int size)
//    {
//        if(size <= 0)
//            return *this;
//        this->__stream << "[" << t[0];
//        for(int i = 1; i < size; i++)
//        {
//            this->__stream << "," << t[i];
//        }
//        this->__stream << "]";
//        return *this;
//    }

//    HtLog &HtLog::operator<<(double t[], int size)
//    {
//        if(size <= 0)
//            return *this;
//        this->__stream << "[" << t[0];
//        for(int i = 1; i < size; i++)
//        {
//            this->__stream << "," << t[i];
//        }
//        this->__stream << "]";
//        return *this;
//    }
    HtLog &debug();
    HtLog &info();
    HtLog &warn();
    HtLog &error();
    HtLog &log(LOG_LEVEL level, int flag=0);
    HtLog &userLog(const char *label, bool sync = false);

    //清空用户日志
    static void cleanUserLog(const char *label);
    //注册自定义日志文件
    static void registerCustomLogFile(const char *filePath, const char *label);
    //设置日志级别，仅对系统日志有效，对用户自定义日志无效，低于设置级别的系统日志不会记录在文件中，也不会在任何界面显示,默认为DEBUG
    static void setLogLevel(LOG_LEVEL level);
    //设置控制台日志级别，定于设定级别的日志不会输出到控制台,默认为DEBUG
    static void setConsoleLogLevel(LOG_LEVEL level);
    //设置显示日志级别，即显示在matic界面上的日志级别，低于该级别的日志不会显示在matic界面上，默认为DEBUG
    static void setDispLogLevel(LOG_LEVEL level);
private:
    HtLogImpl *__impl;

public:
    HtLog &operator<<(const RecordOrder591 & t);
    HtLog &operator<<(const RecordTransaction591 & t);
};


class MQUANT_EXPORT MarginAssureSecurityList
{
public:
    MarginAssureSecurityList();
    MarginAssureSecurityList(MarginAssureSecurityList &obj);
    MarginAssureSecurityList(MarginAssureSecurityList &&obj);
    MarginAssureSecurityList &operator=(MarginAssureSecurityList &obj);
    ~MarginAssureSecurityList();
    void push(const char *symbol, double discountRatio);
    double discountRatio(const char *symbol);
    double discountRatio(int index);
    HtString getSymbol(int index);
    int getSize() const;
private:
    MarginAssureSecurityListImpl *__impl;
};
/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT PositionList
{
public:
    PositionList();
    ~PositionList();
    PositionList(PositionList &obj);
    PositionList(PositionList &&obj);
    PositionList &operator=(PositionList &obj);
    void push(Position *pos);
    const Position& get(int index);
    int getSize() const ;
private:
    PositionListImpl *__impl;

};

/////////下面的函数只在接口返回时使用，非线程安全///////////////
/// 返回值接收请务必使用 OrderList ordList = context->getSysApi()->getOrders(AccountType::STOCK, orderQryReq);这种方式/////////////
/// 不要先声明接收方，然后给接收方赋值的方式//////////////
class MQUANT_EXPORT OrderList
{
public:
    OrderList();
    ~OrderList();
    OrderList(OrderList &obj);
    OrderList(OrderList &&obj);
    OrderList& operator=(OrderList &obj);
    void push(Order *order);
    const Order& get(int index);
    int getSize() const;
    bool isLast() const;
    int totalCount() const;

    void setLast(bool last);
    void setTotalCount(int totalCount);
private:
    OrderListImpl *__impl;
};
/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT ExecutionList
{
public:
    ExecutionList();
    ~ExecutionList();
    ExecutionList(ExecutionList &obj);
    ExecutionList(ExecutionList &&obj);
    ExecutionList& operator=(ExecutionList &obj);
    void push(Execution *execution);
    const Execution& get(int index);
    int getSize() const;
    bool isLast() const;
    int totalCount() const;

    void setLast(bool last);
    void setTotalCount(int totalCount);
private:
    ExecutionListImpl *__impl;
};
/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT KLineDataList
{
public:
    KLineDataList();
    KLineDataList(KLineDataList &obj);
    KLineDataList(KLineDataList &&obj);
    KLineDataList& operator=(KLineDataList &obj);
    ~KLineDataList();
    void push(KLineData *kline);
    const KLineData& get(int index);
    int getSize() const;
private:
    KLineDataListImpl *__impl;

};

class MQUANT_EXPORT TickDataList
{
public:
    TickDataList();
    TickDataList(TickDataList &obj);
    TickDataList(TickDataList &&obj);
    TickDataList& operator=(TickDataList &obj);
    ~TickDataList();
    void push(Tick *tick);
    const Tick& get(int index);
    int getSize() const;
private:
    TickDataListImpl *__impl;
};

/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT EtfConstituentList
{
public:
    EtfConstituentList();
    ~EtfConstituentList();
    EtfConstituentList(EtfConstituentList &obj);
    EtfConstituentList(EtfConstituentList &&obj);
    EtfConstituentList& operator=(EtfConstituentList &obj);
    void push(EtfConstituentInfo *info);
    const EtfConstituentInfo& get(int index);
    int getSize() const;
private:
    EtfConstituentListImpl *__impl;

};
/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT MarginContractList
{
public:
    MarginContractList();
    ~MarginContractList();
    MarginContractList(MarginContractList &obj);
    MarginContractList(MarginContractList &&obj);
    MarginContractList& operator=(MarginContractList &obj);
    void push(MarginContract *contract);
    const MarginContract& get(int index);
    int getSize() const;
private:
    MarginContractListImpl *__impl;

};


/////////下面的函数只在接口返回时使用，非线程安全//////////////
class MQUANT_EXPORT MarginAccountSecurityList
{
public:
    MarginAccountSecurityList();
    ~MarginAccountSecurityList();
    MarginAccountSecurityList(MarginAccountSecurityList &obj);
    MarginAccountSecurityList(MarginAccountSecurityList &&obj);
    MarginAccountSecurityList& operator=(MarginAccountSecurityList &obj);
    void push(const char *symbol, MarginSecurityInfo* info);
    const MarginSecurityInfo& marginSecurityInfo(const char *symbol);
    const MarginSecurityInfo& marginSecurityInfo(int index);
    HtString getSymbol(int index);
    int getSize() const;
private:
    MarginAccountSecurityListImpl *__impl;
};

class MQUANT_EXPORT IndexComponentList
{
public:
    IndexComponentList();
    ~IndexComponentList();
    IndexComponentList(IndexComponentList &obj);
    IndexComponentList(IndexComponentList &&obj);
    IndexComponentList& operator=(IndexComponentList &obj);
    void push(IndexComponent* info);
    const IndexComponent& get(int index);
    int getSize() const;
private:
    IndexComponentListImpl *__impl;
};
}
#endif // MQUANTULTIS_H
