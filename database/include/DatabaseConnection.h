#ifndef _DATABASE_CONNECTION_
#define _DATABASE_CONNECTION_

#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>

class IDBConnection {
public:
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual sql::Connection* getConnection() const = 0;

    virtual ~IDBConnection() = default;
};

class DBConnection : public IDBConnection{
public:
    static DBConnection* get_instance(
        std::string host, 
        std::string username, 
        std::string passwd, 
        std::string databaseName)
    {
        static DBConnection minstance(host, username, passwd, databaseName);
        return &minstance;
    }

    DBConnection(std::string host, std::string username, std::string passwd, std::string databaseName);
    ~DBConnection();

    bool connect();
    void disconnect();

    bool isConnected();
    sql::Connection* getConnection() const;

private:
    std::string _host;
    std::string _username;
    std::string _passwd;
    std::string _databaseName;

    sql::mysql::MySQL_Driver* _driver;
    std::unique_ptr<sql::Connection> _connection;
};
#endif _DATABASE_CONNECTION_