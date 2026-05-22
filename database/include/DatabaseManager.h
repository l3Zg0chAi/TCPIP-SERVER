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
    static DBManager* get_instance(IDBConnection* dbconnection){
        static DBManager minstance(dbconnection);
        return &minstance;
    }

    DBManager(IDBConnection* dbconnection);
    bool createDatabase();
    bool useDatabase();
    bool createTable();
    bool initialize();
private:
    IDBConnection* _dbconnection;
};
#endif _DATABASE_MANAGER_