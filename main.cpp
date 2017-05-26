#include <iostream>
#include <windows.h>
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

HANDLE hConsole;



int main()
{

    Automate yolo;
    string name;
	do {
		cout << Colors::fg_cyan << "Saisir le nom de l'automate a charger : " << Colors::fg_white;
		cin >> name;
		yolo.loading(name);
	} while (yolo.getFound() == false);
    menu(yolo);
	system("pause");
    return 0;
}

void menu(Automate  &arg)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choix = 1;
    while(choix != 0)
    {
		arg.print();
        do
        {
            cin.clear();
            cin >> choix;
            cin.clear();
        }while(cin.fail());

        system("cls");

        switch(choix)
        {
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
        }
    }
}

void charger(Automate &arg)
{
    string name;
    cout << "Saisir le nom de l'automate a charger : ";
    cin >> name;

    arg.reset();
    arg.loading("Automates de test/" + name + ".txt");
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
        cout << "Automate Standart" << endl;
    else
        cout << "Automate Non Standart" << endl;
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
