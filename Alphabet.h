#ifndef ALPHABET_H
#define ALPHABET_H

#include <vector>

class Alphabet
{
    public:
        Alphabet();
        virtual ~Alphabet();
        void addCaratere(char nouveau);
        std::vector<char> getAlphabet();
        unsigned int getTailleAlphabet();
        Alphabet& operator=(const Alphabet &tmp);
        void reset();
        bool inAlphabet(char arg);
    protected:
        std::vector<char> _caracteres;
    private:
};

#endif // ALPHABET_H
