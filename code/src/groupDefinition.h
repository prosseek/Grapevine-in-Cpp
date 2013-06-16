#include <vector>
#include "groupContextSummary.h"
#include "contextSummary.h"

using namespace std;

class GroupDefinition {
    int groupId;
public:
    GroupDefinition(int groupId)
    {
        this->groupId = groupId;
    }
    
    int getId() {return this->groupId;}
    
    void handleContextSummary(GroupContextSummary& currentGroupSummary, const ContextSummary& newSummary);
    void handleGroupSummary(GroupContextSummary& currentGroupSummary, const GroupContextSummary& newSummary);
};


