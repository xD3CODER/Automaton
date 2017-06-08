#include "E3-Alphabet.h"


Alphabet::Alphabet()
{
	// Constructeur
}

Alphabet::~Alphabet()
{
	// Deconstructeur
}

/**
 *	Ajoute un caractere Ã  la suite des autres caracteres deja  present
 */
void Alphabet::addCaratere(char nouveau)
{
	_caracteres.push_back(nouveau);
}

/**
 *	Retourne la taille de l'alphabet (methode size d'un vector)
 */
unsigned int Alphabet::getAlphabetSize() const
{
	return _caracteres.size();
}

/**
 *	Surcharge de l'operateur =, permettant de comparer deux alphabets
 */
Alphabet& Alphabet::operator=(const Alphabet& arg)
{
	_caracteres = arg._caracteres;

	return *this;
}

/**
 *	Recupere le vector contenant l'alphabet (ie: la liste des caracteres)
 */
std::vector<char> Alphabet::getAlphabet() const
{
	return _caracteres;
}

/**
 * Permet de savoir si un caractere est contenu dans l'alphabet
 */
bool Alphabet::inAlphabet(char arg)
{
	unsigned int i;

	// On parcourt l'alphabet
	for (i = 0; i < _caracteres.size(); i++)
	{
		if (_caracteres[i] == arg) // Si le caractere est dans le "vector"
			return true; // Alors il est contenu dans l'alphabet
	}
	return false;
}

/**
 * Supprime TOUT les elements de l'alphabet (les caracteres)
 */
void Alphabet::reset()
{
	_caracteres.clear();
}
