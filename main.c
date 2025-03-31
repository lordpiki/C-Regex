#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

// Types of tokens
enum {
    LITERAL = 1,
    DOT = 2,
    STAR = 3
};

// Declarations of structures
typedef struct token
{
    int type;
    char* str;    
    struct token* next;
} token;

typedef struct group
{
    int mainToken;
    token* tokens[3];
    struct group* next;
} group;

// Declaraions of functions
int main();
void test(char* regex, char* txt, bool expected);

// Regex functions
bool parseRegex(char *regex, char *txt);
token* regexToTokens(char* regex);
int parseToken(char c);
group* tokensToGroups(token* tokens);
bool parseGroup(group* first, char* txt);

// Token functions
token* getNextToken(char* regex, int* index);
token* tokenizeDot(int* index);
token* tokenizeStar(char* regex, int* index);
token* tokenizeLiteral(char* reg, int* index);

// Helper functions
void addToLiteral(char* literal, char letter);
void reverseStr(char* str);

// Init functions
token* initToken(char* str, int type);
group* initGroup(group* last, token* first, token* second, token* third);


bool isLetter(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}


void addToLiteral(char* literal, char letter)
{
}


token* tokenizeDot(int* index)
{
    (*index)++;
    return initToken(NULL, DOT);
}

// Returns the next token in the regex, and moves the index to the next token
token* getNextToken(char* regex, int* index)
{
    
    switch (regex[*index])
    {
        case '.':
            {
                return tokenizeDot(index);
            }
        case '*':
            {
                return tokenizeStar(regex, index);
            }
        default:
            {
                return tokenizeLiteral(regex, index);
            }
    }
    return NULL;
}

void reverseStr(char* str)
{
    int left = 0, right = strlen(str) - 1;
    while (left < right) {
        char temp = str[left];
        str[left] = str[right];
        str[right] = temp;
        left++;
        right--;
    }
}

token* regexToTokens(char* regex)
{
    reverseStr(regex);
    int len = strlen(regex);
    int regIndex = 0;
    token* first = getNextToken(regex, &regIndex);
    token* curr = first;
    // Go over the the regex from the end
    while (regIndex > 0 && curr != NULL)
    {
        curr->next = getNextToken(regex, &regIndex);
        curr = curr->next;
    }
    return first;
}

// Function will parse regex and will return true or false if the txt matches the regex
bool parseRegex(char *regex, char *txt)
{
    // Convert regex to tokens
    token* tokens = regexToTokens(regex);
    // Group tokens
    group* groups = tokensToGroups(tokens);
    // Go through the groups and the txt
    return parseGroup(groups, txt);
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
