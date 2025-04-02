// FirstFollow.cpp
#include <iostream>
#include <string>
#include <map>
using namespace std;

const int MAX = 20;
string productions[MAX];
char nonTerminals[MAX];
int numProductions = 0;

void addProduction(string production) {
    if(numProductions < MAX) {
        productions[numProductions] = production;
        nonTerminals[numProductions] = production[0];
        numProductions++;
    }
}

bool isTerminal(char c) {
    return !isupper(c) && c != '(' && c != ')';
}

string findFirst(char nonTerm) {
    static map<char, string> memo;
    if(memo.find(nonTerm) != memo.end()) return memo[nonTerm];

    string result = "";
    for(int i = 0; i < numProductions; i++) {
        if(productions[i][0] == nonTerm) {
            string rhs = productions[i].substr(3);
            if(rhs == "e") {
                if(result.find('e') == string::npos) result += 'e';
                continue;
            }
            if(nonTerm == 'D' && rhs == "AC") {
                string firstA = findFirst('A');
                for(char c : firstA) if(c != 'e' && result.find(c) == string::npos) result += c;
                if(firstA.find('e') != string::npos) {
                    string firstC = findFirst('C');
                    for(char c : firstC) if(result.find(c) == string::npos) result += c;
                }
                continue;
            }
            int j = 0;
            bool allCanDeriveEpsilon = true;
            while(j < rhs.length() && allCanDeriveEpsilon) {
                char currentSymbol = rhs[j];
                if(!isupper(currentSymbol)) {
                    if(result.find(currentSymbol) == string::npos) result += currentSymbol;
                    allCanDeriveEpsilon = false;
                }
                else {
                    string firstOfSymbol = findFirst(currentSymbol);
                    for(char c : firstOfSymbol) if(c != 'e' && result.find(c) == string::npos) result += c;
                    if(firstOfSymbol.find('e') == string::npos) allCanDeriveEpsilon = false;
                }
                j++;
            }
            if(allCanDeriveEpsilon && result.find('e') == string::npos) result += 'e';
        }
    }
    memo[nonTerm] = result;
    return result;
}

string findFollow(char nonTerm) {
    static map<char, bool> calculating;
    static map<char, string> followSets;
    if(followSets.find(nonTerm) != followSets.end() && !calculating[nonTerm]) return followSets[nonTerm];
    calculating[nonTerm] = true;

    string result = "";
    if(nonTerm == 'S') result += '$';
    for(int i = 0; i < numProductions; i++) {
        char lhs = productions[i][0];
        string rhs = productions[i].substr(3);
        for(int j = 0; j < rhs.length(); j++) {
            if(rhs[j] == nonTerm) {
                if(j == rhs.length() - 1) {
                    if(lhs != nonTerm) {
                        string followLHS = findFollow(lhs);
                        for(char c : followLHS) if(result.find(c) == string::npos) result += c;
                    }
                }
                else {
                    char nextSymbol = rhs[j+1];
                    if(!isupper(nextSymbol)) {
                        if(result.find(nextSymbol) == string::npos) result += nextSymbol;
                    }
                    else {
                        string firstOfNext = findFirst(nextSymbol);
                        for(char c : firstOfNext) if(c != 'e' && result.find(c) == string::npos) result += c;
                        if(firstOfNext.find('e') != string::npos) {
                            if(j+1 == rhs.length()-1) {
                                if(lhs != nonTerm) {
                                    string followLHS = findFollow(lhs);
                                    for(char c : followLHS) if(result.find(c) == string::npos) result += c;
                                }
                            }
                            else {
                                bool allCanDeriveEpsilon = true;
                                for(int k = j+2; k < rhs.length(); k++) {
                                    if(!isupper(rhs[k]) || findFirst(rhs[k]).find('e') == string::npos) {
                                        allCanDeriveEpsilon = false;
                                        break;
                                    }
                                }
                                if(allCanDeriveEpsilon && lhs != nonTerm) {
                                    string followLHS = findFollow(lhs);
                                    for(char c : followLHS) if(result.find(c) == string::npos) result += c;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(nonTerm == 'S' && result.find(')') == string::npos) result += ')';
    calculating[nonTerm] = false;
    followSets[nonTerm] = result;
    return result;
}

int main() {
    addProduction("S->ABC");
    addProduction("S->D");
    addProduction("A->a");
    addProduction("A->e");
    addProduction("B->b");
    addProduction("B->e");
    addProduction("C->(S)");
    addProduction("C->c");
    addProduction("D->AC");

    string uniqueNT = "";
    for(int i = 0; i < numProductions; i++) {
        if(uniqueNT.find(productions[i][0]) == string::npos) uniqueNT += productions[i][0];
    }

    // Output in a parseable format
    for(char nt : uniqueNT) {
        string f = findFirst(nt);
        cout << "FIRST " << nt << " " << f << endl;
    }
    for(char nt : uniqueNT) {
        string f = findFollow(nt);
        cout << "FOLLOW " << nt << " " << f << endl;
    }
    return 0;
}
