#include "adress.h"

Adress :: Adress() {}

Adress :: Adress(string strasse, string gemeinde, string name, unsigned hausnummer, unsigned PLZ, double laengeGrad, double breiteGrad)
    : Ort(laengeGrad, breiteGrad, name), strasse(strasse), gemeinde(gemeinde), hausnummer(hausnummer), PLZ(PLZ) {}

Adress :: Adress(string strasse, string gemeinde, string name, unsigned hausnummer, unsigned PLZ, unsigned ID, double laengeGrad, double breiteGrad)
    : Ort(laengeGrad, breiteGrad, name, ID), strasse(strasse), gemeinde(gemeinde), hausnummer(hausnummer), PLZ(PLZ) {}

Typ Adress :: checkTyp() const
{
    return adr;
}

string Adress :: getStrasse() const {return this->strasse;}

string Adress :: getGemeinde() const {return this->gemeinde;}

unsigned Adress :: getHausnummer() const {return this->hausnummer;}

unsigned Adress :: getPLZ() const {return this->PLZ;}
