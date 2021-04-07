#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
//#include <QString>

using namespace std;

class IPAddress
{
    uint addr;
public:
    IPAddress(const uint value = 0);
    IPAddress(const string &value);
//    IPAddress operator =(const IPAddress &str);
    IPAddress operator +=(const uint value);
    IPAddress operator +(const uint value);
    IPAddress operator -(const uint value);
    bool operator < (const IPAddress &addr1);
    bool operator <= (const IPAddress &addr1);
    bool operator > (const IPAddress &addr1);
    bool operator >= (const IPAddress &addr1);
    bool operator != (const IPAddress &addr1);
    string toString();
    uint toInt();
    bool isNull();
};

#endif // IPADDRESS_H
