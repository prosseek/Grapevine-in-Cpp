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
    ContextSummary();
    ContextSummary(int id, std::map<std::string, int>* db = nullptr, int hops = 3, std::time_t timestamp = 0);
    ContextSummary(const ContextSummary& other);
    ContextSummary& operator=(const ContextSummary& other);
    
    bool sameExceptHops(const ContextSummary& other);
    bool operator==(const ContextSummary& other);
    
    /**
     *   size() returns the elements in this->db
     */
    int size() { return db->size();}
    
    /**
     * keySet() returns the vector string that contains all the keys in this->db
     */
    std::vector<std::string> keySet();
    
    std::time_t getTimestamp() const {return this->timestamp;}
    void setTimestamp(std::time_t timestamp) {this->timestamp = timestamp;}
    
    int getHops() {return this->hops;}
    int setHops(int hops) {this->hops = hops; return hops;}
    
    int getId() const {return this->id;}
    void setId() {this->id = id;}
    
    /**
     * to_string() returns a string format for ContextSummary object
     * http://stackoverflow.com/questions/16431442/c-struct-passing-const-as-this-argument-discards-qualifiers
     */
    std::string to_string() const;
    
    /**
     * get(key) returns the value from the db
     */
    bool get(std::string key, int& result) const;
    
    /**
     * put(key, val) sets the db[key] into val
     */
    void put(std::string key, int value);
    
    bool containsKey(std::string key) const;
    void remove(std::string key);
    ContextSummary* getWireCopy();
    ContextSummary* getCopy();
    int incrementHops();
};

#endif