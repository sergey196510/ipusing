#include "iplistdata.h"

IPData::IPData() :
    id(0),
    parent(0),
    busy(false)
{
}

bool IPData::Add()
{
    QSqlQuery q;

    q.prepare("INSERT INTO ipaddr(descr,parent,busy,addr) VALUES(:name,:parent,:busy,:addr)");
    q.bindValue(":name", name);
    q.bindValue(":parent", parent);
    q.bindValue(":busy", busy);
    q.bindValue(":addr", net.toString().c_str());
    if (!q.exec()) {
        qDebug() << "insert network:" << q.lastError();
        return false;
    }

    return true;
}

bool IPData::Get()
{
    QSqlQuery q;

    if (id == 0)
        return true;

    q.prepare("SELECT parent,busy,addr,name FROM ipaddr WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    if (q.next()) {
        parent = q.value(0).toInt();
        busy = q.value(1).toInt();
        net = q.value(2).toString().toStdString();
        name = q.value(3).toString();
    }

    return true;
}

bool IPData::Remove()
{
    QSqlQuery q;

    if (id == 0)
        return false;

    qDebug() << "Remove network:" << net.toString().c_str();

    q.prepare("DELETE FROM ipaddr WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }

    return true;
}

bool IPData::Update()
{
    QSqlQuery q;

    if (id == 0)
        return false;

    q.prepare("UPDATE ipaddr SET descr = :name, parent = :parent, busy = :busy, addr = :addr WHERE id = :id");
    q.bindValue(":id", id);
    q.bindValue(":name", name);
    q.bindValue(":parent", parent);
    q.bindValue(":busy", busy);
    q.bindValue(":addr", net.toString().c_str());
    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }

    return true;
}

Node::Node() :
    row(0),
    data(nullptr),
    parent(nullptr)
{
    children.clear();
}

void Node::clear()
{

}
