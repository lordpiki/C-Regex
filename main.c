#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// Declarations of structures



// Declaraions of functions
int main();
bool parseRegex(char *regex, char *txt);
void addToLiteral(char* literal, char letter);
bool checkMatching(char* txt, int txtIndex, char token);
bool isLetter(char c);



bool isLetter(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}


bool checkMatching(char* txt, int txtIndex, char token)
{
    return token == '.' || txt[txtIndex] == token;
}


void addToLiteral(char* literal, char letter)
{
}

// Function implementations
bool parseRegex(char *regex, char *txt)
{
    return false;
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
