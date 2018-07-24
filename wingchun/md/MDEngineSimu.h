/*****************************************************************************
/**
 * MDEngineSimu: market data simu engine adapter.
 * @Author xuxiaodong@ZRDFS
 * @2018-07-21
 */

#ifndef WINGCHUN_MDENGINESIMU_H
#define WINGCHUN_MDENGINESIMU_H

#include "IMDEngine.h"
#include "longfist/LFConstants.h"

WC_NAMESPACE_START

class MDEngineSimu: public IMDEngine
{
public:
    /** load internal information from config json */
    virtual void load(const json& j_config);
    virtual void connect(long timeout_nsec);
    virtual void login(long timeout_nsec);
    virtual void logout();
    virtual void release_api();
    virtual void subscribeMarketData(const vector<string>& instruments, const vector<string>& markets);
    virtual bool is_connected() const { return connected; };
    virtual bool is_logged_in() const { return logged_in; };
    virtual string name() const { return "MDEngineSimu"; };

public:
    MDEngineSimu();

private:
    /** internal information */
    string backtest_date;
    string tickdata_path;
    // internal flags
    bool connected;
    bool logged_in;
    int  reqId;
};

DECLARE_PTR(MDEngineSimu);

WC_NAMESPACE_END

#endif 