#include "E3-Alphabet.h"


Alphabet::Alphabet()
{
	// Constructeur
}

Alphabet::~Alphabet()
{
	// Déconstructeur
}

/**
 *	Ajoute un caractère à la suite des autres caractères déjà présent
 */
void Alphabet::addCaratere(char nouveau)
{
	_caracteres.push_back(nouveau);
}

/**
 *	Retourne la taille de l'alphabet (méthode size d'un vector)
 */
unsigned int Alphabet::getAlphabetSize() const
{
	return _caracteres.size();
}

/**
 *	Surcharge de l'opérateur =, permettant de comparer deux alphabets
 */
Alphabet& Alphabet::operator=(const Alphabet& arg)
{
	_caracteres = arg._caracteres;

	return *this;
}

/**
 *	Récupère le vector contenant l'alphabet (ie: la liste des caractères)
 */
std::vector<char> Alphabet::getAlphabet() const
{
	return _caracteres;
}

/**
 * Permet de savoir si un caractère est contenu dans l'alphabet
 */
bool Alphabet::inAlphabet(char arg)
{
	unsigned int i;

	// On parcourt l'alphabet
	for (i = 0; i < _caracteres.size(); i++)
	{
		if (_caracteres[i] == arg) // Si le caractère est dans le "vector"
			return true; // Alors il est contenu dans l'alphabet
	}
	return false;
}

/**
 * Supprime TOUT les éléments de l'alphabet (les caractères)
 */
void Alphabet::reset()
{
	_caracteres.clear();
}
