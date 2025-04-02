#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>

using namespace std;

// Structure to represent a quadruple
struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
};

class QuadrupleGenerator {
private:
    string input;
    int pos = 0;
    int tempCount = 1;
    vector<Quadruple> quadruples;

    // Utility function to get next temporary variable name
    string getNextTemp() {
        return "t" + to_string(tempCount++);
    }

    // Skip whitespaces
    void skipWhitespace() {
        while (pos < input.length() && isspace(input[pos])) {
            pos++;
        }
    }

    // Parse a digit or decimal number
    string parseNumber() {
        skipWhitespace();
        string number;
        
        // Handle negative numbers
        if (input[pos] == '-' && pos + 1 < input.length() && isdigit(input[pos + 1])) {
            number += '-';
            pos++;
        }
        
        // Parse digits before decimal point
        while (pos < input.length() && isdigit(input[pos])) {
            number += input[pos++];
        }
        
        // Parse decimal point and digits after it
        if (pos < input.length() && input[pos] == '.') {
            number += '.';
            pos++;
            while (pos < input.length() && isdigit(input[pos])) {
                number += input[pos++];
            }
        }
        
        return number;
    }

    // Grammar implementation: F → (E) | digit
    string parseF() {
        skipWhitespace();
        
        if (pos < input.length() && input[pos] == '(') {
            pos++; // Skip the '('
            string exprResult = parseE();
            skipWhitespace();
            
            if (pos < input.length() && input[pos] == ')') {
                pos++; // Skip the ')'
                return exprResult;
            } else {
                throw runtime_error("Missing closing parenthesis");
            }
        } else if (pos < input.length() && (isdigit(input[pos]) || (input[pos] == '-' && pos + 1 < input.length() && isdigit(input[pos + 1])))) {
            return parseNumber();
        } else {
            throw runtime_error("Expected a number or parenthesized expression at position " + to_string(pos));
        }
    }

    // Grammar implementation: T → T * F | T / F | F
    string parseT() {
        string leftTerm = parseF(); // Start with F
        
        skipWhitespace();
        while (pos < input.length() && (input[pos] == '*' || input[pos] == '/')) {
            char op = input[pos++];
            string rightFactor = parseF();
            string temp = getNextTemp();
            
            // Create and add quadruple
            Quadruple q;
            q.op = string(1, op);
            q.arg1 = leftTerm;
            q.arg2 = rightFactor;
            q.result = temp;
            quadruples.push_back(q);
            
            leftTerm = temp; // Update leftTerm for potential chaining
        }
        
        return leftTerm;
    }

    // Grammar implementation: E → E + T | E - T | T
    string parseE() {
        string leftExpr = parseT(); // Start with T
        
        skipWhitespace();
        while (pos < input.length() && (input[pos] == '+' || input[pos] == '-')) {
            char op = input[pos++];
            string rightTerm = parseT();
            string temp = getNextTemp();
            
            // Create and add quadruple
            Quadruple q;
            q.op = string(1, op);
            q.arg1 = leftExpr;
            q.arg2 = rightTerm;
            q.result = temp;
            quadruples.push_back(q);
            
            leftExpr = temp; // Update leftExpr for potential chaining
        }
        
        return leftExpr;
    }

public:
    QuadrupleGenerator(string inputExpr) : input(inputExpr) {}
    
    vector<Quadruple> generate() {
        quadruples.clear();
        pos = 0;
        tempCount = 1;
        
        parseE();
        return quadruples;
    }
    
    void printQuadruples() {
        cout << "Operator\tOperand 1\tOperand 2\tResult" << endl;
        for (const auto& q : quadruples) {
            cout << q.op << "\t\t" << q.arg1 << "\t\t" << q.arg2 << "\t\t" << q.result << endl;
        }
    }
};

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);
    
    try {
        QuadrupleGenerator generator(expression);
        generator.generate();
        generator.printQuadruples();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}