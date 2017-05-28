#ifndef TRANSITION_H
#define TRANSITION_H

#include "E3-Etat.h"

class Etat;

class Transition
{
public:
	Transition();
	Transition(Etat* arrivee, char caractere);
	char getCaractere() const;
	Etat* getArrivee() const;
	virtual ~Transition();
	bool operator==(Transition& arg) const;
protected:
	Etat* _arrivee;
	char _caractere;
private:
};

#endif // TRANSITION_H
