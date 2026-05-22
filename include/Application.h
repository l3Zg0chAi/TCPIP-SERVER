#ifndef APPLICATION_H
#define APPLICATION_H
#include "DatabaseManager.h"

class Application {
public:
    static Application* get_instance(IDBManager* dbmanager){
        static Application m_instance(dbmanager);
        return &m_instance;
    }

    Application(IDBManager* dbmanager): _dbmanager(dbmanager){}
    ~Application(){}

    void init();
    void execute();
    void receive_from_client();
private:
    IDBManager* _dbmanager;
};

#endif // APPLICATION_H