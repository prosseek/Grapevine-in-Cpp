//
// Created by smcho on 5/30/13.
// Copyright (c) 2013 ___MPC___. All rights reserved.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __Util_H_
#define __Util_H_

#include <iostream>
#include <list>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>

//namespace bloomier {
    
/**
 * Utility class. All of the methods are static to be used 
 * as static function. 
 */

class Util {
public:
    /**
     * vectorA += vectorB
     * 
     * @param std::vector<T> vectorA
     * @param std::vector<T> vectorB
     */
    template <class T>
    static void addAll(std::vector<T>&, const std::vector<T>);

    /**
     * all the members in map that has vectorA will be removed
     * before: map = {'a':10, 'b':20}, vectorA = [20]
     * after:  map = {'a':10 }
     * 
     * @param std::map<T1, T2> map
     * @param std::vector<T> vectorA
     */
    template <class T1, class T2, class T3>
    static void removeAll(std::map<T1, T2>&, const std::vector<T3>);
    
    template <class T>
    static void removeAll(std::vector<T>&, const std::vector<T>&);
    
    /**
     * For all the elements in the arrays:
     * result = result ^ input
     *
     * @param unsigned char result
     * @param unsigned char input
     */
    //static void byteArrayXor(unsigned char result[], const unsigned char input[]);
    static void byteArrayXor(unsigned char* result, const unsigned char* input, int byteSize);
    
    static void setInArray(unsigned char* result, const unsigned char* input, int byteSize);
    
    /** 
     * get the "byte" size when given "bit" size
     *  return q//8 + (1 if q % 8 != 0 else 0)
     */ 
    static int getByteSize(int value);
    
    /**
     * Check if value is in the setArray
     *
     * @param set<int> setArray
     * @param int value
     */
    static bool in(std::set<int> setArray, int value);
    template <class T> 
    static bool in(std::vector<T> vectorArray, T value);
    
    /**
     * deepcopy from source to dest
     */
    static void deepcopy(const std::map<std::string, int> source, std::map<std::string, int>& dest);
    
    /**
     * getKeys from map
     */
    
    template<class T>
    static std::vector<T> getKeys(std::map<T, int>& map);
    
    template<class T>
    static bool sameTwoVectors(std::vector<T> first, std::vector<T> second);
    
    template<class T>
    static std::string to_string(std::map<T, int>& m);
    
    template<class T>
    static std::string to_string(std::map<T, int>* p);
    
    template <class T>
    static void print(const std::vector<T>* vectorArray);
    //static void print(const std::vector<unsigned char>* vectorArray);
        
    template <class T>
    static void print(const std::vector<T>& vectorArray);
    
    template <class T>
    static void print(T* array, int size);
    
    static void printByteStream(const std::vector<unsigned char>& input);
    
    static bool startswith(std::string mainstring, std::string substring);
    static bool endswith(std::string mainstring, std::string substring);
    
    template<typename T>
    static void andOperation(const std::vector<T>& vectorArray1, const std::vector<T>& vectorArray2, std::vector<T>& result);
    static std::string byteToHexString(unsigned char value);
    
    static void byteArrayToString(const std::vector<unsigned char>& byteArray, std::string& result);
    static void stringToByteArray(const std::string str, std::vector<unsigned char>& result);    
};
//}

#endif //__Util_H_
