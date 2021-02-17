#ifndef IPLISTMODEL_H
#define IPLISTMODEL_H

#include <vector>
#include <algorithm>
#include <QObject>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QtSql>
#include "iplistdata.h"

using namespace std;

class IPListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    IPListModel(QObject *parent);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    void read_data(Node *p, const uint parent = 0);
    void clear(Node *p);

public slots:
    void reloadData();

signals:
    void loadedData();

private:
    QStringList headers;
    vector<IPData> dt;
    Node *nodes;
};

#endif // IPLISTMODEL_H
