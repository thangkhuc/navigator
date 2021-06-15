#ifndef POI_H
#define POI_H
#include "ort.h"


class PoI : public Ort
{
private:
    Kategorie kateogorie;
    string bemerkung;
public:
    PoI();
    virtual ~PoI() {};
    PoI(Kategorie, string, string, double, double); // Konstruktur mit allen Attributen aber ohne ID
    PoI(Kategorie, string, string, double, double, unsigned); // Konstruktur mit ID

    virtual Typ checkTyp() const override;

    Kategorie getKategorie() const;
    string getBemerkung() const;
    string print_kategorie() const;
};

#endif // POI_H
