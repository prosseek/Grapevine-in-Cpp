#include "contextSummary.h"

ContextSummary::ContextSummary(int id, std::map<std::string, int>* db, int hops, std::time_t timestamp)
{
    this->id = id;
    this->db = db;
    this->hops = hops;
    this->timestamp = timestamp;
}