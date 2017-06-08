#include "E3-Etat.h"

Etat::Etat()
{
	// Constructeur par défaut vide
}

/**
 * Constructeur avec paramètre
 */
Etat::Etat(std::string nom)
{
	_nom = nom;
	_exit = false;
	_entry = false;
}

Etat::~Etat()
{
	// Destructeur
}

/**
 * Ajoute une transition à celles déjà stockées en mémoire
 */
void Etat::addTransition(Transition nouveau)
{
	_listTransition.push_back(nouveau);
}

/**
 * Ajoute des transitions à celles déjà stockées en mémoire
 */
void Etat::addTransitions(std::vector<Transition> nouveau)
{
	unsigned int i;
	for (i = 0; i < nouveau.size(); i++)
		_listTransition.push_back(nouveau[i]);
}

/**
 * Permet de savoir l'état possède des transitions qui respectent la règle du déterminisme
 */
bool Etat::transitionsDeterministe()
{
	unsigned int i, j;
	char tmp;

	// On parcourt l'ensemble des transitions
	for (i = 0; i < _listTransition.size(); i++)
	{
		// Pour chaque transition, on stocke son caractère dans une variable tmp
		tmp = _listTransition[i].getCaractere();
		for (j = 0; j < _listTransition.size(); j++) // On compare la transition aux autres
		{
			if (j != i) // Vu qu'on reboucle sur le même tableau, on évite de comparer la même transition
			{
				// Non déterministe si on a deux fois ou plus une transition portant la même étiquette à partir d'un état
				if (_listTransition[j].getCaractere() == tmp)
					return false;
			}
		}
	}
	return true;
}

/**
 * Renvoie vrai si un état a une transition qui renvoie vers un état initial
 */
bool Etat::transitionsEtatInitial()
{
	unsigned int i;

	for (i = 0; i < _listTransition.size(); i++)
	{
		// La transition envoie sur un état qui est une entrée
		if (_listTransition[i].getArrivee()->getEntry())
			return true;
	}
	return false;
}

/**
 * Défini le nom d'un état
 */
void Etat::setNom(std::string nouveau)
{
	_nom = nouveau;
}

/**
 * Défini que l'état est une sortie
 */
void Etat::setExit(bool nouveau)
{
	_exit = nouveau;
}

/**
 * Surcharge opérateur permettant de concaténer deux états
 */
Etat& Etat::operator+(Etat* other)
{
	unsigned int i;
	_nom = _nom + ',' + other->_nom;

	for (i = 0; i < other->getTransitions().size(); i++)
	{
		_listTransition.push_back(other->getTransitions()[i]);
	}
	return *this;
}

/**
 * Permet de regrouper les transitions
 */
void Etat::regrouperTransitions()
{
	unsigned int i, j;
	std::string dejaFait = "";
	char tmpChar;
	Etat* tmp;
	std::vector<Transition> tmpTransitions;

	for (i = 0; i < _listTransition.size(); i++) // Pour chaque transition i
	{
		tmp = nullptr; // pointeur null

		tmpChar = _listTransition[i].getCaractere();
		//Si on a pas encore fait ce caractere
		if (dejaFait.find(tmpChar) == std::string::npos) // Si on a pas déjà fait la transition de caractère
		{
			for (j = 0; j < _listTransition.size(); j++) // On repasse sur chacune des transitions ...
			{
				if (j != i) // ... sauf si i == j
				{
					// Si caractere de transition identique
					if (tmpChar == _listTransition[j].getCaractere())
					{
						if (tmp == nullptr) // Premier passage
						{
							tmp = new Etat;
							// On concatène les noms des deux états d'arrivé
							tmp->_nom = _listTransition[i].getArrivee()->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
						else
						{
							// On concatène le nom de l'état
							tmp->_nom = tmp->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
					}
				}
			}
		}


		if (tmp != nullptr)//Etat mixte (ie composé, (2,3,4))
		{
			tmp->setEntry(false);
			tmpTransitions.push_back(Transition(tmp, tmpChar));
		}
		else if (tmp == nullptr && (dejaFait.find(tmpChar) == std::string::npos))//etat non existant
		{
			tmpTransitions.push_back(_listTransition[i]);
		}


		dejaFait += tmpChar; // On rajoute le caractère comme déjà effectué
	}
	_listTransition.clear(); // On enlève toutes les transitions avant de rajouter les nouvelles
	_listTransition = tmpTransitions;
}

/**
 * Permet de supprimer les transitions en double partant d'un état
 */
void Etat::suppDoublonsTransitions()
{
	unsigned int i, j;
	for (i = 0; i < _listTransition.size(); i++)
	{
		for (j = 0; j < _listTransition.size(); j++)
		{
			if (i != j)
			{
				if (_listTransition[i] == _listTransition[j])
				{
					_listTransition.erase(_listTransition.begin() + j);
				}
			}
		}
	}
}

/**
 * Complétion
 */
void Etat::completion(Etat* P, std::vector<char> abcd)
{
	unsigned int i, j;
	bool found;

	found = false;
	for (i = 0; i < abcd.size(); i++) // Pour chaque lettre de l'alphabet
	{
		for (j = 0; j < _listTransition.size(); j++) // Pour chaque transition
		{
			if (_listTransition[j].getCaractere() == abcd[i]) // Pour chaque lettre on vérifie si il a une transition associée
			{
				found = true;
				break;
			}
		}

		if (!found) // Si aucune transition trouvée pour un caractère de l'alphabet
		{
			this->addTransition(Transition(P, abcd[i])); // On ajoute une transition
		}
	}
}


void Etat::setEntry(bool nouveau)
{
	_entry = nouveau;
}

std::string Etat::getNom() const
{
	return _nom;
}

bool Etat::getExit() const
{
	return _exit;
}

bool Etat::getEntry() const
{
	return _entry;
}

std::vector<Transition> Etat::getTransitions() const
{
	return _listTransition;
}
