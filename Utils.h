#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils
{
public:
	Utils();
	//int getNumberOfChar(std::ifstream &file);
	static void lineJump(int nbOfSpace);
	static void consoleClear();
	std::string ToString(int val) const;
};


#endif // UTILS_H