#ifndef _DATABASE_CONNECTION_
#define _DATABASE_CONNECTION_

#include <string>
#include <memory>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class IDBConnection {
public:
    virtual bool initialize(std::string host, std::string username, std::string passwd, std::string databaseName) = 0; 
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual sql::Connection* getConnection() const = 0;
    virtual std::string getDataBaseName() const = 0;
    virtual ~IDBConnection() = default;
};

class DBConnection : public IDBConnection{
public:
    DBConnection();
    ~DBConnection();

    virtual bool initialize(std::string host, std::string username, std::string passwd, std::string databaseName) override; 
    bool connect() override;
    void disconnect() override;

    bool isConnected() override;
    sql::Connection* getConnection() const override;
    virtual std::string getDataBaseName() const override;

private:
    std::string _host;
    std::string _username;
    std::string _passwd;
    std::string _databaseName;

    sql::mysql::MySQL_Driver* _driver;
    std::unique_ptr<sql::Connection> _connection;
};
#endif _DATABASE_CONNECTION_