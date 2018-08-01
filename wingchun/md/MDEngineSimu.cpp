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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include  <fstream>

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
    readTickQuotes();
}

void MDEngineSimu::readTickQuotes()
{
    string tickdata_file = tickdata_path + backtest_date;
    KF_LOG_INFO(logger, "[MDEngineSimu]" << " (readTickQuotes)" << tickdata_file);
    std::ifstream tick_file(tickdata_file);
    string line;
    while(std::getline(tick_file,line))
    {
        //l1810,9250.000000,9135.000000,2,9410.000000,1,0,12.000000,0.000000,90015297,90015138
        //symbol,lastprice bidprice1 bidvolume1 askprice1 askvolume1 volume openinterest time
        KF_LOG_INFO(logger, "[MDEngineSimu]" << " (readTickQuotes)" << line);
        struct LFMarketDataField data = {};

        vector <string> fields;
        boost::split( fields, line, boost::is_any_of( "," ) );
        memcpy(data.InstrumentID,fields[0].c_str(), 31);
        data.LastPrice = boost::lexical_cast<double>(fields[1]);
        data.BidPrice1 = boost::lexical_cast<double>(fields[2]);
        data.BidVolume1 = (int)boost::lexical_cast<double>(fields[3]);
        data.AskPrice1 = boost::lexical_cast<double>(fields[4]);
        data.AskVolume1 = (int)boost::lexical_cast<double>(fields[5]);
        data.Volume = (int)boost::lexical_cast<double>(fields[6]);
        data.OpenInterest = boost::lexical_cast<double>(fields[7]);
        int nLen = fields[8].length();
        memcpy(data.UpdateTime, (const void*)fields[8].substr(0,nLen-3).c_str(), 9);
        data.UpdateMillisec = boost::lexical_cast<int>(fields[8].substr(nLen-3,3));
        on_market_data(&data); 
    }  
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
