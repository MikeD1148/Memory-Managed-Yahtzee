#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#pragma once
class Player
{
public:
	//Constructor
	Player(char* name, char* password, int highestScore);
	//Destructor
	~Player();
	void displayPlayer() const;
	//Getters for variables
	char* getName();
	char* getPassword();
	int getHighestScore();
	//Setter for highest score
	void setHighestScore(int highestscore);
	//Sort functions
	bool isNameLessThan(const Player* player) const;
	bool isScoreLessThan(const Player* player) const;
private:
	char* name;
	char* password;
	int highestScore;
	//Operator overloader friend so that they can access data members
	friend ostream& operator<<(ostream& os, const Player& rhs);
	friend istream& operator>>(istream& is, Player& rhs);
};

//Inline functions

inline char* Player::getName()
{
	return name;
}

inline char* Player::getPassword()
{
	return password;
}

inline int Player::getHighestScore()
{
	return highestScore;
}

inline void Player::setHighestScore(int highestscore)
{
	highestScore = highestscore;
}
