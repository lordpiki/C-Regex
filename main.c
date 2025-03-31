#include <stdio.h>
#include <stdbool.h>
#include <string.h>

    
// Declartions of functions
int main();
bool parseRegex(char *regex, char *txt);
void addToLiteral(char* literal, char letter);
bool checkMatching(char* txt, int txtIndex, char token);
bool isLetter(char c);
int starToken(char* txt, char previousToken, int txtIndex);
int letterToken(char* txt, char token, int txtIndex, char* regex , int tokenIndex);
bool checkMatching(char* txt, int txtIndex, char token);


bool isLetter(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int letterToken(char* txt, char token, int txtIndex, char* regex, int tokenIndex)
{
    // Check if the next token is one of these '*', '+', '?' 
    // if it is then we will have to check for the amount per token of this token
    char nextToken = regex[tokenIndex + 1];
    switch (nextToken)
    {
        case '*':
            return txtIndex;
            break;
    }
    if (checkMatching(txt, txtIndex, token))
    {
        return txtIndex + 1;
    }
    return -1;
}

// Will return the next index of the str to check, will return -1 if fails
int starToken(char* txt, char previousToken, int txtIndex)
{
    while (checkMatching(txt, txtIndex, previousToken) && txtIndex < strlen(txt))
    {
        txtIndex++;
    }
    return txtIndex;
}

bool checkMatching(char* txt, int txtIndex, char token)
{
    return token == '.' || txt[txtIndex] == token;
}


void addToLiteral(char* literal, char letter)
{
    int len = strlen(literal);
    for (int i = 0; i < len; i++)
    {
        if (literal[i] == 0)
        {
            literal[i] = letter;
            return;
        }
    }
}

// Function implementations
bool parseRegex(char *regex, char *txt)
{
    // this function will check if the regex holds up for this txt exactly, not in a substr or anything like that
    // parse regex (will currently suppert * and . on english characters)
    // the parsing will use a switch case on each char against the currently supported tokens
    // if it doesn't find a token (* or .) then it will add it to the current string literal
    // when encountering '.' the program should just go to the next character
    // when encountering '*' the program should look at the last character in the original string, and then pass it through a function to search as many occouarances of that
    // the function should be aware that '.' is any character and so will always return true if it got it
    
    int regLen = strlen(regex);
    int txtLen = strlen(txt);
    int txtIndex = 0; 
    // parse regex
    // The parser will go over the regex, and then will go forward with the txt according to the current token
    for (int i = 0; i < regLen; i++)
    {
        char token = regex[i];
        switch (token)
        {
            case '.':
                txtIndex++;
                break;
                case '*':
                txtIndex = starToken(txt, regex[i - 1], txtIndex);
                break;
            default:
                txtIndex = letterToken(txt, token, txtIndex, regex, i);
                break;
        }
        if (txtIndex == -1)
        {
            return false;
        }
    }
    return txtIndex == txtLen;
}

void test(char* regex, char* txt, bool expected) {
    bool result = parseRegex(regex, txt);
    printf("Regex: \"%s\", Text: \"%s\" → %s (Expected: %s)\n",
           regex, txt, result ? "MATCH" : "NO MATCH",
           expected ? "MATCH" : "NO MATCH");
}

int main() {
    // Basic matching
    test("hello", "hello", true);
    test("hello", "helloo", false);
    test("hello", "hell", false);
    
    // Wildcard `.` test
    test("h.llo", "hello", true);
    test("h.llo", "hallo", true);
    test("h.llo", "hxllo", true);
    test("h.llo", "hlllo", true);

    // Star `*` test
    test("a*b", "b", true);        // Matches zero 'a'
    test("a*b", "ab", true);       // Matches one 'a'
    test("a*b", "aaab", true);     // Matches multiple 'a'
    test("a*b", "ba", false);      // 'b' must be at the end

    // Combination of `.` and `*`
    test("a.*b", "ab", true);
    test("a.*b", "acb", true);
    test("a.*b", "axyzb", true);
    test("a.*b", "a", false);  // No 'b' at the end

    return 0;
}
