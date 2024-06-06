

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
void getUserQuestion();
string constructQuestion(string &userQuestion);
void tag();
void addTag();
void removeTag();
void removeAllTags();
void printAllTags();
bool searchTags(string tag);

size_t write_callback(char *ptr, size_t size, size_t nmemb, string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

vector<string> TAGS; // user can only have a maximum of 20 tags per quesiont 

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

    getUserQuestion();
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

void getUserQuestion()
{
    bool done = false;
    
    // Before: I had this in my loop and this caused issues
    // 1.  
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    do
    {
        string userQuestion;
        bool tagCheck = false;
        while(!tagCheck)
        {
            printLocation("Stack Surfer");
            cout << "Type your question below or Enter \"tag\" to add tags to your question" << endl;
            cout << "Question: ";
            getline(cin, userQuestion);
            if(userQuestion == "tag") // User Typed Tag 
                tag();
            else    // user typed anything but tag
                tagCheck = true;
        }
       
             

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

void tag()
{
    bool done = false;

    while(!done)
    {
        system("clear");
        printLocation("Tag");
        cout << "Enter Below what you would like to do" << endl;
        cout << "1. Add new Tag " << endl;
        cout << "2. Print all Tags "<< endl;
        cout << "3. Remove Tag" << endl;
        cout << "4. Remove all Tags" << endl;
        cout << "5. Go back" << endl;
        string userChoice = "0";
        cout << "Enter: ";
       
        // Stoi converts string to Integer, (Static_cast does not proerlyl convert string to an integer)
        int result;
        try
        {
            getline(cin,userChoice);
            if(userChoice.empty())
            return;
            result = stoi(userChoice); 
            if(result <= 0 || result >= 5)
                throw(result);
        }catch(int result)
        {
            cerr << "Error: " << result << "is our of range, Please input a value between 1 and 4 " << endl;
        }
        
        switch(result)   
        {
            case 1: 
                addTag();
                break;
            
            case 2:
                printAllTags();
                break;
            
            case 3:
                removeTag();
                break;
            
            case 4:
                removeAllTags();
                break;
            
            case 5:
                done = true;
                break;
        }
    }
    
}

void addTag()
{
    system("clear");
    printLocation("Add Tag");
    bool done = false;
    string current = "";
    cout << "Type and press enter to add tag Below" << endl;
    cout << "Note: Upper/Lower case matters" << endl;
    do
    {
        cout << "Tag: ";
        getline(cin, current); 
        if(current.empty()) // if empty this means that user input nothing so dont do anything 
            done = true;
        else
        {
            bool result = searchTags(current);
            if(result)
                cout << "You already have this tag" << endl;
            
            else
                TAGS.push_back(current);
        }
        
    } while (!done);
    
}
void printAllTags()
{
    system("clear");
    printLocation("All Tags");
    cout << "Note: Adding to many tag may search to specific and no results may be found" << endl;
    for(size_t i = 0; i < TAGS.size(); i++)
    {
        cout << "Tag " << i + 1 << ": " << TAGS[i] << endl;
    }
    cout << "Press enter to go back" << endl;
    bool done = false;
    do
    {
        string goBack ="";
        getline(cin,goBack);
        if (goBack.empty())
            done = true;
        
    } while (!done);
}
void removeTag()
{
    system("clear");
    printLocation("Remove Tag");
    cout << "Type tag you would like to remove or enter nothing to go back" << endl;
    bool done;
    do
    {
        done = false;
        string remove = "";
        cout << "Tag: ";
        getline(cin,remove);
        if(remove.empty())// user did not want to remove a tag
            done = true;
        
        for(int i = 0; i < TAGS.size(); i++)
        {
            if(TAGS[i] == remove)// we found tag user wants to remove  
            {
                cout << "Deleted Tag: " << remove << endl;
                TAGS.erase(TAGS.begin()+(i));
            }
        }
    } while (!done);
    

}

void removeAllTags()
{
    system("clear");
    printLocation("Remove All Tags");
    cout << "Are you sure you want remove all Tag? " << endl;
    cout << "Input \"yes\" or enter nothing to go back" << endl;
    bool done = false;
    do
    {
        string goBack ="";
        getline(cin,goBack);
        if (goBack.empty()) // user does not want to remove all tags
            done = true;
        else if(goBack == "yes" || goBack == "Yes")
        {
            TAGS.clear();
            done = true;
        }  
        else
            cout << "Error: Invalid Input" << endl;
        
    } while (!done);

}

bool searchTags(string tag)
{
    for(int i = 0; i < TAGS.size(); i++)
    {
        if(TAGS[i] == tag)
            return true;
    }
    return false;
    
}