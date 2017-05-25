#include "Etat.h"

Etat::Etat()
{
    //ctor
}

Etat::Etat(std::string nom)
{
    _nom = nom;
    _sortie = false;
    _entree = false;
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
        for(i = 0; i < nouveau.size(); i++)
            _listTransition.push_back(nouveau[i]);
}

bool Etat::transitionsDeterministe()
{
    unsigned int i,j;
    char tmp;

    for(i=0; i<_listTransition.size(); i++)
    {
        tmp = _listTransition[i].getCaractere();
        for(j=0; j<_listTransition.size(); j++)
        {
            if(j != i)
            {
                if(_listTransition[j].getCaractere() == tmp)
                    return false;
            }
        }

    }
    return true;
}

bool Etat::transitionsEtatInitial()
{
    unsigned int i;

    for(i=0; i<_listTransition.size(); i++)
    {
        if(_listTransition[i].getArrivee()->getEntree())
            return true;
    }
    return false;
}


void Etat::setNom(std::string nouveau)
{
    _nom = nouveau;
}

void Etat::setSortie(bool nouveau)
{
    _sortie = nouveau;
}

Etat& Etat::operator+(Etat *other)
{
    unsigned int i;
    _nom = _nom + ',' + other->_nom;

    for(i=0; i<other->getTransitions().size(); i++)
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
    Etat *tmp;
    std::vector<Transition> tmpTransitions;

    for(i = 0; i < _listTransition.size(); i++)
    {
        tmp = NULL;

        tmpChar = _listTransition[i].getCaractere();
        //Si on a pas encore fait ce caractere
        if(dejaFait.find(tmpChar) == std::string::npos)
        {
            for(j = 0; j < _listTransition.size(); j++)
            {
                if(j != i)
                {
                    //Si caractere de transition identique
                    if(tmpChar == _listTransition[j].getCaractere())
                    {
                        if(tmp == NULL)
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


        if(tmp != NULL)//Etat mixte
        {
            tmp->setEntree(false);
            tmpTransitions.push_back(Transition(tmp, tmpChar));
        }
        else if(tmp == NULL && (dejaFait.find(tmpChar) == std::string::npos))//etat déja existant
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
    for(i = 0; i < _listTransition.size(); i++)
    {
        for(j = 0; j < _listTransition.size(); j++)
        {
            if(i != j)
            {
                if(_listTransition[i] == _listTransition[j])
                {
                    _listTransition.erase(_listTransition.begin() + j);
                }
            }
        }
    }
}

void Etat::completion(Etat *P, std::vector<char> abcd)
{
    unsigned int i, j;

    for(i = 0; i < abcd.size(); i++)
    {
        for(j = 0; j < _listTransition.size(); j++)
        {
            if(_listTransition[j].getCaractere() == abcd[i])
                break;
            if(_listTransition[j].getCaractere() != abcd[i])
                this->addTransition(Transition(P, abcd[i]));
        }
    }
}


void Etat::setEntree(bool nouveau)
{
    _entree = nouveau;
}

std::string Etat::getNom()
{
    return _nom;
}

bool Etat::getSortie()
{
    return _sortie;
}

bool Etat::getEntree()
{
    return _entree;
}

std::vector<Transition> Etat::getTransitions()
{
    return _listTransition;
}
