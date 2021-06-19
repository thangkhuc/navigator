#include "ort.h"

unsigned Ort :: staticID = 1;

Ort :: Ort()
{
    this->breiteGrad = 0.0;
    this->laengeGrad = 0.0;
    this->id = staticID;
    staticID++;
}

Ort :: ~Ort() {}

Ort :: Ort(double laengeGrad, double breiteGrad, string name) : laengeGrad(laengeGrad), breiteGrad(breiteGrad), name(name) {this->id = staticID; staticID++;}

Ort :: Ort(double laengeGrad, double breiteGrad, string name, unsigned ID) : laengeGrad(laengeGrad), breiteGrad(breiteGrad), name(name), id(ID)
{
    while (ID >= this-> staticID)
        staticID++;
}

double   Ort :: getLaegneGrad() const {return this->laengeGrad;}

double   Ort :: getBreiteGrad() const {return this->breiteGrad;}

void     Ort :: setBreiteGrad(double breiteGrad) {this->breiteGrad = breiteGrad;}

void     Ort :: setLaengeGrad(double laengeGrad) {this->laengeGrad = laengeGrad;}

void     Ort :: setName(string name) {this->name = name;}

string   Ort :: getName() const {return this->name;}

unsigned Ort :: getID() const {return this->id;}

unsigned Ort :: getStaticId() const {return this->staticID;}

double Ort :: entfernung(Ort* ort) const
{
    // Formel:
    // Luftlinie = Erdradius * arccos(z)
    // mit z = sin(X_a) + sin(X_b) + cos(X_a) * cos(X_b) * cos(Y_a - Y_b)

    double X_a = this->getBreiteGrad() * PI / 180;
    double Y_a = this->getLaegneGrad() * PI / 180;
    double X_b = ort->getBreiteGrad() * PI / 180;
    double Y_b = ort->getLaegneGrad() * PI / 180;

    long double z = sin(X_a) * sin(X_b) + cos(X_a) * cos(X_b) * cos(Y_b - Y_a);

    return 6378.137 * acos(z);
}
