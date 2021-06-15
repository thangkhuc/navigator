#ifndef ADRESS_H
#define ADRESS_H
#include "ort.h"

class Adress : public Ort
{
private:
    string strasse;
    string gemeinde;
    unsigned hausnummer;
    unsigned PLZ;
public:
    Adress();
    virtual ~Adress() {};
    Adress(string, string, string, unsigned, unsigned, double, double); // Konstruktur mit allen Attribute aber ohne ID
    Adress(string, string, string, unsigned, unsigned, unsigned, double, double); // konstruktur mit ID

    virtual Typ checkTyp() const override;

    string getStrasse() const;
    string getGemeinde() const;
    unsigned getHausnummer() const;
    unsigned getPLZ() const;
};

#endif // ADRESS_H
