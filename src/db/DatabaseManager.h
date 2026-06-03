#pragma once

#include <QSqlDatabase>

class DatabaseManager
{
public:
    bool open();
    bool initializeSchema();

private:
    QSqlDatabase m_database;
};