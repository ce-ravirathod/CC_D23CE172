%{
#include <stdio.h>
#include <stdlib.h>

void print_token(const char* type, const char* value);
%}

%option noyywrap

%%

"int"|"char"|"return"|"void"|"long"|"float"|"struct"|"if"|"else"|"while"|"for" {
    print_token("Keyword", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    print_token("Identifier", yytext);
}

[0-9]+(\.[0-9]+)? {
    print_token("Constant", yytext);
}

\"[^\"]*\" {
    print_token("String", yytext);
}

'.' {
    print_token("String", yytext);
}

"{"|"}"|"("|")"|";"|"," {
    print_token("Punctuation", yytext);
}

"+"|"-"|"*"|"/"|"="|"&"|"|"|"<"|">"|"!"|"%" {
    print_token("Operator", yytext);
}

"//".* {
    // Single-line comment, ignore
}

"/*"([^*]|\*+[^/*])*\*+"/" {
    // Multi-line comment, ignore
}

[ \t\n] {
    // Whitespace, ignore
}

. {
    printf("Lexical Error: Unrecognized character '%s'\n", yytext);
}

%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (file) {
            yyin = file;
            yylex();
            fclose(file);
        } else {
            printf("Error opening file.\n");
        }
    } else {
        printf("Usage: %s <filename>\n", argv[0]);
    }
    return 0;
}

void print_token(const char* type, const char* value) {
    printf("Token: %s: %s\n", type, value);
}
