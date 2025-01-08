#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validateString(char *str) {
    int len = strlen(str);
    
    if (len < 2) {
        return 0; // minimum length is 2, null is not allow
    }


    int i = 0;

    while (i < len - 2) {
        if (str[i] != 'a') {
            return 0; //invalid if its not a
        }
        i++;
    }

    //last two char bb
    if (str[i] == 'b' && str[i + 1] == 'b') {
        return 1; 
    }
    return 0; 
}

int main() {
    char str[100];
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0;
    
    if (validateString(str)) {
        printf("Valid String\n");
    } else {
        printf("Invalid String\n");
    }

    return 0;
}
