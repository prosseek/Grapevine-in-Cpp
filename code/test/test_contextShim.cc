#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <string>

#include "contextHandler.h"
#include "contextShim.h"

class QuickTest : public testing::Test {
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

class ContextShimTest : public QuickTest {
 protected:
  virtual void SetUp() {
      s = new ContextShim();
      h = s->getContextHandlerPtr();
      QuickTest::SetUp();
  }
  
  virtual void TearDown() {
      delete s;
      QuickTest::TearDown();
  }
  ContextShim* s;
  ContextHandler* h;
};


TEST_F(ContextShimTest, getContextBytes) {
    std::map<std::string, int> dbx {
          {"GroupsEnumerated",3},
          {"Group0",100},{"Group1",103},{"Group2",104},
          {"IdsAggregated",3},
          {"Id0",10}, {"Id1",20}, {"Id2",30}};
    ContextSummary* s = new ContextSummary(11, dbx, 10, 100000);   // pretty short hops
    h->moveMyContext(s);
    
    // 1. setup the existing summary
    std::map<std::string, int> dba1 {
          {"GroupsEnumerated",3},
          {"Group0",100},{"Group1",103},{"Group2",104},
          {"IdsAggregated",3},
          {"Id0",10}, {"Id1",20}, {"Id2",30}};
    ContextSummary* summaryA = new ContextSummary(1, dba1, 10, 100000);   // pretty short hops
    
    std::map<std::string, int> dba2 {
          {"GroupsEnumerated",3},
          {"Group0",100},{"Group1",103},{"Group2",104},
          {"IdsAggregated",3},
          {"Id0",10}, {"Id1",20}, {"Id2",30}};
    ContextSummary* summaryB = new ContextSummary(2, dba2, 10, 100000);   // pretty long hops
    
    std::map<int, unique_ptr<ContextSummary>> summaryMap;
    summaryMap[1] = unique_ptr<ContextSummary>(summaryA);
    summaryMap[2] = unique_ptr<ContextSummary>(summaryB);
    h->moveReceivedSummaries(summaryMap);
        
    // make group 100
    // h->addGroupDefinition(100);
}

TEST_F(ContextShimTest, processContextBytes) {

}
