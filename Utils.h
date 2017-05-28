#include <fstream>
#include <iostream>
#include <string>
#include <fstream>



class Utils
{
public:
	Utils();
    int getNumberOfChar(std::ifstream &file);
	void lineJump(int nbOfSpace);
	static void consoleClear();
	std::string ToString(int val);
};
