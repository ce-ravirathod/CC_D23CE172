#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>

using namespace std;

map<char, set<char>> computeFirst(map<char, vector<string>> &grammar) {
    map<char, set<char>> first;
    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        for (auto &production : rule.second) {
            char firstSymbol = production[0];
            if (isupper(firstSymbol)) {
                first[nonTerminal].insert(first[firstSymbol].begin(), first[firstSymbol].end());
            } else {
                first[nonTerminal].insert(firstSymbol);
            }
        }
    }
    return first;
}

map<char, set<char>> computeFollow(map<char, vector<string>> &grammar, map<char, set<char>> &first) {
    map<char, set<char>> follow;
    follow['S'].insert('$');

    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        for (auto &production : rule.second) {
            for (size_t i = 0; i < production.length(); ++i) {
                if (isupper(production[i])) {
                    if (i + 1 < production.length()) {
                        char nextSymbol = production[i + 1];
                        if (isupper(nextSymbol)) {
                            follow[production[i]].insert(first[nextSymbol].begin(), first[nextSymbol].end());
                        } else {
                            follow[production[i]].insert(nextSymbol);
                        }
                    } else {
                        follow[production[i]].insert(follow[nonTerminal].begin(), follow[nonTerminal].end());
                    }
                }
            }
        }
    }
    return follow;
}

map<char, map<char, string>> constructParsingTable(map<char, vector<string>> &grammar, map<char, set<char>> &first, map<char, set<char>> &follow) {
    map<char, map<char, string>> parsingTable;

    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        for (auto &production : rule.second) {
            char firstSymbol = production[0];
            if (isupper(firstSymbol)) {
                for (auto &terminal : first[firstSymbol]) {
                    parsingTable[nonTerminal][terminal] = production;
                }
            } else {
                parsingTable[nonTerminal][firstSymbol] = production;
            }
        }
    }

    return parsingTable;
}

bool validateString(string input, map<char, map<char, string>> &parsingTable) {
    stack<char> st;
    st.push('$');
    st.push('S');

    size_t i = 0;
    while (!st.empty()) {
        char top = st.top();
        st.pop();

        if (top == '$' && i == input.length()) {
            return true;
        }

        if (isupper(top)) {
            if (parsingTable[top].find(input[i]) != parsingTable[top].end()) {
                string production = parsingTable[top][input[i]];
                for (int j = production.length() - 1; j >= 0; --j) {
                    st.push(production[j]);
                }
            } else {
                return false;
            }
        } else if (top == input[i]) {
            i++;
        } else {
            return false;
        }
    }

    return false;
}

int main() {
    map<char, vector<string>> grammar = {
        {'S', {"aB", "bA", "c"}},
        {'A', {"a", "b"}},
        {'B', {"b", "c"}}
    };

    auto first = computeFirst(grammar);
    auto follow = computeFollow(grammar, first);
    auto parsingTable = constructParsingTable(grammar, first, follow);

    bool isLL1 = true;
    for (auto &rule : grammar) {
        char nonTerminal = rule.first;
        for (auto &production : rule.second) {
            char firstSymbol = production[0];
            if (isupper(firstSymbol)) {
                for (auto &terminal : first[firstSymbol]) {
                    if (parsingTable[nonTerminal].find(terminal) != parsingTable[nonTerminal].end()) {
                        isLL1 = false;
                        break;
                    }
                }
            }
        }
    }

    if (isLL1) {
        cout << "The grammar is LL(1)." << endl;
    } else {
        cout << "The grammar is not LL(1)." << endl;
    }

    vector<string> testCases = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};

    for (auto &testCase : testCases) {
        if (validateString(testCase, parsingTable)) {
            cout << "Valid string: " << testCase << endl;
        } else {
            cout << "Invalid string: " << testCase << endl;
        }
    }

    return 0;
}
