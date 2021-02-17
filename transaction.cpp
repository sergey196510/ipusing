#include "transaction.h"

int Transaction::level = 0;

Transaction::Transaction() :
    status(false)
{
    level++;
}

Transaction::~Transaction()
{
    if (level == 1 && status == true) {
        Rollback();
        status = false;
    }
    level--;
}

void Transaction::Begin()
{
    QSqlQuery q;

    if (level == 1) {
        q.exec("BEGIN");
        status = true;
    }
}

void Transaction::Commit()
{
    QSqlQuery q;

    if (level == 1 && status == true) {
        q.exec("COMMIT");
        status = false;
    }
}

void Transaction::Rollback()
{
    QSqlQuery q;

    if (level == 1 && status == true) {
        q.exec("ROLLBACK");
        status = false;
    }
}
