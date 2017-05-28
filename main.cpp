#include <iostream>
#include "Automate.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void menu(Automate &arg);
void charger(Automate &arg);
void connaitreSynchrone(Automate &arg);
void connaitreDeterministe(Automate &arg);
void connaitreStandart(Automate &arg);
void connaitreComplet(Automate &arg);
void determiniser(Automate &arg);
void completer(Automate &arg);
void standardiser(Automate &arg);
void inverser(Automate &arg);
void minimiser(Automate &arg);
void testerMot(Automate &arg);
void reload(Automate &arg);

int main()
{

    Automate automate;
    string name;
	do {
		cout << "Saisir le nom de l'automate a charger : ";
		cin >> name;
		automate.loading(name);
	} while (automate.getFound() == false);

    Utils::consoleClear();
    menu(automate);

    #if _WIN32
	   system("pause");
    #endif

    return 0;
}

void menu(Automate  &arg)
{
    int choix = 1;
    while(choix != 0)
    {
		arg.print();
        do {
            cin.clear();
            cin >> choix;
            cin.clear();
        } while(cin.fail());

        Utils::consoleClear();

        switch(choix) {
            case 0:
                return;
                break;
            case 1:
                charger(arg);
                break;
            case 2:
                connaitreSynchrone(arg);
                break;
            case 3:
                connaitreDeterministe(arg);
                break;
            case 4:
                connaitreStandart(arg);
                break;
            case 5:
                connaitreComplet(arg);
                break;
            case 6:
                determiniser(arg);
                break;
            case 7:
                completer(arg);
                break;
            case 8:
                standardiser(arg);
                break;
            case 9:
                inverser(arg);
                break;
            case 10:
                minimiser(arg);
                break;
            case 11:
                testerMot(arg);
                break;
            case 12:
                reload(arg);
                break;
            default:
                break;
        }
    }
}

void charger(Automate &arg)
{
    string name;
    do {
        cout << "Saisir le nom de l'automate a charger : ";
        cin >> name;

        arg.reset();
        arg.loading(name);
    } while (arg.getFound() == false);
}

void connaitreSynchrone(Automate &arg)
{
    if(arg.isSynchrone())
        cout << "Automate Synchrone" << endl;
    else
        cout << "Automate Asynchrone" << endl;
}

void connaitreDeterministe(Automate &arg)
{
    if(arg.isDeterministe())
        cout << "Automate Deterministe" << endl;
    else
        cout << "Automate Non Deterministe" << endl;
}

void connaitreStandart(Automate &arg)
{
    if(arg.isStandart())
        cout << "Automate Standard" << endl;
    else
        cout << "Automate Non Standard" << endl;
}

void connaitreComplet(Automate &arg)
{
    if(arg.isComplet())
        cout << "Automate Complet" << endl;
    else
        cout << "Automate Non Complet" << endl;
}

void determiniser(Automate & arg)
{
    arg.determiniser();
}

void completer(Automate &arg)
{
    arg.completer();
}

void standardiser(Automate &arg)
{
    arg.standardiser();
}

void inverser(Automate &arg)
{
    arg.inverser();
}

void minimiser(Automate &arg)
{
    arg.minimiser();
}

void testerMot(Automate &arg)
{
    string mot;
    cin.clear();
    cout << "Saisir le mot a tester : ";
    cin >> mot;


    arg.analyseMot(mot);
}

void reload(Automate &arg)
{
    arg.reload();
}
