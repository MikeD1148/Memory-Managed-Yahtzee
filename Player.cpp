#include "Player.h"
#include <iostream>
#include <string>

using namespace std;

Player::Player(char* name, char* password, int highestScore)
{
	this->highestScore = highestScore;
	this->name = name;
	this->password = password;
}

//Destructor freeing memory from char pointers when object is deleted
Player::~Player()
{
	free(name);
	free(password);
}

//Displaying name and highscore from current object
void Player::displayPlayer() const
{
	cout << name << "\t\t\t";
	cout << highestScore << "\n";
}

//Alphabetical sort function using strings
bool Player::isNameLessThan(const Player* player) const
{
	string name1 = name;
	string name2 = player->name;
	return (name1 < name2);
}

//Highest score sort function
bool Player::isScoreLessThan(const Player* player) const
{
	int score1 = highestScore;
	int score2 = player->highestScore;
	return (score1 > score2);
}

//Operator overloading output
ostream& operator<<(ostream& os, const Player& rhs)
{
	os << rhs.name << "," << rhs.password << "," << rhs.highestScore;
	return os;
}

//Operator overloading input
istream& operator>>(istream& is, Player& rhs)
{
	is.getline(rhs.name, sizeof(char) * 20, ',');
	is.getline(rhs.password, sizeof(char) * 20, ',');
	is >> rhs.highestScore;
	//Removes endl from input stream
	is.ignore();
	return is;
}