#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cassert>
#include <cstring>
#include <cctype>
using namespace std;
const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_CHAR_NOT_INCLUDING_ZERO_BYTE = 250;
const int MAX_DOC_CHAR_INCLUDING_ZERO_BYTE = MAX_DOC_CHAR_NOT_INCLUDING_ZERO_BYTE + 1;

int removeRule(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int n, int pos);
int cleanupRules(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nRules);
bool checkLettersAndLower(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int pos);
int findWords(char words[][MAX_WORD_LENGTH + 1], char doc[], int i);
int findNextSpace(char Cstring[], int pos);
int copyCString(char destination[], char source[], int length);
bool findMatch(const char input[], char words[][MAX_WORD_LENGTH + 1], int wordCount);


int printCString(char a[][MAX_WORD_LENGTH+1], int n)
{
    //prints an array by looping through each index and printing each element one by one in a particular format
    if (n == 0)
        return 0;
    cerr << "{";
    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
            cerr << "\"" << a[i] << "\"";
        else
            cerr << "\"" << a[i] << "\", ";
    }
    cerr << "}" << endl;
    return 1;
}

// complete, not tested
int removeRule(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int n, int pos)
{
    //error if array length is negative, and you can't remove from an array of 0 length (FAQ #4)
    if (n <= 0)
        return -1;
    //negative index position (which would be out of bounds) is an error
    if (pos < 0)
        return -1;
    //error if position is after the last index of the array (FAQ #4)
    if (pos > n - 1)
        return -1;
    //loop through and move each element one index lower, or one index to the left
    for (int i = pos; i < n - 1; i++)
    {
        strcpy(wordin[i], wordin[i + 1]);
        strcpy(wordout[i], wordout[i + 1]);
        //cstring equivalent of wordin[i] = wordin[i + 1];
        //cstring equivalent of wordout[i] = wordout[i + 1];
    }

    printCString(wordin, n);
    printCString(wordout, n);
    cerr << endl;
    return pos;
}



int cleanupRules(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nRules)
{
    printCString(wordin, nRules);
    printCString(wordout, nRules);
    cerr << endl;

    if (nRules <= 0)
        return 0;
    //first I will make sure all the match rules are cleaned up WITHOUT removing duplicates yet
    int removeCount = 0;
    int nRulesLeft = nRules;
    for (int i = 0; i < nRules - removeCount; i++)
    {
        //A win word is the empty string. (or for C string, 0th index is zero byte)
        if (wordin[i][0] == '\0')
        {
            if (nRules - removeCount > 0)
            {
                removeRule(wordin, wordout, nRules - removeCount, i);
                removeCount++;
                i--;
            }
            //else
        }

        //A word in the match rule contains a character that is not a letter.
        //needs to be before checking if win and wout are same because you need to take in account of cases where...
        //win is Family and wout is FAMIly, which is that they are the same word
        else if (checkLettersAndLower(wordin, wordout, i) == false)
        {
            removeRule(wordin, wordout, nRules - removeCount, i);
            removeCount++;
            i--;
        }

        //The win word and the wout words are the same.
        else if (strcmp(wordin[i], wordout[i]) == 0)
        {
            removeRule(wordin, wordout, nRules - removeCount, i);
            removeCount++;
            i--;
        }


    }
    //now dealing with the duplicates
    
    { 
        for (int i = 0; i < nRules - removeCount; i++)
        {
            for (int j = i + 1; j < nRules - removeCount; j++)
            {
                if (i >= 0)
                {
                    if (strcmp(wordin[i], wordin[j]) == 0) //if two different wordin match
                    {
                        if (wordout[i][0] == '\0') //checks if the first rule's wordout is a empty Cstring, which is a one word rule
                        {
                            removeRule(wordin, wordout, nRules - removeCount, j); //will then remove the other rule
                            removeCount++;
                            j--;
                        }
                        else if (wordout[j][0] == '\0') //checks if the first rule's wordout is a empty Cstring, which is a one word rule
                        {
                            removeRule(wordin, wordout, nRules - removeCount, i); //will then remove the other rule
                            removeCount++;
                            i--;
                        }
                        //by now, the scenarios where there was a one-word rule have been taken care of
                        //now, we must check if both wout words are the same and remove one of the rules if they are
                        //if both win words are same but both wout words aren't the same, nothing will be removed
                        else if (strcmp(wordout[i], wordout[j]) == 0)
                        {
                            removeRule(wordin, wordout, nRules - removeCount, j); //will then remove one of the two word rules
                            removeCount++;
                            j--;
                        }
                    }
                }
            }
        }
    }
    printCString(wordin, nRules - removeCount);
    printCString(wordout, nRules - removeCount);
    return nRules - removeCount;
}

bool checkLettersAndLower( char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int pos)
{
    for (int j = 0; wordin[pos][j] !='\0'; j++)
    {
        if (isalpha(wordin[pos][j]) == false)
            return false;
        else
            wordin[pos][j]=tolower(wordin[pos][j]);
    }
    
    for (int k = 0; wordout[pos][k] != '\0'; k++)
    {
        if (isalpha(wordout[pos][k]) == false)
            return false;
        else
            wordout[pos][k] = tolower(wordout[pos][k]);
    }
    return true;
}



int determineScore(const char document[], const char wordin[][MAX_WORD_LENGTH + 1], const char wordout[][MAX_WORD_LENGTH + 1], int nRules)
{
    cerr << document << "end" << endl;
    //finds the length of the document (including zero byte) so we can use this as a bound later
    int documentLength;
    for (documentLength = 0; document[documentLength] != '\0'; documentLength++);
    //checks if the document is just empty, which would have a score of 0
    if (documentLength == 0)
        return 0;
    //two more just in case I add spaces to the starting and end of a perfectly formatted document which has no spaces at start or end
    char doc[MAX_DOC_CHAR_INCLUDING_ZERO_BYTE + 2];
    for (int i = 0; i < MAX_DOC_CHAR_INCLUDING_ZERO_BYTE + 2; i++)
        doc[i] = '\0';
    //first, get rid of all non alphabets 
    int i;
    int j = 0;
    int offset = 0;
    bool previousNonAlpha = false;
    if (!isspace(document[0]))
    {
        doc[0] = ' ';
        offset = 1;
    }
    for (i=0 ; i + j < documentLength; )
    {
        if (isalpha(document[i+j]))
        {
            doc[i + offset] = tolower(document[i + j]);
            i++;
            previousNonAlpha = false;
        }
        //skips duplicate spaces
        else if(isspace(document[i + j]))
        {
            if (previousNonAlpha)
                j++;
            else {
                doc[i + offset] = document[i + j];
                i++;
            }
            previousNonAlpha = true;
        }
        else
        {
            j++;
        }
            
    }
    //ignore warning below, isn't possible to reach here if i is 0 or less and thus doc[i - 1] can't be out of bounds
    //doc[i] = ' ';
    //doc[i + 1] = '\0';
    if (doc[i] != ' ')
    {
        doc[i + 1] = ' ';
        doc[i + 2] = '\0';
    }
    else
        doc[i + 1] = '\0';
    

    cerr << doc << "end" << endl;
    //second, go through the document and find where words are (we can store the words in an array of Cstrings, if that makes it easier)
    //only half + 1 words can fit, because even if all words were one letter long, you would still have spaces between each word (except last or first word)
    char words[MAX_DOC_CHAR_INCLUDING_ZERO_BYTE / 2 + 1][MAX_WORD_LENGTH + 1];
    int wordCount = findWords(words, doc, documentLength);
    //third, match the rules and take in account score
    int matchCount = 0;
    for (int i = 0; i < nRules; i++)
    {
        if (wordout[i] == "") //check if it is a one word rule
        {
            if (findMatch(wordin[i], words, wordCount))
                matchCount++;
        }
        //must be a two word rule, if you make it here
        else
        {
            //check if there is a match between the wordin word and there is no matches for the wordout word, and increment matchCount accordingly
            if (findMatch(wordin[i], words, wordCount) && !(findMatch(wordout[i], words, wordCount)))
                matchCount++;
        }
    }
    return matchCount;
}

bool findMatch(const char input[], char words[][MAX_WORD_LENGTH + 1], int wordCount)
{
    for (int i = 0; i < wordCount; i++)
        if (strcmp(input, words[i]) == 0)
            return true;
    return false;
}

int findWords(char words[][MAX_WORD_LENGTH + 1], char doc[], int documentLength)
{
    int wordCount = 0;
    int wordLength = 0;
    int lastSpaceIndex = 0;
    int nextSpaceIndex;

    //fix below code
    /*for (int i = 0; doc[i] != '\0'; i++)
    {
        if (strchr(&doc[lastSpaceIndex + 1], ' ') == nullptr) //if no space before zero byte, returns a nullptr
        {
            printCString(words, wordCount);
            return wordCount;
        }
        nextSpaceIndex = strchr(&doc[lastSpaceIndex + 1], ' ') - &doc[lastSpaceIndex + 1];
        strncpy(words[i], &doc[lastSpaceIndex + 1], nextSpaceIndex-1);
        wordCount++;
        lastSpaceIndex = nextSpaceIndex;
    }*/
    //stores text between spaces into the words array of cstrings
    for (int i = 0; (doc[i] != '\0' && i < MAX_DOC_CHAR_INCLUDING_ZERO_BYTE + 2); i++)
    {
        nextSpaceIndex = findNextSpace(doc, lastSpaceIndex + 1);
        if (nextSpaceIndex == -1)
            return wordCount;
        if (nextSpaceIndex - lastSpaceIndex == 1)
            return wordCount;
        //copies the text between two spaces into words array of cstrings
        copyCString(words[i], &doc[lastSpaceIndex + 1], nextSpaceIndex - lastSpaceIndex - 1);
        wordCount++;
        //sets up for next iteration (moves forward in doc)
        lastSpaceIndex += nextSpaceIndex - lastSpaceIndex;
        printCString(words, wordCount);
    }
    printCString(words, wordCount);
    return wordCount;
}

int copyCString(char destination[], char source[], int length)
{
    int i;
    if (length > MAX_WORD_LENGTH)
        destination[0] = '\0';// since no rules should be able to match since they are limited to just 20 characters not including zero byte
    else
    {
        for (i = 0; (source[i] != '\0' && i < length); i++)
        {
            if (i == MAX_WORD_LENGTH)
            {
                destination[i] = '\0';
                return -1; //means string was too long and zero byte cut it off
            }
            destination[i] = source[i];
        }
        destination[i] = '\0';
    }
    return 0; //means everything was copied without a problem
}


int findNextSpace(char Cstring[], int pos)
{
    int originalPosition = pos;
    if (pos < 0)
        return -1;
    for (int i = pos; (Cstring[i] != 0 && i < MAX_DOC_CHAR_INCLUDING_ZERO_BYTE + 2); i++)
        if (Cstring[i] == ' ')
            return i;
    return -1;
}

int main()
{
    // cleanupRules Tests
    //test 0
    cerr << "test 0" << endl;
    char test0win[12][MAX_WORD_LENGTH + 1] = {
        "confusion", "FAMILY","charm",      "hearty","house"};
    char test0wout[12][MAX_WORD_LENGTH + 1] = {
        "",          "TIES",  "confusion", "hearty","intrigue"};
    assert(cleanupRules(test0win, test0wout, 5) == 4);

    //test 001
    cerr << "test 001" << endl;
    char test001win[2][MAX_WORD_LENGTH + 1] = {
        "", "FAMILY"
    };
    char test001wout[2][MAX_WORD_LENGTH + 1] = {
        "",  "TIES",  
    };
    assert(cleanupRules(test001win, test001wout, 2) == 1);

    //test 0001
    cerr << "test 0001" << endl;
    char test0001win[11][MAX_WORD_LENGTH + 1] = {
        "", "FAMILY","charm",      "hearty","house",  "worn-out","family",  "charm",   "ties",         "",     "charm",
    };
    char test0001wout[11][MAX_WORD_LENGTH + 1] = {
        "",          "TIES",  "confusion", "hearty","intrigue", "younger","first",      "",     "family", "frightened",     "",
    };
    assert(cleanupRules(test0001win, test0001wout, 11) == 5);
    
    //test 01
    cerr << "test 01" << endl;
    char test01win[12][MAX_WORD_LENGTH + 1] = {
        "", "FAMILY","charm",      "hearty","house",  "worn-out","family",  "charm",   "ties",         "",     "charm","FaMiLy"
    };
    char test01wout[12][MAX_WORD_LENGTH + 1] = {
        "",          "TIES",  "confusion", "hearty","intrigue", "younger","first",      "",     "family", "frightened",     "",    "tIeS"
    };
    assert(cleanupRules(test01win, test01wout, 12) == 5);
    
   

    //test 02
    cerr << "test 02" << endl;
    char test02win[12][MAX_WORD_LENGTH + 1] = {
        "confusion", "FAMILY","charm",      "hearty","house",  "worn-out","family",  "charm",   "ties",         "",     "charm","FaMiLy"
    };
    char test02wout[12][MAX_WORD_LENGTH + 1] = {
        "",          "TIES",  "confusion", "hearty","intrigue", "younger","first",      "",     "family", "frightened",     "",    "tIeS"
    };
    assert(cleanupRules(test02win, test02wout, 12) == 6);

    //test 03
    cerr << "test 03" << endl;
    char test03win[12][MAX_WORD_LENGTH + 1] = {
        "confusion", "CoNFUSION","charm",      "hearty","confuSIOn ",  "confusion","family",  "charm",   "ties",
    };
    char test03wout[12][MAX_WORD_LENGTH + 1] = {
        "hello",          "TIES",  "confusion", "hearty","hello", "younger","first",      "",     "family",
    };
    assert(cleanupRules(test03win, test03wout, 9) == 6);

    //test 04
    cerr << "test 04" << endl;
    char test04win[12][MAX_WORD_LENGTH + 1] = {
        "confusion", "CoNFUSION","charm",      "hearty","confuSIOn ",  "confusion","family",  "charm",   "ties", "confusion"
    };
    char test04wout[12][MAX_WORD_LENGTH + 1] = {
        "hello",          "TIES",  "confusion", "hearty","hello", "younger","first",      "",     "family", ""
    };
    assert(cleanupRules(test04win, test04wout, 10) == 4);

    //test 05
    cerr << "test 05" << endl;
    char test05win[12][MAX_WORD_LENGTH + 1] = { "" };
    char test05wout[12][MAX_WORD_LENGTH + 1] = { "" };
    assert(cleanupRules(test05win, test05wout, 1) == 0);

    //test 06
    cerr << "test 06" << endl;
    char test06win[12][MAX_WORD_LENGTH + 1] = { "" };
    char test06wout[12][MAX_WORD_LENGTH + 1] = { "" };
    assert(cleanupRules(test06win, test06wout, -1) == 0);

    //test 07
    cerr << "test 07" << endl;
    assert(cleanupRules(test04win, test04wout, -1) == 0);
    assert(cleanupRules(test04win, test04wout, 0) == 0);
    
    //Tests given by Prof Smallberg for the second function (with exception of my own created cleanupRules tests below)
    const int TEST1_NRULES = 3;
    char test1win[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
        "family", "unhappy", "horse",
    };
    char test1wout[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
        "",       "horse",   "",};

    assert(cleanupRules(test1win, test1wout, 3) == 3);
   
    assert(determineScore("horse:stable ratio is 10:1",
        test1win, test1wout, TEST1_NRULES) == 0);
    //my own addition
    assert(determineScore(" Happy  families are all alike ;  every unhappy family is unhappy in its own way . ",
        test1win, test1wout, TEST1_NRULES) == 2);
    //my own addition
    assert(determineScore(" Happy  families are all alike ;  every unhappy family is unhappy in its own way .  .  . ",
        test1win, test1wout, TEST1_NRULES) == 2);

    assert(determineScore("Happy families are all alike; every unhappy family is unhappy in its own way.",
        test1win, test1wout, TEST1_NRULES) == 2);

    
    //My own modified tests
    assert(determineScore("Happy horses are all alike; every unhappy horse is unhappy in its own way.",
        test1win, test1wout, TEST1_NRULES - 1) == 0);
    assert(determineScore("Happy horses are all alike; every unhappy horse is unhappy in its own way.",
        test1win, test1wout, TEST1_NRULES) == 1);
    assert(determineScore("A horse!  A horse!  My kingdom for a horse!",
        test1win, test1wout, TEST1_NRULES) == 1);
    
    assert(determineScore("**** 2020 ****",
        test1win, test1wout, TEST1_NRULES) == 0);
    assert(determineScore("",
        test1win, test1wout, TEST1_NRULES) == 0);
    assert(determineScore("In Yuval Noah Harari's Big data, google, and the end of free will, Harari describes the concept of Dataism, which he defines as the belief thateverything can be broken down to data at a fundamental level, and that people are algorithms that process  ", test1win, test1wout, TEST1_NRULES) == 0  );
    assert(determineScore("In Yuval Noah Harari's Big data, google, and the end of free will, Harari describes the concept of Dataism, which he defines as the belief thateverything can be broken down to data at a fundamental level, and that people are algorithms that process  ", test1win, test1wout, -1) == 0);

    
    const int TEST2_NRULES = 6;
    const int TEST3_NRULES = 5;
    char test2win[TEST2_NRULES][MAX_WORD_LENGTH + 1] = {
        "family", "unhappy", "horse", "unhappy", "unhappy", "unhappy"
    };
    char test2wout[TEST2_NRULES][MAX_WORD_LENGTH + 1] = {
        "",       "horse",   "",        "happy", "all", "hello"};
    char test3win[TEST3_NRULES][MAX_WORD_LENGTH + 1] = {
       "family", "unhappy", "horse", "data", "kungpao"
    };
    char test3wout[TEST3_NRULES][MAX_WORD_LENGTH + 1] = {
        "",       "horse",   "", "hello", "harari" };
    assert(cleanupRules(test2win, test2wout, 3) == 3);

    assert(determineScore("horse:stable ratio is 10:1",
        test2win, test2wout, TEST2_NRULES) == 0);
    //my own addition
    assert(determineScore(" Happy  families are all alike ;  every unhappy family is unhappy in its own way . ",
        test2win, test2wout, TEST2_NRULES) == 3);
    //my own addition
    assert(determineScore(" Happy  families are all alike ;  every unhappy family is unhappy in its own way .  .  . ",
        test2win, test2wout, TEST2_NRULES) == 3);

    assert(determineScore("Happy families are all alike; every unhappy family is unhappy in its own way.",
        test2win, test2wout, TEST2_NRULES) == 3);



    assert(determineScore("Happy horses are all alike; every unhappy horse is unhappy in its own way.",
        test2win, test2wout, TEST2_NRULES) == 2);
    assert(determineScore("Happy horses are all alike; every unhappy horse is unhappy in its own way.",
        test2win, test2wout, TEST2_NRULES-1) == 1);
    assert(determineScore("A horse!  A horse!  My kingdom for a horse!",
        test3win, test3wout, TEST3_NRULES) == 1);

    assert(determineScore("**** 2020 ****",
        test3win, test3wout, TEST3_NRULES) == 0);
    assert(determineScore("",
        test3win, test3wout, TEST3_NRULES) == 0);
    assert(determineScore("In Yuval Noah Harari's Big data, google, and the end of free will, Harari describes the concept of Dataism, which he defines as the belief that everything can be broken down to data at a fundamental level, and that people are algorithms that process ", test3win, test3wout, TEST3_NRULES) == 1);
    assert(determineScore("InYuvalNoahHarari'sBigdata, google, and the end of free will, Harari describes the concept of Dataism, which he defines as the belief that everything can be broken down to data at a fundamental level, and that people are algorithms that process ", test3win, test3wout, TEST3_NRULES) == 1);
    assert(determineScore("InYuvalNoahHarari'sBi, google, and the end of free will, Harari describes the concept of Dataism, which he defines as the belief that everything can be broken down to data at a fundamental level, and that people are algorithms that process ", test3win, test3wout, TEST3_NRULES) == 1);

    cout << "All tests succeeded" << endl;
}