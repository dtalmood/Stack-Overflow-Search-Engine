#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <string>
using namespace std;

class authentication 
{
    public: 
    static int menu();
    static void login();
    static void logout();
    static void newMember();
    static void forgotPassword();
    static void encryptPassword();
};



#endif 