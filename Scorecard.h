#include <iostream>
#include <fstream>

using namespace std;

#pragma once
class Scorecard
{
public:
	Scorecard(const Scorecard& scorecard);
	//Constructor
	Scorecard(string date, int Aces, int Twos, int Threes, int Fours, int Fives, int Sixes);
	//Getter for total
	int getTotal();
	//Getter for date
	string getDate();
	void displayScorecard() const;
private:
	string date;
	int Aces;
	int Twos;
	int Threes;
	int Fours;
	int Fives;
	int Sixes;
	int total;
	friend ostream& operator<<(ostream& os, const Scorecard& rhs);
	friend istream& operator>>(istream& is, Scorecard& rhs);
};

//Inline functions

inline int Scorecard::getTotal()
{
	total = Aces + Twos + Threes + Fours + Fives + Sixes;
	return total;
}

inline string Scorecard::getDate()
{
	return date;
}