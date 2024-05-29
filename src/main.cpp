#include <iostream>
#include <iomanip>
using namespace std;

void printMenu();

int main()
{
    printMenu();
    cout << "Type your question and once done press enter to search. " << endl;
    cout << "Question: ";
    string userQuestion;
    getline(cin, userQuestion);
    cout << "FUCK YOU" << endl;
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
