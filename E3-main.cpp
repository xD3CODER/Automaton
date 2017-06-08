#include <iostream>
#include "E3-Automate.h"
#include <stdlib.h>
#include "E3-Color.h"
#include "E3-Utils.h"


using namespace ColorConsole;
using namespace std;

void menu(Automate& arg);
void charger(Automate& arg);
void askSync(Automate& arg);
void askDeterministe(Automate& arg);
void askStandard(Automate& arg);
void askComplet(Automate& arg);
void doDeterministe(Automate& arg);
void doComplet(Automate& arg);
void doStandard(Automate& arg);
void doInvert(Automate& arg);
void doMinify(Automate& arg);
void testingWord(Automate& arg);
void reload(Automate& arg);
bool choice = false;

int main()
{
	Automate automate;
	string name;

	//On demande à l'utilisateur de choisir un automate
	charger(automate);

	Utils::consoleClear();

	menu(automate);


#if _WIN32
	system("pause");
#endif
	return 0;
}

/*
 * On affiche le menu des options, si l'une des option n'est pas valide
 * on informe l'utilisateur et on recommence
 */
void menu(Automate& arg)
{
	int choix = 99;
	while (choix != 0)
	{
		arg.print();
		do
		{
			cin.clear();
			cin.ignore(100, '\n'); // On sécurise l'entrée 
			cin >> choix;
			cin.clear();
		}
		while (cin.fail());

		Utils::consoleClear();

		switch (choix)
		{
		case 0:
			return;
		case 1:
			charger(arg);
			choix = 99;
			break;
		case 2:
			askSync(arg);
			choix = 99;
			break;
		case 3:
			askDeterministe(arg);
			choix = 99;
			break;
		case 4:
			askStandard(arg);
			choix = 99;
			break;
		case 5:
			askComplet(arg);
			choix = 99;
			break;
		case 6:
			doDeterministe(arg);
			choix = 99;
			break;
		case 7:
			doComplet(arg);
			choix = 99;
			break;
		case 8:
			doStandard(arg);
			choix = 99;
			break;
		case 9:
			doInvert(arg);
			choix = 99;
			break;
		case 10:
			doMinify(arg);
			choix = 99;
			break;
		case 11:
			testingWord(arg);
			choix = 99;
			break;
		case 12:
			reload(arg);
			choix = 99;
			break;
		default:
			Utils::lineJump(1);
			cout << fg_red << " Option non reconnue" << fg_white << endl;
			menu(arg); // Boucle recursive
			break;
		}
	}
}


void charger(Automate& arg)
{
	Utils::lineJump(1);
	string name;
	do
	{
		cout << " Saisir le nom de l'automate a charger : ";
		cin >> name;
		arg.reset();
		arg.loading(name);
	}
	while (arg.getFound() == false);
	// Tant que l'utilisateur n'a pas entré un automate présent dans le dossier, on reboucle
}

// On test si automate async
void askSync(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isSynchrone())
		cout << fg_green << " L'automate est synchrone" << fg_white << endl;
	else
		cout << fg_red << " L'automate est asynchrone" << fg_white << endl;
}

// On test si automate deterministe
void askDeterministe(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isDeterministe())
		cout << fg_green << " L'automate est deterministe" << fg_white << endl;
	else
		cout << fg_red << " L'automate n'est pas deterministe" << fg_white << endl;
}

// On test si automate standard
void askStandard(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isStandard())
		cout << fg_green << " L'automate est standard" << endl;
	else
		cout << fg_red << " L'automate n'est pas standard" << endl;
}

// On test si automate complet
void askComplet(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isComplet())
		cout << fg_green << " L'automate est complet" << fg_white << endl;
	else
		cout << fg_red << " L'automate n'est pas complet" << fg_white << endl;
}

// On lance la determination
void doDeterministe(Automate& arg)
{
	arg.determiniser();
}

// On lance la completion
void doComplet(Automate& arg)
{
	arg.completer();
}

// On lance la standardisation
void doStandard(Automate& arg)
{
	arg.standardiser();
}

// On lance la complementarisation
void doInvert(Automate& arg)
{
	arg.inverser();
}

// On lance la minification
void doMinify(Automate& arg)
{
	arg.minimiser();
}

// On test un mot sur l'automate
void testingWord(Automate& arg)
{
	Utils::lineJump(1);
	string mot;
	cin.clear(); // On clear l'entrée
	cout << " Quel mot souhaitez-vous tester ? " << endl;
	cin >> mot;
	arg.analyseMot(mot); // On test le mot
}

// On recharge l'automate
void reload(Automate& arg)
{
	arg.reload();
}
