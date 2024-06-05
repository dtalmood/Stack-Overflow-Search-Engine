// Built in libraries 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <stdlib.h>

// My Libraries 
#include "authentication.hpp"

// 3rd Party Libraries 
#include "json.hpp"

//Curl: 
#include <curl/curl.h>

// libbcrypt: this is a wrapper for Bcrypt encrption algorithm 
#include "bcrypt/BCrypt.hpp"


using namespace std;
namespace bson = bsoncxx;
using bson::builder::basic::kvp;
using bson::builder::basic::make_document;
using bson::v_noabi::document::view;

int authentication::menu()
{
    printLocation("Stack Surfer");
    cout << "1: Login " << endl;
    cout << "2: New Member" << endl;
    cout << "3. Forgot Password" << endl;
    cout << "4. Exit" << endl;
    
    bool invalid = false;
    int userInput;
    do
    {
        cout << "Input: ";
        cin >> userInput;
        
        // Detcts whether the value entered fits the value defined in the variable.
        // if cin.fail is true this means the instream is  broken
        if(cin.fail())
        {
            cout << "Error: Please enter a integer" << endl;
            // repairs input stream
            cin.clear(); 
            // clears the buffer, without this  cin >> userinput will always the latest value we entered 
            cin.ignore(std::numeric_limits<int>::max(),'\n'); 
            invalid = true;
        }
        else if(userInput <= 0 && userInput >= 5) // checks if input is 
        {
            cout << "Error: Please enter a integer between 1 and 4" << endl;
            invalid = true;
        }
        else   
            invalid = false;
        

    }while(invalid);
    
    return userInput;
}   

void authentication::printLocation(string title)
{
    int titleWidth = 40;
    int titleLength = 12;
    int leftPadding = (titleWidth - titleLength) / 2;
    cout << setw(titleWidth) << setfill('=') << "" << endl;
    cout << setw(leftPadding) << setfill(' ') << "" << title << setw(titleLength) << setfill(' ') << "" << endl;
    cout << setw(titleWidth) << setfill('=') << "" << endl;
}

bool authentication::login(mongocxx::database& db)
{
     mongocxx::collection collection = db["Users"];
    system("clear");
    printLocation("Login");
    string username;
    string password;
    
    bool invalidLogin = false;

    cout << "To return to menu type: \"exit\" to" << endl;
    do 
    {
       cout << "Username: ";
       cin >> username;
       if (username == "exit") {
            cout << "Returning to menu..." << endl;
            return false;
        }

        cout << "Password: ";
        cin >> password;
        if(password == "exit") {
            cout << "Returning to menu..." << endl;
            return false;
        }
    
       invalidLogin = findLogin(collection, "username",username,"password",password); 
       if(!invalidLogin)
            cerr << "Username or Password is incorrect: " << endl;

    } while(!invalidLogin);
    
}

void authentication::newMember(mongocxx::database& db) 
{
    system("clear");
    printLocation("New Member");
    string username;
    string email;
    string password;
    
    bool invalidUser = false;
    bool invalidEmail = false;
    cout << "To return to menu type: \"exit\" to" << endl;
    do 
    {
       cout << "Username: ";
       cin >> username;
       if (username == "exit") {
            cout << "Returning to menu..." << endl;
            return;
        }

        cout << "Password: ";
        cin >> password;
        if(password == "exit") {
            cout << "Returning to menu..." << endl;
            return;
        }

        cout << "Email: ";
        cin >> email;
        if (email == "exit") {
            cout << "Returning to menu..." << endl;
            return;
        }
        
       // Check 1: Username has already been taken 
       invalidUser = !checkIfDataExists(db, username, true);
       // Check 2: Email already exists in database 
       invalidEmail = !checkIfDataExists(db, email, false);
    } while(invalidUser || invalidEmail);
    
    string hash = BCrypt::generateHash(password);// this generates our hashed password
    mongocxx::collection collection = db["Users"];
    insertDocument(collection, createDocument({ {"username", username}, {"password", hash}, {"email", email}}));
    
}

void authentication::forgotPassword(mongocxx::database& db)
{
    string email;
    string code;
    bool invalid = true;
    do
    {
       cout << "Enter your email below\nEmail:";
       cin >> email ;
        // TODO: Check if The Email exsits in our data base 
       
        // TODO: Generate a random code 9 digit code       
       
        cout << "Code Sent to Email Address\nCode:" << endl;
        cin  >> code;

        // TODO: Check if the code generated mathces what the user input 
        
        invalid = false;
        
    } while(invalid);

}
bool authentication::checkIfDataExists(const mongocxx::database& db, const string& data, bool type)
{
   
    mongocxx::collection collection = db["Users"];
    if(type) // check if username exists in our collection 
    {
        auto filter = make_document(kvp("username", data));
        auto cursor = collection.find(filter.view());
        // if cursor.begin DOES NOT Equal cursor.end means username was found inside one of the documents 
        if(cursor.begin() != cursor.end())
        {
            cout << "Username is already in use\n" << endl;
            return false;
        }
    }
    else // check if email already exists in our colelction
    {
        auto filter = make_document(kvp("email", data));
        auto cursor = collection.find(filter.view());
        // if cursor Does not Equal End that means email was found inside one of the documents 
        if(cursor.begin() != cursor.end())// did not find username 
        {
            cout << "Email is already in use\n" << endl;
            return false;
        }
        
    }
    return true;
}

bool authentication::findLogin(mongocxx::collection& collection, const string& key1, const string& value1, const string& key2, const string& value2)
{
    // we create a doucment we want to search for 
    auto filter = bsoncxx::builder::stream::document{} << key1 << value1 << bsoncxx::builder::stream::finalize;

     //Checks The DataBase->Collection and sees if there is a document that has the username provided by the user 
    auto cursor = collection.find(filter.view());

    // If our cursour = end that means it did not find anything  
    if(cursor.begin() == cursor.end())
        return false;

    // if we pass if statment above this means we foudn a username that matches in our data base

    // we point to beinging of said document  
    auto doc = *cursor.begin();
    
    // grab second value stored in document which is password 
    auto hashedPassword = doc[key2];

    // now hashed password is in form bsoncxx::types::value we need to convert it to a stirng 
    string hashPasscodeString = hashedPassword.get_utf8().value.to_string();

    auto result =  BCrypt::validatePassword(value2,hashPasscodeString);
    
    if(!result) // if reuslt = 0 (false) this means user did not input correct password 
        return false;

    return true;
}

bsoncxx::document::value authentication::createDocument(const vector<pair<string, string>>& keyValues)
{
	bsoncxx::builder::stream::document document{};
	for (auto& keyValue : keyValues)
	{
		document << keyValue.first << keyValue.second;
	}
	return document << bsoncxx::builder::stream::finalize;
}

// Insert a document into the given collection.
void authentication::insertDocument(mongocxx::collection& collection, const bsoncxx::document::value& document)
{
    collection.insert_one(document.view());
}