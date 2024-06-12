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
    static void printLocation(string title);
    static bool login(mongocxx::database& db);
    static void newMember(mongocxx::database& db);
    static void forgotPassword(mongocxx::database& db);
    static bool checkIfDataExists(const mongocxx::database& db, const string& data, bool type); 
    static bool findLogin(mongocxx::collection& collection, const string& key1, const string& value1, const string& key2, const string& value2);
    static bsoncxx::document::value createDocument(const vector<pair<string, string>>& keyValues);
    static void insertDocument(mongocxx::collection& collection, const bsoncxx::document::value& document);
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
    static void sendEmail(const string &to, const string &code);
    static string getPasswordLocal();
};

#endif 