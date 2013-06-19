#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "groupUtils.h"
#include "contextSummary.h"
#include "contextHandler.h"

using namespace std;

//http://stackoverflow.com/questions/427589/inspecting-standard-container-stdmap-contents-with-gdb
//#define SHOW(X) cout << # X " = " << ((X).get()->to_string()) << endl

void testPrint( map<int, unique_ptr<GroupContextSummary>> & m, int i )
{
    cout <<  m[i].get()->to_string() << endl;
  // SHOW( m[i] );
  // SHOW( m.find(i)->first );
}

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
         ContextHandler::resetContextHandler();
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

TEST_F(ContextHandlerTest, moveMyContext) {
    // The summary is automatically destroyed, so it should be
    // 1. pointer (otherwise, it will cause an error as the allocated value will be already gone)
    // 2. forget about the summary, it's not mine anymore
    std::map<std::string, int> db {
           {"GroupsEnumerated",3},
           {"Group0",101},{"Group1",102},{"Group2",103},
           {"IdsAggregated",5},
           {"Id0",10}, {"Id1",20}, {"Id2",30}, {"Id3",40}, {"Id4",50}};
    ContextSummary* summary = new ContextSummary(1, db);
    
    std::map<std::string, int> db2 {
           {"GroupsEnumerated",3},
           {"Group0",101},{"Group1",102},{"Group2",103},
           {"IdsAggregated",5},
           {"Id0",10}, {"Id1",20}, {"Id2",30}, {"Id3",40}, {"Id4",50}};
    ContextSummary* summary2 = new ContextSummary(2, db2);
    
    h->moveMyContext(*summary);
    EXPECT_TRUE(summary == h->getMyContext());
    
    h->moveMyContext(*summary2);
    EXPECT_TRUE(summary2 == h->getMyContext());
    
    // delete summary; // <-- This will raise an error 
} 

TEST_F(ContextHandlerTest, moveReceivedSummaries) {

    map<int, unique_ptr<ContextSummary>> summaries;
	auto s = unique_ptr<ContextSummary>(new ContextSummary(*summary));
        
    summaries[1] = move(s);
    
	h->moveReceivedSummaries(summaries);
    auto res = h->moveReceivedSummaries();
    EXPECT_TRUE(*summary == *res[1].get());
}

TEST_F(ContextHandlerTest, getInstance) {
    ContextHandler* c1 = ContextHandler::getInstance();
    ContextHandler* c2 = ContextHandler::getInstance();
    EXPECT_EQ(c1, c2);
}

TEST_F(ContextHandlerTest, setupGroupDefinitionByMoving) {
    auto g = new GroupDefinition(100);
    auto g2 = h->setupGroupDefinitionByMoving(*g); // setup parameter is always reference
    
    EXPECT_TRUE(g == g2);
    
    auto d = h->getGroupDefinition(100);
    EXPECT_TRUE(d != NULL);
    auto e = h->getGroupContextSummary(100);
    EXPECT_TRUE(d != NULL);
    
    d = h->getGroupDefinition(101);
    EXPECT_TRUE(d == NULL);
    e = h->getGroupContextSummary(101);
    EXPECT_TRUE(d == NULL);
    
    // You should not free the g, as it's owned by the handler.
    // delete g;
    // --> gv(7163) malloc: *** error for object 0x10d509830: pointer being freed was not allocated
}

TEST_F(ContextHandlerTest, setupGroupDefinition) {
    auto groupPtr = h->setupGroupDefinition(100);
    
    auto d = h->getGroupDefinition(100);
    EXPECT_TRUE(d == groupPtr);
    auto e = h->getGroupContextSummary(100);
    EXPECT_TRUE(d != NULL);
    
    d = h->getGroupDefinition(101);
    EXPECT_TRUE(d == NULL);
}

TEST_F(ContextHandlerTest, addGroupDefinitionByCopying) {

    std::map<std::string, int> db {
           {"GroupsEnumerated",3},
           {"Group0",101},{"Group1",102},{"Group2",103},
           {"IdsAggregated",5},
           {"Id0",10}, {"Id1",20}, {"Id2",30}, {"Id3",40}, {"Id4",50}};
    ContextSummary* summary = new ContextSummary(1, db);
    
    std::map<std::string, int> db2 {
          {"GroupsEnumerated",3},
          {"Group0",101},{"Group1",103},{"Group2",104},
          {"IdsAggregated",3},
          {"Id0",10}, {"Id1",20}, {"Id2",30}};
    ContextSummary* summary2 = new ContextSummary(2, db2);
    
    h->copyMyContext(*summary); 
    EXPECT_TRUE(summary != h->getMyContext());
    delete summary;
    
    // 1. set myContext as summary(id 1)
    summary = new ContextSummary(1, db);
    h->moveMyContext(*summary);
    EXPECT_TRUE(summary == h->getMyContext());
    
    // 2. received summaries 
    map<int, unique_ptr<ContextSummary>> receivedSummaries;
    auto copiedSummary = new ContextSummary(*summary2);
    receivedSummaries[2] = unique_ptr<ContextSummary>(copiedSummary);
    h->moveReceivedSummaries(receivedSummaries); // {22:self.summary2})
    EXPECT_TRUE(h->getReceivedSummary(2) == copiedSummary);
    EXPECT_TRUE(h->getReceivedSummary(12) == NULL);
    
    // 3. make group with id 100
    auto g = new GroupDefinition(100); 
    EXPECT_TRUE(h->getGroupContextSummary(100) == NULL);
    h->addGroupDefinitionByMoving(g);
    EXPECT_TRUE(h->getGroupContextSummary(100) != NULL);
    
    // check if group 100 has member 1
    vector<int> members;
    GroupContextSummary* gs = h->getGroupContextSummary(100);
    cout << gs->to_string() << endl;
    GroupUtils::getGroupMembers(*gs, members);
    Util::print(members);
    // 
    // groupSummary = self.c.getGroupContext(100)
    // members = getGroupMembers(groupSummary)
    // self.assertTrue(sorted([1, 22]) == sorted(members))
}

TEST_F(ContextHandlerTest, performGroupFormations) {
    // // # set group definistions in ContextHandler
    // auto g = new GroupDefinition(100); 
    // //map<int, unique_ptr<GroupDefinition>> groupDefinitions;
    // //groupDefinitions[100] = g; //  = {100:g}
    // 
    // h->setupGroupDefinition(*g);
    // 
    // // # You *should* get group summary from the getGroupContext method
    // auto groupSummary = h->getGroupContextSummary(100);
    // // std::cout << groupSummary->getId() << std::endl;
    // // std::cout << groupSummary->to_string() << std::endl;
    // 
    // // # myContext is from the summary with id 1
    // auto myContext = summary;
    // // std::cout << myContext->to_string() << std::endl;
    // // # handle it to make groupSummary to contain [1]
    // g->handleContextSummary(*groupSummary, *myContext);
    // // 
    // // # before -> there should be only [1]
    // // static void getGroupMembers(const ContextSummary& summary, std::vector<int>& result);
    // std::vector<int> members;
    // GroupUtils::getGroupMembers(*groupSummary, members);
    // cout << members[0]; 
    // // self.assertTrue(sorted([1]) == sorted(members))
    // // 
    // // # after -> we have summaries with id 22
    // // summaries = [self.summary2]
    // //g->performGroupFormations(*groupDefinitions, summaries)
    // // groupSummary = self.c.getGroupContext(100)
    // // members = getGroupMembers(groupSummary)
    // // 
    // // self.assertTrue(sorted([1,22]) == sorted(members))
}


