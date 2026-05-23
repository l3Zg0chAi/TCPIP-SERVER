#include "DatabaseManager.h"
#include "Logger.h"

bool DBManager::initialize()
{
    DEBUG_LOG ("DB manager initialize");

    bool ret = true;
    ret = createDatabase();
    ret = useDatabase();
    ret = createTable();
    return ret;
}

bool DBManager::createDatabase()
{
    if (_dbconnection == nullptr || _dbconnection->getConnection() == nullptr) return false;

    std::unique_ptr<sql::Statement> stmt(_dbconnection->getConnection()->createStatement());
    std::string query =
        "CREATE DATABASE IF NOT EXISTS " + _dbconnection->getDataBaseName();
    stmt->execute(query);

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