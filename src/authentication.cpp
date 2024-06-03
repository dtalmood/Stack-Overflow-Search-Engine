#include <authentication.hpp>
#include <iostream>
// MONGO DB DRIVERS
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
using namespace std;


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


void authentication::login()
{
    string username;
    string password;
    bool invalid = true;
    do
    {
       cout << "Username: ";
       cin >> username;
       cout << "Password: ";
       cin >> password;
        // check our centeral database if username exists 
        
    } while(invalid);
    
}

void authentication::logout()
{

}

void authentication::newMember()
{
}

void authentication::forgotPassword()
{

}
void authentication::encryptPassword()
{
    
}