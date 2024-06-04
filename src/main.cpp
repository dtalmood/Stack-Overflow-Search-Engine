

// Built in libraries 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

// My Libraries 
#include "authentication.hpp"

// 3rd Party Libraries 
#include "json.hpp"

//Curl: 
#include <curl/curl.h>

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
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void search(const string& url);
void printData(string readBuffer);
void viewMongoDBServer(mongocxx::database& db);
void searchDataInMongoDBServer(mongocxx::database& db);
void findDocument(mongocxx::collection& collection, const string& key, const string& value);
void printCollection(mongocxx::collection& collection);
void printLocation();
void searchEngine();
string constructQuestion(string &userQuestion);

size_t write_callback(char *ptr, size_t size, size_t nmemb, string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

int main() 
{
    // This establishes a MONOGO DB INSTANCE 
    mongocxx::instance instance{};
    // we state the server we want to connect to 
    mongocxx::uri uri("mongodb://localhost:27017"); 
    // we connect to our server 
    mongocxx::client client(uri);
    // we establish that we want to look at the Database Labled "UserData" 
    mongocxx::database db = client["UserData"]; 
    
    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_DEFAULT);

    bool done = false;
    while(!done)
    {
         system("clear");
         int result = authentication::menu();
         switch(result)
        {
            case 1:
                done = authentication::login(db);
                break;
            
            case 2:
                authentication::newMember(db);
                break;

            case 3:
                authentication::forgotPassword(db);
                break;
            
            case 4: 
                cout << "Good bye" << endl;
                exit(0);
                break;

        }
    }

    searchEngine();
    curl_global_cleanup();
    return 0;
}

void viewMongoDBServer(mongocxx::database& db) 
{
    cout << "Check Mongo Connection" << endl;
    mongocxx::collection collection = db["Users"]; // we look at collection "Users"
    printCollection(collection);
}

void searchDataInMongoDBServer(mongocxx::database& db) 
{
    cout << "Search MongoDB Connection" << endl;
    mongocxx::collection collection = db["Users"];
    findDocument(collection, "username", "dtalmood");
}


void search(const string& url)
{
    CURL *curl;
    CURLcode res;
    string readBuffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            printData(readBuffer);
        }
        curl_easy_cleanup(curl);
    }
}

string constructQuestion(string &userQuestion) 
{
    string baseURL = "https://api.stackexchange.com/2.3/search/advanced?";
    string encodedQuery = curl_easy_escape(nullptr, userQuestion.c_str(), userQuestion.length());
    baseURL += "order=desc&sort=relevance&q=" + encodedQuery;
    baseURL += "&site=stackoverflow";
    string key = "OUgS5vV1jD7kdtN8*nYZKg((";
    baseURL += "&key=" + key;
    cout << "Constructed String: " << baseURL << endl;
    return baseURL;
}

void printData(string readBuffer) 
{
    auto json = nlohmann::json::parse(readBuffer);
    for (const auto& item : json["items"]) {
        cout << "Title: " << item["title"] << endl;
        cout << "Creation Date: " << item["creation_date"] << endl;
    }
}

void printCollection(mongocxx::collection& collection) 
{
    if (collection.count_documents({}) == 0) {
        cout << "Collection is empty." << endl;
        return;
    }
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
}

void findDocument(mongocxx::collection& collection, const string& key, const string& value) 
{
    auto filter = make_document(kvp(key, value));
    auto cursor = collection.find(filter.view());
    for (auto&& doc : cursor) {
        cout << bsoncxx::to_json(doc) << endl;
    }
}

void printLocation(string title)
{
    system("clear");
    int titleWidth = 40;
    int titleLength = 12;
    int leftPadding = (titleWidth - titleLength) / 2;
    cout << setw(titleWidth) << setfill('=') << "" << endl;
    cout << setw(leftPadding) << setfill(' ') << "" << title << setw(titleLength) << setfill(' ') << "" << endl;
    cout << setw(titleWidth) << setfill('=') << "" << endl;
}

void searchEngine()
{
    bool done = false;
    // Before: I had this in my loop and this caused issues
    // 1.  
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    do
    {
        printLocation("Stack Surfer");
        cout << "Type your question and once done press enter to search." << endl;
        
        cout << "Question: ";
        string userQuestion;
        getline(cin, userQuestion);

        if (userQuestion.empty()) {
            cout << "No input detected. Exiting search." << endl;
            done = true;
            continue;
        }

        string constructedString = constructQuestion(userQuestion);
        search(constructedString);

        cout << "Type 'done' to exit or press Enter to search for another question." << endl;
        string check;
        getline(cin, check);

        if (check == "done") {
            done = true;
        }

    } while (!done);
}
