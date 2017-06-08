#include "E3-Etat.h"

Etat::Etat()
{
	// Constructeur par d�faut vide
}

/**
 * Constructeur avec param�tre
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
 * Ajoute une transition � celles d�j� stock�es en m�moire
 */
void Etat::addTransition(Transition nouveau)
{
	_listTransition.push_back(nouveau);
}

/**
 * Ajoute des transitions � celles d�j� stock�es en m�moire
 */
void Etat::addTransitions(std::vector<Transition> nouveau)
{
	unsigned int i;
	for (i = 0; i < nouveau.size(); i++)
		_listTransition.push_back(nouveau[i]);
}

/**
 * Permet de savoir l'�tat poss�de des transitions qui respectent la r�gle du d�terminisme
 */
bool Etat::transitionsDeterministe()
{
	unsigned int i, j;
	char tmp;

	// On parcourt l'ensemble des transitions
	for (i = 0; i < _listTransition.size(); i++)
	{
		// Pour chaque transition, on stocke son caract�re dans une variable tmp
		tmp = _listTransition[i].getCaractere();
		for (j = 0; j < _listTransition.size(); j++) // On compare la transition aux autres
		{
			if (j != i) // Vu qu'on reboucle sur le m�me tableau, on �vite de comparer la m�me transition
			{
				// Non d�terministe si on a deux fois ou plus une transition portant la m�me �tiquette � partir d'un �tat
				if (_listTransition[j].getCaractere() == tmp)
					return false;
			}
		}
	}
	return true;
}

/**
 * Renvoie vrai si un �tat a une transition qui renvoie vers un �tat initial
 */
bool Etat::transitionsEtatInitial()
{
	unsigned int i;

	for (i = 0; i < _listTransition.size(); i++)
	{
		// La transition envoie sur un �tat qui est une entr�e
		if (_listTransition[i].getArrivee()->getEntry())
			return true;
	}
	return false;
}

/**
 * D�fini le nom d'un �tat
 */
void Etat::setNom(std::string nouveau)
{
	_nom = nouveau;
}

/**
 * D�fini que l'�tat est une sortie
 */
void Etat::setExit(bool nouveau)
{
	_exit = nouveau;
}

/**
 * Surcharge op�rateur permettant de concat�ner deux �tats
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
		if (dejaFait.find(tmpChar) == std::string::npos) // Si on a pas d�j� fait la transition de caract�re
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
							// On concat�ne les noms des deux �tats d'arriv�
							tmp->_nom = _listTransition[i].getArrivee()->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
						else
						{
							// On concat�ne le nom de l'�tat
							tmp->_nom = tmp->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
					}
				}
			}
		}


		if (tmp != nullptr)//Etat mixte (ie compos�, (2,3,4))
		{
			tmp->setEntry(false);
			tmpTransitions.push_back(Transition(tmp, tmpChar));
		}
		else if (tmp == nullptr && (dejaFait.find(tmpChar) == std::string::npos))//etat non existant
		{
			tmpTransitions.push_back(_listTransition[i]);
		}


		dejaFait += tmpChar; // On rajoute le caract�re comme d�j� effectu�
	}
	_listTransition.clear(); // On enl�ve toutes les transitions avant de rajouter les nouvelles
	_listTransition = tmpTransitions;
}

/**
 * Permet de supprimer les transitions en double partant d'un �tat
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
 * Compl�tion
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
			if (_listTransition[j].getCaractere() == abcd[i]) // Pour chaque lettre on v�rifie si il a une transition associ�e
			{
				found = true;
				break;
			}
		}

		if (!found) // Si aucune transition trouv�e pour un caract�re de l'alphabet
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
