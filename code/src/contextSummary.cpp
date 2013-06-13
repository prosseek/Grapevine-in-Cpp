#include "contextSummary.h"

ContextSummary::ContextSummary(int id, std::map<std::string, int>* db, int hops, std::time_t timestamp)
{
    this->id = id;
    this->db = db;
    this->hops = hops;
    this->timestamp = timestamp;
}

std::vector<std::string> ContextSummary::keySet()
{
    std::vector<std::string> result;

    for(auto key: *(this->db))
        result.push_back(key.first);
    return result;
}