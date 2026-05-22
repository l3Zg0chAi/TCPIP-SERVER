#ifndef APPLICATION_H
#define APPLICATION_H
#include "DatabaseManager.h"

class Application {
public:
    static Application* get_instance(){
        static Application m_instance;
        return &m_instance;
    }

    void init();
    void execute();
    void receive_from_client();
};

#endif // APPLICATION_H