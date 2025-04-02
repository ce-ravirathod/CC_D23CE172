#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

class ConstantFoldingOptimizer {
private:
    // Tokenizer to break expression into tokens
    vector<string> tokenize(const string& expression) {
        vector<string> tokens;
        string currentToken;
        
        for (size_t i = 0; i < expression.length(); i++) {
            char c = expression[i];
            
            // Handle whitespace
            if (isspace(c)) {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                continue;
            }
            
            // Handle operators and parentheses
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                tokens.push_back(string(1, c));
                continue;
            }
            
            // Build numbers and variable names
            currentToken += c;
        }
        
        // Add last token if exists
        if (!currentToken.empty()) {
            tokens.push_back(currentToken);
        }
        
        return tokens;
    }
    
    // Check if a string is a numeric constant
    bool isConstant(const string& token) {
        if (token.empty()) {
            return false;
        }
        
        size_t i = 0;
        if (token[0] == '-' || token[0] == '+') {
            i = 1;
            if (token.length() == 1) {
                return false;  // Just a sign
            }
        }
        
        bool hasDecimal = false;
        for (; i < token.length(); i++) {
            if (token[i] == '.' && !hasDecimal) {
                hasDecimal = true;
            } else if (!isdigit(token[i])) {
                return false;
            }
        }
        
        return true;
    }
    
    // Convert string to double
    double toNumber(const string& token) {
        return stod(token);
    }
    
    // Apply operator on two operands
    double applyOperator(char op, double a, double b) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) {
                    throw runtime_error("Division by zero");
                }
                return a / b;
            default:
                throw runtime_error("Unknown operator");
        }
    }
    
    // Check if character is an operator
    bool isOperator(const string& token) {
        return token.length() == 1 && 
               (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/');
    }
    
    // Get precedence of an operator
    int getPrecedence(char op) {
        if (op == '+' || op == '-') {
            return 1;
        }
        if (op == '*' || op == '/') {
            return 2;
        }
        return 0;
    }
    
    // Convert infix expression to postfix
    vector<string> infixToPostfix(const vector<string>& infix) {
        vector<string> postfix;
        stack<string> operators;
        
        for (const string& token : infix) {
            if (token == "(") {
                operators.push(token);
            } else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == "(") {
                    operators.pop(); // Remove the '('
                }
            } else if (isOperator(token)) {
                char currentOp = token[0];
                while (!operators.empty() && operators.top() != "(" && 
                       getPrecedence(operators.top()[0]) >= getPrecedence(currentOp)) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            } else {
                postfix.push_back(token);
            }
        }
        
        // Pop remaining operators
        while (!operators.empty()) {
            postfix.push_back(operators.top());
            operators.pop();
        }
        
        return postfix;
    }
    
    // Evaluate postfix expression with constant folding
    string evaluatePostfixWithFolding(const vector<string>& postfix) {
        stack<string> operandStack;
        
        for (const string& token : postfix) {
            if (isOperator(token)) {
                // Need at least two operands for operation
                if (operandStack.size() < 2) {
                    throw runtime_error("Invalid expression");
                }
                
                string b = operandStack.top(); operandStack.pop();
                string a = operandStack.top(); operandStack.pop();
                char op = token[0];
                
                // If both operands are constants, fold them
                if (isConstant(a) && isConstant(b)) {
                    double result = applyOperator(op, toNumber(a), toNumber(b));
                    // Format result to avoid trailing zeros
                    ostringstream ss;
                    ss << result;
                    string resultStr = ss.str();
                    // If result is a whole number, remove decimal part
                    if (resultStr.find('.') != string::npos) {
                        resultStr = resultStr.substr(0, resultStr.find_last_not_of('0') + 1);
                        if (resultStr.back() == '.') {
                            resultStr.pop_back();
                        }
                    }
                    operandStack.push(resultStr);
                } else {
                    // If at least one operand is a variable, keep the expression
                    string expr = "(" + a + " " + op + " " + b + ")";
                    operandStack.push(expr);
                }
            } else {
                operandStack.push(token);
            }
        }
        
        if (operandStack.size() != 1) {
            throw runtime_error("Invalid expression");
        }
        
        string result = operandStack.top();
        // Remove unnecessary outer parentheses
        if (result.length() >= 2 && result.front() == '(' && result.back() == ')') {
            result = result.substr(1, result.length() - 2);
        }
        
        return result;
    }

public:
    string optimize(const string& expression) {
        // Tokenize input expression
        vector<string> tokens = tokenize(expression);
        
        // Convert to postfix
        vector<string> postfix = infixToPostfix(tokens);
        
        // Evaluate with constant folding
        string optimized = evaluatePostfixWithFolding(postfix);
        
        // Clean up the output to match expected format
        string result;
        bool inParentheses = false;
        for (size_t i = 0; i < optimized.length(); i++) {
            char c = optimized[i];
            
            // Skip unnecessary spaces
            if (c == ' ' && (i > 0 && (optimized[i-1] == '(' || optimized[i-1] == ' '))) {
                continue;
            }
            
            // Track parentheses state
            if (c == '(') inParentheses = true;
            else if (c == ')') inParentheses = false;
            
            result += c;
        }
        
        return result;
    }
};

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);
    
    try {
        ConstantFoldingOptimizer optimizer;
        string optimized = optimizer.optimize(expression);
        cout << "Optimized expression: " << optimized << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}