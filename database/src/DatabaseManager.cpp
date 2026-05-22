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