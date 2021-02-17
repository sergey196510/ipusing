#include "ipaddress.h"

IPAddress::IPAddress(const uint value) :
    addr(value)
{
//    addr = value;
}

IPAddress::IPAddress(const string &value)
{
    stringstream ss(value);
    uint arr[4];

    int c = count(value.begin(), value.end(), '.');
    if (c != 3)
        addr = 0;
    else {
        ss >> arr[0];
        ss.get();
        ss >> arr[1];
        ss.get();
        ss >> arr[2];
        ss.get();
        ss >> arr[3];
        addr = (arr[0]<<24) + (arr[1]<<16) + (arr[2]<<8) + arr[3];
    }
}

IPAddress IPAddress::operator =(const IPAddress &a)
{
    addr = a.addr;
    return *this;
}

IPAddress IPAddress::operator +=(const uint value)
{
    addr += value;
    return *this;
}

IPAddress IPAddress::operator +(const uint value)
{
    IPAddress a = addr+value;
    return a;
}

IPAddress IPAddress::operator -(const uint value)
{
    IPAddress a = addr-value;
    return a;
}

bool IPAddress::operator <(const IPAddress &a)
{
    return addr < a.addr;
}

bool IPAddress::operator <=(const IPAddress &a)
{
    return addr <= a.addr;
}

bool IPAddress::operator >(const IPAddress &a)
{
    return addr > a.addr;
}

bool IPAddress::operator >=(const IPAddress &a)
{
    return addr >= a.addr;
}

bool IPAddress::operator != (const IPAddress &a)
{
    return addr != a.addr;
}

string IPAddress::toString()
{
    stringstream ss;

    ss << (addr>>24) % 256 << '.' << (addr>>16) % 256 << '.' << (addr>>8) % 256 << '.' << addr % 256;
//    QString str = QString("%1.%2.%3.%4")
//            .arg((addr>>24) % 256)
//            .arg((addr>>16) % 256)
//            .arg((addr>>8) % 256)
//            .arg(addr % 256);

    return ss.str();
}

uint IPAddress::toInt()
{
    return addr;
}

bool IPAddress::isNull()
{
    if(addr == 0)
        return true;
    return false;
}
