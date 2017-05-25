#include "Transition.h"

Transition::Transition()
{
    //ctor
}

Transition::Transition(Etat *arrivee, char caractere)
{
    _arrivee = arrivee;
    _caractere = caractere;
}

Transition::~Transition()
{
    _arrivee = NULL;
}

char Transition::getCaractere()
{
    return _caractere;
}

Etat* Transition::getArrivee()
{
    return _arrivee;
}
bool Transition::operator==(Transition &arg)
{
    if(_caractere == arg.getCaractere())
    {
        if(_arrivee == arg.getArrivee())
            return true;
    }
    return false;
}
