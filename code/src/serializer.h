#include <string>
#include <iostream>
#include <vector>

#include "util.h"
#include "serializerUtils.h"

using namespace std;

class Serializer {
    int bufferPointer = 0;
    vector<unsigned char> result;
    string tempResultInString;
    
    vector<unsigned char> intToByteArray(int value);
    int byteArrayToInt(const vector<unsigned char>& value);
    string byteArrayToString(const vector<unsigned char>& value); // , string str);
    time_t byteArrayToTimestamp(const vector<unsigned char>& value);
    vector<unsigned char> timestampToByteArray(time_t value);
public:
    Serializer() {}
    
    Serializer(const std::string& result)
    {
        Util::stringToByteArray(result, this->result);
    }

    Serializer(const vector<unsigned char>& result)
    {
        this->result = result;
    }
    
    // reset result and bufferPointer
    void setResult(const vector<unsigned char>& result)
    {
        this->result = result;
        bufferPointer = 0; 
    }
    
    void setResult(const std::string& result)
    {
        vector<unsigned char> res;
        Util::stringToByteArray(result, res);
        setResult(res);
    }
    
    vector<unsigned char> const& getResult() const
    {
        return this->result;
    }
    
    string const& getResultAsString()
    {
        tempResultInString.clear();
        Util::byteArrayToString(this->result, tempResultInString);
        return tempResultInString;
    }
    
    string to_string()
    {
        string res;
        Util::byteArrayToString(this->result, res);
        return to_string(res);
    }
    
    string to_string(const string& value, bool hex = false)
    {
        string res;

        for (unsigned char i: value) {        
            string temp = hex ? "[" + Util::byteToHexString(i) + "]" : "[" + std::to_string(i) + "]";
            res += temp;
        }
        return res;
    }
    
    //template<typename T>
    void writeObjectData(int value) // , string typeName)
    {
        vector<unsigned char> temp;
        temp = intToByteArray(value);
        this->result.insert(this->result.end(), temp.begin(), temp.end());
    }
    
    void writeObjectData(string value)
    {
        int length = value.size();
        writeObjectData(length);
        
        vector<unsigned char> temp;
        Util::stringToByteArray(value, temp);

        this->result.insert(this->result.end(), temp.begin(), temp.end());
    }
    
    void writeObjectData(time_t value)
    {
        vector<unsigned char> temp;
        temp = timestampToByteArray(value);
        this->result.insert(this->result.end(), temp.begin(), temp.end());
    }
    
    void readObjectData(const vector<unsigned char>& buffer, int& result)
    {
        result = byteArrayToInt(buffer);
    }
    
    void readObjectData(const vector<unsigned char>& buffer, string& result)
    {
        // extract the size of string length encoded in 4 bytes
        std::vector<unsigned char>  sub(&buffer[0], &buffer[4]);
        int length = byteArrayToInt(sub);
        
        std::vector<unsigned char>  sub2(&buffer[4], &buffer[4 + length]);
        result = byteArrayToString(sub2);
    }
    
    void readObjectData(const vector<unsigned char>& buffer, time_t& result)
    {
        result = byteArrayToTimestamp(buffer);
    }
    
    // template<typename T>
    // T autoReadObjectData(string type)
    // {
    //     
    // }
    // // def autoReadObjectData(self, type):
    // //     if type == "int":
    // //         length = 4
    // //     elif type.startswith("string"): # input is string5 or something
    // //         length = int(type[len("string"):])
    // //         type = "string"
    // //     elif type == "timestamp":
    // //         # 13 is the string length converted from time.time()
    // //         length = 13 # len(buffer)
    // //     else:
    // //         print >> sys.stderr, "***" + type + "***"
    // //         raise Exception("only int/string/timestamp allowed")
    // //         
    // //     buffer = self.result[self.bufferPointer:self.bufferPointer + length]
    // //     result = self.readObjectData(buffer, type)
    // //     #print result
    // //     self.bufferPointer += length
    // //     return result
    // // 
    
    void reset()
    {
        result.clear();
        bufferPointer = 0;
    }

    int size()
    {
        return this->result.size();
    }
};