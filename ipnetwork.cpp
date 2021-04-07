#include "ipnetwork.h"

IPNetwork::IPNetwork(const string value)
{
    *this = value;
/*
    if (value.length() == 0)
        return;

    QPair<string,string> lst;
    auto pos = value.find("/");
    lst.first = value.substr(0,pos);
    if (pos == string::npos)
        lst.second = "32";
    else
        lst.second = value.substr(pos+1);

    addr = lst.first;

    int c = count(lst.second.begin(), lst.second.end(), '.');
    if (c == 3) {
        mask = lst.second;
    }
    else if (c == 0) {
        int bits = stoi(lst.second);
        if (bits >=0 && bits <= 32)
            mask = (0xffffffff << (32-bits));
        else
            mask = 0;
    }
    else {
        mask = 0;
    }
    */
}

IPNetwork::IPNetwork(const IPAddress &a, const uint bits) :
    addr(a)
{
    if (bits <= 32)
        mask = (0xffffffff << (32-bits));
    else
        mask = 0;
}

IPNetwork IPNetwork::operator =(const string &value)
{
    if (value.length() == 0)
        return *this;

    pair<string,string> lst;
    auto pos = value.find("/");
    lst.first = value.substr(0,pos);

    if (count(lst.first.begin(), lst.first.end(), '.') != 3)
        lst.first = "";

    if (pos == string::npos)
        lst.second = "32";
    else
        lst.second = value.substr(pos+1);

    addr = lst.first;

    long c = count(lst.second.begin(), lst.second.end(), '.');
    if (c == 3) {
        mask = lst.second;
    }
    else if (c == 0) {
        int bits = stoi(lst.second);
        if (bits >=0 && bits <= 32)
            mask = (0xffffffff << (32-bits));
        else
            mask = 0;
    }
    else {
        mask = 0;
    }

    return *this;
}

bool IPNetwork::operator <(const IPNetwork &n)
{
    return addr < n.addr;
}

bool IPNetwork::Contain(IPNetwork &net)
{
    if (addr <= net.addr && Broadcast() >= net.Broadcast())
        return true;
    return false;
}

bool IPNetwork::Crossing(IPNetwork &net)
{
    uint a = Network().toInt();
    uint b = Broadcast().toInt();
    uint c = net.Network().toInt();
    uint d = net.Broadcast().toInt();

    if (a >= c && a <= d)
        return true;
    else if (b >= c && b <= d)
        return true;

    else if (c >= a && c <= b)
        return true;
    else if (d >= a && d <= b)
        return true;

    return false;
}

int CountOnes2_FF(uint n)
{
    int res = 0;
    while (n) {
        res++;
        n &= n-1;
    }

    return res;
}

string IPNetwork::toString()
{
    stringstream ss;
    uint m = mask.toInt();
    int bits = CountOnes2_FF(m);

    ss << addr.toString() << '/' << bits;
//    QString str = QString("%1/%2").arg(addr.toString().c_str()).arg(bits);

    return ss.str();
}

IPAddress IPNetwork::Network()
{
    return (addr.toInt() & mask.toInt());
//    return a;
}

IPAddress IPNetwork::Broadcast()
{
    uint b = ~mask.toInt();
    return (addr.toInt() + b);
//    return a;
}

uint IPNetwork::size()
{
    uint b = ~mask.toInt();
    return b+1;
}
