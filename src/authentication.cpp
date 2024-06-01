#include <authentication.hpp>
#include <iostream>

void authenticator::login()
{
    bool invalid = true;
    
}
void authenticator::logout()
{

}

void authenticator::menu()
{
    cout << "1. Login " << endl;
    cout << "2. New Login" << endl;
}

void authenticator::newMember()
{
    bool validUsername = true;
    string userName;
    string password;
    
    do
    {
        cout << "UserName: " << endl;
        cin >> userName;
        cout << "Password: " << endl;
        cin >> password;
        // check if username exits 
    }while(validUsername);
}