#ifndef IPLISTDATA_H
#define IPLISTDATA_H

#include <QObject>
#include <vector>
#include "ipnetwork.h"

using namespace std;

struct IPData {
    uint id;
    uint parent;
    bool busy;
    IPNetwork net;
    QString name;
public:
    IPData();
//    uint Id() { return id; }
//    uint Parent() { return parent; }
//    bool Busy() { return busy; }
//    IPNetwork Net() const { return net; }
//    QString Name() const { return name; }
//    void setId(uint i) { id = i; }
//    void setParent(uint p) { parent = p; }
//    void setBusy(bool b) { busy = b; }
//    void setNet(const QString &n) { net = n; }
//    void setNet(const IPNetwork &n) { net = n; }
//    void setName(const QString &n) { name = n; }
};

struct Node {
    int row;
    IPData *data;
    Node *parent;
    vector<Node*> children;
public:
    void clear();
//    int Row() { return row; }
//    Node *Parent() const { return parent; }
//    IPData *Data() const { return data; }
//    vector<Node*> Children() const { return children; }
//    void setRow(int r) { row = r; }
//    void setData(IPData *d) { data = d; }
//    void setParent(Node *n) { parent = n; }
    Node();
};

#endif
