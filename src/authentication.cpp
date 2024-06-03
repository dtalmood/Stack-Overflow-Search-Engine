

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


int authentication::menu()
{
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


void authentication::login(mongocxx::database& db)
{
    // Look insdie of colleciton users insode of Dur data base which has all our documents
    mongocxx::collection collection = db["Users"]; 
    string username;
    string password;
    bool invalid = true;
    do
    {
       cout << "Username: ";
       cin >> username;
       cout << "Password: ";
       cin >> password;
       invalid = false;
        // check our centeral database if username exists 
    
        
    } while(invalid);
    
}

void authentication::logout()
{

}

void authentication::newMember(mongocxx::database& db)
{

    string username;
    string password;
    string email;
    bool invalid = true;
    do
    {
       cout << "Username: ";
       cin >> username;
       cout << "Password: ";
       cin >> password;
       cout << "Email: ";
       cin >> email;
       invalid = false;
        // Check 1: Check is Username has already been taken 
        
        
        
    } while(invalid);
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
void authentication::encryptPassword()
{
    
}

