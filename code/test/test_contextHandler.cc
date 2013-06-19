#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <string>
#include "contextHandler.h"

using namespace std;

class QuickTest : public testing::Test {
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
  ContextHandler* h;
};

class ContextHandlerTest : public QuickTest {
 protected:
     virtual void SetUp() {
         QuickTest::SetUp();
         summary = new ContextSummary(1, db);
         summary2 = new ContextSummary(2, db2);
         h = ContextHandler::getInstance();
         // summary3 = new ContextSummary(3, &db3);
         // groupSummary = new GroupContextSummary(101, &groupDb);
         // groupSummaryNull = new GroupContextSummary(100, &groupDbNull);
     }

     virtual void TearDown() {
         QuickTest::TearDown();
         delete summary;
         delete summary2;
         // delete summary3;
         // delete groupSummary;
         // delete groupSummaryNull;
         result.clear();
     }

     std::map<std::string, int> db {
           {"GroupsEnumerated",3},
           {"Group0",101},{"Group1",102},{"Group2",103},
           {"IdsAggregated",5},
           {"Id0",10}, {"Id1",20}, {"Id2",30}, {"Id3",40}, {"Id4",50}};
     std::vector<int> aggregatedIds {10,20,30,40,50};
     
     std::map<std::string, int> db2 {
           {"GroupsEnumerated",3},
           {"Group0",101},{"Group1",103},{"Group2",104},
           {"IdsAggregated",3},
           {"Id0",10}, {"Id1",20}, {"Id2",30}};
     std::vector<int> aggregatedIds2 {10,20,30};
     ContextSummary* summary;
     ContextSummary* summary2;
     vector<int> result;
};

TEST_F(ContextHandlerTest, setMyContext) {
    h->setMyContext(*summary);
    EXPECT_TRUE(summary == h->getMyContext());
}

TEST_F(ContextHandlerTest, setReceivedSummariesMap) {

    map<int, unique_ptr<ContextSummary>> summaries;
	auto s = unique_ptr<ContextSummary>(new ContextSummary(*summary));
        
    summaries[1] = move(s);
    
	h->setReceivedSummaries(summaries);
    auto res = h->getReceivedSummaries();
    EXPECT_TRUE(*summary == *res[1].get());
}

TEST_F(ContextHandlerTest, getInstance) {
    ContextHandler* c1 = ContextHandler::getInstance();
    ContextHandler* c2 = ContextHandler::getInstance();
    EXPECT_EQ(c1, c2);
}

TEST_F(ContextHandlerTest, setupGroupDefinition) {
    auto g = new GroupDefinition(100);
    h->setupGroupDefinition(*g); // setup parameter is always reference
    auto d = h->getGroupDefinition(100);
    EXPECT_TRUE(d != NULL);
    auto e = h->getGroupContextSummary(100);
    EXPECT_TRUE(d != NULL);
    
    d = h->getGroupDefinition(101);
    EXPECT_TRUE(d == NULL);
    e = h->getGroupContextSummary(101);
    EXPECT_TRUE(d == NULL);
}

TEST_F(ContextHandlerTest, setupGroupDefinitionFromId) {
    h->setupGroupDefinition(100);
    
    auto d = h->getGroupDefinition(100);
    EXPECT_TRUE(d != NULL);
    auto e = h->getGroupContextSummary(100);
    EXPECT_TRUE(d != NULL);
    
    d = h->getGroupDefinition(101);
    EXPECT_TRUE(d == NULL);
}

TEST_F(ContextHandlerTest, performGroupFormations) {
    // # set group definistions in ContextHandler
    auto g = new GroupDefinition(100); 
    // auto groupDefinitions = {100:g}
    
    h->setupGroupDefinition(*g);
    
    // # You *should* get group summary from the getGroupContext method
    auto groupSummary = h->getGroupContextSummary(100);
    // std::cout << groupSummary->getId() << std::endl;
    // std::cout << groupSummary->to_string() << std::endl;
    
    // # myContext is from the summary with id 1
    auto myContext = summary;
    // std::cout << myContext->to_string() << std::endl;
    // # handle it to make groupSummary to contain [1]
    g->handleContextSummary(*groupSummary, *myContext);
    // 
    // # before -> there should be only [1]
    // members = getGroupMembers(groupSummary)
    // self.assertTrue(sorted([1]) == sorted(members))
    // 
    // # after -> we have summaries with id 22
    // summaries = [self.summary2]
    // self.c.performGroupFormations(groupDefinitions, summaries)
    // groupSummary = self.c.getGroupContext(100)
    // members = getGroupMembers(groupSummary)
    // 
    // self.assertTrue(sorted([1,22]) == sorted(members))
}

