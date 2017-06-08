#include "E3-Automate.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

/* Définition d'un préfixe pour le chargement des fichiers. Utile si les automates sont dans un dossier différent */
const string Automate::FILES_PREFIX = "E3-";

Automate::Automate()
{
	// Constructeur vide
}

Automate::~Automate()
{
	// Destructeur vide
}

/*
 * Recherche de la position en memoire 
 * d'un certain etat
 */
int Automate::findStatePosition(string nom)
{
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		if (nom == _listStates[i]->getNom())
			return i;
	}
	return -1;
}

/**
* Méthode de minimisation de l'automate incomplet non fonctionnel
*/
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

/**
* Méthode de standardisation de l'automate
*/
void Automate::standardiser()
{
	if (this->isStandard())
		return;

	// On crée un état d'entrée
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
	cout << entree;
	entree->setEntry(true);
	_listStates.push_back(entree);
}

/**
* Méthode de chargement d'un automate dans la mémoire
* @see const string Automate::FILES_PREFIX, ligne 10.
*/
void Automate::loading(string fileName)
{
	this->_fileName = fileName;
	ifstream origine;
	origine.open(FILES_PREFIX + fileName + ".txt", ios::in); //Ouverture du fichier en lecture

	if (!origine)
	{
		Utils::consoleClear();
		_util.lineJump(1);
		cerr << fg_red << " Automate '" + fileName + "' introuvable !" << fg_white << endl;
		this->setFound(false);
		return;
	}
	Utils::consoleClear();
	this->setFound(true);

	int tmpInt, nbrLecture, source, dest;
	char tmpChar;


	origine >> tmpInt;//nombre de carateres dans l'alphabet

	for (int i = 0; i < tmpInt; i++) //initialisation de l'alphabet avec le nombre de caractères trouvés
	{
		_abcd.addCaratere(97 + i); //Ajout du caractère avec code ASCI
	}

	origine >> tmpInt; //nombre d'etats

	for (int i = 0; i < tmpInt; i++)//initialisation des états
	{
		_listStates.push_back(new Etat(_util.ToString(i)));
	}

	origine >> nbrLecture;//nombre d'états initiaux

	for (int i = 0; i < nbrLecture; i++)//mettre les états initiaux
	{
		origine >> tmpInt;
		_listStates[findStatePosition(_util.ToString(tmpInt))]->setEntry(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre d'états terminaux

	for (int i = 0; i < nbrLecture; i++)//mettre les états terminaux
	{
		origine >> tmpInt;
		_listStates[findStatePosition(_util.ToString(tmpInt))]->setExit(true); // On initialise l'état lue comme étant un état initial grace a son index
	}

	origine >> nbrLecture;//nombre de transition

	for (int i = 0; i < nbrLecture; i++)//mettre les transitions
	{
		origine >> source;
		origine >> tmpChar;
		origine >> dest;

		// On créer une transition depuis l'état de départ vers l'état d'arrivée contenant le caractere de l'alphabet qui permet de faire cette transition
		_listStates[findStatePosition(_util.ToString(source))]->addTransition(Transition(_listStates[findStatePosition(_util.ToString(dest))], tmpChar));
	}

	_util.lineJump(1);
}

/**
* Automate synchrone ?
*/
bool Automate::isSynchrone()
{
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		// On regarde si pour chaque transition il existe un caractere epsilon
		vector<Transition> tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			if (tmp[j].getCaractere() == '*')
				return false; // Si oui alors l'automate est asynchrone
		}
	}
	return true;
}

/**
* Automate déterministe ?
*/
bool Automate::isDeterministe()
{
	unsigned int i, entrees = 0;
	//Si il y a plusieurs entrees l'automate n'est pas détermilniste

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

/**
* Automate standard ?
*/
bool Automate::isStandard()
{
	//Si il y a plusieurs entrees l'automate n'est pas standard
	unsigned int entrees = 0;
	for (unsigned int i = 0; i < _listStates.size(); i++) // On parcours les etats
	{
		if (_listStates[i]->getEntry()) // Si l'etat est une entrée
		{
			entrees++;
			if (entrees > 1)
				return false;
		}
	}

	//Si une transition revient sur l'etat initial l'automate n'est pas standard
	for (unsigned int i = 0; i < _listStates.size(); i++) // On parcours les etats
	{
		if (_listStates[i]->transitionsEtatInitial())
		{
			return false;
		}
	}
	return true;
}

/**
* Automate complet ?
*/
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
		if (_listStates[i]->getTransitions().size() != tailleAlphabet) // On regarde si il y a autant de caracteres que de transitions
		{
			return false; // si non = non standard
		}
	}

	return true;
}

/**
* Liste les etats initiaux si l'automate est asynchrone
*/
vector<int> Automate::getEpsilons(Etat* a)
{
	vector<Transition> tmpTransitions = a->getTransitions(); // On liste les transitions partant de l'etat initial

	for (int j = 0; j < tmpTransitions.size(); j++) // On parcours les transitions
	{
		if (tmpTransitions[j].getCaractere() == '*') // Si la transition a une etiquette epsilon
		{
			// Alors sa destination est également initiale
			Etat* o = tmpTransitions[j].getArrivee();
			_epsilon.push_back(stoi(o->getNom()));
			// On réitere l'opération en recursif jusqu'a la fin de l'automate
			getEpsilons(o);
			//cout << o->getNom() << endl;
		}
	}
	return _epsilon; // On retourne tous les états qui sont initiaux
}

/**
* Déterminise un automate
*/
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
			if (tmp == nullptr) // Si on a pas encore creer un etat inial on le creer
			{
				tmp = new Etat("");
				tmp->setNom(_listStates[i]->getNom());
				tmp->addTransitions(_listStates[i]->getTransitions());
			}
			else // Sinon on ajoute le nouvel etat a l'etat initial
			{
				tmp->setNom(tmp->getNom() + "," + _listStates[i]->getNom());
				tmp->addTransitions(_listStates[i]->getTransitions());
			}

			// On ajoute les transitions epsilons dans le cas d'un automate asynchrone
			vector<int> az = getEpsilons(_listStates[i]);
			for (int z = 0; z < az.size(); z++)
			{
				tmp->setNom(tmp->getNom() + "," + _listStates[az[z]]->getNom());
				tmp->addTransitions(_listStates[az[z]]->getTransitions());
			}
		}
	}
	//Supprime les transitions identiques
	tmp->suppDoublonsTransitions();

	//regroupe les transitions
	tmp->regrouperTransitions();
	tmp->setEntry(true);
	deterministe._listStates.push_back(tmp); // On initialise notre nouvel automate avec les etats initiaux

	//Temps que tous les états des transitions n'existent pas on les créer
	for (unsigned int i = 0; i < deterministe._listStates.size(); i++) // On parcours notre nouvel automate
	{
		vector<Transition> tmpTransitions = deterministe._listStates[i]->getTransitions(); // Pour chaque etat on liste les transitions

		for (unsigned int j = 0; j < tmpTransitions.size(); j++) // On parcours chaque transition
		{
			//Si l'etat ne fait pas parti de notre nouvel automate
			if (deterministe.findStatePosition(tmpTransitions[j].getArrivee()->getNom()) == -1)
			{
				// On l'ajoute
				deterministe._listStates.push_back(tmpTransitions[j].getArrivee());

				//Si la cible de transition ne fesait pas partie de l'automate d'origine
				if (this->findStatePosition(tmpTransitions[j].getArrivee()->getNom()) == -1)
				{
					//Il faut lui créer ses transitions en fonction de l'automate d'origine
					//Parcours tout les etats initiaux et si le nom est contenu dans le nouvel état alors somme des états
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
	*this = deterministe; // On enregistre notre automate
}

/**
* Affiche la view principale du programme, avec les items du menu à gauche et à droite les infos de l'automate
* NB : utilisation de setw pour pour fixer une taille
*/
void Automate::print()
{
	_util.lineJump(1);
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

	cout << fg_white << setw(50) << left << " [0] Quitter"
		<< fg_yellow << setw(20) << left << "*** Informations sur l'automate ***" << endl;

	cout << fg_white << setw(50) << left << " [1] Changer d'automate"
		<< setw(20) << left << " Nom de l'automate : " + _fileName << ".txt" << endl;

	cout << fg_white << setw(50) << left << " [2] Savoir si l'aumotate est synchrone"
		<< setw(20) << left << " Elements dans l'alphabet : " + _util.ToString(_abcd.getAlphabetSize()) << endl;


	cout << fg_white << setw(50) << left << " [3] Savoir si l'automate est deterministe"
		<< setw(20) << left << " Nombre d'etats : " + _util.ToString(_listStates.size()) << endl;


	cout << fg_white << setw(50) << left << " [4] Savoir si l'automate est standard"
		<< setw(20) << left << " Nombre d'etats initiaux : " + _util.ToString(_initialStates.size()) << endl;


	cout << fg_white << setw(50) << left << " [5] Savoir si l'automate est complet"
		<< setw(20) << left << " Nombre d'etats terminaux : " + _util.ToString(_finalStates.size()) << endl;


	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			cpt++;
		}
	}
	cout << fg_white << setw(50) << left << " [6] Determiniser l'automate"
		<< setw(20) << left << " Nombre de transitions : " + _util.ToString(cpt) << endl;
	//cpt = 0;


	cout << fg_white << setw(50) << left << " [7] Completer l'automate"
		<< fg_yellow << setw(20) << left << "***********************************" << endl;


	cout << fg_white << " [8] Standardiser l'automate" << endl;

	cout << fg_white << setw(50) << " [9] Complementariser l'automate"
		<< fg_cyan << setw(20) << left << "********* Automate obtenu *********" << endl;

	cout << fg_white << setw(50) << left << " [10] Minimiser l'automate"
		<< setw(20) << left << " Etats initiaux : ";
	cout << "[";
	//Les états initiaux
	for (unsigned int i = 0; i < _initialStates.size(); i++)
	{
		cout << fg_magenta << "(" << _initialStates[i] << ")";
	}
	cout << fg_white << "]" << fg_white << endl;


	cout << fg_white << setw(50) << left << " [11] Entrer un mot a tester"
		<< setw(20) << left << " Etats terminaux : ";
	cout << fg_white << "[";
	//Les etats terminaux01
	for (unsigned int i = 0; i < _finalStates.size(); i++)
	{
		cout << fg_magenta << "(" << _finalStates[i] << ")";
	}
	cout << fg_white << "]" << endl;
	cout << fg_white << setw(50) << left << " [12] Recharger l'automate"
		<< setw(20) << left << " Liste des transitions : " << endl;
	//Les transitions
	for (unsigned int i = 0; i < _listStates.size(); i++)
	{
		tmp = _listStates[i]->getTransitions();
		for (unsigned int j = 0; j < tmp.size(); j++)
		{
			cout << fg_magenta << setw(50) << left << " " << left << "  (" << _listStates[i]->getNom() << ")" << fg_white << " -> " << tmp[j].getCaractere() << " -> " << fg_magenta << "(" << tmp[j].getArrivee()->getNom() << ")" << fg_white << endl;
		}
	}

	cout << fg_cyan << setw(50) << left << " " << left << "***********************************" << fg_white << endl;
}

void Automate::completer()
{
	if (this->isComplet()) // Si c'est complet on sort
		return;


	Etat* poubelle = new Etat("P");
	//Initialisatin de la poubelle
	for (unsigned int i = 0; i < _abcd.getAlphabetSize(); i++)
	{
		// On parcours notre alphabet et on ajoute un transition possedant l'etiquette parcourue
		poubelle->addTransition(Transition(poubelle, _abcd.getAlphabet()[i]));
	}

	for (unsigned int i = 0; i < _listStates.size(); i++) // On parcours les etats
	{
		if (_listStates[i]->getTransitions().size() != _abcd.getAlphabetSize()) // Si il n'y a pas autant de transitions que de characteres dans l'alphabet c'est qu'il manque une transition
		{
			_listStates[i]->completion(poubelle, _abcd.getAlphabet()); // On ajoute une transition vers l'etat poubelle
		}
	}

	_listStates.push_back(poubelle); // On ajoute notre etat poubelle a nos etats
}

/*
 * Complementarisation de l'automate
 */
void Automate::inverser()
{
	if (this->isComplet() == false) // L'automate doit etre complet
		this->completer();

	for (unsigned int i = 0; i < _listStates.size(); i++) // On parcours les etats
	{
		if (_listStates[i]->getExit()) // Les terminaux sont supprimés
			_listStates[i]->setExit(false);
		else
			_listStates[i]->setExit(true); // Les non terminaux deviennent terminaux
	}
}


/*
 * Reconaissance d'un mot
 */
void Automate::analyseMot(string mot)
{
	Etat* actuel = new Etat;

	for (unsigned int i = 0; i < mot.size(); i++) // On parcours chaque charactere de notre mot
	{
		// Si l'un de nos caractere n'est pas dans l'alphabet on s'arrete
		if (_abcd.inAlphabet(mot[i]) == false && mot != "*")
		{
			cout << fg_red << mot << " n'est pas un mot reconaissable par cet automate" << fg_white << endl;
			return;
		}
	}

	// L'automate doit etre ADC
	if (this->isDeterministe() == false)
		this->determiniser();
	if (this->isComplet() == false)
		this->completer();

	for (unsigned int i = 0; i < _listStates.size(); i++) // On parcours nos etats
	{
		if (_listStates[i]->getEntry())
		{
			actuel = _listStates[i]; // Actuel devient l'etat initial
			break;
		}
	}

	// Si notre etat initial est également terminal, on reconnait le mot vide
	if (actuel->getExit() == true && mot == "*")
	{
		cout << fg_green << "L'automate reconnait le mot vide" << fg_white << endl;
		this->reload();
		return;
	}


	for (unsigned int i = 0; i < mot.size(); i++) // On parcours chaque charactere de notre mot
	{
		vector<Transition> tmp = actuel->getTransitions(); // On liste les transitions de notre etat initial

		for (unsigned int j = 0; j < tmp.size(); j++) // On parcours chaque transition
		{
			if (tmp[j].getCaractere() == mot[i]) // Si l'une des transition possede une etiquette contenant le caractere actuel
				actuel = tmp[j].getArrivee(); // L'etat actuel devient l'etat d'arrivée 
		}
	}

	if (actuel->getExit() == true) // Si l'etat actuel est une sortie, notre mot est reconnu
		cout << fg_green << mot << " est un mot reconaissable par cet automate" << fg_white << endl;
	else
		cout << fg_red << mot << " n'est pas un mot reconaissable par cet automate" << fg_white << endl;

	this->reload(); // On recharge l'automate
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
