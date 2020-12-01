//Project 2

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int main()
{
	//variables declared
	string cheeseType;
	double value;
	string importer;
	double amount = 0;
	
	//takes in cheese type
	cout << "Cheese type: ";
	getline(cin, cheeseType);

	//takes in value
	cout << "Value: ";
	cin >> value;
	
	//This is to keep errors from occuring after inputting double and using getline for string afterwards
	cin.ignore(10000, '\n');
	
	//takes in importer
	cout << "Importer: ";
	getline(cin, importer);
	
	cout << "---" << endl;

	

	//if statements to check for empty strings and non-positive values
	if (cheeseType == "")
	{
		cout << "You must enter a cheese type" << endl;
		return 1;
	}
	else if (value <= 0)
	{
		cout << "The value must be positive" << endl;
		return 1;
	}
	else if (importer == "")
	{
		cout << "You must enter an importer" << endl;
		return 1;
	}
	
	//deals with duty for first 1000
	amount += value * .011;
	
	//sets the correct rate for the next 12000 dollars and above that and calculates the added duty 
	if (value > 1000)
	{ 
		//setting up variables that hold the percentage duty rates to add on top of the already 1.1%
		double rateBetween1000and13000 = 0.02 - 0.011;
		double rateAbove1300 = 0.029 - 0.02;
		
		if (cheeseType == "cheshire" || cheeseType == "stilton")
		{
			rateBetween1000and13000 = 0.014 - 0.011;
			rateAbove1300 = 0.029 - 0.014;
		}
			
		amount += (value - 1000) * (rateBetween1000and13000);
		
		//deals with duty over 13000
		if (value > 13000)
			amount += rateAbove1300 * (value - 13000);
	}

	//Will make the double be output in a format like dollars look like usually
	cout.setf(ios::fixed);
	cout.precision(2);
	
	//Will output the final sentence
	cout << "The import duty for " << importer << " is $" << amount << endl;

}