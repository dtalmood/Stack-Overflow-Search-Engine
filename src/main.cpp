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

// holds query Data After reading StackOverFlow API
struct SearchResult 
{
    string title;
    int acceptedAnswerId;
    string creationDate;
    string link;
    int viewCount;
    int reputation;
    int answerCount;
    int acceptanceRate;
    int score;
};

// this string holds which sort the user wants to run 
string sortAlgoithmChoosen = "";

// this will hold user Preferences of filters they do and do not want to see when they are searching for question
map<string, bool> hashMap = {{"Creation Date", false}, {"Views",true},{"Reputatin",false}, 
                                       {"Answer Count",false},{"Acceptance Rate",true},{"Score",false}};

string searchAPI(const string& url);
void printData(vector<SearchResult> data);
void viewMongoDBServer(mongocxx::database& db);
void searchDataInMongoDBServer(mongocxx::database& db);
void findDocument(mongocxx::collection& collection, const string& key, const string& value);
void printCollection(mongocxx::collection& collection);
void printLocation();
void getUserQuestion();
string constructQuestion(string &userQuestion);
void moreOptions();
void addTag();
void removeTag();
void removeAllTags();
void printAllTags();
bool searchTags(string tag);
void updateFilter();
void selectSort(string whichSort);
vector<SearchResult> runSort(vector<SearchResult> &result);

bool sortByViewCount(const SearchResult &a, const SearchResult &b) {
    return a.viewCount > b.viewCount;
}

bool sortByAnswerCount(const SearchResult &a, const SearchResult &b) {
    return a.answerCount > b.answerCount;
}

bool sortByScore(const SearchResult &a, const SearchResult &b) {
    return a.score > b.score;
}

vector<SearchResult> parseSearchResults(string readBuffer);




size_t write_callback(char *ptr, size_t size, size_t nmemb, string *userdata) {
    userdata->append(ptr, size * nmemb);
    return size * nmemb;
}

vector<string> TAGS; // user can only have a maximum of 20 tags per quesiont 

int main() 
{
    system("mongod --config /opt/homebrew/etc/mongod.conf");
    // This establishes a MONOGO DB INSTANCE 
    mongocxx::instance instance{};
    // we state the server we want to connect to 
    mongocxx::uri uri("mongodb://localhost:27017"); 
    // we connect to our server 
    mongocxx::client client(uri);
    // we establish that we want to look at the Database Labled "UserData" 
    mongocxx::database db = client["UserData"]; 
    // We want to look in Collection Users
    std::string collectionName = "Users";
    // we access the Collection
    mongocxx::collection collection = db[collectionName];

    // Insert a dummy document into the collection
    bsoncxx::builder::stream::document document{};
    document << "dummy_field" << "dummy_value";
    collection.insert_one(document.view());

    std::cout << "Database and collection are ready." << std::endl;

    // Remove the dummy document
    collection.delete_one(document.view());

    // now if the DB does not exist and and Colelction does not exist it we 
    // created a dummy variable so it connects to it
    
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
    system("brew services stop mongodb/brew/mongodb-community");
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


string searchAPI(const string& url)
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
        } 
        // else 
        // {
        //     printData(readBuffer);
        // }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

string constructQuestion(string &userQuestion) 
{
    string baseURL = "https://api.stackexchange.com/2.3/search/advanced?";
    string encodedQuery = curl_easy_escape(nullptr, userQuestion.c_str(), userQuestion.length());
    baseURL += "order=desc&sort=relevance&q=" + encodedQuery;

    if(!TAGS.empty())
    {
        baseURL += "&tagged=";
        for(int i = 0; i < TAGS.size(); i++)
        {
            baseURL += TAGS[i];
        }
        baseURL += ";";
    }    

    baseURL += "&site=stackoverflow";
    string key = "OUgS5vV1jD7kdtN8*nYZKg((";  
    baseURL += "&key=" + key;
    //cout << "Constructed String: " << baseURL << endl;
    return baseURL;
}

void printData(vector<SearchResult> data) 
{
    cout << "\n";
    for (const auto& item : data) 
    {
        cout << "Title: " << item.title << endl;
        //cout << "Accepted Answer ID: " << item.acceptedAnswerId << endl;
        
        if(hashMap["Views"])
            cout << "View Count: " << item.viewCount << endl;
        
        if(hashMap["Creation Date"])
             cout << "Creation Date: " << item.creationDate << endl;

        cout << "Link: " << item.link << endl;
        
        if(hashMap["Reputation"])
        {
            if(item.reputation == -1)
                cout << "Reputation: N/A" << endl;
            else 
                cout << "Reputation: " << item.reputation << endl;
        }
       
        if(hashMap["Acceptance Rate"])
        {
            if(item.acceptanceRate == -1)
                cout << "Acceptance Rate: N/A " << endl;
            else
                cout << "Acceptance Rate: " << item.acceptanceRate << endl;
        }
        
        if(hashMap["Answer Count"])
            cout << "Answer Count: " << item.answerCount << endl;

        if(hashMap["Score"]) 
        {
            if(item.score == -1)
                cout << "Score: N/A" << endl;
            
            else    
                cout << "Score: " << item.score << endl;
        }
          
        cout << "\n"; 
    }
}




// this prints the data Normally
// void printData(string readBuffer) 
// {
//     cout << "\n"; 
//     auto json = nlohmann::json::parse(readBuffer);
//     for (const auto& item : json["items"]) 
//     {
        
//         cout << "Title: " << item["title"] << endl;
       
//         // Accented Answer ID can potentially have nothing 
//         if (item.find("accepted_answer_id") != item.end()) 
//             cout << "Accepted Answer_id: " << item["accepted_answer_id"] << endl;
//         else   
//             cout << "N/A" << endl;
//         cout << "View Count: " << item["view_count"] << endl;
//         cout << "Creation Date: " << item["creation_date"] << endl;
//         cout << "Link: " << item["link"] << endl;
       
//        // reputaiotn can potentialy have nothing
//         if (item.find("reputation") != item.end()) 
//             cout << "Reputation: " << item["reputation"] << endl;
//         else 
//             cout << "N/A" << endl;
        
//         // Acceptance Rate Can potentialy have nothing 
//         if (item.find("accept_rate") != item.end()) 
//             cout << "Acceptance Rate: " << item["accept_rate"] << endl;
//         else    
//             cout << "N/A" << endl;

//         cout << "Answer Count: " << item["answer_count"] << endl;
//         cout << "Score: " << item["score"] << endl;
//         cout << "\n"; 
//     }
// }

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
            cout << "Type your question below, Enter \"Options\" to add tags to your question or to add/remove search filters" << endl;
            cout << "Question: ";
            getline(cin, userQuestion);
            if(userQuestion == "options" || userQuestion == "Options" ) // User Typed Tag 
                moreOptions();

            else    // user typed anything but tag
                tagCheck = true;
        }
       
             

        if (userQuestion.empty()) {
            cout << "No input detected. Exiting search." << endl;
            done = true;
            continue;
        }

        string constructedString = constructQuestion(userQuestion);
        string readBuffer = searchAPI(constructedString);
        vector<SearchResult> result = parseSearchResults(readBuffer);
        
        if(sortAlgoithmChoosen == "") // no sort algorithm was just print the data as is  
            printData(result);

        else
        {
            vector<SearchResult> sortedResult = runSort(result); // here we call function and see which sort to use 
            printData(sortedResult);
        }
        cout << "Type 'done' to exit, hit Enter to search another question or type filters to view filter options" << endl;
        string check;
        getline(cin, check);

        if (check == "done") {
            done = true;
        }

    } while (!done);
}

void moreOptions()
{
    bool done = false;

    while(!done)
    {
        system("clear");
        printLocation("Options");
        cout << "Current Sort: " << sortAlgoithmChoosen  << endl;
        cout << "Enter Below what you would like to do:" << endl;
        cout << "1. Add new Tag " << endl;
        cout << "2. Print all Tags "<< endl;
        cout << "3. Remove Tag" << endl;
        cout << "4. Remove all Tags" << endl;
        cout << "5. Update Search Filter " << endl;
        // WORK FROM HERE
        cout << "6. Sort by Views" << endl;
        cout << "7. Sort by Answer Count" << endl;
        cout << "8. Sort by Score" << endl;
        cout << "9. Go back" << endl;
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
                updateFilter();
                break;

            case 6:
                selectSort("Views"); //  Views 
                break;

            case 7:
                selectSort("Answers"); // Answer Count
                break;
            
            case 8: 
                selectSort("Score"); // Score
                break;
            
            case 9: 
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

vector<SearchResult> parseSearchResults(string readBuffer) {
    vector<SearchResult> results;

    auto json = nlohmann::json::parse(readBuffer);
    for (const auto& item : json["items"]) { // Ensure to access the correct part of the JSON
        SearchResult current;
        current.title = item["title"].get<string>();

        //  .get<int>()  ensures  type is correct and handle missing keys appropriately
        current.acceptedAnswerId = item.value("accepted_answer_id", -1);
        current.viewCount = item["view_count"].get<int>();
        current.creationDate = to_string(item["creation_date"].get<int>());
        current.link = item["link"].get<string>();
        current.reputation = item.value("reputation", -1);
        current.acceptanceRate = item.value("accept_rate", -1);
        current.answerCount = item["answer_count"].get<int>();
        current.score = item["score"].get<int>();

        results.push_back(current);
    }

    return results;
}

// 
void updateFilter() 
{
    system("clear");
    
    bool done = false;
    do {
        printLocation("Filters");
        cout << "Available Filters: " << endl;
        int i = 0;
        for(const auto &items: hashMap) 
        {
            if(items.second) 
                cout << i << ": " << items.first << ", On" << endl; // show the data 
            else    
                cout << i << ": " << items.first << ", Off" << endl; // hid ethe data 
            i++;
        }
        cout << "Enter the number associated with items you want filtered when searching, Enter nothing to return to previous menu" << endl;
        string input; 
        getline(cin, input);
        if(input.empty()) { // user input nothing, go back 
            done = true; 
        } else if(stoi(input) <= 0 || stoi(input) >= 8) { // check if input is a valid number
            cerr << "Error: Invalid Input. Please enter a value between 1 and 6." << endl;
        } else {
            int idx = stoi(input) - 1;
            auto itter = hashMap.begin();
            advance(itter, idx);
            itter->second = !(itter->second);
            cout << "Filter " << itter->first << " toggled " << (itter->second ? "on" : "off") << endl;
        }
        cout << "Hit Enter to Apply next filter" << endl;
        cin.ignore(); // Clear input buffer
    } while (!done);  
}


void selectSort(string whichSort)
{
    if(sortAlgoithmChoosen == "")// if empty this means user has not choosen an sort yet 
        sortAlgoithmChoosen = whichSort;
    
    else if(sortAlgoithmChoosen ==  whichSort)// if they are same that means we delect a sorting method
        sortAlgoithmChoosen = "";
    
    else // if not empty and not the same then we pick our new sorting method
        sortAlgoithmChoosen = whichSort;



}

vector<SearchResult> runSort(vector<SearchResult> &result)
{
    if(sortAlgoithmChoosen == "Views")
        sort(result.begin(), result.end(), sortByViewCount);
    
    else if(sortAlgoithmChoosen == "Answers")
        sort(result.begin(), result.end(), sortByAnswerCount);

    else
        sort(result.begin(), result.end(), sortByScore);

    return result;
}