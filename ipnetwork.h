#ifndef IPNETWORK_H
#define IPNETWORK_H

#include <QtCore>
#include <algorithm>
#include "ipaddress.h"

using namespace std;

class IPNetwork {
    IPAddress addr;
    IPAddress mask;
public:
    IPNetwork(const string value = "");
    IPNetwork(const IPAddress &addr, const uint mask);
    string toString();
    IPNetwork operator =(const string &value);
    bool operator <(const IPNetwork &n);
    bool Contain(IPNetwork &net);
    bool Crossing(IPNetwork &net);
    IPAddress Network();
    IPAddress Broadcast();
    uint size();
};

#endif
