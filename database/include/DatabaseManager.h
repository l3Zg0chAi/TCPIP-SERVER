#ifndef _DATABASE_MANAGER_
#define _DATABASE_MANAGER_

#include "DatabaseConnection.h"

class IDBManager {
public:
    virtual bool initialize() = 0; 
    virtual bool createDatabase() = 0;
    virtual bool useDatabase() = 0;
    virtual bool createTable() = 0;   
    virtual ~IDBManager() = default;
};

class DBManager : public IDBManager {
public:
    DBManager(IDBConnection* dbconnection) : _dbconnection(dbconnection){}
    bool initialize() override;
    bool createDatabase() override;
    bool useDatabase() override;
    bool createTable() override;

private:
    IDBConnection* _dbconnection;
};
#endif _DATABASE_MANAGER_