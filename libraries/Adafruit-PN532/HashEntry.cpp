//
//  HashEntry.cpp
//  
//
//  Created by bobbie Okparanyote on 2015-10-15.
//
//


#ifndef HashEntry_h
#define HashEntry_h
#include "HashEntry.h"
#include <WString.h>
#include <stdio.h>
using namespace std;
int HashEntry::getKey()
{
    return key;
}
String HashEntry::getValue()
{
    return value;
}
int HashEntry::getSeat()
{
    return seat;
}


#endif /* HashEntry_hpp */

