//
//  HashMap.cpp
//  
//
//  Created by bobbie Okparanyote on 2015-10-15.
//
//

#include "HashMap.h"
using namespace std;

String HashMap::get(int key)
{
    int i =0;
    while (table[i] != NULL && table[i]->getKey() != key) i++;
    if (table[i] == NULL)
        return "Not Registered";
    else
        return table[i]->getValue();
}
int HashMap::getS(int key)
{
    int i =0;
    while (table[i] != NULL && table[i]->getKey() != key) i++;
    if (table[i] == NULL)
        return 0;
    else
        return table[i]->getSeat();
}

void HashMap::put(int key,String value,int seat)
{
    int i =0;
    while (table[i] != NULL && table[i]->getKey() != key) i++;
    if (table[i] != NULL)
        delete table[i];
    table[i] = new HashEntry(key, value,seat);
}
