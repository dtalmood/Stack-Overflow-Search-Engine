// Built in libraries 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <stdio.h>      /* printf, NULL */
#include <fstream>
#include <ctime>
#include <time.h>

// My Libraries 
#include "authentication.hpp"

// 3rd Party Libraries 
#include "json.hpp"

//Curl: 
#include <curl/curl.h>

// libbcrypt: this is a wrapper for Bcrypt encrption algorithm 
#include "bcrypt/BCrypt.hpp"

// export MY_VARIABLE = "olqwtcairvkkpdld"
using namespace std;
namespace bson = bsoncxx;
using bson::builder::basic::kvp;
using bson::builder::basic::make_document;
using bson::v_noabi::document::view;


//SMTP SETUP 
const string SMTP_SERVER = "smtp://smtp.gmail.com:587";
const string USERNAME = "dtalmood1@gmail.com";
const string FROM = "dtalmood1@gmail.com";

struct upload_status {
    int lines_read;
    vector<string> payload;
};


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

    cout << "To return to menu type nothing to return" << endl;
    do 
    {
       cout << "Username: ";
       cin.ignore(numeric_limits<streamsize>::max(),'\n');
       getline(cin,username);
       if(username.empty()) 
       {
            
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
    cout << "To return to menu enter nothing" << endl;
    do 
    {
      
       cout << "Username: ";
       cin.ignore(numeric_limits<streamsize>::max(),'\n');
       getline(cin,username);
       if (username.empty())
       {     
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
       if(invalidUser)
            cout << "Username is already in use\n" << endl;
            
       // Check 2: Email already exists in database 
       invalidEmail = !checkIfDataExists(db, email, false);
       if(invalidEmail)
            cout << "Email is already in use\n" << endl;

    } while(invalidUser || invalidEmail);
    
    string hash = BCrypt::generateHash(password);// this generates our hashed password
    mongocxx::collection collection = db["Users"];
    insertDocument(collection, createDocument({ {"username", username}, {"password", hash}, {"email", email}}));
    
}

void authentication::forgotPassword(mongocxx::database& db)
{
    string email;
    int code;
    int userInput;
    string password1;
    string password2;
    bool invalid = true;
    do
    {
       system("clear");
       printLocation("Forgot Password");
       cout << "Enter your email below, or enter nothing to return to Previous menu.\nEmail:";
       cin.ignore(numeric_limits<streamsize>::max(),'\n');
       getline(cin,email);
       
       // if empty return to menu
       if(email.empty())
            return;;

        // now ensure that the email provided is in our databse
        bool result = checkIfDataExists(db, email, false);
        if(result)
        {
            cout << "Email not found: \nPlease check the email and try again\nPress enter to return to previous menu" << endl;
            cin.ignore(std::numeric_limits<int>::max(),'\n'); 
            while(!email.empty())
                getline(cin,email);
            
            return;;
        }
        
        code  = randomCodeGenerator();
        sendEmail(email,code);

        cout << "Code Sent to Email Address, Please Enter Code Below\nCode:";
        cin  >> userInput;
        
        cin.ignore(std::numeric_limits<int>::max(),'\n'); 
        if(userInput == code)
        {
            cout << "Code Match!\nPlease Enter new Password below" << endl;
            while(true)
            {
                cout << "Password:";
                
                getline(cin,password1);
                cout << "Input Password Again:";
                getline(cin,password2);
                if(password1 == password2)
                {
                    updatePassword(db, email, password1);
                    cout << "Password has reset!\nPress enter to return" << endl;
                    cin.ignore(std::numeric_limits<int>::max(),'\n'); 
                    while(!email.empty())
                        getline(cin,email);
                    return;;
                }
                    
                else
                {
                    cout << "Password does not match, please re-enter password below" << endl;
                }
            }
            
        }
        else
        {
            cout << "Code Does not Match" << endl;
            invalid = true;
        }
        
        
        
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
            // username was found in our database
            return false;
        }
    }
    else // check if email already exists in our colelction
    {
        auto filter = make_document(kvp("email", data));
        auto cursor = collection.find(filter.view());
        // if cursor Does not Equal End that means email was found inside one of the documents 
        if(cursor.begin() != cursor.end()) 
        {
            // Email was found in our data base
            return false;
        }
        
    }

    // if it reaches here it means email or username was not found in our database  
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

void authentication::updatePassword(mongocxx::database& db, string email, string newPassword)
{
    mongocxx::collection collection = db["Users"]; // we collection in our data base that has user data 
    string hashPassword = BCrypt::generateHash(newPassword);// this generates our hashed password

    collection.update_one(bsoncxx::builder::stream::document{} << "email" << email << bsoncxx::builder::stream::finalize,
		bsoncxx::builder::stream::document{} << "$set" << bsoncxx::builder::stream::open_document << "password" << hashPassword << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
}


size_t authentication::payload_source(void *ptr, size_t size, size_t nmemb, void *userp) 
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    if (upload_ctx->lines_read < upload_ctx->payload.size()) {
        data = upload_ctx->payload[upload_ctx->lines_read].c_str();
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;
        return len;
    }

    return 0;
}

void authentication::sendEmail(const string &to, const int &code) 
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };
    string codeString = to_string(code);

    // Compose email payload
    upload_ctx.payload.push_back("Date: Mon, 29 Nov 2021 21:54:29 +0000\r\n");
    upload_ctx.payload.push_back("To: " + to + "\r\n");
    upload_ctx.payload.push_back("From: " + FROM + " (Your Name)\r\n");
    upload_ctx.payload.push_back("Subject: Password Reset Code\r\n");
    upload_ctx.payload.push_back("\r\n"); // Empty line to divide headers from body
    upload_ctx.payload.push_back("Your password reset code is: " + codeString + "\r\n");
    
    const string PASSWORD = getPasswordLocal();

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, SMTP_SERVER.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM.c_str());
        recipients = curl_slist_append(recipients, to.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
         {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}
string authentication::getPasswordLocal()
{
    string passcode;
    fstream new_file;
    
    // We put the pascode in bashRC file so its local to us and no one else can see it
    string filePath = getenv("HOME");
    filePath += "/.bashrc";

    // this opens the file path s
    new_file.open(filePath,ios::in);
    
    // check if we found the file 
    if(new_file.is_open())
    {
        // We found the file! 
        getline(new_file,passcode); // Here we are reading literally the first line becuase thats where the passcode is stored
        return passcode.substr(22,16);
    }
   
    else
    {
        cout << "Error: File not found" << endl;
        return "";
    }

}
int authentication::randomCodeGenerator()
{
    srand (time(NULL));
    return rand()%10000000;
}