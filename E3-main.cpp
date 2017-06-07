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
	do
	{
		Utils::lineJump(1);
		cout << fg_white << " Saisir le nom de l'automate a charger : ";
		cin >> name;
		automate.loading(name);
	}
	while (automate.getFound() == false);

	Utils::consoleClear();


	menu(automate);


#if _WIN32
	system("pause");
#endif
	return 0;
}

bool valid = false;

void menu(Automate& arg)
{
	int choix = 99;
	while (choix != 0)
	{
		arg.print();
		do
		{
			cin.clear();
			cin.ignore(100, '\n');
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
			menu(arg);
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
}

void askSync(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isSynchrone())
		cout << fg_green << " L'automate est synchrone" << fg_white << endl;
	else
		cout << fg_red << " L'automate est asynchrone" << fg_white << endl;
}

void askDeterministe(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isDeterministe())
		cout << fg_green << " L'automate est deterministe" << fg_white << endl;
	else
		cout << fg_red << " L'automate n'est pas deterministe" << fg_white << endl;
}

void askStandard(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isStandard())
		cout << fg_green << " L'automate est standard" << endl;
	else
		cout << fg_red << " L'automate n'est pas standard" << endl;
}

void askComplet(Automate& arg)
{
	Utils::lineJump(1);
	if (arg.isComplet())
		cout << fg_green << " L'automate est complet" << fg_white << endl;
	else
		cout << fg_red << " L'automate n'est pas complet" << fg_white << endl;
}

void doDeterministe(Automate& arg)
{
	arg.determiniser();
}

void doComplet(Automate& arg)
{
	arg.completer();
}

void doStandard(Automate& arg)
{
	arg.standardiser();
}

void doInvert(Automate& arg)
{
	arg.inverser();
}

void doMinify(Automate& arg)
{
	arg.minimiser();
}

void testingWord(Automate& arg)
{
	Utils::lineJump(1);
	string mot;
	cin.clear();
	cout << " Quel mot souhaitez-vous tester ? " << endl;
	cin >> mot;
	arg.analyseMot(mot);
}

void reload(Automate& arg)
{
	arg.reload();
}
