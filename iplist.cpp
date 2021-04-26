#include "iplist.h"

IPList::IPList(QWidget *parent) :
    QTreeView(parent)
{
    iplm = new IPListModel(this);
    setModel(iplm);
//    expandList();

//    connect(this, SIGNAL(updatedata()), iplm, SLOT(reloadData()));
    connect(this, &IPList::updatedata, iplm, &IPListModel::reloadData);
//    connect(iplm, SIGNAL(loadedData()), SLOT(expandList()));
    connect(iplm, &IPListModel::loadedData, this, &IPList::expandAll);
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(expandList()));
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(expandList()));

    QAction *act1 = new QAction();
    act1->setText("Add free");
    act1->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    act1->setStatusTip(tr("Add free network"));
//    connect(act1, SIGNAL(triggered(bool)), SLOT(addFreeNetwork()));
    connect(act1, &QAction::triggered, this, &IPList::addFreeNetwork);
    sact.append(act1);

//    QAction *act2 = new QAction();
//    act2->setText("Delete free");
//    act2->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_D));
//    act2->setStatusTip(tr("Delete free network"));
//    connect(act2, SIGNAL(triggered(bool)), SLOT(delFreeNetwork()));
//    connect(act2, &QAction::triggered, this, &IPList::delFreeNetwork);
//    sact.append(act2);

    QAction *act3 = new QAction();
    act3->setText("Add used");
    act3->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_U));
    act3->setStatusTip(tr("Add user network"));
    connect(act3, SIGNAL(triggered(bool)), SLOT(addUsedNetwork()));
    sact.append(act3);

    QAction *actd = new QAction();
    actd->setText("Delete used");
    actd->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_B));
    actd->setStatusTip(tr("Delete user network"));
    connect(actd, SIGNAL(triggered(bool)), SLOT(delUsedNetwork()));
    sact.append(actd);

    QAction *actf = new QAction();
    actf->setText("Find Free");
    actf->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    actf->setStatusTip(tr("Find free network"));
    connect(actf, SIGNAL(triggered(bool)), SLOT(findFreeNetwork()));
    sact.append(actf);

    QAction *act4 = new QAction();
    act4->setText("Update description");
    act4->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));
    act4->setStatusTip(tr("Update description of network"));
    connect(act4, &QAction::triggered, this, &IPList::updateDescription);
    sact.append(act4);
}

void IPList::expandList()
{
//    qDebug() << "slot expandList()";
    resizeColumnToContents(0);
    expandAll();
}

void IPList::addFreeNetwork()
{
    QSqlQuery q;

    unique_ptr<editFreeNetwork> efn(new editFreeNetwork(this));
    if (efn->exec()) {
        if (efn->Network().length() == 0)
            return;
        IPNetwork net = efn->Network();
        qDebug() << net.toString().c_str();
        QString descr = efn->Description();

        IPData d;
        d.net = net.toString().c_str();
        d.name = descr;
        d.Add();

//        q.prepare("INSERT INTO ipaddr(addr,descr) VALUES(:addr,:descr)");
//        q.bindValue(":addr", net.toString().c_str());
//        q.bindValue(":descr", descr);
//        if (!q.exec()) {
//            qDebug() << q.lastError() << "\n";
//            return;
//        }

        emit updatedata();
    }
}

/*
bool IPList::deleteNetwork(const Node *n)
{
    QSqlQuery q;
    Transaction tr;

    tr.Begin();

    if (n == nullptr)
        return true;

    if (n->data->busy) {
        QMessageBox::critical(this,
                              "Удаление сети",
                              QString("Вы не можете удалить сеть %1,\nпоскольку она занята").arg(n->data->net.toString().c_str()));
        tr.Rollback();
        return false;
    }

    for (auto item: n->children) {
        if (deleteNetwork(item)) {
            tr.Rollback();
            return false;
        }
    }

    if (n->data->Remove() == false) {
        tr.Rollback();
        return false;
    }

    delete n;

    tr.Commit();
    return true;
}
*/

Node *IPList::get_current()
{
    QModelIndex idx = selectionModel()->currentIndex();
    if (!idx.isValid())
        return nullptr;
    Node *n = static_cast<Node*>(idx.internalPointer());
    return n;
}

/*
void IPList::delFreeNetwork()
{
    Transaction tr;

    Node *n = get_current();
    if (n == nullptr)
        return;

    int r = QMessageBox::question(this,
                                  "Удаление сети",
                                  QString("Удаление сети: %1 - %2\nс подсетями?").arg(n->data->net.toString().c_str()).arg(n->data->name.toStdString().c_str()));
    if (r == QMessageBox::Yes) {
        QSqlQuery q;
        tr.Begin();
        if (deleteNetwork(n)) {
            tr.Commit();
            emit updatedata();
        }
        else
            tr.Rollback();
    }
}
*/

pair<IPAddress,IPAddress> IPList::find_network(const Node *n, IPNetwork &myNet)
{
    pair<IPAddress, IPAddress> pr;
    IPNetwork net;

    for (Node *n1: n->children) {
        if (n1->data->net.Contain(myNet)) {
            if (n1->data->busy == false) {
                pr.first = n1->data->net.Network();
                pr.second = n1->data->net.Broadcast();
//                net = n1->data->net;
//                delete_network(n1->data->id);
                n1->data->Remove();
                return pr;
            }
            else {
                QMessageBox::critical(this,
                                      "Сеть занята",
                                      QString("Найденная сеть %1 занята").arg(n1->data->net.toString().c_str()));
                return pr;
            }
        }
    }

    return pr;
}

bool IPList::write_new_networks(const pair<IPAddress,IPAddress> &Address, IPNetwork &myNet, const uint parent, const QString &str)
{
    pair<IPAddress,IPAddress> pr;

    pr.first = Address.first;
    pr.second = myNet.Network()-1;
    if (!calc_networks(pr, parent))
        return false;

    if (!write_network(myNet, parent, str, 1))
        return false;

    pr.first = myNet.Broadcast()+1;
    pr.second = Address.second;
    if (!calc_networks(pr, parent))
        return false;

    return true;
}

void IPList::addUsedNetwork()
{
    uint parent;
    pair<IPAddress,IPAddress> pr;

    Node *n = get_current();
    if (n == nullptr)
        return;

    unique_ptr<editBusyNetwork> efr(new editBusyNetwork(this));

    if (n->data->parent == 0) {
        if (n->children.size() == 0) {
            efr->setNetwork(n->data->net.toString().c_str());
        }
    }
    else if (n->data->busy == true) {
        QMessageBox::critical(this,
                              "Allocate network",
                              "This network is busy");
        return;
    }
    else {
        efr->setNetwork(n->data->net.toString().c_str());
    }

    if (!efr->exec())
        return;

    Transaction tr;
    tr.Begin();

    IPNetwork myNet = efr->Network().toStdString();

    if (n->data->net.Contain(myNet) == false) {
        QMessageBox::critical(this,
                          "Сеть вне диапазона",
                          QString("Введенная сеть %1 вне диапазона %2").arg(myNet.toString().c_str()).arg(n->data->net.toString().c_str()));
        return;
    }

    if (n->data->parent) {
        if (n->data->busy) {
            QMessageBox::critical(this,
                                  "Сеть занята",
                                  QString("Выбранная сеть %1 занята").arg(n->data->net.toString().c_str()));
            return;
        }
        //delete_network(n->data->id);
        n->data->Remove();

        parent = n->parent->data->id;
        pr.first = n->data->net.Network();
        pr.second = n->data->net.Broadcast();
    }
    else {
        parent = n->data->id;
        if (n->children.size() == 0) {
            pr.first = n->data->net.Network();
            pr.second = n->data->net.Broadcast();
        }
        else {
//            IPNetwork net;
            pr = find_network(n, myNet);
//            pr.first = net.Network();
//            pr.second = net.Broadcast();
        }
    }

    if (pr.first.isNull() || pr.second.isNull())
        return;

//    pr.first = firstAddress;
//    pr.second = lastAddress;
    if (!write_new_networks(pr, myNet, parent, efr->Description()))
        return;

    tr.Commit();

    emit updatedata();
}

void IPList::delUsedNetwork()
{
    pair<IPAddress,IPAddress> pr;
//    IPAddress firstAddress, lastAddress;
    Transaction tr;

    Node *n = get_current();
    if (n == nullptr)
        return;

    Node *p = n->parent;

    if (n->data->busy == false) {
        QMessageBox::critical(this,
                          "Удаление сети",
                          QString("Выбранная сеть %1 свободна").arg(n->data->net.toString().c_str()));
        return;
    }

    int r = QMessageBox::question(this,
                                  "Delete network",
                                  QString("Delete network: %1 - %2?").arg(n->data->net.toString().c_str()).arg(n->data->name));
    if (r == QMessageBox::Yes) {
        std::vector<Node*>::iterator cn, idx, l, r;
        tr.Begin();
        pr.first = n->data->net.Network();
        pr.second = n->data->net.Broadcast();
        bool flag = false;

//        for (uint i = 0; i < n->parent->children.size(); ++i) {
        for (cn = n->parent->children.begin(); cn != n->parent->children.end(); ++cn) {
//        for (Node *cn: parent->children) {
            if (*cn == n) {
                idx = cn;
                flag = true;
                break;
            }
        }
        l = idx;
        r = idx;
        if (flag) {

            if (l == p->children.begin()) {
                pr.first = n->data->net.Network();
            }
            else {
                do {
                    --l;
                    Node *tmp = *l;
                    qDebug() << tmp->data->net.Network().toString().c_str();
                    if (tmp->data->busy == true)
                        break;
                    pr.first = tmp->data->net.Network();
//                    if (delete_network(tmp->data->id) == false)
                    if (tmp->data->Remove() == false)
                        return;
                } while (l != p->children.begin());
            }
            IPAddress tmp = pr.first;
            qDebug() << "Left address:" << tmp.toString().c_str();

            if (r == p->children.end()-1) {
                pr.second = n->data->net.Broadcast();
            }
            else {
                ++r;
                while (r != p->children.end()) {
                    Node *tmp = *r;
                    if (tmp->data->busy == true)
                        break;
                    pr.second = tmp->data->net.Broadcast();
//                    if (delete_network(tmp->data->id) == false)
                    if (tmp->data->Remove() == false)
                        return;
                    ++r;
                }
            }
            tmp = pr.second;
            qDebug() << "Last address:" << tmp.toString().c_str();

            qDebug() << "delete_network";
//            if (delete_network(n->data->id) == false)
            if (n->data->Remove() == false)
                return;
            qDebug() << pr.first.toString().c_str() << pr.second.toString().c_str();
            if (calc_networks(pr, n->parent->data->id) == false)
                return;
        }
        tr.Commit();
        emit updatedata();
    }
}

bool IPList::calc_networks(const pair<IPAddress,IPAddress> &Address, const uint parent)
{
    IPAddress faddr = Address.first;
    IPAddress laddr = Address.second;

    while (faddr < Address.second) {
        uint i = 32;
        while (i > 0) {
            IPNetwork net(faddr, i);
            qDebug() << net.Network().toString().c_str() << i << net.Broadcast().toString().c_str() << laddr.toString().c_str();
            if (net.Network() != faddr || net.Broadcast() > Address.second) {
                IPNetwork net2(faddr, i+1);
                if (!write_network(net2, parent, "Free", 0))
                    return false;
                faddr = net2.Broadcast()+1;
                break;
            }
            i--;
        }
        if (i <= 0) {
            break;
        }
    }

    return true;
}

bool IPList::write_network(IPNetwork &net, const uint parent, const QString &descr, const int busy)
{
    IPData d;

    d.name = descr;
    d.parent = parent;
    d.busy = busy;
    d.net = net.toString().c_str();
    return d.Add();

//    QSqlQuery q;

//    qDebug() << "Write network:" << net.toString().c_str();
//    q.prepare("INSERT INTO  ipaddr(addr,parent,descr, busy) VALUES(:addr,:parent,:descr,:busy)");
//    q.bindValue(":addr", net.toString().c_str());
//    q.bindValue(":parent", parent);
//    q.bindValue(":descr", descr);
//    q.bindValue(":busy", busy);
//    if (!q.exec()) {
//        qDebug() << q.lastError() << "\n";
//        return false;
//    }
//    emit updatedata();
//    return true;
}

/*
bool IPList::delete_network(const uint id)
{
    QSqlQuery q;

    q.prepare("DELETE FROM ipaddr WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError() << "\n";
        return false;
    }
    return true;
}
*/

void IPList::findFreeNetwork()
{
    Node *n = get_current();
    if (n == nullptr)
        return;

    unique_ptr<editSizeNetwork> esn(new editSizeNetwork(this));
    if (esn->exec() == false)
        return;

    int bits = esn->Value();
    uint size = 1 << (32-bits);
    uint min = 0;
    Node *mn = nullptr;
    for (auto n1: n->children) {
        if (n1->data->net.size() < size || n1->data->busy == true)
            continue;
        if (min == 0 || min > n1->data->net.size()) {
            min = n1->data->net.size();
            mn = n1;
        }
    }

    if (min == 0) {
        QMessageBox::critical(this,
                          "Поиск сети",
                          QString("Свободная сеть размером %1 бит не найдена").arg(bits));
        return;
    }

    unique_ptr<editBusyNetwork> ebn(new editBusyNetwork(this));
    QString str = QString("%1/%2").arg(mn->data->net.Network().toString().c_str()).arg(bits);
    ebn->setNetwork(str);
    if (!ebn->exec())
        return;

    Transaction tr;
    tr.Begin();

    IPNetwork myNet(str.toStdString());
    pair<IPAddress,IPAddress> la = find_network(n, myNet);
    IPAddress firstAddress = la.first;
    IPAddress lastAddress = la.second;

    if (firstAddress.isNull() || lastAddress.isNull())
        return;

    if (!write_new_networks(la, myNet, n->data->id, ebn->Description()))
        return;

    tr.Commit();
}

void IPList::updateDescription()
{
    Node *n = get_current();
    if (n == nullptr)
        return;

    unique_ptr<editBusyNetwork> net(new editBusyNetwork(this));
    net->hideNetwork();
    net->setNetwork(n->data->net.toString().c_str());
    net->setDescription(n->data->name);

    if (net->exec()) {
        n->data->name = net->Description();
        n->data->Update();
    }

    emit updatedata();
}
