#include "Automate.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string Automate::FILES_PREFIX = "Automates/";

Automate::Automate()
{
    //ctor
}

Automate::~Automate()
{
    //dtor
}

int Automate::rechercheIndiceEtat(std::string nom)
{
    for(unsigned int i=0; i < _listEtats.size(); i++)
    {
        if(nom == _listEtats[i]->getNom())
            return i;
    }
    return -1;
}


void Automate::minimiser()
{
    unsigned int i;
    std::vector<Etat*> terminaux, nonTerminaux;
    for(i = 0; i < _listEtats.size(); i++)
    {
        if(_listEtats[i]->getSortie())
            terminaux.push_back(_listEtats[i]);
        else
            nonTerminaux.push_back(_listEtats[i]);
    }

}

void Automate::standardiser()
{
	if (this->isStandart())
		return;
    unsigned int i;
    std::vector<Transition> tmp;
    Etat *entree = new Etat("i");

    entree->setEntree(true);

    for(i = 0; i < _listEtats.size(); i++)
    {
        //Si l'etat est une entree on recopie ses transitions
        if(_listEtats[i]->getEntree())
        {
            //Si une entree est une sortie
            if(_listEtats[i]->getSortie())
                entree->setSortie(true);

            tmp = _listEtats[i]->getTransitions();
            entree->addTransitions(tmp);
            entree->suppDoublonsTransitions();

            //on enleve les sortie originelles
            _listEtats[i]->setEntree(false);
        }
    }
    entree->setEntree(true);
    _listEtats.push_back(entree);

}


void Automate::loading(string fileName)
{
	this->_fileName = fileName;
	ifstream origine;
	origine.open(Automate::FILES_PREFIX + fileName +".txt", ios::in); //Ouverture du fichier en lecture

	if (!origine) {
		cerr << "Automate introuvable !" << endl;
		this->setFound(false);
        return;
    } else {
        this->setFound(true);
	}

	int tmpInt, nbrLecture, source, dest;
	char tmpChar;


	origine >> tmpInt;//nombre de carateres dans l'alphabet

	for (int i = 0; i < tmpInt; i++) //initialisation de l'alphabet avec le nombre de caractères trouvés
	{

		_abcd.addCaratere(97 + i); //Ajout du caractère avec code ASCI

	}


	/*
	cout << Colors::fg_green << "*** Informations sur l'automate ***" << Colors::fg_white << endl;
	cout << " Nom de l'automate : " + fileName << ".txt" << endl;
	cout << " Elements dans l'alphabet : " + _util.ToString(tmpInt) << endl;
	*/
	origine >> tmpInt;//nombre d'etats

	//cout << " Nombre d'etats : " + _util.ToString(tmpInt) << endl;

	for (int i = 0; i<tmpInt; i++)//initialisation des états
	{
		_listEtats.push_back(new Etat(_util.ToString(i)));
	}

	origine >> nbrLecture;//nombre d'états initiaux

	//cout << "   Nombre d'etats initiaux : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i<nbrLecture; i++)//mettre les états initiaux
	{

		origine >> tmpInt;
       _listEtats[rechercheIndiceEtat(_util.ToString(tmpInt))]->setEntree(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre d'états terminaux
	//cout << "   Nombre d'etats terminaux : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i<nbrLecture; i++)//mettre les états terminaux
	{
		origine >> tmpInt;
		_listEtats[rechercheIndiceEtat(_util.ToString(tmpInt))]->setSortie(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre de transition
	//cout << " Nombre de transitions : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i<nbrLecture; i++)//mettre les transitions
	{
		origine >> source;
		origine >> tmpChar;
		origine >> dest;

		// On créer une transition depuis l'état de départ vers l'état d'arrivée contenant le caractere de l'alphabet qui permet de faire cette transition
		_listEtats[rechercheIndiceEtat(_util.ToString(source))]->addTransition(Transition(_listEtats[rechercheIndiceEtat(_util.ToString(dest))], tmpChar));
	}

	// cout << Colors::fg_green << "***********************************" << Colors::fg_white << endl;
	 _util.lineJump(1);
}

bool Automate::isSynchrone()
{
    unsigned int i,j;
    std::vector<Transition> tmp;
    for(i=0; i<_listEtats.size(); i++)
    {
        tmp = _listEtats[i]->getTransitions();
        for(j=0; j<tmp.size(); j++)
        {
            if(tmp[j].getCaractere() == '*')
                return false;
        }

    }
    return true;
}

bool Automate::isDeterministe()
{
    unsigned int i, entrees = 0;
    //Si il y a plusieurs entrees l'autoimate n'est pas détermilniste

    for(i=0; i<_listEtats.size(); i++)
    {
        if(_listEtats[i]->getEntree())
        {
            entrees++;
            if(entrees > 1)
                return false;
        }
    }

    //Si il y a deux transitions possible avec le meme caratère

    for(i=0; i<_listEtats.size(); i++)
    {
        if(_listEtats[i]->transitionsDeterministe() == false)
            return false;
    }


    return true;//Automate déterministe
}

bool Automate::isStandart()
{
    unsigned int i, entrees;
    //Si il y a plusieurs entrees l'automate n'est pas standart
    entrees = 0;
    for(i=0; i<_listEtats.size(); i++)
    {
        if(_listEtats[i]->getEntree())
        {
            entrees++;
            if(entrees > 1)
                return false;
        }
    }

    //Si une transition revient sur l'etat initial l'automate n'est as standart
    for(i=0; i<_listEtats.size(); i++)
    {
        if(_listEtats[i]->transitionsEtatInitial())
        {
            return false;
        }
    }
    return true;
}

bool Automate::isComplet()
{
    unsigned int i, tailleAlphabet;

    //Si un automate n'est pas déterministe alors il ne peux pas etre complet
	if (this->isDeterministe() == false)
	{
		return false;
	}

    //Si un etat n'as pas autant de transitions qu'il y a de charactere dans l'alphabet
    tailleAlphabet = _abcd.getTailleAlphabet();
    for(i=0; i<_listEtats.size(); i++)
    {
		//cout << _listEtats[i]->getTransitions().size() + "  " + tailleAlphabet;

        if(_listEtats[i]->getTransitions().size() != tailleAlphabet)
        {
            return false;
        }
    }

    return true;
}

void Automate::determiniser()
{
    unsigned int i, j, k;
    std::vector<Transition> tmpTransitions;
    Automate deterministe;
    Etat *tmp = NULL;

    //On recopie l'alphabet
    deterministe._abcd = _abcd;

    //Somme des etats initiaux
    for(i=0; i<_listEtats.size(); i++)
    {

        if(_listEtats[i]->getEntree())
        {
            if(tmp == NULL)
            {
                tmp = new Etat("");
                tmp->setNom(_listEtats[i]->getNom());
                tmp->addTransitions(_listEtats[i]->getTransitions());
            }
            else
            {
                tmp->setNom(tmp->getNom() + "," + _listEtats[i]->getNom());
                tmp->addTransitions(_listEtats[i]->getTransitions());
            }

        }
    }
    //Supprime les transitions identiques
    tmp->suppDoublonsTransitions();

    //regroupe les transitions
    tmp->regrouperTransitions();
    tmp->setEntree(true);

    deterministe._listEtats.push_back(tmp);

    //Temps que tous les états des transitions n'existent pas on les créer
    for(i = 0; i < deterministe._listEtats.size(); i++)
    {
        tmpTransitions = deterministe._listEtats[i]->getTransitions();



        for(j = 0; j < tmpTransitions.size(); j++)
        {
            //Si la cible de transition ne fait pas partie de l'automate
            if(deterministe.rechercheIndiceEtat(tmpTransitions[j].getArrivee()->getNom())== -1)
            {
                deterministe._listEtats.push_back(tmpTransitions[j].getArrivee());

                //Si la cible de transition ne fesait pas partie de l'automate d'origine
                if(this->rechercheIndiceEtat(tmpTransitions[j].getArrivee()->getNom())== -1)
                {
                    //Il faut lui créer ses transitions en fonction de l'automate d'origine
                    //Parcour tout les etats initiaux et si le nom est contenu dans le nouvel état alors somme des états
                    for(k = 0; k < _listEtats.size(); k++)
                    {
                        if(tmpTransitions[j].getArrivee()->getNom().find(_listEtats[k]->getNom()) != std::string::npos)
                        {
                            deterministe._listEtats.back()->addTransitions(_listEtats[k]->getTransitions());
                            if(_listEtats[k]->getSortie())
                                deterministe._listEtats.back()->setSortie(true);
                        }
                    }
                    deterministe._listEtats.back()->suppDoublonsTransitions();
                    deterministe._listEtats.back()->regrouperTransitions();
                }
            }
        }
    }
    *this = deterministe;
}


void Automate::print()
{	unsigned int i, j, cpt;
cpt = 0;
std::vector<Transition> tmp;
	_finalStates.clear();
	_initialStates.clear();
	// Nombre d'états initiaux
	for (i = 0; i <_listEtats.size(); i++)
	{
		if (_listEtats[i]->getEntree())
			_initialStates.push_back(_listEtats[i]->getNom());
	}
	for (i = 0; i <_listEtats.size(); i++)
	{
		if (_listEtats[i]->getSortie())
			_finalStates.push_back(_listEtats[i]->getNom());
	}


	cout << setw(50) << left << "0: Quitter"
      << setw(20) << left <<  "*** Informations sur l'automate ***" <<  endl;

	cout << setw(50) << left << "1: Charger un autre Automate"
		<< setw(20) << left << " Nom de l'automate : " + _fileName << ".txt" << endl;

	cout << setw(50) << left << "2: Savoir si l'aumotate est Synchrone"
		<< setw(20) << left << " Elements dans l'alphabet : " + _util.ToString(_abcd.getTailleAlphabet()) << endl;



	cout << setw(50) << left << "3: Savoir si l'automate est Deterministe"
		<< setw(20) << left << " Nombre d'etats : " + _util.ToString(_listEtats.size()) << endl;


	cout << setw(50) << left << "4: Savoir si l'automate est Standard"
	<< setw(20) << left << " Nombre d'etats initiaux : " + _util.ToString(_initialStates.size()) << endl;



	cout << setw(50) << left << "5: Savoir si l'automate est Complet"
	<< setw(20) << left << " Nombre d'etats terminaux : " + _util.ToString(_finalStates.size()) << endl;


	for (i = 0; i < _listEtats.size(); i++)
	{
		tmp = _listEtats[i]->getTransitions();
		for (j = 0; j < tmp.size(); j++)
		{
			cpt++;
		}
	}
	cout << setw(50) << left << "6: Determiniser l'automate"
		<< setw(20) << left << " Nombre de transitions : " + _util.ToString(cpt) << endl;
	cpt = 0;





	cout << setw(50) << left << "7: Completer l'automate"
		<< setw(20) << left <<  "***********************************" <<  endl;


	cout << "8: Standardiser l'automate" << endl;

	cout << setw(50) << "9: Complementariser l'automate"
		<< setw(20) << left <<  "********* Automate obtenu *********" << endl;

	cout << setw(50) << left << "10: Minimiser l'automate"
		<< setw(20) << left << " Etats initiaux ";
	cout << "[";
	//Les états initiaux
	for (i = 0; i <_initialStates.size(); i++)
	{
		std::cout << "(" << _initialStates[i] << ")";
	}
	cout << "]" << endl;



	cout << setw(50) << left <<  "11: Tester un mot sur l'automate"
		<< setw(20) << left << " Etats Terminaux ";
	cout << "[";
	//Les etats terminaux
	for (i = 0; i <_finalStates.size(); i++)
	{
		std::cout << "(" << _finalStates[i] << ")";
	}
	cout << "]" << endl;


	//Les transitions
	for (i = 0; i < _listEtats.size(); i++)
	{
		tmp = _listEtats[i]->getTransitions();
		for (j = 0; j < tmp.size(); j++)
		{

			cout << setw(50) << left << " " << setw (20) << left  << " (" + _listEtats[i]->getNom() + ") -> " + tmp[j].getCaractere() + " -> (" + tmp[j].getArrivee()->getNom() + ")" << std::endl;
		}
	}







	//  std::cout << cpt << " ";



	// std::cout << "\n" <<cpt << " ";
	_util.lineJump(1);

	//nbr transitions
	_util.lineJump(1);





}

void Automate::completer()
{
    unsigned int i;
    Etat *poubelle;
    if(this->isComplet())
        return;


    poubelle = new Etat("P");
    //Initialisatin de la poubelle
    for(i = 0; i < _abcd.getTailleAlphabet(); i++)
    {
        poubelle->addTransition(Transition(poubelle, _abcd.getAlphabet()[i]));
    }

    for(i=0; i<_listEtats.size(); i++)
    {
        if(_listEtats[i]->getTransitions().size() != _abcd.getTailleAlphabet())
        {
            _listEtats[i]->completion(poubelle, _abcd.getAlphabet());
        }
    }

    _listEtats.push_back(poubelle);


}

void Automate::inverser()
{
    unsigned int i;

    if(this->isComplet() == false)
        this->completer();

    for(i = 0; i < _listEtats.size(); i++)
    {
        if(_listEtats[i]->getSortie())
            _listEtats[i]->setSortie(false);
        else
            _listEtats[i]->setSortie(true);
    }
}

void Automate::analyseMot(std::string mot)
{
    unsigned int i, j;
    std::vector<Transition> tmp;
    Etat* actuel = new Etat;

    for(i = 0; i < mot.size(); i++)
    {
        if(_abcd.inAlphabet(mot[i]) == false && mot != "*")
        {
            std::cout << mot << " n'est pas un mot reconnu par l'automate" << std::endl;
            return;
        }
    }

    if(this->isDeterministe() == false)
        this->determiniser();
    if(this->isComplet() == false)
        this->completer();

    //Init actuel avec l'entree
    for(i = 0; i < _listEtats.size(); i++)
    {
        if(_listEtats[i]->getEntree())
        {
            actuel = _listEtats[i];
            break;
        }
    }

    if(actuel->getSortie() == true && mot == "*") {
        std::cout << "L'automate reconnait le mot vide." << std::endl;
        this->reload();
        return;
    }

    for(i = 0; i < mot.size(); i++)
    {
        tmp = actuel->getTransitions();

        for(j = 0; j < tmp.size(); j++)
        {
            if(tmp[j].getCaractere() == mot[i])
                actuel = tmp[j].getArrivee();
        }

    }

    if(actuel->getSortie() == true)
        std::cout << mot << " est un mot reconnu par l'automate" << std::endl;
    else
        std::cout << mot << " n'est pas un mot reconnu par l'automate" << std::endl;

    this->reload();
}

void Automate::reset()
{
    unsigned int i;
    _abcd.reset();

    for(i = 0; i < _listEtats.size(); i++)
        delete _listEtats[i];

    _listEtats.clear();
}

void Automate::reload() {
    std::string filename;
    filename = this->_fileName;

    this->reset();
    this->loading(filename);
}

Automate& Automate::operator=(const Automate &arg)
{
    _listEtats = arg._listEtats;
    _abcd = arg._abcd;

    return *this;
}
