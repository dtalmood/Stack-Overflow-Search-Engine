#include <iostream>
#include <iomanip>
#include <curl/curl.h> 
#include "json.hpp"
/*  
    Notes:
        1. libcurl: This library handles netowrk operations specifically for 
            1a. Downloading files
            1b. Making HTTP requests
            1c. Interacting with web services.

        2. json.hpp: After receiving the data from the network operation 
        (which is often in JSON format), you use the json.hpp library 
        (nlohmann/json) to parse this JSON data. This allows you to easily
         access and manipulate the data within your C++ application.

        3. C-Make: whne we have alot of stuff that need to consitnelty write to the command line
        we have cmake so that we cna run it and automatily runs build files 

        To Build Project to Run: enure termins is in SRC folder 
            2. cmake ..  (we are in src folder )
            4. make
            5. ./MyProject

        To re-run build again 
            1. rm -r build 
            2. do steps again 
*/

using namespace std;

void printMenu();
void search(string userQuestion);
string constructQuestion(string &userQuestion);
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){}

int main()
{
    printMenu();
    cout << "Type your question and once done press enter to search. " << endl;
    cout << "Question: ";
    string userQuestion;
    getline(cin, userQuestion);
    string constructedString = constructQuestion(userQuestion);
    //search(constructedString);
    return 0;
}

void printMenu()
{
    int titleWidth = 40;
    int titleLength = 12; // Length of "STACK SURFER"
    int leftPadding = (titleWidth - titleLength) / 2; // Calculate left padding

    cout << setw(titleWidth) << setfill('=') << "" << endl;
    cout << setw(leftPadding) << setfill(' ') << "" << "STACK SURFER" << setw(titleLength) << setfill(' ') << "" << endl;
    cout << setw(titleWidth) << setfill('=') << "" << endl;
}
void search(string userQuestion)
{
    cout << "Entered Tester " << endl << endl;
    CURL *curl; // Declare a pointer to a CURL object
    CURLcode res; // Declare a variable to hold the result of the curl operation

    curl_global_init(CURL_GLOBAL_DEFAULT); // Initialize the cURL library globally
    curl = curl_easy_init(); // Starts our Curl Session 

    if(curl) 
    { // Check if the session was initialized successfully
        curl_easy_setopt(curl, CURLOPT_URL, "https://exanple.con/"); // Set the URL for the cURL request
        res = curl_easy_perform(curl); // Perform the request, res will get the return code
        curl_easy_cleanup(curl); // Clean up the CURL easy session
    }
   
    curl_global_cleanup(); // Clean up the cURL library globally

}

string constructQuestion(string & userQuestion)
{
    //this base url is to need to access stack over API
    string baseURL = "https://api.stackexchange.com/2.3/search/advanced?order=desc&sort=activity&q=";
    for(int i = 0; i < userQuestion.length(); i++)
    {
        if(userQuestion[i] != 32) // while character is not a space 
            baseURL += userQuestion[i];
        
        
        else if(userQuestion[i] == 32 && (userQuestion[i+1] != 32 ))
        {
            baseURL += "+";
        }

    } 
    string key = "OUgS5vV1jD7kdtN8*nYZKg((";
    baseURL += "&key=" + key;
    
    cout << "Constructed Stirng: " << baseURL << endl;
    return baseURL;
}