#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include <string>
#include <iostream>
#include <authentication.hpp>
// Mongocxx: 
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

// BSONCXX: Additional includes for BSON
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
using namespace std;

class authentication 
{
    public: 
    static int menu();
    static void login(mongocxx::database& db);
    static void logout();
    static void newMember(mongocxx::database& db);
    static void forgotPassword(mongocxx::database& db);
    static void encryptPassword();
    static bool checkIfDataExists(const mongocxx::database& db, const string& data, bool type); 
};



#endif 