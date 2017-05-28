#include "E3-Etat.h"

Etat::Etat()
{
	//ctor
}

Etat::Etat(std::string nom)
{
	_nom = nom;
	_exit = false;
	_entry = false;
}

Etat::~Etat()
{
	//dtor
}

void Etat::addTransition(Transition nouveau)
{
	_listTransition.push_back(nouveau);
}

void Etat::addTransitions(std::vector<Transition> nouveau)
{
	unsigned int i;
	for (i = 0; i < nouveau.size(); i++)
		_listTransition.push_back(nouveau[i]);
}

bool Etat::transitionsDeterministe()
{
	unsigned int i, j;
	char tmp;

	for (i = 0; i < _listTransition.size(); i++)
	{
		tmp = _listTransition[i].getCaractere();
		for (j = 0; j < _listTransition.size(); j++)
		{
			if (j != i)
			{
				if (_listTransition[j].getCaractere() == tmp)
					return false;
			}
		}
	}
	return true;
}

bool Etat::transitionsEtatInitial()
{
	unsigned int i;

	for (i = 0; i < _listTransition.size(); i++)
	{
		if (_listTransition[i].getArrivee()->getEntry())
			return true;
	}
	return false;
}


void Etat::setNom(std::string nouveau)
{
	_nom = nouveau;
}

void Etat::setExit(bool nouveau)
{
	_exit = nouveau;
}

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

void Etat::regrouperTransitions()
{
	unsigned int i, j;
	std::string dejaFait = "";
	char tmpChar;
	Etat* tmp;
	std::vector<Transition> tmpTransitions;

	for (i = 0; i < _listTransition.size(); i++)
	{
		tmp = nullptr;

		tmpChar = _listTransition[i].getCaractere();
		//Si on a pas encore fait ce caractere
		if (dejaFait.find(tmpChar) == std::string::npos)
		{
			for (j = 0; j < _listTransition.size(); j++)
			{
				if (j != i)
				{
					//Si caractere de transition identique
					if (tmpChar == _listTransition[j].getCaractere())
					{
						if (tmp == nullptr)
						{
							tmp = new Etat;
							tmp->_nom = _listTransition[i].getArrivee()->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
						else
						{
							tmp->_nom = tmp->_nom + "," + _listTransition[j].getArrivee()->_nom;
						}
					}
				}
			}
		}


		if (tmp != nullptr)//Etat mixte
		{
			tmp->setEntry(false);
			tmpTransitions.push_back(Transition(tmp, tmpChar));
		}
		else if (tmp == nullptr && (dejaFait.find(tmpChar) == std::string::npos))//etat déja existant
		{
			tmpTransitions.push_back(_listTransition[i]);
		}


		dejaFait += tmpChar;
	}
	_listTransition.clear();
	_listTransition = tmpTransitions;
}

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

void Etat::completion(Etat* P, std::vector<char> abcd)
{
	unsigned int i, j;
	bool found;

	found = false;
	for (i = 0; i < abcd.size(); i++)
	{
		for (j = 0; j < _listTransition.size(); j++)
		{
			if (_listTransition[j].getCaractere() == abcd[i])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			this->addTransition(Transition(P, abcd[i]));
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
