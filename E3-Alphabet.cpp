#include "E3-Alphabet.h"


Alphabet::Alphabet()
{
	//ctor
}

Alphabet::~Alphabet()
{
	//dtor
}

void Alphabet::addCaratere(char nouveau)
{
	_caracteres.push_back(nouveau);
}

unsigned int Alphabet::getAlphabetSize() const
{
	return _caracteres.size();
}

Alphabet& Alphabet::operator=(const Alphabet& arg)
{
	_caracteres = arg._caracteres;

	return *this;
}

std::vector<char> Alphabet::getAlphabet() const
{
	return _caracteres;
}

bool Alphabet::inAlphabet(char arg)
{
	unsigned int i;

	for (i = 0; i < _caracteres.size(); i++)
	{
		if (_caracteres[i] == arg)
			return true;
	}
	return false;
}

void Alphabet::reset()
{
	_caracteres.clear();
}
