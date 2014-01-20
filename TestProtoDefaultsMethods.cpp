#include <tuple>
#include "gtest/gtest.h"
#include "Range.h"
#include "PcapDiskUsage.h"
#include "Conf.h"
#include "include/global.h"
#include "ConfSlave.h"
#include "Conf.h"
#include "ProtoDefaults.h"

using namespace protoDefines;

TEST(TestProtoDefaults, GetConfDefaultsOK) {
   ProtoDefaults getDefaults{{}};
   auto defaults1 = getDefaults.GetConfDefaults("BASE");
   EXPECT_TRUE(defaults1.size() > 0);
   ASSERT_TRUE(defaults1.find("dpiThreads") != defaults1.end());
   
   auto element = defaults1.find("dpiThreads")->second;
   EXPECT_TRUE((*std::get<indexRange>(element)).Validate("4")); //4 DPI Threads

   auto defaults2 = getDefaults.GetConfDefaults("TEST");
   EXPECT_TRUE(defaults2.size() == 0);
}

TEST(TestProtoDefaults, GetRangeOK){
   auto conf = networkMonitor::ConfSlave::Instance().GetConf();
   auto pcapLocations = conf.GetPcapCaptureLocations();
   ProtoDefaults getDefaults{pcapLocations};
   
   auto confDefaults = getDefaults.GetConfDefaults("BASE");
   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "pcapRecordsToClearPerCycle")).Validate("test1")); //Int
   EXPECT_TRUE((*getDefaults.GetRange(confDefaults, "pcapRecordsToClearPerCycle")).Validate("1")); //Int
   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "pcapRecordsToClearPerCycle")).Validate("0")); //Int
   EXPECT_TRUE(getDefaults.GetRange(confDefaults, "pcapRecordsToClearPerCycle").use_count() > 0);

   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "qosmosTCPReAssemblyEnabled")).Validate("100")); //Bool
   EXPECT_TRUE((*getDefaults.GetRange(confDefaults, "qosmosTCPReAssemblyEnabled")).Validate("true")); //Bool
   EXPECT_TRUE((*getDefaults.GetRange(confDefaults, "qosmosTCPReAssemblyEnabled")).Validate("FALSE")); //Bool
   
   
   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "dpiThreads")).Validate("13")); //Int
   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "dpiThreads")).Validate("FALSE")); //Int
   
   EXPECT_TRUE((*getDefaults.GetRange(confDefaults, "pcapInterface")).Validate("test")); //string
   EXPECT_TRUE(!(*getDefaults.GetRange(confDefaults, "pcapInterface")).Validate("")); //string
   EXPECT_TRUE((*getDefaults.GetRange(confDefaults, "pcapInterface")).Validate("eth0")); //string
   
   //Shouldn't exist:
   EXPECT_TRUE(getDefaults.GetRange(confDefaults, "pcapInterface2").use_count() == 0);
}


TEST(TestProtoDefaults,  TriggerDefaultOnPcapCaptureSizeLimit){
   ProtoDefaults getDefaults{{"xyz"}}; // nonsense value that DiskUsage cannot use
  
   auto confDefaults = getDefaults.GetConfDefaults("BASE");
   auto rangePtr = getDefaults.GetRange(confDefaults, "captureSizeLimit"); // int
   auto maxFromDefaults = rangePtr->StringifyMax();
   EXPECT_EQ(80000, std::stoul(maxFromDefaults));
}

TEST(TestProtoDefaults, GetConfParamDefaultOK){
   auto conf = networkMonitor::ConfSlave::Instance().GetConf();
   auto pcapLocations = conf.GetPcapCaptureLocations();
   ProtoDefaults getDefaults{pcapLocations};
   
   auto confDefaults = getDefaults.GetConfDefaults("BASE");
   
   const std::string str("4");
   EXPECT_TRUE(getDefaults.GetConfParamDefault(confDefaults, "dpiThreads").size() > 0);
   EXPECT_TRUE(str.compare(getDefaults.GetConfParamDefault(confDefaults, "dpiThreads")) == 0);
   EXPECT_TRUE(getDefaults.GetConfParamDefault(confDefaults, "dpiThreads2").size() == 0);
}

TEST(TestProtoDefaults, GetConfParamOK){
   auto conf = networkMonitor::ConfSlave::Instance().GetConf();
   auto pcapLocations = conf.GetPcapCaptureLocations();
   ProtoDefaults getDefaults{pcapLocations};
   
   auto confDefaults = getDefaults.GetConfDefaults("BASE");
      
   EXPECT_TRUE((std::get<indexDefault>(getDefaults.GetConfParam(confDefaults, "dpiThreads2"))).size() == 0);
   EXPECT_TRUE(std::get<indexRange>(getDefaults.GetConfParam(confDefaults, "dpiThreads2")) == nullptr);
   EXPECT_TRUE((std::get<indexType>(getDefaults.GetConfParam(confDefaults, "dpiThreads2"))).size() == 0);
   
   std::string str("dpiThreads");
   EXPECT_TRUE((std::get<indexDefault>(getDefaults.GetConfParam(confDefaults, str))).size() > 0);
   EXPECT_TRUE(std::get<indexRange>(getDefaults.GetConfParam(confDefaults, str)) != nullptr);
   EXPECT_TRUE((std::get<indexType>(getDefaults.GetConfParam(confDefaults, str))).size() > 0);

   EXPECT_TRUE((std::get<indexDefault>(getDefaults.GetConfParam(confDefaults, str))).compare("4") == 0);
   EXPECT_TRUE((*std::get<indexRange>(getDefaults.GetConfParam(confDefaults, str))).Validate("4")); //Int
   EXPECT_FALSE((*std::get<indexRange>(getDefaults.GetConfParam(confDefaults, str))).Validate("f")); //Int
}
