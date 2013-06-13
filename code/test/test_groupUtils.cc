// #include <limits.h>
// #include <time.h>
// #include <gtest/gtest.h>
// #include <list>
// #include <map>
// #include <string>
// #include "grapevine.h"
// #include "groupUtils.h"
// #include "util.h"

#include <limits.h>
#include <time.h>
#include <gtest/gtest.h>
#include <list>
#include <map>
#include <string>

#include "util.h"

using namespace std;

class QuickTest : public testing::Test {
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

class GroupUtilsTest : public QuickTest {
 protected:
  virtual void SetUp() {
      QuickTest::SetUp();
  }
  
  virtual void TearDown() {
      //std::cout << "Tearing down...\n";
      // aList.clear();
      // bList.clear();
      // aMap.clear();
      QuickTest::TearDown();
  }

  // std::vector<int> aList; // = {10, 20, 30, 40};
  // std::vector<int> bList;
  // std::map<std::string, int> aMap;
};

TEST_F(GroupUtilsTest, getDeclaredMemberships) {
    // vector<string> keys;
    // Util::print(keys);
    
    /*
     * getDeclaredMemberships(summary)
     * Given summary, returns all the "GROUP_DECLARATIONS_ENUMERATED" in a vector
     */
    //this->summary
    //GroupUtils::getDeclaredMemberships()
    // std::vector<int> aList = {10, 20, 30, 40}; 
    // //bList.push_back(100); bList.push_back(200);
    // EXPECT_EQ(4, aList.size());
    // //Util::addAll(aList, bList);
    // EXPECT_EQ(6, aList.size());
}
