#include "E3-Automate.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string Automate::FILES_PREFIX = "E3-Automates/E3-";

Automate::Automate()
{
	//ctor
}

Automate::~Automate()
{
	//dtor
}

int Automate::rechercheIndiceEtat(string nom)
{
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (nom == _listStates[i]->getNom())
			return i;
	}
	return -1;
}


void Automate::minimiser()
{
	vector<Etat*> terminaux, nonTerminaux;
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getExit())
			terminaux.push_back(_listStates[i]);
		else
			nonTerminaux.push_back(_listStates[i]);
	}
}

void Automate::standardiser()
{
	if (this->isStandard())
		return;


	Etat* entree = new Etat("i");

	entree->setEntry(true);

	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		//Si l'etat est une entree on recopie ses transitions
		if (_listStates[i]->getEntry())
		{
			//Si une entree est une sortie
			if (_listStates[i]->getExit())
				entree->setExit(true);

			vector<Transition> tmp = _listStates[i]->getTransitions();
			entree->addTransitions(tmp);
			entree->suppDoublonsTransitions();

			//on enleve les sortie originelles
			_listStates[i]->setEntry(false);
		}
	}
	entree->setEntry(true);
	_listStates.push_back(entree);
}


void Automate::loading(string fileName)
{
	this->_fileName = fileName;
	ifstream origine;
	origine.open(FILES_PREFIX + fileName + ".txt", ios::in); //Ouverture du fichier en lecture

	if (!origine)
	{
		Utils::consoleClear();
		cerr << "Automate introuvable !" << endl;
		this->setFound(false);
		return;
	}
	this->setFound(true);

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

	for (int i = 0; i < tmpInt; i++)//initialisation des états
	{
		_listStates.push_back(new Etat(_util.ToString(i)));
	}

	origine >> nbrLecture;//nombre d'états initiaux

	//cout << "   Nombre d'etats initiaux : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i < nbrLecture; i++)//mettre les états initiaux
	{
		origine >> tmpInt;
		_listStates[rechercheIndiceEtat(_util.ToString(tmpInt))]->setEntry(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre d'états terminaux
	//cout << "   Nombre d'etats terminaux : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i < nbrLecture; i++)//mettre les états terminaux
	{
		origine >> tmpInt;
		_listStates[rechercheIndiceEtat(_util.ToString(tmpInt))]->setExit(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre de transition
	//cout << " Nombre de transitions : " + _util.ToString(nbrLecture) << endl;
	for (int i = 0; i < nbrLecture; i++)//mettre les transitions
	{
		origine >> source;
		origine >> tmpChar;
		origine >> dest;

		// On créer une transition depuis l'état de départ vers l'état d'arrivée contenant le caractere de l'alphabet qui permet de faire cette transition
		_listStates[rechercheIndiceEtat(_util.ToString(source))]->addTransition(Transition(_listStates[rechercheIndiceEtat(_util.ToString(dest))], tmpChar));
	}

	// cout << Colors::fg_green << "***********************************" << Colors::fg_white << endl;
	_util.lineJump(1);
}

bool Automate::isSynchrone()
{
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		vector<Transition> tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			if (tmp[j].getCaractere() == '*')
				return false;
		}
	}
	return true;
}

bool Automate::isDeterministe()
{
	unsigned int i, entrees = 0;
	//Si il y a plusieurs entrees l'autoimate n'est pas détermilniste

	for (i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getEntry())
		{
			entrees++;
			if (entrees > 1)
				return false;
		}
	}

	//Si il y a deux transitions possible avec le meme caratère

	for (i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->transitionsDeterministe() == false)
			return false;
	}


	return true;//Automate déterministe
}

bool Automate::isStandard()
{
	//Si il y a plusieurs entrees l'automate n'est pas standart
	unsigned int entrees = 0;
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getEntry())
		{
			entrees++;
			if (entrees > 1)
				return false;
		}
	}

	//Si une transition revient sur l'etat initial l'automate n'est as standart
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->transitionsEtatInitial())
		{
			return false;
		}
	}
	return true;
}

bool Automate::isComplet()
{
	//Si un automate n'est pas déterministe alors il ne peux pas etre complet
	if (this->isDeterministe() == false)
	{
		return false;
	}

	//Si un etat n'as pas autant de transitions qu'il y a de charactere dans l'alphabet
	unsigned int tailleAlphabet = _abcd.getAlphabetSize();
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		//cout << _listEtats[i]->getTransitions().size() + "  " + tailleAlphabet;

		if (_listStates[i]->getTransitions().size() != tailleAlphabet)
		{
			return false;
		}
	}

	return true;
}

void Automate::determiniser()
{
	Automate deterministe;
	Etat* tmp = nullptr;

	//On recopie l'alphabet
	deterministe._abcd = _abcd;

	//Somme des etats initiaux
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getEntry())
		{
			if (tmp == nullptr)
			{
				tmp = new Etat("");
				tmp->setNom(_listStates[i]->getNom());
				tmp->addTransitions(_listStates[i]->getTransitions());
			}
			else
			{
				tmp->setNom(tmp->getNom() + "," + _listStates[i]->getNom());
				tmp->addTransitions(_listStates[i]->getTransitions());
			}
		}
	}
	//Supprime les transitions identiques
	tmp->suppDoublonsTransitions();

	//regroupe les transitions
	tmp->regrouperTransitions();
	tmp->setEntry(true);

	deterministe._listStates.push_back(tmp);

	//Temps que tous les états des transitions n'existent pas on les créer
	for (unsigned int i = 0; i < deterministe._listStates.size(); i++)
	{
		vector<Transition> tmpTransitions = deterministe._listStates[i]->getTransitions();


		for (unsigned int j = 0; j < tmpTransitions.size(); j++)
		{
			//Si la cible de transition ne fait pas partie de l'automate
			if (deterministe.rechercheIndiceEtat(tmpTransitions[j].getArrivee()->getNom()) == -1)
			{
				deterministe._listStates.push_back(tmpTransitions[j].getArrivee());

				//Si la cible de transition ne fesait pas partie de l'automate d'origine
				if (this->rechercheIndiceEtat(tmpTransitions[j].getArrivee()->getNom()) == -1)
				{
					//Il faut lui créer ses transitions en fonction de l'automate d'origine
					//Parcour tout les etats initiaux et si le nom est contenu dans le nouvel état alors somme des états
					for (unsigned int k = 0; k < _listStates.size(); k++)
					{
						if (tmpTransitions[j].getArrivee()->getNom().find(_listStates[k]->getNom()) != string::npos)
						{
							deterministe._listStates.back()->addTransitions(_listStates[k]->getTransitions());
							if (_listStates[k]->getExit())
								deterministe._listStates.back()->setExit(true);
						}
					}
					deterministe._listStates.back()->suppDoublonsTransitions();
					deterministe._listStates.back()->regrouperTransitions();
				}
			}
		}
	}
	*this = deterministe;
}


void Automate::print()
{
	unsigned int cpt = 0;
	vector<Transition> tmp;
	_finalStates.clear();
	_initialStates.clear();
	// Nombre d'états initiaux
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getEntry())
			_initialStates.push_back(_listStates[i]->getNom());
	}
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getExit())
			_finalStates.push_back(_listStates[i]->getNom());
	}


	cout << setw(50) << left << "0: Quitter"
		<< setw(20) << left << "*** Informations sur l'automate ***" << endl;

	cout << setw(50) << left << "1: Changer d'automate"
		<< setw(20) << left << " Nom de l'automate : " + _fileName << ".txt" << endl;

	cout << setw(50) << left << "2: Savoir si l'aumotate est synchrone"
		<< setw(20) << left << " Elements dans l'alphabet : " + _util.ToString(_abcd.getAlphabetSize()) << endl;


	cout << setw(50) << left << "3: Savoir si l'automate est deterministe"
		<< setw(20) << left << " Nombre d'etats : " + _util.ToString(_listStates.size()) << endl;


	cout << setw(50) << left << "4: Savoir si l'automate est standard"
		<< setw(20) << left << " Nombre d'etats initiaux : " + _util.ToString(_initialStates.size()) << endl;


	cout << setw(50) << left << "5: Savoir si l'automate est complet"
		<< setw(20) << left << " Nombre d'etats terminaux : " + _util.ToString(_finalStates.size()) << endl;


	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			cpt++;
		}
	}
	cout << setw(50) << left << "6: Determiniser l'automate"
		<< setw(20) << left << " Nombre de transitions : " + _util.ToString(cpt) << endl;
	//cpt = 0;


	cout << setw(50) << left << "7: Completer l'automate"
		<< setw(20) << left << "***********************************" << endl;


	cout << "8: Standardiser l'automate" << endl;

	cout << setw(50) << "9: Complementariser l'automate"
		<< setw(20) << left << "********* Automate obtenu *********" << endl;

	cout << setw(50) << left << "10: Minimiser l'automate"
		<< setw(20) << left << " Etats initiaux ";
	cout << "[";
	//Les états initiaux
	for (unsigned int i = 0; i < _initialStates.size(); i++)
	{
		cout << "(" << _initialStates[i] << ")";
	}
	cout << "]" << endl;


	cout << setw(50) << left << "11: Entrer un mot a tester"
		<< setw(20) << left << " Etats Terminaux ";
	cout << "[";
	//Les etats terminaux
	for (unsigned int i = 0; i < _finalStates.size(); i++)
	{
		cout << "(" << _finalStates[i] << ")";
	}
	cout << "]" << endl;


	//Les transitions
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			cout << setw(50) << left << " " << setw(20) << left << " (" + _listStates[i]->getNom() + ") -> " + tmp[j].getCaractere() + " -> (" + tmp[j].getArrivee()->getNom() + ")" << endl;
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
	if (this->isComplet())
		return;


	Etat* poubelle = new Etat("P");
	//Initialisatin de la poubelle
	for (unsigned int i = 0; i < _abcd.getAlphabetSize(); i++)
	{
		poubelle->addTransition(Transition(poubelle, _abcd.getAlphabet()[i]));
	}

	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getTransitions().size() != _abcd.getAlphabetSize())
		{
			_listStates[i]->completion(poubelle, _abcd.getAlphabet());
		}
	}

	_listStates.push_back(poubelle);
}

void Automate::inverser()
{
	if (this->isComplet() == false)
		this->completer();

	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getExit())
			_listStates[i]->setExit(false);
		else
			_listStates[i]->setExit(true);
	}
}

void Automate::analyseMot(string mot)
{
	Etat* actuel = new Etat;

	for (unsigned int i = 0; i < mot.size(); i++)
	{
		if (_abcd.inAlphabet(mot[i]) == false && mot != "*")
		{
			cout << mot << " n'est pas un mot reconaissable par cet automate" << endl;
			return;
		}
	}

	if (this->isDeterministe() == false)
		this->determiniser();
	if (this->isComplet() == false)
		this->completer();

	//Init actuel avec l'entree
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (_listStates[i]->getEntry())
		{
			actuel = _listStates[i];
			break;
		}
	}

	if (actuel->getExit() == true && mot == "*")
	{
		cout << "L'automate reconnait le mot vide" << endl;
		this->reload();
		return;
	}

	for (unsigned int i = 0; i < mot.size(); i++)
	{
		vector<Transition> tmp = actuel->getTransitions();

		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			if (tmp[j].getCaractere() == mot[i])
				actuel = tmp[j].getArrivee();
		}
	}

	if (actuel->getExit() == true)
		cout << mot << " est un mot reconaissable par cet automate" << endl;
	else
		cout << mot << " n'est pas un mot reconaissable par cet automate" << endl;

	this->reload();
}

void Automate::reset()
{
	_abcd.reset();

	for (unsigned int i = 0; i < _listStates.size(); i++)
		delete _listStates[i];

	_listStates.clear();
}

void Automate::reload()
{
	string filename = this->_fileName;

	this->reset();
	this->loading(filename);
}

Automate& Automate::operator=(const Automate& arg)
{
	_listStates = arg._listStates;
	_abcd = arg._abcd;

	return *this;
}
