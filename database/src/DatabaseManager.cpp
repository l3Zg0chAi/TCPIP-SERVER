#include "DatabaseManager.h"
#include "Logger.h"
DBManager::DBManager(IDBConnection *dbconnection)
    : _dbconnection(dbconnection)
{
    DEBUG_LOG("create Data Base Manager");
}

void DBManager::initialize()
{
    if (_dbconnection == nullptr) return;
    if (!_dbconnection->connect()){
        return;
    }

    createDatabase();
    useDatabase();
    createTable();
}

void DBManager::createDatabase()
{
}

void DBManager::useDatabase()
{
}

void DBManager::createTable()
{
}
