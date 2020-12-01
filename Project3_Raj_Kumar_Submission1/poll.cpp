//Project 3 poll.cpp code
#include <iostream>
#include <cctype>
#include <string>
#include <cassert>
using namespace std;

//declare all the functions
bool isSyntacticallyCorrect(string pollData);
int tallyVotes(string pollData, char party, int& voteTally);
bool isValidStateCode(string stateCode);
bool hasZeroElectoralVotesAnywhere(string pollData);

int main()
{
	assert(isSyntacticallyCorrect("38TXR55CAD")); //checks if this is correctly identified as correct
	assert(isSyntacticallyCorrect("38tXR55CAD")); //checks if program sees that tX is still Texas and thus still correct
	assert(!isSyntacticallyCorrect("38TXR55CAD1")); //checks if having correct syntax plus another extra letter is false
	assert(!isSyntacticallyCorrect("38MXR55CAD")); //checks if MX is recognized as not being a state
	int votes;
	votes = -999;    // so we can detect whether tallyVotes sets votes
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", 'd', votes) == 0 && votes == 84);
	votes = -999;    // so we can detect whether tallyVotes sets votes
	assert(tallyVotes("38TXR55CAD", '%', votes) == 2 && votes == -999);
	
	
	int v = 9999; 
	//checks that this is considered not correct syntax
	assert(tallyVotes("38TXR55CAD1", 'D', v) == 1 && v == 9999);
	assert(tallyVotes("29NYD38TXR4ID", 'D', v) == 1 && v == 9999);
	
	v = 1;
	//checks if 2 is returned b/c non alphabet party
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", '@', v) == 2 && v == 1);
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", '1', v) == 2 && v == 1);
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", '^', v) == 2 && v == 1);

	v = -70;
	//checks if 3 is returned and votes are unchanged if there is a state with 0 electoral votes
	assert(tallyVotes("29NDR38TXR0HID", 'D', v) == 3 && v == -70);
	assert(tallyVotes("0NYD38TXR4HID", 'D', v) == 3 && v == -70);
	assert(tallyVotes("29NYD0TXR4HID", 'D', v) == 3 && v == -70);

	//makes sure the empty string works (returns 0) but still doesn't change votes
	assert(tallyVotes("", 'D', v) == 0 && v == 0);

	//General Check
	assert(tallyVotes("29NYD38TXR4HID", 'D', v) == 0 && v == 33);
	
	v = 75;
	//sees if this correctly sets v to 0 or not
	assert(tallyVotes("29NYR", 'D', v) == 0 && v == 0);
	
	//Checks if the examples given in the spec work the same way as spec says
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", 'D', v) == 0 && v == 84);
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", 'R', v) == 0 && v == 44);
	assert(tallyVotes("38TXR55CAD6Msr29nYd06UTL", 'L', v) == 0 && v == 6);
	cout << "All tests succeeded" << endl;


}

//Works Correctly
bool isSyntacticallyCorrect(string pollData)

{
	//empty strings are ok
	if (pollData.length() == 0)
		return true;
	//loop that keeps checking for forecasts in the pollData
	//if there isn't a complete forecast (atleast 4 characters long) possible, then don't loop
	while (pollData.length() >= 4)
	{
		//check if first two characters of pollData are digits and then remove them from string
		if (isdigit(pollData.at(1)) && isdigit(pollData.at(0)))
			pollData = pollData.substr(2);
		//check if first characters of pollData is a digit and then remove it from string
		else if (isdigit(pollData.at(0)))
			pollData = pollData.substr(1);
		else
			return false;
		//check if the state code is valid and then delete state code from string
		if (isValidStateCode(pollData.substr(0, 2)))
			pollData = pollData.substr(2);
		else
			return false;
		//check if the party character is a letter and then remove from string
		if (!(isalpha(pollData.at(0))))
			return false;
		else
			pollData = pollData.substr(1);
			
	}
	//if there isn't a complete forecast (atleast 4 characters long) possible,
	//there must only be no characters left in the string for it to be correct syntax
	if (pollData.length() == 0)
		return true;
	//if there is still some characters left but not a complete forecast, 
	//this is incorrect syntax
	else
		return false;
}

//assumes that the pollData has passed isSyntacticallyCorrect successfully
bool hasZeroElectoralVotesAnywhere (string pollData)
{
	//empty strings are ok and have no 0 electoral votes (says on FAQ)
	if (pollData.length() == 0)
		return false;
	
	//loop that keeps checking for forecasts in the pollData
	//if there isn't a complete forecast (atleast 4 characters long) possible, then don't loop
	while (pollData.length() >= 4)
	{

		//check if first two characters of pollData are digits
		if (isdigit(pollData.at(1)) && isdigit(pollData.at(0)))
		{
			//if electoral vote is 00, there is an 0 electoral vote
			if (pollData.at(1) == '0' && pollData.at(0) == '0')
				return true;
			//remove these two characters from string
			pollData = pollData.substr(2);
		}

		//check if first character of pollData is digit and then see if its 0
		else if (isdigit(pollData.at(0)))
		{
			//if electoral vote is 0, there is an 0 electoral vote
			if (pollData.at(0) == '0')
				return true;
			//remove character from string
			pollData = pollData.substr(1);
		}
		//remove rest of the forecast (isn't relevant for this function)
		pollData = pollData.substr(3);
	}
	//if the entire thing has no 0 electoral votes found, there are no 0 electoral votes in string
	return false;
}

//Works Correctly
int tallyVotes(string pollData, char party, int& voteTally)
{
	// Preliminary Checks
	//Makes sure pollData has correct syntax
	if (!(isSyntacticallyCorrect(pollData)))
		return 1;
	//Makes sure party is a letter
	if (!(isalpha(party)))
		return 2;
	//Makes sure there are no zero electoral vote states
	if (hasZeroElectoralVotesAnywhere(pollData))
		return 3;

	// Actual tallyVotes logic below
		voteTally = 0;
	if (pollData.length() == 0)
		return 0;
	//repeat until another forecast can't fit in the pollData
	while (pollData.length() >= 4)
	{
		//if first two characters of pollData are digits
		if (isdigit(pollData.at(1)) && isdigit(pollData.at(0)))
		{
			//figures out the number of electoral votes there are in first two indexes of string
			int tare = '0';
			int tens = 10 * (pollData.at(0) - tare);
			int ones = pollData.at(1) - tare;
			
			//adds the number of electoral votes to the voteTally
			if (toupper(pollData.at(4)) == toupper(party))
			{
				voteTally += tens + ones;
			}
			//now delete the last forecast from the string
			pollData = pollData.substr(5);
		}
		//if only first character of pollData are digits
		else if (isdigit(pollData.at(0)))
		{
			//figures out the number of electoral votes there are in first index of string
			int tare = '0';
			int numVotes = pollData.at(0) - tare;
			//adds the number of electoral votes to the voteTally
			if (toupper(pollData.at(3)) == toupper(party))
			{
				voteTally += numVotes;
			}
		//now delete the last forecast from the string
		pollData = pollData.substr(4);
		}
	}
	//if it made it to the end, everything was successful so return 0
	return 0;
}

//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-letter state code, or
// false otherwise. Has been changed by Raj to work for uppercase and lowercase input

bool isValidStateCode(string stateCode)
{
	//changes the statecode input to UPPER CASE
	if (islower(stateCode.at(0)))
		stateCode[0] = toupper(stateCode.at(0));
	if (islower(stateCode.at(1)))
		stateCode[1] = toupper(stateCode.at(1));
	const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MO.MS.MT.NE.NV.NH.NJ.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2 &&
        stateCode.find('.') == string::npos &&  // no '.' in stateCode
        codes.find(stateCode) != string::npos);  // match found
}