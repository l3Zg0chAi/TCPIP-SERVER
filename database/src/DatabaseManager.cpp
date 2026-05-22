#include "DatabaseManager.h"
#include "Logger.h"
DBManager::DBManager(IDBConnection *dbconnection)
    : _dbconnection(dbconnection)
{
    DEBUG_LOG("create Data Base Manager");
}

bool DBManager::initialize()
{
    if (_dbconnection == nullptr) return false;
    if (!_dbconnection->connect()){
        return false;
    }

    bool ret = true;
    ret = createDatabase();
    ret = useDatabase();
    ret = createTable();
    return ret;
}

bool DBManager::createDatabase()
{
    return true;
}

bool DBManager::useDatabase()
{
    return true;
}

bool DBManager::createTable()
{
    return true;
}
