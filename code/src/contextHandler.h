#include <cstddef>
#include <map>
#include <vector>
#include <memory>

#include "groupDefinition.h"
#include "contextSummary.h"
#include "grapevine.h"

class ContextHandler {
    // """
    // It's a singleton, so it should be invoked with ConstextHandler.getInstance()
    // """
    int tau = 3;
    static ContextHandler* singleton; // = nullptr;
    map<int, unique_ptr<GroupDefinition>> groupDefinitions;
    map<int, unique_ptr<GroupContextSummary>> groupContextSummaries;
    map<int, unique_ptr<ContextSummary>> receivedSummaries;
    unique_ptr<ContextSummary> myContext;
protected:  
    // Need unit test
    // destructor removes all the allocated pointers, and reset the map
    template <typename T>
    void resetDictionary(map<int, unique_ptr<T>>& m)
    {
        int size = m.size();
        for (auto& val: m) 
        {
            val.second.reset();
        }
        m.clear();
    }
      
    template<typename T>
    void setMap(map<int, unique_ptr<T>>& oldMap, map<int, unique_ptr<T>>& newMap)
    {
        resetDictionary<T>(oldMap);
        oldMap = move(newMap);
    }
    
    template<typename T>
    void setMap(map<int, unique_ptr<T>>& oldMap, vector<unique_ptr<T>>& newVector)
    {
        resetDictionary<T>(oldMap);
        for (int i = 0; i < newVector.size(); i++) {
            int index = newVector[i]->getId();
            oldMap[index] = move(newVector[i]);// std::move(r[i]);
        }
    }
    
    template<typename T>
    T* getFromMap(map<int, unique_ptr<T>>& m, int gid)
    {
        if (m.find(gid) != m.end()) {
            return m[gid].get();
        }
        return reinterpret_cast<T*>(NULL);
    }
public:
    //     def resetAllSummarydata(self):
    //         self.setMyContext(None)
    //         self.resetGroupDefinitions()
    //         self.setgroupContextSummaries({})
    //         self.setReceivedSummaries({})
    void resetAllSummarydata()
    {
        removeLocalSummary();
        resetDictionary<GroupContextSummary>(groupContextSummaries);
        resetDictionary<GroupDefinition>(groupDefinitions);
        resetDictionary<ContextSummary>(receivedSummaries);
    }
    
    ~ContextHandler()
    {
        resetAllSummarydata();
    }
    ////////////////////////////
    
    ContextHandler()
    {
        
    }
    
    // Singleton implementation
    // TODO - Think this is the best implementation
    static ContextHandler* getInstance()
    {
        if (ContextHandler::singleton == nullptr)
        {
            ContextHandler::singleton = new ContextHandler();
        }
        return ContextHandler::singleton;
    }
    
    static void resetContextHandler()
    {
        if (ContextHandler::singleton != nullptr)
        {
            delete ContextHandler::singleton;
        }
        ContextHandler::singleton = nullptr;
    }
    
    void setTau(int tau) {this->tau = tau;}
    int getTau() {return this->tau;}
    
    void setReceivedSummaries(map<int, unique_ptr<ContextSummary>>& receivedSummaries)
    {
        return setMap<ContextSummary>(this->receivedSummaries, receivedSummaries);
    }
    void setGroupContextSummaries(map<int, unique_ptr<GroupContextSummary>>& groupContextSummaries)
    {
        return setMap<GroupContextSummary>(this->groupContextSummaries, groupContextSummaries);
    }
    void setGroupDefinitions(map<int, unique_ptr<GroupDefinition>>& groupDefinitions)
    {
        return setMap<GroupDefinition>(this->groupDefinitions, groupDefinitions);
    }
    
    void setReceivedSummaries(vector<unique_ptr<ContextSummary>>& v)
    {
        return setMap<ContextSummary>(this->receivedSummaries, v);
    }
    void setGroupContextSummaries(vector<unique_ptr<GroupContextSummary>>& v)
    {
        return setMap<GroupContextSummary>(this->groupContextSummaries, v);
    }
    void setGroupDefinitions(vector<unique_ptr<GroupDefinition>>& v)
    {
        return setMap<GroupDefinition>(this->groupDefinitions, v);
    }

    map<int, unique_ptr<ContextSummary>> getReceivedSummaries()
    {
        return move(this->receivedSummaries);
    }  
    map<int, unique_ptr<GroupContextSummary>> getGroupContextSummaries()
    {
        return move(this->groupContextSummaries);
    }
    map<int, unique_ptr<GroupDefinition>> getDefinitions()
    {
        return move(this->groupDefinitions);
    }
    
    GroupDefinition* getGroupDefinition(int gid) // , GroupDefinition& result)
    {
        return getFromMap<GroupDefinition>(groupDefinitions, gid);
    }
    GroupContextSummary* getGroupContextSummary(int gid) // , GroupContextSummary* & result)
    {
        return getFromMap<GroupContextSummary>(groupContextSummaries, gid);
    }
    ContextSummary* getReceivedSummary(int gid) // , GroupContextSummary* & result)
    {
        return getFromMap<ContextSummary>(receivedSummaries, gid);
    }
    
    
    vector<unique_ptr<ContextSummary>> getValues(const map<int, unique_ptr<ContextSummary>>& receivedSummaries)
    {
        // get values from summary
        vector<unique_ptr<ContextSummary>> result;
        for (auto& items: receivedSummaries)
        {
            result.push_back(unique_ptr<ContextSummary>(new ContextSummary(*items.second.get())));
        }
        
        return result;
    }
    
    void performGroupFormations(map<int, unique_ptr<GroupDefinition>>& groupDefinitions, 
                                map<int, unique_ptr<ContextSummary>>& summaries)
    {
        // std::vector<unique_ptr<ContextSummary>> result;
        //     
        // for(auto& item: summaries)
        //     result.push_back(move(item.second));
        // return result;
        
        // ???????
        auto values = getValues(summaries);
        performGroupFormations(groupDefinitions, values);
    }
                                
    void performGroupFormations(map<int, unique_ptr<GroupDefinition>>& groupDefinitions, 
                                vector<unique_ptr<ContextSummary>>& summaries)                            
    {
        // iterate over all the group definitions
        for (auto& gd: groupDefinitions)
        {
            int gid = gd.second->getId();
            GroupContextSummary* groupSummary = getGroupContextSummary(gid);
            
            for (auto& summary: summaries)
            {
                int uid = summary->getId();
                if (gid == uid)
                {
                    // summary is actually group, so we can
                    // safely downcast it into GroupContextSummary&
                    gd.second->handleGroupSummary(*groupSummary, 
                       static_cast<GroupContextSummary&>(*summary.get()));
                }
                else 
                {
                    gd.second->handleContextSummary(*groupSummary, *summary.get());
                }
            }
        }
    }
    
    void setupGroupDefinition(int gId)
    {
        groupContextSummaries[gId] = unique_ptr<GroupContextSummary>(new GroupContextSummary(gId));
        groupDefinitions[gId] = unique_ptr<GroupDefinition>(new GroupDefinition(gId));
    }
    
    // GroupDefinition is created outside
    // However, I want to keep the groupDefinition in a map
    void setupGroupDefinition(GroupDefinition& groupDefinition)
    {
        int gId = groupDefinition.getId();
        groupContextSummaries[gId] = unique_ptr<GroupContextSummary>(new GroupContextSummary(gId));
        groupDefinitions[gId] = unique_ptr<GroupDefinition>(&groupDefinition);
    }
    
    void addGroupDefinition(GroupDefinition& groupDefinition)
    {
        setupGroupDefinition(groupDefinition);
        int gId = groupDefinition.getId();
        auto groupSummary = getGroupContextSummary(gId);
        auto myContext = getMyContext();
        
        if (myContext != NULL) 
        {
            groupDefinition.handleContextSummary(*groupSummary, *myContext);
        }
        performGroupFormations(groupDefinitions, receivedSummaries);
    }
    
    /* 
        Local Summary (MyContext) 
    */
    
    /*
        setMyContext removes the existing summary, and set the new summary to the myContext
    */
    void setMyContext(ContextSummary& summary)
    {
        removeLocalSummary();
        this->myContext = unique_ptr<ContextSummary>(&summary);
    }
    // This is OK, as we return the reference 
    // of what is already there. 
    ContextSummary* getMyContext()  {return myContext.get();}
    
    void removeLocalSummary()
    {
        myContext.reset();
    }
    
    void updateLocalSummary(ContextSummary& summary)
    {
        // from summary, deepcopy the object 
        removeLocalSummary();
        this->myContext = unique_ptr<ContextSummary>(new ContextSummary(summary));
        
        for (auto& groupDefiniton: groupDefinitions)
        {
            int gId = groupDefiniton.second->getId();
            // getX always returns pointer
            // the input is always reference
            auto groupSummary = getGroupContextSummary(gId);
            groupDefiniton.second->handleContextSummary(*groupSummary, *myContext.get());
        }
    }
    
    void handleIncomingSummaries(vector<unique_ptr<ContextSummary>> summaries)
    {
        vector<unique_ptr<ContextSummary>> summariesToPut;
        auto myContext = getMyContext();
        vector<int> receivedSummaryIds = Util::getKeys(receivedSummaries);
        for (auto& summary: summaries)
        {
            int uid = summary->getId();
            summary->incrementHops();
            if (myContext != NULL && myContext->getId() == uid) continue; // skip if the summary is me
            
            if (Util::in(receivedSummaryIds, uid))
            {
                auto existing = getReceivedSummary(uid);
                auto summaryTimeStamp = summary->getTimestamp();
                auto existingTimeStamp = existing->getTimestamp();
                if ( summaryTimeStamp < existingTimeStamp \
                   || (summaryTimeStamp == existingTimeStamp) \
                       && summary->getHops() >= existing->getHops())
                       continue;
                // Is this OK? What would be the policy? Copy or reference copy? 
                summariesToPut.push_back(unique_ptr<ContextSummary>(new ContextSummary(*summary)));
            }
        }
        performGroupFormations(groupDefinitions, summariesToPut);
    }
    
    // Don't forget that get returns pointer. 
    unique_ptr<ContextSummary> get(int uid)
    {
        if (getMyContext() != NULL && myContext->getId() == uid) {
            return unique_ptr<ContextSummary>(new ContextSummary(*myContext.get()));
        }
        if (Util::in(Util::getKeys(groupContextSummaries), uid)) {
            return unique_ptr<GroupContextSummary>(new GroupContextSummary(*getGroupContextSummary(uid)));
        }
        if (Util::in(Util::getKeys(receivedSummaries), uid)) {
            return unique_ptr<ContextSummary>(new ContextSummary(*getReceivedSummary(uid)));
        }
        return unique_ptr<ContextSummary>(reinterpret_cast<ContextSummary*>(NULL));
    }
    
    vector<unique_ptr<ContextSummary>> getSummariesToSend()
    {
        vector<unique_ptr<ContextSummary>> result;
        
        if (getMyContext() != NULL) {
            result.push_back(unique_ptr<ContextSummary>(new ContextSummary(*getMyContext())));
        }
        
        for (auto& groupSummary: groupContextSummaries)
        {
            auto summaryPointer = groupSummary.second.get();
            //auto uniquePtr = unique_ptr<GroupContextSummary>(new GroupContextSummary(*summaryPointer));
            result.push_back(unique_ptr<GroupContextSummary>(new GroupContextSummary(*summaryPointer)));
        }
        
        for (auto& summary: receivedSummaries)
        {
            int hops = summary.second->getHops();
            if (hops < this->tau)
            {
                auto summaryPointer = summary.second.get();
                //auto uniquePtr = unique_ptr<GroupContextSummary>(new GroupContextSummary(*summaryPointer));
                result.push_back(unique_ptr<ContextSummary>(new ContextSummary(*summaryPointer)));
            }
        }
        
        return result;
    }
    
    void setTauAndRemoveSummaries(int tau)
    {
        //TODO
        // How to find the item to remove during iterations?
        setTau(tau);
        vector<int> deleteKeys;
        for (auto& summary: receivedSummaries)
        {
            int hops = summary.second->getHops();
            if (hops >= tau)
            {
                // get the pointer and delete it.
                deleteKeys.push_back(summary.first);
            }
        }
        for (int key: deleteKeys)
        {
            auto ip = receivedSummaries.find(key);
            //auto ip = find(receivedSummaries.begin(), receivedSummaries.end(), key);
            receivedSummaries.erase(ip);
        }
    }
}; 
    
//   ??? Do we need to return copied one? or naive one?               
//     def getGroupSummary(self, gid):
//         groupSummary = self.groupContextSummaries[gid]
//         if groupSummary is not None:
//             return groupSummary.getGroupCopy()
//         return None
