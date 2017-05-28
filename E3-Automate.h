#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <vector>
#include "E3-Alphabet.h"
#include "E3-Etat.h"
#include "E3-Utils.h"

class Automate
{
public:
	Automate();
	virtual ~Automate();
	int rechercheIndiceEtat(std::string nom);
	void loading(std::string nomFichier);
	bool isSynchrone();
	bool isDeterministe();
	bool isStandard();
	bool isComplet();
	void determiniser();
	void minimiser();
	void standardiser();
	void completer();
	void inverser();
	void print();
	void analyseMot(std::string mot);
	void reset();
	void reload();
	void setFound(bool state) { _found = state; };
	bool getFound() const { return _found; };
	Automate& operator=(const Automate& arg);
protected:
	bool _found = false;
	std::vector<std::string> _initialStates, _finalStates;
	std::string _fileName;
	Utils _util;
	Alphabet _abcd;
	std::vector<Etat*> _listStates; // Contient les index de tous les etats de l'automate
private:
	static const std::string FILES_PREFIX;
};

#endif // AUTOMATE_H
