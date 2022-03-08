#include "Scorecard.h"
#include <string>

Scorecard::Scorecard(string date, int Aces, int Twos, int Threes, int Fours, int Fives, int Sixes)
{
	this->date = date;
	this->Aces = Aces;
	this->Twos = Twos;
	this->Threes = Threes;
	this->Fours = Fours;
	this->Fives = Fives;
	this->Sixes = Sixes;
}

//Display scorecard for current object
void Scorecard::displayScorecard() const
{
	cout << "Date:\t" << date << endl << "\n";
	cout << "----- SCORECARD -----" << endl;
	cout << "\tAces:\t\t" << Aces << endl;
	cout << "\tTwos:\t\t" << Twos << endl;
	cout << "\tThrees:\t\t" << Threes << endl;
	cout << "\tFours:\t\t" << Fours << endl;
	cout << "\tFives:\t\t" << Fives << endl;
	cout << "\tSixes:\t\t" << Sixes << endl;
	cout << "--------------------------" << endl;
	cout << "Total:\t\t\t" << Aces + Twos + Threes + Fours + Fives + Sixes << endl;
	cout << "--------------------------" << endl << "\n\n";
	cout << "1) View the previous scorecard" << endl;
	cout << "2) View the next scorecard" << endl;
	cout << "9) Return to player statistics screen" << endl;
	cout << "\nPlease choose an option:" << endl;
}

//Operator overloading output stream for the scorecard class
ostream& operator<<(ostream& os, const Scorecard& rhs)
{
	os << rhs.date << "," << rhs.Aces << "," << rhs.Twos << "," << rhs.Threes << "," << rhs.Fours << "," << rhs.Fives << "," << rhs.Sixes;
	return os;
}

//Operator overloading input stream for the scorecard class
istream& operator>>(istream& is, Scorecard& rhs)
{
	char t;
	getline(is, rhs.date, ',');
	is >> rhs.Aces;
	is >> t;
	is >> rhs.Twos;
	is >> t;
	is >> rhs.Threes;
	is >> t;
	is >> rhs.Fours;
	is >> t;
	is >> rhs.Fives;
	is >> t;
	is >> rhs.Sixes;
	is.ignore();
	return is;
}