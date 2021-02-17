#include "iplistmodel.h"

IPListModel::IPListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    headers << "IPAddress" << "Description";

    nodes = new Node;

    read_data(nodes, 0);
}

int IPListModel::rowCount(const QModelIndex &parent) const
{
    vector<Node*> n;
    if (!parent.isValid())
        n = nodes->children;
    else
        n = static_cast<Node*>(parent.internalPointer())->children;
    return static_cast<int>(n.size());
}

int IPListModel::columnCount(const QModelIndex &) const
{
    return headers.size();
}

QVariant IPListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto *n = static_cast<Node*>(index.internalPointer());
    IPData *d = n->data;
    if (role == Qt::DisplayRole) {
        int column = index.column();
        switch (column) {
        case 0:
            return d->net.toString().c_str();
//            break;
        case 1:
            return d->name;
//            break;
        default:
            break;
        }
    }
    if (role == Qt::ForegroundRole) {
        if (n->data->busy == false)
            return QColor(Qt::gray);
    }

    return QVariant();
}

QVariant IPListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return headers[section];
    return QVariant();
}

QModelIndex IPListModel::index(int row, int column, const QModelIndex &parent) const
{
    size_t r = static_cast<size_t>(row);

    if (!hasIndex(row,column,parent))
        return QModelIndex();

    Node *childInfo;
    if (!parent.isValid()) {
        childInfo = const_cast<Node*>(nodes->children[r]);
    }
    else {
        Node *parentInfo = static_cast<Node*>(parent.internalPointer());
        childInfo = parentInfo->children[r];
    }
    auto idx = createIndex(row,column,childInfo);
    return idx;
}

QModelIndex IPListModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    auto *childInfo = static_cast<Node*>(child.internalPointer());
    auto *parentInfo = childInfo->parent;
    if (parentInfo != nodes) {
        auto offset = parentInfo->row;
        return createIndex(offset, 0, parentInfo);
    }
    return QModelIndex();
}

void IPListModel::clear(Node *p)
{
    for (uint i = 0; i < p->children.size(); ++i) {
        clear(p->children.at(i));
    }
    p->children.clear();
    delete p->data;
    delete p;
}

void IPListModel::read_data(Node *p, const uint parent)
{
    QSqlQuery q;
    int row = 0;

    q.prepare("SELECT id,parent,busy,addr,descr FROM ipaddr WHERE parent = :parent ORDER BY addr");
    q.bindValue(":parent", parent);
    if (!q.exec()) {
        qDebug() << q.lastError() << Qt::endl;
        return;
    }
    while (q.next()) {
        IPData *d = new IPData;
        d->id = q.value(0).toUInt();
        d->parent = q.value(1).toUInt();
        d->busy = q.value(2).toBool();
        d->net = q.value(3).toString().toStdString();
        d->name = q.value(4).toString();

        Node *n = new Node;
        n->data = d;
        n->parent = p;
        n->row = row;
        p->children.push_back(n);

//        dt.push_back(d);
        read_data(n, d->id);
        ++row;
    }

    std::sort(
                p->children.begin(), p->children.end(), [](Node *n1, Node *n2)
                {
                    return n1->data->net < n2->data->net;
                }
    );

    if (parent == 0) {
        emit loadedData();
        qDebug() << "Signal loadedData()";
    }
}

void IPListModel::reloadData()
{
    beginResetModel();
    clear(nodes);
    nodes = new Node;
    read_data(nodes, 0);
    endResetModel();
}
