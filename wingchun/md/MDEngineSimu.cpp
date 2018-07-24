/*****************************************************************************
/**
 * MDEngineSimu: market data simu engine adapter.
 * @Author xuxiaodong@ZRDFS
 * @2018-07-21
 */

#include "MDEngineSimu.h"
#include "TypeConvert.hpp"
#include "Timer.h"
#include "longfist/LFUtils.h"

USING_WC_NAMESPACE

MDEngineSimu::MDEngineSimu(): IMDEngine(SOURCE_SIMU), connected(false), logged_in(false), reqId(0)
{
    logger = yijinjing::KfLog::getLogger("MdEngine.Simu");
}

void MDEngineSimu::load(const json& j_config)
{
    backtest_date = j_config[WC_CONFIG_KEY_BACKTEST_DATE].get<string>();
    tickdata_path = j_config[WC_CONFIG_KEY_TICKDATA_PATH].get<string>();
    KF_LOG_INFO(logger, "[MDEngineSimu]" << " (load)");
}

void MDEngineSimu::connect(long timeout_nsec)
{
    connected = true;
    KF_LOG_INFO(logger, "[MDEngineSimu]" << " (connect)");
}

void MDEngineSimu::login(long timeout_nsec)
{
    logged_in = true;
    KF_LOG_INFO(logger, "[MDEngineSimu]" << " (login)");
}

void MDEngineSimu::logout()
{
    connected = false;
    logged_in = false;
}

void MDEngineSimu::release_api()
{
}

void MDEngineSimu::subscribeMarketData(const vector<string>& instruments, const vector<string>& markets)
{
    int nCount = instruments.size();
    char* insts[nCount];
    for (int i = 0; i < nCount; i++)
        insts[i] = (char*)instruments[i].c_str();
    //api->SubscribeMarketData(insts, nCount);
}

BOOST_PYTHON_MODULE(libsimumd)
{
    using namespace boost::python;
    class_<MDEngineSimu, boost::shared_ptr<MDEngineSimu> >("Engine")
    .def(init<>())
    .def("init", &MDEngineSimu::initialize)
    .def("start", &MDEngineSimu::start)
    .def("stop", &MDEngineSimu::stop)
    .def("wait_for_stop", &MDEngineSimu::wait_for_stop);
}
