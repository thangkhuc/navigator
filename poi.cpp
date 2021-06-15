#include "poi.h"

PoI :: PoI() {}

PoI :: PoI(Kategorie kategorie, string bemerkung, string name, double laengeGrad, double breiteGrad)
    : Ort(laengeGrad, breiteGrad, name), kateogorie(kategorie), bemerkung(bemerkung) {}

PoI :: PoI(Kategorie kategorie, string bemerkung, string name, double laengeGrad, double breiteGrad, unsigned ID)
    : Ort(laengeGrad, breiteGrad, name, ID), kateogorie(kategorie), bemerkung(bemerkung) {}

Typ PoI :: checkTyp() const
{
    return poI;
}

Kategorie PoI :: getKategorie() const {return this->kateogorie;}

string PoI :: getBemerkung() const {return this->bemerkung;}

string PoI :: print_kategorie() const
{
    if (this->kateogorie == Tankestelle)
        return  "Tankestelle";
    else if (this->kateogorie == Restaurant)
        return "Restaurant";
    else
        return "Sehenswuerdigkeit";
}

