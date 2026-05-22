#include "DatabaseConnection.h"
#include "Logger.h"

DBConnection::DBConnection(std::string host, std::string username, std::string passwd, std::string databaseName)
    : _host(host),
      _username(username),
      _passwd(passwd),
      _databaseName(databaseName),
      _driver(nullptr),
      _connection(nullptr)
{
    DEBUG_LOG("create DB Connection to");
}

DBConnection::~DBConnection(){
    disconnect();
}
bool DBConnection::connect()
{
    try {
        _driver = sql::mysql::get_mysql_driver_instance();

        _connection.reset(
            _driver->connect(_host, _username, _passwd)
        );

        if (!_connection) {
            DEBUG_LOG("[Database] Failed to create connection.");
            return false;
        }

        DEBUG_LOG("[Database] Connected successfully.");
        return true;
    }
    catch (sql::SQLException& e) {
        DEBUG_LOG("[Database] Connection error: %s", e.what());
        DEBUG_LOG("[Database] Error code: %d", e.getErrorCode());
        DEBUG_LOG("[Database] SQL state: %s", e.getSQLState());
        return false;
    }
}

void DBConnection::disconnect()
{
    if (_connection) {
        try {
            if (!_connection->isClosed()) {
                _connection->close();
            }
        }
        catch (sql::SQLException& e) {
            DEBUG_LOG("[Database] Disconnected error: %s", e.what());
        }

        _connection.reset();
        DEBUG_LOG("[Database] Disconnected");
    }
}

bool DBConnection::isConnected()
{
    try {
        return _connection && !_connection->isClosed();
    }
    catch (sql::SQLException&) {
        return false;
    }
}

sql::Connection* DBConnection::getConnection() const {
    return _connection.get();
}
