#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

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
    token* main;
    token* secondary;
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
token* getNextToken(char* regex, int* index, token* lastToken);
token* tokenizeDot(int* index, token* lastToken);
token* tokenizeStar(char* regex, int* index, token* lastToken);
token* tokenizeLiteral(char* regex, int* index, token* lastToken);

// Helper functions
void reverseStr(char* str);

// Init functions
token* initToken(char* str, int type, token* lastToken);
// TODO
group* initGroup(group* last, token* first, token* second, token* third);

// Free functions
void freeToken(token* token);
void freeGroup(token* group);

// Helpful functions
void printToken(token* token);
void printAllTokens(token* first);
void printGroups(group* first);


void printToken(token* token)
{
    switch (token->type)
    {
        case DOT:
            {
                printf(".");
                break;
            }
        case STAR:
            {
                printf("*");
                break;
            }
        case LITERAL:
            {
                printf("%s", token->str);
                break;
            }
        default:
            {
                printf("ERROR UNKOWN TYPE: %d", token->type);
            }
    }
}

void printAllTokens(token* first)
{
    token* curr = first;
    while (curr != NULL)
    {
        printToken(curr);
        printf("->");
        curr = curr->next;
    }
    printf("NULL\n");
}

void freeToken(token* token)
{
    free(token->str);
    free(token);
}

bool isLetter(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}


token* initToken(char* str, int type, token* lastToken)
{
    // Allocate memory for token
    token* t = (token*)malloc(sizeof(token));
    // Set the token values
    t->str = str;
    t->type = type;
    t->next = lastToken;
    return t;
}

token* tokenizeDot(int* index, token* lastToken)
{
    (*index)++;
    return initToken(NULL, DOT, lastToken);
}

token* tokenizeStar(char* regex, int* index, token* lastToken)
{
    // Create star token
    (*index)++;
    token* starToken = initToken(regex, STAR, lastToken);
    
    // create fake index to not tamper with the existing index
    int tempIndex = *index;
    // Get the next token in order for the star token to work properly
    token* nextToken = getNextToken(regex, &tempIndex, starToken);
    // If the next token is literal, take only the first letter and make it a seperate token
    (*index)++;
    if (nextToken->type == LITERAL)
    {
        // Allocate memory for newText instead of using the same memory as nextToken
        char* newTxt = strndup(nextToken->str + strlen(nextToken->str) - 1, 1);
        freeToken(nextToken);
        return initToken(newTxt, LITERAL, starToken);
    }
    return nextToken;
}

// Function will make the literal a string until the next non letter char in the regex
// The function will reverse the string becaues the regex is reversed
token* tokenizeLiteral(char* regex, int* index, token* lastToken)
{
    int firstIndex = *index;
    int regexLen = strlen(regex);

    // Go forward in the regex until we meet a non-letter character to calculate the length of the literal
    for (;*index < regexLen && isLetter(regex[*index]); (*index)++) {};
    int strLen = *index - firstIndex;

    // Allocate memory and copy from regex to a new string
    char* str = strndup(regex + firstIndex, strLen);
    
    // Reverse the string because the regex is reversed
    reverseStr(str);
    return initToken(str, LITERAL, lastToken);
}

// Returns the next token in the regex, and moves the index to the next token
token* getNextToken(char* regex, int* index, token* lastToken)
{
    switch (regex[*index])
    {
        case '.':
            {
                return tokenizeDot(index, lastToken);
            }
        case '*':
            {
                return tokenizeStar(regex, index, lastToken);
            }
        default:
            {
                return tokenizeLiteral(regex, index, lastToken);
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
    token* first = getNextToken(regex, &regIndex, NULL);
    token* curr = first;
    // Go over the the regex from the end
    while (regIndex < len && curr != NULL)
    {
        // The function will get the curr and set it's next to the token, and then will return the token
        curr = getNextToken(regex, &regIndex, curr);
    }
    return curr;
}

group* tokensToGroups(token* tokens)
{
    return NULL;
}

// Function will parse regex and will return true or false if the txt matches the regex
bool parseRegex(char *regex, char *txt)
{
    // Convert regex to tokens
    token* tokens = regexToTokens(regex);
    // Group tokens
    group* groups = tokensToGroups(tokens);
    // Go through the groups and the txt
    // return parseGroup(groups, txt);
    return 0;
}

void test(char* regex, char* txt, bool expected) {
    bool result = parseRegex(regex, txt);
    printf("Regex: \"%s\", Text: \"%s\" → %s (Expected: %s)\n",
           regex, txt, result ? "MATCH" : "NO MATCH",
           expected ? "MATCH" : "NO MATCH");
}

int main() {

    printf("\n");
    char regex[] = "abm*d*";
    printAllTokens(regexToTokens(regex));

    // // Basic matching
    // test("hello", "hello", true);
    // test("hello", "helloo", false);
    // test("hello", "hell", false);
    // 
    // // Wildcard `.` test
    // test("h.llo", "hello", true);
    // test("h.llo", "hallo", true);
    // test("h.llo", "hxllo", true);
    // test("h.llo", "hlllo", true);
    //
    // // Star `*` test
    // test("a*b", "b", true);        // Matches zero 'a'
    // test("a*b", "ab", true);       // Matches one 'a'
    // test("a*b", "aaab", true);     // Matches multiple 'a'
    // test("a*b", "ba", false);      // 'b' must be at the end
    //
    // // Combination of `.` and `*`
    // test("a.*b", "ab", true);
    // test("a.*b", "acb", true);
    // test("a.*b", "axyzb", true);
    // test("a.*b", "a", false);  // No 'b' at the end
    //
    return 0;
}
