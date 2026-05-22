#ifndef _DATABASE_MANAGER_
#define _DATABASE_MANAGER_

#include "DatabaseConnection.h"

class IDBManager {
public:
    virtual bool initialize() = 0; 
    virtual bool createDatabase() = 0;
    virtual bool useDatabase() = 0;
    virtual bool createTable() = 0;   
    virtual bool setDBConnection(IDBConnection* dbconnection){
        if (_dbconnection != dbconnection){
            _dbconnection = dbconnection;
        }
    }
    virtual ~IDBManager() = default;

protected:
    IDBConnection* _dbconnection;
};

class DBManager : public IDBManager {
public:
    bool initialize() override;
    bool createDatabase() override;
    bool useDatabase() override;
    bool createTable() override;
};
#endif _DATABASE_MANAGER_