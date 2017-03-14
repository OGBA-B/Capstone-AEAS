//
//  HashMap.hpp
//  
//
//  Created by bobbie Okparanyote on 2015-10-15.
//
//

#ifndef HashMap_hp
#define HashMap_hp
#include <HashEntry.h>
#include <stdio.h>
using namespace std;
class HashMap {
private:
    int TABLE_SIZE=128;
    HashEntry **table;
public:
    HashMap() {
        table = new HashEntry*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }
    
    String get(int key);
    int getS(int key); 
    void put(int key, String value,int seat);
    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL)
                delete table[i];
        delete[] table;
    }
    
};


#endif /* HashMap_hpp */
