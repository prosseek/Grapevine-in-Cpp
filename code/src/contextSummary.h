#ifndef __CONTEXT_SUMMARY_H__
#define __CONTEXT_SUMMARY_H__

#include <string>
#include <map>
#include <ctime>
#include <cstddef>
#include <vector>

class ContextSummary {
    int id;
    int hops;
    std::map<std::string, int>* db;
    std::time_t timestamp;
    
public:
    ContextSummary(int id, std::map<std::string, int>* db = nullptr, int hops = 3, std::time_t timestamp = 0);
    int size() { return db->size();}
    std::vector<std::string> keySet();
};

#endif