#include "iplist.h"

IPList::IPList(QWidget *parent) :
    QTreeView(parent)
{
    iplm = new IPListModel(this);
    setModel(iplm);
//    expandList();

    connect(this, SIGNAL(updatedata()), this, SLOT(expandList()));
    connect(this, &IPList::updatedata, iplm, &IPListModel::reloadData);
//    connect(iplm, SIGNAL(loadedData()), SLOT(expandList()));
//    connect(iplm, &IPListModel::loadedData, this, &IPList::expandAll);
//    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(expandList()));
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(expandList()));

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));

    QAction *act1 = new QAction();
    act1->setText("Add free");
    act1->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    act1->setStatusTip(tr("Add free network"));
//    connect(act1, SIGNAL(triggered(bool)), SLOT(addFreeNetwork()));
    connect(act1, &QAction::triggered, this, &IPList::addFreeNetwork);
    sact.append(act1);

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
    qDebug() << "slot expandList()";
    resizeColumnToContents(0);
//    expandAll();
}

void IPList::addFreeNetwork()
{
    QSqlQuery q;

    unique_ptr<editBusyNetwork> efn(new editBusyNetwork(this));
    if (efn->exec()) {
        if (efn->Network().length() == 0)
            return;
        IPNetwork net = efn->Network().toStdString();
        qDebug() << net.toString().c_str();
        QString descr = efn->Description();

        IPData d;
        d.net = net.toString().c_str();
        d.name = descr;
        d.Add();

        emit updatedata();
    }
}

Node *IPList::get_current()
{
//    QModelIndex idx = selectionModel()->currentIndex();
    QList<QModelIndex> idx = selectionModel()->selectedIndexes();
    if (idx.size() == 0 || !idx[0].isValid()) {
	QMessageBox::critical(this, tr("Операция прервана"), tr("Ничего не выбрано"));
        return nullptr;
    }
    Node *n = static_cast<Node*>(idx[0].internalPointer());
    return n;
}

pair<IPAddress,IPAddress> IPList::find_network(const Node *n, IPNetwork &myNet)
{
    pair<IPAddress, IPAddress> pr;
    IPNetwork net;

    for (auto n1: n->children) {
        if (n1->data->net.Contain(myNet)) {
            if (n1->data->busy == false) {
                pr.first = n1->data->net.Network();
                pr.second = n1->data->net.Broadcast();
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

    auto n = get_current();
    if (n == nullptr)
        return;

    unique_ptr<editBusyNetwork> efr(new editBusyNetwork(this));

    if (n->data->parent == 0) {
        if (n->children.size() == 0) {
            efr->setNetwork(n->data->net);
        }
    }
    else if (n->data->busy == true) {
        QMessageBox::critical(this,
                              "Allocate network",
                              "This network is busy");
        return;
    }
    else {
        efr->setNetwork(n->data->net);
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
            pr = find_network(n, myNet);
        }
    }

    if (pr.first.isNull() || pr.second.isNull())
        return;

    if (!write_new_networks(pr, myNet, parent, efr->Description()))
        return;

    tr.Commit();

    emit updatedata();
}

void IPList::delUsedNetwork()
{
    pair<IPAddress,IPAddress> pr;
    Transaction tr;

    auto n = get_current();
    if (n == nullptr)
        return;

    auto p = n->parent;

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

        for (cn = n->parent->children.begin(); cn != n->parent->children.end(); ++cn) {
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
                    if (tmp->data->Remove() == false)
                        return;
                    ++r;
                }
            }
            tmp = pr.second;
            qDebug() << "Last address:" << tmp.toString().c_str();

            qDebug() << "delete_network";
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
}

void IPList::findFreeNetwork()
{
    auto n = get_current();
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
    ebn->hideNetwork();
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
    auto n = get_current();
    if (n == nullptr)
        return;

    unique_ptr<editBusyNetwork> net(new editBusyNetwork(this));
    net->hideNetwork();
    net->setNetwork(n->data->net);
    net->setDescription(n->data->name);

    if (net->exec()) {
        n->data->name = net->Description();
        n->data->Update();
    }

    emit updatedata();
}

void IPList::updateSelection()
{
    qDebug() << "updateSelection";
}
