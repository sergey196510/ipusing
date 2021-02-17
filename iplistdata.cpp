#include "iplistdata.h"

IPData::IPData() :
    id(0),
    parent(0),
    busy(false)
{
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
