#ifndef TRANSITION_H
#define TRANSITION_H

#include "Etat.h"

class Etat;

class Transition
{
    public:
        Transition();
        Transition(Etat *arrivee, char caractere);
        char getCaractere();
        Etat* getArrivee();
        virtual ~Transition();
        bool operator==(Transition &arg);
    protected:
        Etat *_arrivee;
        char _caractere;
    private:
};

#endif // TRANSITION_H
