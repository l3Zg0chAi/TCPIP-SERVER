#ifndef _DATABASE_MANAGER_
#define _DATABASE_MANAGER_

#include "DatabaseConnection.h"

class IDBManager {
public:
    virtual void createDatabase() = 0;
    virtual void useDatabase() = 0;
    virtual void createTable() = 0;
    virtual void initialize() = 0;    
};

class DBManager : public IDBManager {
public:
    static IDBManager* get_instance(){
        static DBManager minstance;
        return &minstance;
    }

    DBManager(IDBConnection* dbconnection);
    void createDatabase();
    void useDatabase();
    void createTable();
    void initialize();
private:
    IDBConnection* _dbconnection;
};
#endif _DATABASE_MANAGER_