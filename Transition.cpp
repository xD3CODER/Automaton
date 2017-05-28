#include "Transition.h"

Transition::Transition()
{
	//ctor
}

Transition::Transition(Etat* arrivee, char caractere)
{
	_arrivee = arrivee;
	_caractere = caractere;
}

Transition::~Transition()
{
	_arrivee = nullptr;
}

char Transition::getCaractere() const
{
	return _caractere;
}

Etat* Transition::getArrivee() const
{
	return _arrivee;
}

bool Transition::operator==(Transition& arg) const
{
	if (_caractere == arg.getCaractere())
	{
		if (_arrivee == arg.getArrivee())
			return true;
	}
	return false;
}
