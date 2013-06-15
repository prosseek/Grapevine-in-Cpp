#include <sstream>

#include "contextSummary.h"
#include "util.h"

ContextSummary::ContextSummary(int id, std::map<std::string, int>* db, int hops, std::time_t timestamp)
{
    this->id = id;
    this->db = db;
    this->hops = hops;
    this->timestamp = timestamp;
}

ContextSummary::ContextSummary(const ContextSummary& other)
{
    //std::cout << "copy constructor";
    this->id = other.id;
    this->hops = other.hops;
    this->timestamp = other.timestamp;
    
    //TODO
    // Now it just copies the pointer of the db, but it may be deepcopy ultimately
    this->db = other.db;
    
    // std::cout << this->to_string() << std::endl;
    // std::cout << other.to_string() << std::endl;
}

std::vector<std::string> ContextSummary::keySet()
{
    return Util::getKeys(*(this->db));
}

std::string ContextSummary::to_string() const
{
    //return "(%d)[%d]:%s - (%s)" % (self.uid, self.hops, str(self.db), self.timestamp)
    //std::string dbString = Util::to_string(*(this->db));
    std::string dbString = Util::to_string(db);
    
    std::ostringstream out;  
    out << "(" << this->id << ")" << "[" << this->hops << "]:" << dbString << "-(" << this->timestamp << ")"; 
    
    return out.str();
}

bool ContextSummary::sameExceptHops(const ContextSummary& other)
{
    return (this->id == other.id) && \
           (this->db == other.db) && \
           (this->timestamp == other.timestamp);
}

bool ContextSummary::operator==(const ContextSummary& other)
{
    return sameExceptHops(other) && this->hops == other.hops;
}

bool ContextSummary::get(std::string key, int& result) const
{
    bool contained = containsKey(key);
    if (contained == true) {
        result = (*db)[key];
        return true;
    }
    return false;
}

void ContextSummary::put(std::string key, int value)
{
    //TODO - for bloomier filter, when key is not in the db, it should throw an error
    (*db)[key] = value;
}

bool ContextSummary::containsKey(std::string key) const
{
    std::map<std::string,int>::iterator it = db->find(key);
    if (it == db->end()) return false;
    return true;
}

void ContextSummary::remove(std::string key)
{
    std::map<std::string,int>::iterator it = db->find(key);
    if (it == db->end()) return;
    db->erase(it);
}

/*
 * WARNING
 * The pointer should be freed!!!
 */
ContextSummary* ContextSummary::getWireCopy()
{
    ContextSummary* newCopy = new ContextSummary(*this);
    return newCopy;
}

ContextSummary* ContextSummary::getCopy()
{
    return getWireCopy();
}

int ContextSummary::incrementHops()
{
    int hops = this->getHops();
    return this->setHops(hops+1);
}