#include <iostream>
#include <string>
using namespace std;

// Global variables for input string, current position and error flag
string str;
int ip,f=0;

// Forward declarations for recursive functions
void L2();
void L();
void S();

// S → ( L ) | a
void S(){
    if(str[ip]=='('){
        ip++;
        L();
        if(str[ip]==')'){
            ip++;
        } else {
            f=1;  // Error flag if ')' not found
        }
    } else if (str[ip]=='a'){
        ip++;
    } else {
        f=1;  // Error flag if neither '(' nor 'a' found
    }
}

// L → S L'
void L(){
    S();
    L2();
}

// L' → , S L' | ϵ
void L2(){
    if(str[ip]==','){
        ip++;
        S();
        L2();
    }
    // ϵ case - do nothing
}

int main()
{
    getline(cin, str);
    ip = 0;  // Initialize position counter

    S();  // Start parsing from start symbol

    // Check if parsing was successful and entire string was consumed
    if(f==0 && ip==str.length())
        cout << "Valid string" << endl;
    else
        cout << "Invalid string" << endl;
    return 0;
}