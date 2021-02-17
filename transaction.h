#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QtSql>

class Transaction
{
    static int level;
    bool status;

public:
    Transaction();
    ~Transaction();
    void Begin();
    void Commit();
    void Rollback();
};

#endif // TRANSACTION_H
