
#include <fstream>
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>



class Utils
{

public:
	Utils();
    int getNumberOfChar(std::ifstream &file);
	void lineJump(int nbOfSpace);
	void consoleClear();
	std::string ToString(int val);

};
