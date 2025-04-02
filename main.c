#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

// Types of tokens
enum {
    NONE = 0,
    LITERAL = 1,
    DOT = 2,
    STAR = 3,
    PLUS = 4,
    QUESTION = 5,
    PARAN = 6,
    CARRET = 7,
    DOLLAR = 8
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
bool parseGroup(group* group, char* txt, int* index);

bool parseGroupDot(group* g, char* txt, int* index);
bool parseGroupStar(group* g, char* txt, int* index);
bool parseGroupLiteral(group* g, char* txt, int* index);
bool parseGroupPlus(group* g, char* txt, int* index);
bool parseGroupQuestion(group* g, char* txt, int* index);
bool parseGroupParan(group* g, char* txt, int* index);
bool parseGroupCarret(group* g, char* txt, int* index);
bool parseGroupDollar(group* g, char* txt, int* index);

bool parseGroupAmount(group* g, char* txt, int* index, int min, int max);

// Token functions
token* getNextToken(char* regex, int* index, token* lastToken);
token* seperateNextToken(char* regex, int* index, token* lastToken);
token* tokenizeDot(int* index, token* lastToken);
token* tokenizeStar(char* regex, int* index, token* lastToken);
token* tokenizePlus(char* regex, int* index, token* lastToken);
token* tokenizeQuestion(char* regex, int* index, token* lastToken);
token* tokenizeLiteral(char* regex, int* index, token* lastToken);
token* tokenizeParentheses(char* regex, int* index, token* lastToken);
token* tokenizeCarret(char* regex, int* index, token* lastToken);
token* tokenizeDollar(char* regex, int* index, token* lastToken);


// Group functions
group* getNextGroup(token** currToken, group* lastGroup);
group* groupDot(token** currToken, group* lastGroup);
group* groupStar(token** currToken, group* lastGroup);
group* groupLiteral(token** currToken, group* lastGroup);
group* groupPlus(token** currToken, group* lastGroup);
group *groupQuestion(token **currToken, group *lastGroup);
group *groupParan(token **currToken, group *lastGroup);
group *groupCarret(token **currToken, group *lastGroup);
group *groupDollar(token **currToken, group *lastGroup);



// Helper functions
void reverseStr(char* str);
void moveIndex(int* index);
void moveIndexAmount(int* index, int amount);
void moveToken(token** currToken);
bool checkMatching(char* txt, int index, token* t);

// Init functions
token* initToken(char* str, int type, token* lastToken);
// TODO
group* initGroup(token* main, token* secondary, group* lastGroup);

// Free functions
void freeToken(token* token);
void freeGroup(group* group);

// Helpful functions
void printToken(token* token);
void printAllTokens(token* first);
void printGroup(group* group);
void printAllGroups(group* first);



void moveIndex(int* index)
{
    (*index)++;
}


void moveIndexAmount(int* index, int amount)
{
    (*index) += amount;
}

void moveToken(token** currToken)
{
    (*currToken) = (*currToken)->next;
}

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
        case PLUS:
            {
                printf("+");
                break;
            }
        case QUESTION:
            {
                printf("?");
                break;
            }
        case PARAN:
            {
                printf("[%s]", token->str);
                break;
            }
        case CARRET:
            {
                printf("^");
                break;
            }
        case DOLLAR:
            {
                printf("$");
                break;
            }
        case NONE:
            {
                printf("START");
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

void printGroup(group* group)
{
    printf("|");
    printToken(group->main);
    if (group->secondary != NULL)
    {
        printf("->");
        printToken(group->secondary);
    }
    printf("|");
}

void printAllGroups(group* first)
{
    group* curr = first;
    while (curr != NULL)
    {
        printGroup(curr);
        printf(" => ");
        curr = curr->next;
    }
    printf("NULL\n");
}

void freeToken(token* token)
{
    // Check if token and token->str are not NULL
    if (token == NULL)
        return;

    if (token->str != NULL)
        free(token->str);
    free(token);
}

void freeGroup(group* group)
{
    freeToken(group->main);
    freeToken(group->secondary);
    free(group);
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
    // t->next = lastToken;
    if (lastToken != NULL)
    {
        lastToken->next = t;
    }
    return t;
}


group* initGroup(token* main, token* secondary, group* lastGroup)
{
    // Allocate memory for the token
    group* g = (group*)malloc(sizeof(group));
    g->main = main;
    g->secondary = secondary;
    // if (lastGroup != NULL)
    //     lastGroup->next = g;
    g->next = lastGroup;
    return g;
}

token* tokenizeDot(int* index, token* lastToken)
{
    moveIndex(index);
    return initToken(NULL, DOT, lastToken);
}

token* tokenizeStar(char* regex, int* index, token* lastToken)
{
    // Create star token
    token* starToken = initToken(NULL, STAR, lastToken);
    moveIndex(index);
    
    return seperateNextToken(regex, index, starToken);
}

 
token* tokenizePlus(char* regex, int* index, token* lastToken)
{

    // Create star token
    token* plusToken = initToken(NULL, PLUS, lastToken);
    moveIndex(index);
    
    return seperateNextToken(regex, index, plusToken);
}
token* tokenizeQuestion(char* regex, int* index, token* lastToken)
{

    // Create Question token
    token* questionToken = initToken(NULL, QUESTION, lastToken);
    moveIndex(index);
    
    return seperateNextToken(regex, index, questionToken);
}

token* tokenizeCarret(char* regex, int* index, token* lastToken)
{
    moveIndex(index);
    return initToken(NULL, CARRET, lastToken);
}

token* tokenizeDollar(char* regex, int* index, token* lastToken)
{
    moveIndex(index);
    return initToken(NULL, DOLLAR, lastToken);
}

token* seperateNextToken(char* regex, int* index, token* lastToken)
{

    // create fake index to not tamper with the existing index
    int tempIndex = *index;
    // Get the next token in order for the star token to work properly
    token* nextToken = getNextToken(regex, index, lastToken);
    // If the next token is literal, take only the first letter and make it a seperate token
    if (nextToken->type == LITERAL)
    {
        // Allocate memory for newText instead of using the same memory as nextToken
        char* newTxt = strndup(nextToken->str + strlen(nextToken->str) - 1, 1);
        *index = tempIndex;
        moveIndex(index);
        freeToken(nextToken);
        return initToken(newTxt, LITERAL, lastToken);
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

token* tokenizeParentheses(char* regex, int* index, token* lastToken)
{
    int firstIndex = *index;
    int regexLen = strlen(regex);

    // Go forward in the regex until we meet a non-letter character to calculate the length of the literal
    for (;*index < regexLen && '[' != regex[*index]; (*index)++) {};
    int strLen = *index - firstIndex;

    // Allocate memory and copy from regex to a new string
    // We don't want the [] to be in the str, because we know it's a [] from the type
    char* str = strndup(regex + firstIndex + 1, strLen - 1);
    
    moveIndex(index);
    // Reverse the string because the regex is reversed
    reverseStr(str);
    return initToken(str, PARAN, lastToken);
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
        case '+':
            {
                return tokenizePlus(regex, index, lastToken);
            }
        case '?':
            {
                return tokenizeQuestion(regex, index, lastToken);
            }
        case ']':
            {
                return tokenizeParentheses(regex, index, lastToken);
            }
        case '^':
            {
                return tokenizeCarret(regex, index, lastToken);
            }
        case '$':
            {
                return tokenizeDollar(regex, index, lastToken);
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
    // token* first = getNextToken(regex, &regIndex, NULL);
    token* first = initToken(NULL, NONE, NULL);
    token* curr = first;
    // Go over the the regex from the end
    while (regIndex < len && curr != NULL)
    {
        // The function will get the curr and set it's next to the token, and then will return the token
        curr = getNextToken(regex, &regIndex, curr);
    }
    // Reverse the string back to normal
    reverseStr(regex);
    return first;
}

group* groupDot(token** currToken, group* lastGroup)
{
    group* g =  initGroup(*currToken, NULL, lastGroup);
    moveToken(currToken);
    return g;
}

group* groupStar(token** currToken, group* lastGroup)
{
    token* starToken = *currToken;
    moveToken(currToken);
    group* g =  initGroup(starToken, *currToken, lastGroup);
    moveToken(currToken);
    return g;
}

group* groupPlus(token** currToken, group* lastGroup)
{
    token* plusToken = *currToken;
    moveToken(currToken);
    group* g =  initGroup(plusToken, *currToken, lastGroup);
    moveToken(currToken);
    return g;
}

group *groupQuestion(token **currToken, group *lastGroup)
{
    token* questionToken = *currToken;
    moveToken(currToken);
    group* g =  initGroup(questionToken, *currToken, lastGroup);
    moveToken(currToken);
    return g;
}

group *groupParan(token **currToken, group *lastGroup)
{
    group* g = initGroup(*currToken, NULL, lastGroup);
    moveToken(currToken);
    return g;
}

group* groupLiteral(token** currToken, group* lastGroup)
{
    group* g =  initGroup(*currToken, NULL, lastGroup);
    moveToken(currToken);
    return g;
}

group* groupCarret(token** currToken, group* lastGroup)
{
    group* g =  initGroup(*currToken, NULL, lastGroup);
    moveToken(currToken);
    return g;
}

group* groupDollar(token** currToken, group* lastGroup)
{
    group* g =  initGroup(*currToken, NULL, lastGroup);
    moveToken(currToken);
    return g;
}

group* getNextGroup(token** currToken, group* lastGroup)
{
    switch ((*currToken)->type)
    {
        case DOT:
            {
                return groupDot(currToken, lastGroup);
                break;
            }
        case STAR:
            {
                return groupStar(currToken, lastGroup);
                break;
            }
        case LITERAL:
            {
                return groupLiteral(currToken, lastGroup);
                break;
            }
        case PLUS:
            {
                return groupPlus(currToken, lastGroup);
                break;
            }
        case QUESTION:
            {
                return groupQuestion(currToken, lastGroup);
                break;
            }
        case PARAN:
            {
                return groupParan(currToken, lastGroup);
                break;
            }
        case CARRET:
            {
                return groupCarret(currToken, lastGroup);
                break;
            }
        case DOLLAR:
            {
                return groupDollar(currToken, lastGroup);
                break;
            }
    }
    return NULL;
}

group* tokensToGroups(token* tokens)
{
    moveToken(&tokens);
    token* currToken = tokens;
    group* first = getNextGroup(&currToken, NULL);
    group* curr = first;
    while (currToken != NULL)
    {
        curr = getNextGroup(&currToken, curr);
    }
    return curr;
}



bool parseGroupDot(group* g, char* txt, int* index)
{
    moveIndex(index);
    return parseGroup(g->next, txt, index); // check the next groups
}

bool between(char c, char min, char max)
{
    return c >= min && c <= max;
}

bool checkParan(char c, token* t)
{
    int len = strlen(t->str);
    for (int i = 0; i < len; i++)
    {
        if (c == t->str[i])
            return true;
        if (t->str[i] == '-' && between(c, t->str[i-1], t->str[i+1]))
            return true;
    }
    return false;
}

bool checkMatching(char* txt, int index, token* t)
{
    switch (t->type)
    {
        case DOT:
            {
                return true;
            }
        case LITERAL:
            {
                return t->str[0] == txt[index];
            }
        case PARAN:
            {
                return checkParan(txt[index], t);
            }
        default:
        {
            printf("ERROR: UNKOWN TYPE: %d", t->type);
            return false;
        }
    }
    return false;
}

bool parseGroupStar(group* g, char* txt, int* index)
{
    // moveIndexAmount(index, -1);
    // int savedIndex = *index;
    // do
    // {
    //     moveIndex(index);
    //     if (parseGroup(g->next, txt, index))
    //         return true;
    //     savedIndex++;
    //     *index = savedIndex;
    // } while (checkMatching(txt, *index, g->secondary) && *index < strlen(txt));
    // return false;
    return parseGroupAmount(g, txt, index, 0, strlen(txt));
}


bool parseGroupPlus(group* g, char* txt, int* index)
{
    return checkMatching(txt, *index, g->secondary) && parseGroupAmount(g, txt, index, 1, strlen(txt));
}

bool parseGroupQuestion(group* g, char* txt, int* index)
{
    return parseGroupAmount(g, txt, index, 0, *index + 1);
}

bool parseGroupParan(group* g, char* txt, int* index)
{
    return checkMatching(txt, *index, g->main);
}

bool parseGroupAmount(group* g, char* txt, int* index, int min, int max)
{
    moveIndexAmount(index, min-1);
    int savedIndex = *index;
    do
    {
        moveIndex(index);
        if (parseGroup(g->next, txt, index))
            return true;
        savedIndex++;
        *index = savedIndex;
    } while (checkMatching(txt, *index, g->secondary) && *index < strlen(txt) && *index < max);
    return false;
}

bool parseGroupLiteral(group* g, char* txt, int* index)
{
    // Check length of the txt against the index
    if (strlen(txt) <= *index)
        return false;
    if (0 == strncmp(txt + *index, g->main->str, strlen(g->main->str)))
    {
        moveIndexAmount(index, strlen(g->main->str));
        return parseGroup(g->next, txt, index); // check the next groups

    }
    return false;
}

bool parseGroupCarret(group* g, char* txt, int* index)
{
    return *index == 0 && parseGroup(g->next, txt, index);
}

bool parseGroupDollar(group* g, char* txt, int* index)
{
    // The dollar is always the last group
    return *index == strlen(txt);
}

bool parseGroup(group* group, char* txt, int* index)
{

    // Function will return if the current group matches
    if (group == NULL)
        return true;
    // printGroup(group);
    // printf("%d\n", *index);
    switch (group->main->type)
    {
        case DOT:
            {
                return parseGroupDot(group, txt, index);
            }
        case STAR:
            {
                return parseGroupStar(group, txt, index);
            }
        case LITERAL:
            {
                return parseGroupLiteral(group, txt, index);
            }
        case PLUS:
            {
                return parseGroupPlus(group, txt, index);
            }
        case QUESTION:
            {
                return parseGroupQuestion(group, txt, index);
            }
        case PARAN:
            {
                return parseGroupParan(group, txt, index);
            }
        case CARRET:
            {
                return parseGroupCarret(group, txt, index);
            }
        case DOLLAR:
            {
                return parseGroupDollar(group, txt, index);
            }
        default:
            {
                return false;
            }
    }
    return false;
}

// Function will parse regex and will return true or false if the txt matches the regex
bool parseRegex(char *regex, char *txt)
{
    if (strlen(regex) == 0)
        return strlen(txt) == 0;
    // Convert regex to tokens
    token* tokens = regexToTokens(regex);
    // Group tokens
    group* groups = tokensToGroups(tokens);
    // Go through the groups and the txt
    int txtIndex = 0;
    for (int i = 0; i < strlen(txt); i++)
    {
        txtIndex = i;
        if (parseGroup(groups, txt, &txtIndex))
            return true;
    }
    return false;
}

void test(char* regex, char* txt, bool expected)
{
    // Create new str for the regex
    if (strlen(regex) == 0)
        return;
    char* newRegex = strdup(regex);
    // Print the test, the tokens, and the groups
    printf("\nRegex: \"%s\", Text: \"%s\"\n", regex, txt);
    printAllGroups(tokensToGroups(regexToTokens(newRegex)));
    bool result = parseRegex(newRegex, txt);
    printf("\nRegex: \"%s\", Text: \"%s\" → %s (Expected: %s)",
           regex, txt, result ? "MATCH" : "NO MATCH",
           expected ? "MATCH" : "NO MATCH");
    printf(" Correct? %s\n", result == expected ? "YES" : "NO");
}



int main() {
    printf("Running tests...\n");

    // --- Literals ---
    test("abc", "abc", true);
    test("abc", "abcd", false);
    test("abc", "aBc", false);  // Case-sensitive test
    test("hello", "hello", true);
    test("hello", "hell", false);

    // --- Wildcard '.' ---
    test("a.c", "abc", true);
    test("a.c", "adc", true);
    test("a.c", "ac", false);   // Missing middle character
    test("a.c", "abdc", false); // Extra character in between

    // --- Quantifiers: '*' (zero or more) ---
    test("a*b", "b", true);     // Zero 'a'
    test("a*b", "ab", true);    // One 'a'
    test("a*b", "aaab", true);  // Multiple 'a's
    test("a*b", "a", false);    // Must end in 'b'

    // --- Quantifiers: '+' (one or more) ---
    test("a+b", "b", false);    // Needs at least one 'a'
    test("a+b", "ab", true);
    test("a+b", "aaab", true);
    test("a+b", "a", false);

    // --- Quantifiers: '?' (zero or one) ---
    test("a?b", "b", true);
    test("a?b", "ab", true);
    test("a?b", "aab", false);
    test("a?b", "a", false);

    // --- Character Classes '[]' ---
    test("[abc]", "a", true);
    test("[abc]", "b", true);
    test("[abc]", "c", true);
    test("[abc]", "d", false);
    test("[a-z]", "m", true);   // Lowercase range
    test("[a-z]", "Z", false);  // Uppercase should not match

    // --- Character Classes with Quantifiers ---
    test("[aeiou]+", "e", true);
    test("[aeiou]+", "aeiou", true);
    test("[aeiou]+", "bcd", false);
    test("[0-9]+", "12345", true);
    test("[0-9]+", "abc", false);

    // --- Anchors: '^' (Start of String) ---
    test("^abc", "abc", true);
    test("^abc", "xabc", false);
    test("^abc", "abcdef", true);

    // --- Anchors: '$' (End of String) ---
    test("abc$", "abc", true);
    test("abc$", "abcx", false);
    test("abc$", "xabc", false);
    test("abc$", "xyzabc", true);

    // --- Both Anchors '^' and '$' ---
    test("^abc$", "abc", true);  // Must match exactly
    test("^abc$", "abcd", false);
    test("^abc$", "xabc", false);

    printf("Tests completed.\n");
    return 0;
}
