#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <string>
using namespace std;

class authenticator 
{
    private:
    string username;
    string password;

    public: 
    void login();
    void logout();
    void menu();
    void newMember();
};



#endif 