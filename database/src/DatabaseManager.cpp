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

    query =
        "USE " + _dbconnection->getDataBaseName();
    stmt->execute(query);

    query = 
        "CREATE TABLE IF NOT EXISTS PACKET_TABLE ("
        "id INT AUTO_INCREMENT PRIMARY KEY,"
        "PDUID INT UNSIGNED NOT NULL,"
        "PAYLOAD_LENGTH INT UNSIGNED NOT NULL,"
        "PAYLOAD_DATA BLOB NOT NULL"
        ");";
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