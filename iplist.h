#ifndef IPLIST_H
#define IPLIST_H

#include <vector>
#include <memory>
#include <QObject>
#include <QAction>
#include <QWidget>
#include <QTreeView>
#include <QMessageBox>
#include <QtSql>
//#include <QPair>
#include "iplistmodel.h"
#include "editbusynetwork.h"
#include "editsizenetwork.h"
#include "transaction.h"

using namespace std;

class IPList : public QTreeView
{
    Q_OBJECT

public:
    IPList(QWidget *parent = nullptr);
    QList<QAction*> sact;

public slots:
    void expandList();

signals:
    void updatedata();

private:
    IPListModel *iplm;
//    bool deleteNetwork(const Node *n);
    bool calc_networks(const pair<IPAddress,IPAddress> &, const uint parent);
    bool write_network(IPNetwork &, const uint parent, const QString &descr, const int busy);
    bool delete_network(const uint id);
    Node *get_current();
    pair<IPAddress,IPAddress> find_network(const Node *n, IPNetwork &myNet);
    bool write_new_networks(const pair<IPAddress,IPAddress> &, IPNetwork &, const uint parent, const QString &str);

private slots:
    void addFreeNetwork();
//    void delFreeNetwork();
    void addUsedNetwork();
    void delUsedNetwork();
    void findFreeNetwork();
    void updateDescription();
};

#endif // IPLIST_H
