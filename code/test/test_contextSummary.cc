// #include <limits.h>
// #include <time.h>
// #include <gtest/gtest.h>
// #include <list>
// #include <map>
// #include <string>
// 
// #include "grapevine.h"
// #include "groupUtils.h"



#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <string>

#include "contextSummary.h"
#include "util.h"

//using namespace std;

class QuickTest : public testing::Test {
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

class ContextSummaryTest : public QuickTest {
 protected:
  virtual void SetUp() {
      QuickTest::SetUp();
      m["abc"] = 10;
      m["def"] = 20;
      m["xyz"] = 100;
      c = new ContextSummary(1, &this->m);
  }
  
  virtual void TearDown() {
      delete c;
  }
  std::map<std::string, int> m;
  ContextSummary* c;
};

TEST_F(ContextSummaryTest, size) {    
    EXPECT_EQ(3, c->size());
}

TEST_F(ContextSummaryTest, getKeys) {  
    // We expect to get 3 elements
    std::vector<std::string> keys = c->keySet();
    EXPECT_EQ(3, keys.size());
    // Util::print(keys);
    Util::getKeys(this->m);
    EXPECT_TRUE(Util::sameTwoVectors(keys, Util::getKeys(this->m)));
}