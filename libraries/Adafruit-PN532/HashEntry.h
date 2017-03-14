//
//  HashEntry.hpp
//  
//
//  Created by bobbie Okparanyote on 2015-10-15.
//
//
#include <stdio.h>
#include <WString.h>
using namespace std;
class HashEntry{
private:
    int key;
    String value;
    int seat;
public:
    HashEntry(int key, String value,int seat) {
        this->key = key;
        this->value = value;
        this->seat = seat;
    }
    int getKey();
    String getValue();
    int getSeat();
};