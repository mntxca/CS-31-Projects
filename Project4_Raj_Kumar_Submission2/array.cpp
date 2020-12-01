#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

// function prototypes
int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
bool consecutiveCheck(const string a1[], int n1, const string a2[], int n2, int i); //made to help subsequence
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int divide(string a[], int n, string divider);
bool sortStringArray(string a[], int n); //made to help divide
int printArray(string a[], int n); //helps check other functions

int main()
{
	string h[7] = { "martha", "mark", "joe", "susan", "", "kamala", "lindsey" };
	assert(lookup(h, 7, "kamala") == 5);
	assert(lookup(h, 7, "joe") == 2);
	assert(lookup(h, 2, "joe") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "martha", "mark", "lindsey", "sara" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "martha?" && g[3] == "sara?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "lindsey?" && g[3] == "mark?");

	string e[4] = { "joe", "susan", "", "kamala" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "mark", "mark", "mark", "susan", "susan" };
	assert(countRuns(d, 5) == 2);

	string f[3] = { "lindsey", "joe", "mike" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "lindsey");

	assert(divide(h, 7, "lindsey") == 3);
	
	//TESTS FOR EACH FUNCTION, ONE BY ONE

	//appendToAll tests
	cerr << endl << "appendToAll tests" << endl;
	string people[5] = { "donald", "joe", "mike", "lindsey", "kamala" };
	assert(appendToAll(people, 5, "!!!") == 5);  // returns 5
		// now people[0] is "donald!!!", people[1] is "joe!!!", ...,
		// and people[4] is "kamala!!!"
	assert(appendToAll(people, -1, "!!!") == -1); //returns -1, since you can't have negative array length
	assert(appendToAll(people, 0, "!!!") == 0);//checks if returns 0 because this changes nothing and n=0
	printArray(people, 5); //to see if array actually changed or not

	//positionOfMax tests
	cerr << endl << "positionOfMax tests" << endl;
	string candidate[6] = { "jamie", "lindsey", "mark", "susan", "joe", "donald" };
	string d7[5] = { "mark", "mark", "mark", "susan", "susan" };
	assert(positionOfMax(candidate, 6) == 3);   // returns 3, since  susan  is latest in alphabetic order
	assert(positionOfMax(candidate, 3) == 2);	//returns 2, since mark is latest in alphabetical order
	assert(positionOfMax(candidate, 0) == -1);	//returns -1, since you can't find a max in nothing
	assert(positionOfMax(candidate, -1) == -1);	//returns -1, since you can't have negative array length
	assert(positionOfMax(d7, 5) == 3);	//returns 3, since susan is latest in alphabetical order and appears first at index 3

	//rotateLeft tests
	cerr << endl << "rotateLeft tests" << endl;
	string politician[5] = { "kamala", "jamie", "lindsey", "sara", "mark" };
	string candidate123[6] = { "jamie", "lindsey", "mark", "susan", "joe", "donald" };
	assert(rotateLeft(politician, 5, 1) == 1);  // returns 1 to check if the correct result of 1 is returned, to check if it just works 
		// politician now contains:  "kamala", "lindsey", "sara", "mark", "jamie"
	assert(rotateLeft(candidate123, 6, 3) == 3); //to check if the correct result of 3 is returned, to check if it just works
	assert(rotateLeft(candidate123, 6, 7) == -1); //can't look out of bounds
	assert(rotateLeft(candidate123, 6, -1) == -1); //can't look out of bounds
	assert(rotateLeft(politician, 0, 1) == -1); //n is 0 so return -1, said in FAQ #4

	//countRuns tests
	cerr << endl << "countRuns tests" << endl;
	string d2[9] = {"susan", "donald", "mike", "mike", "joe", "joe", "joe", "mike", "mike"};
	assert(countRuns(d2, 9) == 5);  //  returns 5
		   //  The five sequences of consecutive identical items are
		   //    "susan"
		   //    "donald"
		   //    "mike", "mike"
		   //    "joe", "joe", "joe"
		   //    "mike", "mike"
	assert(countRuns(d2, -1) == -1);	//-1 because you can't have negative number of elements
	assert(countRuns(d2, 0) == 0);	//0 runs in empty array, check with smallberg on monday
	assert(countRuns(d2, 1) == 1);	//one run if only one element
	assert(countRuns(d7, 5) == 2);	//mark is first run, susan is second run

	//flip tests
	cerr << endl << "flip tests" << endl;
	string folks[6] = { "donald", "mike", "", "susan", "sara", "jamie" };
	assert(flip(folks, 4) == 4);  // returns 4
		// folks now contains:  "susan"  ""  "mike"  "donald"  "sara"  "jamie"
	assert(flip(d2, 8) == 8); // see if this flips a longer string correctly or not
	assert(flip(d2, 1) == 1); //see if this is the same or not
	assert(flip(folks, 0) == 0);  //makes no sense to flip an empty array

	//differ tests
	cerr << endl << "differ tests" << endl;
	string folks2[6] = { "donald", "mike", "", "susan", "sara", "jamie" };
	string folks21[6] = { "donald", "mike", "", "susan", "sara", "jamie" };
	string folks22[7] = { "donald", "mike", "", "susan", "sara", "jamie", "kungpaochicken"};
	string group[5] = { "donald", "mike", "jamie", "", "susan" };
	//to check if program just works or not in regular cases
	assert(differ(folks2, 6, group, 5) == 2);  //  returns 2
	assert(differ(folks2, 2, group, 1) == 1);  //  returns 1
	assert(differ(folks2, 6, folks21, 6) == 6);  //  returns 1
	assert(differ(folks2, 6, folks22, 7) == 6);  //  returns 6
	//to check if program understands that an input of n1 or n2 being 0 will mean that they differ from the very start, the 0th index
	assert(differ(folks2, 6, folks22, 0) == 0);  //  returns 0
	assert(differ(folks2, 0, folks22, 6) == 0);  //  returns 0
	assert(differ(folks2, 0, folks22, 0) == 0);  //  returns 0
	//to check if function recognizes these are invalid inputs, can't have negative array length
	assert(differ(folks2, 6, folks22, -2) == -1);  //  returns -1
	assert(differ(folks2, -2, folks22, 3) == -1);  //  returns -1
	//to check if n1 or n2 is returned (whichever is shorter) if reached end of either string
	assert(differ(folks2, 6, folks2, 6) == 6);  //  returns 6

	//subsequence tests
	cerr << endl << "subsequence tests" << endl;
	string names[10] = { "kamala", "mark", "sara", "martha", "donald", "lindsey" };
	string names1[10] = { "mark", "sara", "martha" };
	string names2[10] = { "kamala", "martha" };
	string names3[1] = { "lindsey" };
	string names4[10] = {"sara", "martha", "donald" };
	assert(subsequence(names, 6, names1, 3) == 1);  // returns 1 b/c mark is at index 1 in names
	assert(subsequence(folks2, 5, folks21, 2) == 0);  // returns 0 to check what happens if second string array is part of first string array from very start (index 0)
	assert(subsequence(folks2, 5, folks2, 6) == -1);  // returns -1 b/c n2 can't fit inside n1 and function recognizes this
	assert(subsequence(names, 6, names4, 3) == 2);  // returns 2 b/c sara is at index 2 in names
	assert(subsequence(names, 6, names3, 1) == 5);  // returns 5 b/c lindsey is at index 5 in names
											   
	//lookupAny tests
	cerr << endl << "lookupAny tests" << endl;
	string set1[10] = { "jamie", "donald", "martha", "mark" };
	//to see if regular test cases work as expected
	assert(lookupAny(names, 6, set1, 4) == 1);  // returns 1 (a1 has "mark" there)
	assert(lookupAny(names, 6, folks2, 5) == 2);  // returns 2 (a1 has "sara" there)
	assert(lookupAny(names, 6, d2, 9) == 4);  // returns 1 (a1 has "mark" there)
	string set2[10] = { "susan", "joe" };
	assert(lookupAny(names, 6, set2, 2) == -1);  // returns -1 to check if function can see that there is no matches
	// to see if having 0 length n1 or n2 is correctly identified as -1, which shows there can't be a match between the two arrays if one is empty
	assert(lookupAny(names, 6, set2, 0) == -1);  // returns -1
	assert(lookupAny(names, 0, set2, 6) == -1);  // returns -1 

	//divide tests
	cerr << endl << "divide tests" << endl;
	string candidate45[6] = { "jamie", "lindsey", "mark", "susan", "joe", "donald" };
	assert(divide(candidate45, 6, "kamala") == 3);  //  returns 3
		// candidate must now be
		//      "jamie"  "joe"  "donald"  "lindsey"  "susan"  "mark"
		// or   "joe"  "donald"  "jamie"  "mark"  "lindsey"  "susan"
		// or one of several other orderings.
		// All elements < "kamala" (i.e., "joe", "donald", and "jamie")
		//   come before all others
		// All elements > "kamala" (i.e., "susan", "lindsey", and "mark")
		//   come after all others
	string candidate56[4] = { "mark", "sara", "lindsey", "mike" };
	assert(divide(candidate56, 4, "mike") == 2);  //  returns 2
		// candidate2 must now be either
		//      "mark"  "lindsey"  "mike"  "sara"
		// or   "lindsey"  "mark"  "mike"  "sara"
		// All elements < "mike" (i.e., "lindsey" and "mark") come
			// before all others.
		// All elements > "mike" (i.e., "sara") come after all others.
	string candidate1234[1] = { "mark" };
	string candidate12345[3] = { "mark", "mike", "pence" };
	//to check if regular cases work or not
	assert(divide(candidate1234, 1, "mike") == 1);  //  returns 1 b/c mike is greater than the one string in candidate 1234
	assert(divide(candidate1234, 1, "a") == 0);  //  returns 0 b/c index 0 works
	//odd or edge cases (explanation why is given)
	assert(divide(candidate, 6, "z") == 6);  //  returns 6, which is n, to check what happens when z is greater than all strings in candidate
	assert(divide(candidate12345, 3, "a") == 0);  //  returns 0, to check what happens when a is less than all strings in candidate12345

	cerr << endl << "All tests succeeded" << endl;
}

//complete, completely tested and commented
int appendToAll(string a[], int n, string value)
{
	//do not return -1 if n is 0 b/c FAQ #4
	if (n < 0)
		return -1;
	//loops through and adds value to each element in the array
	for (int i = 0; i < n; i++)
		a[i] += value;
		return n;
}

//complete, main tested and commented 
int lookup(const string a[], int n, string target)
{
	//included 0 b/c target string won't be in array if array is empty
	if (n <= 0)
		return -1;
	//loops through and checks if the target and the element at index i match, if yes, returns i value, if not keeps on going
	//if no match is found after the loop, return -1 to show no matches were found
	for (int i = 0; i < n; i++)
		if (a[i] == target)
			return i;
	return -1;
}

//complete, completely tested and commented
int positionOfMax(const string a[], int n)
{
	//says return -1 if string has no interesting elements in SPEC
	if (n <= 0)
		return -1;
	//use this max variable to keep track of the index of the maximum value string as you iterate through the array of strings
	int max = 0;
	//loop through string array, comparing the max value found up until then to the string at the current index i, 
	//setting max to i if the element at index i is greater than the element at index max
	for (int i = 1; i < n; i++)
		if (a[i] > a[max])
			max = i;
	return max;
}

// complete, completely tested and commented 
int rotateLeft(string a[], int n, int pos)
{
	//error if array length is negative, and you can't rotate an array of 0 length (FAQ #4)
	if (n <= 0)
		return -1;
	//negative index position (which would be out of bounds) is an error
	if (pos < 0)
		return -1;
	//error if position is after the last index of the array (FAQ #4)
	if (pos > n - 1)
		return -1;
	printArray(a, n);
	//make temporary string to store the value that will go to the end of the array
	string temp;
	temp = a[pos];
	//loop through and move each element one index lower, or one index to the left
	for (int i = pos; i < n - 1; i++)
	{
		a[i] = a[i+1];
	}
	//set the last index in array to the value of the element that was originaly at the index of pos
	a[n - 1] = temp;
	printArray(a, n);
	return pos;
}

//complete, completely tested and commented
int countRuns(const string a[], int n)
{
	if (n < 0)
		return -1;
	//makes sense for this one to just return 0 for arrays with 0 length because there are 0 runs
	if (n == 0)
		return 0;
	//having atleast one element means atleast one run
	int runCount = 1;
	//loops through array and checks if consecutive elements are different, if yes, adds one to the runcount
	for (int i = 1; i < n; i++)
	{
		if (a[i] != a[i - 1])
			runCount++;
	}
	return runCount;
}

//complete, completely tested and commented
int flip(string a[], int n) 
{
	//makes most sense if you return 0 when n is 0 because you can just do nothing to it
	if (n < 0)
		return -1;
	printArray(a, n);
	// use temp string to help swap the values of the corresponding indexes
	string temp;
	//loops through and swaps the first and last, then second and second last, then third and third last...etc
	for (int i = 0; i < (n/2); i++)
	{
		temp = a[n - i - 1];
		a[n - i - 1] = a[i];
		a[i]=temp;
	}
	printArray(a, n);
	return n;
}

//complete, completely tested and commented
int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)
		return -1;
	// lowestLength is the lower length out of the two strings to keep...
	//for loop from going out of bounds of either array
	//if either n1 or n2 is 0 then this will return 0 b/c it will skip loop and return lowestLength, which is what makes sense from SPEC
	int lowestLength;
	if (n1 < n2)
		lowestLength = n1;
	else
		lowestLength = n2;
	//loops through and returns the first time a1 and a2 have a different element
	for (int i = 0; i < lowestLength; i++)
	{
		if (a1[i] != a2[i])
			return i;
	}
	//returns lowestLength if one of them ends before any differences or both are duplicate arrays
	return lowestLength;
}

//complete, completely tested and commented
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	//first check if n2 is 0, as this is always subsequent to EVERYTHING
	if (n2 == 0)
		return 0;
	//Second check if n1 is 0, as this is won't allow anything to be subsequent of it unless n2 == 0
	else if (n1 == 0)
		return -1;
	//Third check if n2 is larger than n1, as a2 can't fit and be subsequent if its larger than a1
	else if (n2 > n1)
		return -1;
	//if all tests are "passed" run the following code
	else 
	{
		//loop through each index of a1, and check if the first element of a2 matches
		//if match, will call funtion consecutive check to see if the entire a2 array is there as part of a1 starting at the index i
		for (int i = 0; i < n1; i++)
		{
				if (a1[i] == a2[0])
				{
					if (consecutiveCheck(a1, n1, a2, n2, i))
						return i;
				}
		}
	}
	//if there was no times in which n2 was subsequent, return -1
	return -1;
}

//complete, completely tested (through subsequence calls) and commented
bool consecutiveCheck(const string a1[], int n1, const string a2[], int n2, int i)
{
	//make variable to keep track of how many consecutive matches there are
	int consecutiveCount = 0;
	int j = 0;
	//make sure the a2 can even fit in n1 starting from index i of a1
	if (n1 < i + n2)
		return false;
	//loop through and check each consecutive index to see if the entire a2 actually is part of a1 element by element
	for (;n1>=i+n2;) 
	{
		//if there are the same amount as consecutives as n2, true!
		if (n2 == consecutiveCount)
			return true;
		//if you made it to after the last index of either array, return false
		if (j == n2 || i == n1)
			return false;
		//if the two strings match, add one to the counter that keeps track of the number of consecutive matches
		//if they don't match, n2 is not part of n1 starting at the originally inputted i value, so return false
		if (a1[i] == a2[j])
			consecutiveCount++;
		else
			return false;
		i++;
		j++;
	}

}

//complete, completely tested and commented 
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	//first check if n1 or n2 is 0, as there are no common strings if there are no strings in one 
	if (n2 <= 0 || n1 <= 0)
		return -1;
	//loop and try to match each index of a1 (0 to n1 - 1)to any index of a2 (0 to n2 - 1 ), if there is a match, return the current i value
	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			if (a1[i] == a2[j])
				return i;
		}
	}
	//no match means there was no common elements, which returns -1
	return -1;
}

//complete, completely tested and commented
int divide(string a[], int n, string divider)
{
	// can't have negative array lengths
	if (n < 0)
		return -1;
	//makes sense to just return 0 as you can't really do much and it doesn't seem to be an error
	else if (n == 0)
		return 0;
	//to see the "before" part of the before-after transformation that is common in ads
	printArray(a, n);
	//sort first
	sortStringArray(a, n);
	//ensures that n is returned if no strings are greater than the divider
	if (divider > a[n - 1])
		return n;
	//checks if the element at index i is greater than or equal to divider, which will be the correct value to return if sorted properly
	for (int i = 0; i < n; i++)
	{
		if (!(a[i] < divider))
			return i;
	}
	return n;
}

//complete, completely tested (as part of divide tests) and commented
bool sortStringArray(string a[], int n)
{
	//empty array is already sorted
	if (n == 0)
		return true;
	//sorts all the strings in increasing order by comparing neighboring strings and swapping them if they are in the incorrect order
	//this process is repeated n times to ensure the entire array is sorted
	for (int i = 1; i < n; i++)
	{
		for (int i = 1; i < n; i++)
		{
			string temp;
			if (a[i - 1] > a[i])
			{
				temp = a[i];
				a[i] = a[i - 1];
				a[i - 1] = temp;
			}
		}
	}
	//check how the array looks after sort to see if there is a difference
	printArray(a, n);
	//sorted now, so return true
	return true;
}

//complete, completely tested (as part of divide) and human tested and commented
int printArray(string a[], int n)
{
	//prints an array by looping through each index and printing each element one by one in a particular format
	if (n == 0)
		return 0;
	cerr << "{";
	for (int i = 0; i < n; i++)
	{
		if(i==n-1)
			cerr << "\"" << a[i] << "\"";
		else
			cerr << "\"" << a[i] << "\", ";
	}
	cerr << "}" << endl;
	return 1;
}

