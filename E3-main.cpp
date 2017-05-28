#include <iostream>
#include "E3-Automate.h"
#include <stdlib.h>

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

int main()
{
	Automate automate;
	string name;
	do
	{
		cout << "Saisir le nom de l'automate a charger : ";
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

void menu(Automate& arg)
{
	int choix = 1;
	while (choix != 0)
	{
		arg.print();
		do
		{
			cin.clear();
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
			break;
		case 2:
			askSync(arg);
			break;
		case 3:
			askDeterministe(arg);
			break;
		case 4:
			askStandard(arg);
			break;
		case 5:
			askComplet(arg);
			break;
		case 6:
			doDeterministe(arg);
			break;
		case 7:
			doComplet(arg);
			break;
		case 8:
			doStandard(arg);
			break;
		case 9:
			doInvert(arg);
			break;
		case 10:
			doMinify(arg);
			break;
		case 11:
			testingWord(arg);
			break;
		case 12:
			reload(arg);
			break;
		default:
			break;
		}
	}
}

void charger(Automate& arg)
{
	string name;
	do
	{
		cout << "Saisir le nom de l'automate a charger : ";
		cin >> name;

		arg.reset();
		arg.loading(name);
	}
	while (arg.getFound() == false);
}

void askSync(Automate& arg)
{
	if (arg.isSynchrone())
		cout << "L'automate est synchrone" << endl;
	else
		cout << "L'automate est asynchrone" << endl;
}

void askDeterministe(Automate& arg)
{
	if (arg.isDeterministe())
		cout << "L'automate est deterministe" << endl;
	else
		cout << "L'automate n'est pas deterministe" << endl;
}

void askStandard(Automate& arg)
{
	if (arg.isStandard())
		cout << "L'automate est standard" << endl;
	else
		cout << "L'automate n'est pas standard" << endl;
}

void askComplet(Automate& arg)
{
	if (arg.isComplet())
		cout << "L'automate est complet" << endl;
	else
		cout << "L'automate n'est pas complet" << endl;
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
	string mot;
	cin.clear();
	cout << "Quel mot souhaitez-vous tester ? " << endl;
	cin >> mot;
	arg.analyseMot(mot);
}

void reload(Automate& arg)
{
	arg.reload();
}
