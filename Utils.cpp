#include "Utils.h"
#include <fstream>
#include <ctype.h>
#include <iostream>

using namespace std;


Utils::Utils()
{
}

/*
int Utils::getNumberOfChar(ifstream &file)
{
	int nbOfChar = 0;
	char c;
	string occ;
	while (file.get(c)) { // Lecture du fichier char by char
		if (isalpha(c)) // Si le caractère est une lettre
		{
			if (occ.find(c) == string::npos) { // Si la lettre n'a pas été vu précédemment
				occ += c; // On ajoute la lettre à notre string de vérification
				nbOfChar++; // On incrémente le nombre de caractères trouvés
			}
		}
	}

	return nbOfChar;
}
*/
void Utils::lineJump(int nbOfJump)
{
	for (int i = 0; i < nbOfJump; i++)
	{
		cout << "" << endl;
	}
}

void Utils::consoleClear()
{
#if _WIN32
	system("cls");
#elif __linux__
		system("clear");
#endif
}

string Utils::ToString(int val) const
{
	return to_string(val);
}
