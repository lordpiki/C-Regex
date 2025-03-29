#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// Declartions of functions
int main();
bool parseRegex(char *regex, char *txt);
bool isLetter(char c);

// Function implementations
bool parseRegex(char *regex, char *txt)
{
    // parse regex (will currently suppert * and . on english characters)
    // the parsing will use a switch case on each char against the currently supported tokens
    // if it doesn't find a token (* or .) then it will add it to the current string literal
    // when encountering '.' the program should just go to the next character
    // when encountering '*' the program should look at the last character in the original string, and then pass it through a function to search as many occouarances of that
    // the function should be aware that '.' is any character and so will always return true if it got it
    
    // String literal for matching anything with the string, let's say current string literals are 50 chars max as to not waste memeory resizing the string
    // will need to be fixed\optimized later
    char literal[50] = {0};
    int regLen = strlen(regex);
    int txtLen = strlen(txt);
    
    // parse regex
    for (int i = 0; i < regLen; i++)
    {
        switch (regex[i]):
        {
            case '.':
        }
    }
    return false;
}

int main()
{
    printf("Hello, World!!!\n");
    return 0;
}

