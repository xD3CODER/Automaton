#include <iostream>
#include <ios>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "Alphabet.h"
#include "Etat.h"
#include "Transition.h"
#include "Utils.h"

class Automate
{
    public:
        Automate();
        virtual ~Automate();
        int rechercheIndiceEtat(std::string nom);
        void loading(std::string nomFichier);
        bool isSynchrone();
        bool isDeterministe();
        bool isStandart();
        bool isComplet();
        void determiniser();
        void minimiser();
        void standardiser();
        void completer();
        void inverser();
        void print();
        void analyseMot(std::string mot);
        void reset();
        Automate& operator=(const Automate &arg);
    protected:
		std::vector<std::string> _initialStates , _finalStates;
		std::string _fileName;
		Utils _util;
        Alphabet _abcd;
        std::vector<Etat*> _listEtats; // Contient les index de tous les etats de l'automate
    private:
};
