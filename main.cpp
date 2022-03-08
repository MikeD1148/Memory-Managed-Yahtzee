#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <iterator>
#include "Player.h"
#include "Scorecard.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <crtdbg.h>
#include <stdio.h>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

//Funciton initialisation
void choosePlayer(vector<Player*>& vectorStorage);
void addPlayer(vector<Player*>& vectorStorage);
void removePlayer(vector<Player*>& vectorStorage);
void sortPlayersAlphabetically(vector<Player*>& vectorStorage);
void sortByHighestScore(vector<Player*>& vectorStorage);
bool sortOnAlphabet(const Player* lhs, const Player* rhs);
bool sortOnHighestScore(const Player* lhs, const Player* rhs);
void writeBack(vector<Player*>& vectorStorage);
int playerYahtzeeMenu(string name, int highestScore);
void yahtzeeGame(vector<Scorecard*>& cardStorage);
void viewScorecards(vector<Scorecard*>& cardStorage, int totalGames, string name);
void displayDiceFace(const int diceface);
void displaySelectedDiceFace(const int selectedDiceFace);
string encrypt(string word);
string decrypt(string word);

int main()
{
	//Memory leak detector
#ifdef _DEBUG
	//  _CrtSetBreakAlloc(167);
	_onexit(_CrtDumpMemoryLeaks);
#endif
	//Make the vector to store player list
	vector<Player*> vectorStorage;
	//Player menu
	int option;
	//Open file stream
	ifstream input("storage.csv");
	//Successfully opened file stream
	if (input.is_open())
	{
		//Repeat for number of lines in csv file
		while (!input.eof())
		{
			//Allocate memory for name and password pointers
			char* charName = (char*)malloc(sizeof(char) * 20);
			char* charPassword = (char*)malloc(sizeof(char) * 20);
			//Create object pointer
			Player* a = new Player(charName, charPassword, 0);
			//Read data into object
			input >> *a;
			//Load object pointer into vector
			vectorStorage.push_back(a);
		}
		//Close input stream
		input.close();
		//Remove any empty objects from the vector as they're added to the vector if the file is empty
		vector<Player*>::const_iterator it4(vectorStorage.begin());
		while (it4 != vectorStorage.end())
		{
			string tempName = (*it4)->getName();
			if (tempName == "")
			{
				delete* it4;
				remove(vectorStorage.begin(), vectorStorage.end(), *it4);
				vectorStorage.pop_back();
				break;
			}
			++it4; 
		}

		do
		{
			//Iterate through vector printing
			cout << "\nWelcome to Champion Yahtzee\n---------------------------\nPlayer Name             Highest Score\n-----------             -------------\n";
			vector<Player*>::const_iterator it(vectorStorage.begin());
			while (it != vectorStorage.end())
			{
				(*it)->displayPlayer();
				++it;
			}
			//Message for player
			cout << "\n1) Choose Player\n2) Add Player\n3) Remove Player\n4) Sort Players Alphabetically\n5) Sort By Highest Score\n9) Exit\n";
			cout << "Please choose an option:\n";
			//User input
			cin >> option;
			//Directs player to their chosen input
			switch (option)
			{
			case 1: choosePlayer(vectorStorage); break;
			case 2: addPlayer(vectorStorage); break;
			case 3: removePlayer(vectorStorage); break;
			case 4: sortPlayersAlphabetically(vectorStorage); break;
			case 5: sortByHighestScore(vectorStorage); break;
			//Saves to csv file after user has exited application
			case 9: writeBack(vectorStorage); cout << "\n\tExit\n"; break;					
			default:
				cout << "Invalid input!\n";
			}

		} while (option != 9);

	//Deallocates all the memory in the vector
	for (vector<Player*>::iterator i = vectorStorage.begin(); i != vectorStorage.end(); ++i)
	{
		delete* i;
		*i = NULL;
	}
	}
	//Couldn't open file stream
	else
		cout << "Cannot open file for reading";
	//Program ends
	return 0;
}

void choosePlayer(vector<Player*>& vectorStorage)
{
	//Data type initialisation
	string searchName;
	string searchPassword; 
	bool noName = true;

	vector<Player*>::const_iterator it(vectorStorage.begin());
	cout << "Please enter Player name:" << endl;
	//Allowing for names with spaces
	cin.get();
	getline(cin, searchName);
	//Converting to lowercase as name is not case specific
	transform(searchName.begin(), searchName.end(), searchName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	//Iterate through vector searching for matching name
	while (it != vectorStorage.end()) {
		string name = (*it)->getName();
		//Converting to lowercase
		transform(name.begin(), name.end(), name.begin(),
			[](unsigned char c) { return std::tolower(c); });
		string password = decrypt((*it)->getPassword());
		//If the name was found
		if (searchName == name)
		{
			noName = false;
			//If password matches the players menu is loaded and returns the highest score to update the main csv file
			cout << "Please enter Player password:" << endl;
			cin >> searchPassword;
			if (searchPassword == password)
			{
				int highscore = (*it)->getHighestScore();
				(*it)->setHighestScore(playerYahtzeeMenu(name, highscore));
				break;

			}
			else
				cout << "Password was incorrect" << endl;
		}
		++it;
	}
	if (noName == true)
		cout << "There is no Player called: " << searchName;
}

void addPlayer(vector<Player*>& vectorStorage)
{
	//Data type initialisation
	string newNameString, tempName, tempNameString;
	bool inuse = false;
	//Player enters name
	cout << "Please enter a new Player name:\n(Your Player name and password must be less than 20 characters long)" << endl;
	cin.get();
	getline(cin, newNameString);
	//Iterator checks if name already exists or is too long, if not it will add the player to the vector and make an empty csv file
	vector<Player*>::const_iterator it(vectorStorage.begin());
	while (it != vectorStorage.end())
	{
		tempName = (*it)->getName();
		//Converting to lowercase
		transform(tempName.begin(), tempName.end(), tempName.begin(),
			[](unsigned char c) { return std::tolower(c); });
		tempNameString = newNameString;
		transform(tempNameString.begin(), tempNameString.end(), tempNameString.begin(),
			[](unsigned char c) { return std::tolower(c); });
		if (tempName == tempNameString)
		{
			inuse = true;
			break;
		}
		++it;
	}
	if (newNameString.length() > 20 || inuse == true)
		cout << "Name is invalid" << endl;

	//Creating new player
	else
	{
		//Allocating 20 characters to name and password so they can't be too long
		char* newName = (char*)malloc(sizeof(char) * 20);
		char* newPassword = (char*)malloc(sizeof(char) * 20);
		string passwordString;
		cout << "Please enter a password:" << endl;
		cin >> passwordString;
		passwordString = encrypt(passwordString);
		strcpy_s(newPassword, 20, passwordString.c_str());
		strcpy_s(newName, 20, newNameString.c_str());
		vectorStorage.push_back(new Player(newName, newPassword, 0));
		Scorecard temp("0", 0, 0, 0, 0, 0, 0);
		ofstream output1(newNameString + ".csv");
		output1 << temp;
		output1.close();
		ofstream output2(newNameString + ".csv");
		output2.close();
	}
}

void removePlayer(vector<Player*>& vectorStorage)
{
	//Data type initialisation
	string searchName, searchPassword;
	bool noName = true;
	vector<Player*>::const_iterator it(vectorStorage.begin());
	cout << "Please enter Player name:" << endl;
	//Allowing for names with spaces like iron man
	cin.get();
	getline(cin, searchName);
	string searchNameCopy =searchName + ".csv";
	//Converting to lowercase
	transform(searchName.begin(), searchName.end(), searchName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	//Iterate through vector searching for name not case specific
	while (it != vectorStorage.end()) {
		string name = (*it)->getName();
		//Converting to lowercase
		transform(name.begin(), name.end(), name.begin(),
			[](unsigned char c) { return std::tolower(c); });
		string password = decrypt((*it)->getPassword());
		//Finding player
		if (searchName == name)
		{
			noName = false;
			cout << "Please enter Player password:" << endl;
			cin >> searchPassword;
			//Validating password (password is encrypted in csv file so it has to be decrypted)
			if (searchPassword == password)
			{
				//Player is removed from vector and the players scorecard file is deleted
				const char* temp = searchNameCopy.c_str();
				remove(temp);
				delete* it;
				remove(vectorStorage.begin(), vectorStorage.end(), *it);
				vectorStorage.pop_back();
				break;
			}
			else
			{
				cout << "Password was incorrect" << endl;
				break;
			}
		}
		++it;
		
	}
	if (noName == true)
		{
		cout << "There is no Player called: " << searchName << endl;
		}

}

void sortPlayersAlphabetically(vector<Player*>& vectorStorage)
{
	//Sort vector algorithm to sort player's names
	sort(vectorStorage.begin(), vectorStorage.end(), sortOnAlphabet);
}

void sortByHighestScore(vector<Player*>& vectorStorage)
{
	//Sort vector algorithm to sort player's high scores
	sort(vectorStorage.begin(), vectorStorage.end(), sortOnHighestScore);
}

bool sortOnAlphabet(const Player* lhs, const Player* rhs)
{
	return (*lhs).isNameLessThan(rhs);
}

bool sortOnHighestScore(const Player* lhs, const Player* rhs)
{
	return (*lhs).isScoreLessThan(rhs);
}

void writeBack(vector<Player*>& vectorStorage)
{
	
		//Write
		ofstream output("storage.csv");
		//Check file is open
		if (output.is_open())
		{
			for (vector<Player*>::iterator it = vectorStorage.begin(); it != vectorStorage.end(); ++it)
			{
				//Create object pointer
				Player* a = *it;
				if (it != vectorStorage.end() - 1)
				{
					output << *a << "\n";
				}
				else
				{
					output << *a;
				}
			}
			output.close();
	
		}

		else
			cout << "Cannot open file for writing";
			
}

int playerYahtzeeMenu(string name, int highestScore)
{
	
	//Make the vector to store scorecard list							I was unable to dynamically resize a 2 dimensional array without memory leaks
	vector<Scorecard*> cardStorage;
				ifstream input(name+".csv");
				int option;
				int i = 0;
				int accumulatedScore = 0;
				while (!input.eof())
				{
					//Create object pointer
					Scorecard* a = new Scorecard("", 0, 0, 0, 0, 0, 0);
					//Read data into object
					input >> *a;
					//Load object pointer into vector
					cardStorage.push_back(a);
					i++;
				}
				input.close();

				vector<Scorecard*>::const_iterator it4(cardStorage.begin());
				while (it4 != cardStorage.end())
				{
					string tempDate = (*it4)->getDate();
					if (tempDate == "")
					{
						delete* it4;
						remove(cardStorage.begin(), cardStorage.end(), *it4);
						cardStorage.pop_back();
						i--;
						break;
					}
					++it4;
				}

				bool playerscreen = true;
				while (playerscreen == true)
				{
					vector<Scorecard*>::const_iterator it(cardStorage.begin());
					//Get accumulated score
					int Acc = 0;
					while (it != cardStorage.end())
					{
						Acc += (*it)->getTotal();
						++it;
					}
					cout << "\n\nWelcome to Champion Yahtzee " << name << endl;
					cout << "--------------------------------------" << endl;
					cout << "Highest Score:		" << highestScore << endl;
					cout << "Total Games:		" << i << endl;
					cout << "Accumulated Score:	" << Acc << endl;
					if (i != 0)
					{
						cout << "AverageScore:		" << Acc / i << endl;
					}
					else
					{
						cout << "AverageScore:		0" << endl;
					}
					cout << "\n1) Play game" << endl;
					cout << "2) View historic scorecards" << endl;
					cout << "9) Return to player chooser screen" << endl;
					cout << "\nPlease choose an option:" << endl;

					cin >> option;
					switch (option)
					{
					case 1: yahtzeeGame(cardStorage); i++; break;
					case 2: viewScorecards(cardStorage, i, name); break;
					case 9: cout << "\n\n"; playerscreen = false; break;
					default:
						cout << "Invalid input!\n";
					}

					vector<Scorecard*>::const_iterator it2(cardStorage.begin());
					//Update highest score
					int temp;
					while (it2 != cardStorage.end())
					{
						temp = (*it2)->getTotal();
						if (temp > highestScore)
							highestScore = temp;
						++it2;
					}
				}

				//Write
				ofstream output(name + ".csv");
				//Check file is open
				if (output.is_open())
				{
					for (vector<Scorecard*>::iterator it3 = cardStorage.begin(); it3 != cardStorage.end(); ++it3)
					{
						//Create object pointer
						Scorecard* a = *it3;
						if (it3 != cardStorage.end() - 1)
						{
							output << *a << "\n";
						}
						else
						{
							output << *a;
						}
					}
					output.close();
				}

				else
					cout << "Cannot open file for writing";

				//Deallocates all the memory in the vector
				for (vector<Scorecard*>::iterator i = cardStorage.begin(); i != cardStorage.end(); ++i)
				{
					delete* i;
					*i = NULL;
				}
				return 	highestScore;
}

void yahtzeeGame(vector<Scorecard*>& cardStorage)
{
	int dice1, dice2, dice3, dice4, dice5;
	int Aces = 0; int Twos = 0; int Threes = 0; int Fours = 0; int Fives = 0; int Sixes = 0;
	int selectDice, score;
	bool select1, select2, select3, select4, select5, reselect1, reselect2, reselect3, reselect4, reselect5, reselect6, invalid;
	reselect1 = false; 
	reselect2 = false; 
	reselect3 = false; 
	reselect4 = false; 
	reselect5 = false;
	reselect6 = false;
	invalid = false;
	for (int b = 0; b < 6; b++)
	{
		select1 = false;
		select2 = false;
		select3 = false;
		select4 = false;
		select5 = false;
		cout << "Dice Roll Set #" << (b + 1) << " of 6" << endl;
		cout << "---------------------------\n\n";
		cout << "---- SCORECARD ----" << endl;
		cout << "Aces:	" << Aces << endl;
		cout << "Twos:	" << Twos << endl;
		cout << "Threes:	" << Threes << endl;
		cout << "Fours:	" << Fours << endl;
		cout << "Fives:	" << Fives << endl;
		cout << "Sixes:	" << Sixes << endl;
		cout << "-------------------" << endl;
		cout << "Total: " << (Aces + Twos + Threes + Fours + Fives + Sixes) << endl;
		cout << "-------------------" << endl;

		for (int i = 0; i < 3; i++)
		{
			//Randomise and print dice if not selected
			if (select1 == false)
			{
				dice1 = rand() % 6 + 1;
				cout << "Dice 1" << endl;
				displayDiceFace(dice1);
			}
			if (select2 == false)
			{
				dice2 = rand() % 6 + 1;
				cout << "Dice 2" << endl;
				displayDiceFace(dice2);
			}

			if (select3 == false)
			{
				dice3 = rand() % 6 + 1;
				cout << "Dice 3" << endl;
				displayDiceFace(dice3);
			}

			if (select4 == false)
			{
				dice4 = rand() % 6 + 1;
				cout << "Dice 4" << endl;
				displayDiceFace(dice4);
			}

			if (select5 == false)
			{
				dice5 = rand() % 6 + 1;
				cout << "Dice 5" << endl;
				displayDiceFace(dice5);
			}



			cout << "Enter the dice number you wish to keep / press any number to continue:" << endl;
			bool selectDiceBool = true;
			int b = 0;
			while (selectDiceBool == true) {
				cin >> selectDice;
				if (selectDice > 0 && selectDice < 6)
				{
					switch (selectDice)
					{
					case 1:displaySelectedDiceFace(dice1); select1 = true; break;
					case 2:displaySelectedDiceFace(dice2); select2 = true; break;
					case 3:displaySelectedDiceFace(dice3); select3 = true; break;
					case 4:displaySelectedDiceFace(dice4); select4 = true; break;
					case 5:displaySelectedDiceFace(dice5); select5 = true; break;
					}
				}
				else {
					selectDiceBool = false;
				}

			}
		}
		invalid = false;
		while(invalid == false)
		{
		cout << "Please choose how to score your dice" << endl;
		if(reselect1 == false)
		{
			cout << "Aces" << endl;
		}
		if (reselect2 == false)
		{
			cout << "Twos" << endl;
		}
		if (reselect3 == false)
		{
			cout << "Threes" << endl;
		}
		if (reselect4 == false)
		{
			cout << "Fours" << endl;
		}
		if (reselect5 == false)
		{
			cout << "Fives" << endl;
		}
		if (reselect6 == false)
		{
			cout << "Sixes" << endl;
		}
		cout << "Score against:" << endl;
		cin >> score;
		switch (score)
		{
		case 1: {
			if (reselect1 == false)
			{
				invalid = true;
				reselect1 = true;
				if (dice1 == 1)
				{
					Aces += dice1;
				}
				if (dice2 == 1)
				{
					Aces += dice2;
				}
				if (dice3 == 1)
				{
					Aces += dice3;
				}
				if (dice4 == 1)
				{
					Aces += dice4;
				}
				if (dice5 == 1)
				{
					Aces += dice5;
				}
			}
		}break; 
		case 2: {
			if (reselect2 == false)
			{
				invalid = true;
				reselect2 = true;
				if (dice1 == 2)
				{
					Twos += (dice1 * 2);
				}
				if (dice2 == 2)
				{
					Twos += (dice2 * 2);
				}
				if (dice3 == 2)
				{
					Twos += (dice3 * 2);
				}
				if (dice4 == 2)
				{
					Twos += (dice4 * 2);
				}
				if (dice5 == 2)
				{
					Twos += (dice5 * 2);
				}
			}
		} break; 
		case 3: {
			if (reselect3 == false)
			{
				invalid = true;
				reselect3 = true;
				if (dice1 == 3)
				{
					Threes += (dice1 * 3);
				}
				if (dice2 == 3)
				{
					Threes += (dice2 * 3);
				}
				if (dice3 == 3)
				{
					Threes += (dice3 * 3);
				}
				if (dice4 == 3)
				{
					Threes += (dice4 * 3);
				}
				if (dice5 == 3)
				{
					Threes += (dice5 * 3);
				}
			}
		} break; 
		case 4: {
			if (reselect4 == false)
			{
				invalid = true;
				reselect4 = true;
				if (dice1 == 4)
				{
					Fours += (dice1 * 4);
				}
				if (dice2 == 4)
				{
					Fours += (dice2 * 4);
				}
				if (dice3 == 4)
				{
					Fours += (dice3 * 4);
				}
				if (dice4 == 4)
				{
					Fours += (dice4 * 4);
				}
				if (dice5 == 4)
				{
					Fours += (dice5 * 4);
				}
			}
		} break; 
		case 5: {
			if (reselect5 == false)
			{
				invalid = true;
				reselect5 = true;
				if (dice1 == 5)
				{
					Fives += (dice1 * 5);
				}
				if (dice2 == 5)
				{
					Fives += (dice2 * 5);
				}
				if (dice3 == 5)
				{
					Fives += (dice3 * 5);
				}
				if (dice4 == 5)
				{
					Fives += (dice4 * 5);
				}
				if (dice5 == 5)
				{
					Fives += (dice5 * 5);
				}
			}
		} break; 
		case 6: {
			if (reselect6 == false)
			{
				invalid = true;
				reselect6 = true;
				if (dice1 == 6)
				{
					Sixes += (dice1 * 6);
				}
				if (dice2 == 6)
				{
					Sixes += (dice2 * 6);
				}
				if (dice3 == 6)
				{
					Sixes += (dice3 * 6);
				}
				if (dice4 == 6)
				{
					Sixes += (dice4 * 6);
				}
				if (dice5 == 6)
				{
					Sixes += (dice5 * 6);
				}
			}
		} break; 
		}
		}
	}
	cout << "Dice Roll Set #6 of 6" << endl;
	cout << "---------------------------\n\n";
	cout << "---- SCORECARD ----" << endl;
	cout << "Aces:	" << Aces << endl;
	cout << "Twos:	" << Twos << endl;
	cout << "Threes:	" << Threes << endl;
	cout << "Fours:	" << Fours << endl;
	cout << "Fives:	" << Fives << endl;
	cout << "Sixes:	" << Sixes << endl;
	cout << "-------------------" << endl;
	cout << "Total: " << (Aces + Twos + Threes + Fours + Fives + Sixes) << endl;
	cout << "-------------------" << endl;
	//Getting the current time and putting it into a string
	auto t = time(nullptr);
	auto tm = *localtime(&t);
	ostringstream oss;
	oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
	auto str = oss.str();
	cardStorage.push_back(new Scorecard(str, Aces, Twos, Threes, Fours, Fives, Sixes));
	

}

void viewScorecards(vector<Scorecard*>& cardStorage, int totalGames, string name)
{
	//Data type initialisation
	int option;
	int i = 1;
	//Iterate through vector displaying scorecards
	vector<Scorecard*>::const_iterator it(cardStorage.begin());
	while (it != cardStorage.end())
	{
		cout << "\n" << name << " Yahtzee History" << endl;
		cout << "----------------" << endl;
		cout << "Game:	" << i << " of " << totalGames << endl;
		(*it)->displayScorecard();
		cin >> option;
		//If a scorecard below 0 or below is selected the player menu is shown
		if (option == 1) {
			i--;
			if (i < 1)
				break;
			else
			--it;
		}
		//If the number is greater than the amount of scorecards available the player menu is shown
		else if (option == 2) {
			++it;
			i++;
		}

		else if (option == 9){
			break;
		}
		else {
			cout << "Invalid input" << endl; break;
		}
			
		
	}
}

void displayDiceFace(const int diceface)
{
	//Graphical dice face are shown when the random numbers are selected to simulate rolling a dice
	cout << "*********\n";
	switch (diceface)
	{
	case 1:
		cout << "*   1   *\n*   1   *\n*   1   *\n*   1   *\n*   1   *\n";
		break;
	case 2:
		cout << "* 22222 *\n*     2 *\n* 22222 *\n* 2     *\n* 22222 *\n";
		break;
	case 3:
		cout << "* 33333 *\n*     3 *\n*  3333 *\n*     3 *\n* 33333 *\n";
		break;
	case 4:
		cout << "* 4   4 *\n* 4   4 *\n* 44444 *\n*     4 *\n*     4 *\n";
		break;
	case 5:
		cout << "* 55555 *\n* 5     *\n* 55555 *\n*     5 *\n* 55555 *\n";
		break;
	case 6:
		cout << "* 66666 *\n* 6     *\n* 66666 *\n* 6   6 *\n* 66666 *\n";
		break;
	}
	cout << "*********\n";
}

void displaySelectedDiceFace(const int selectedDiceFace)
{
	//Graphical design to show a selected dice face (differentiated by hashtags)
	cout << "#########\n";
	switch (selectedDiceFace)
	{
	case 1:
		cout << "#   1   #\n#   1   #\n#   1   #\n#   1   #\n#   1   #\n";
		break;
	case 2:
		cout << "# 22222 #\n#     2 #\n# 22222 #\n# 2     #\n# 22222 #\n";
		break;
	case 3:
		cout << "# 33333 #\n#     3 #\n#  3333 #\n#     3 #\n# 33333 #\n";
		break;
	case 4:
		cout << "# 4   4 #\n# 4   4 #\n# 44444 #\n#     4 #\n#     4 #\n";
		break;
	case 5:
		cout << "# 55555 #\n# 5     #\n# 55555 #\n#     5 #\n# 55555 #\n";
		break;
	case 6:
		cout << "# 66666 #\n# 6     #\n# 66666 #\n# 6   6 #\n# 66666 #\n";
		break;
	}
	cout << "#########\n";
}

string encrypt(string word)
{
	//An encryption routine that increases the ascii character of each char of the password by 1 and returns the encrypted password
	string encryptedPassword;
	char x;
	for (int i = 0; i < word.length(); i++)
	{
		x = (word.at(i)) + 1;
		encryptedPassword.push_back(x);
	}
	return encryptedPassword;
}

string decrypt(string word)
{
	//A decryption routine that decreases the ascii character of each char of the password by 1 and returns the decrypted password
	string decryptedPassword;
	char x;
	for (int i = 0; i < word.length(); i++)
	{
		x = (word.at(i)) - 1;
		decryptedPassword.push_back(x);
	}
	return decryptedPassword;
}