#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <string>
#include "util.h"
#include "groupContextSummary.h"

using namespace std;

class QuickTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

class GroupContextSummaryTest : public QuickTest {
 protected:
     virtual void SetUp() {
         QuickTest::SetUp();
         m["abc"] = 10;
         m["def"] = 20;
         m["xyz"] = 100;
         c = new GroupContextSummary(1, &this->m);
         c2 = new GroupContextSummary(1, &this->m);
     }

     virtual void TearDown() {
         delete c;
         delete c2;
     }
     std::map<std::string, int> m;
     GroupContextSummary* c;
     GroupContextSummary* c2;
};


TEST_F(GroupContextSummaryTest, to_string) {
    string expected = "G(1)[3]:{abc:10,def:20,xyz:100}-(0)";
    EXPECT_EQ(this->c->to_string(), expected);
}
