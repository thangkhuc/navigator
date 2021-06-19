#ifndef ORT_H
#define ORT_H
#include <iostream>
#include <cmath>
#include <vector>
#include <exception>

#define PI 3.14159265

using namespace std;

enum Typ {adr, poI};
enum Kategorie{Tankestelle = 1, Restaurant = 2, Sehenswuerdigkeit = 3};

class Ort
{
private:
    static unsigned staticID;
    unsigned id;
    double laengeGrad;
    double breiteGrad;
    string name;
public:
    Ort();
    virtual ~Ort();
    Ort(double, double, string);
    Ort(double, double, string, unsigned);

    virtual Typ checkTyp() const = 0;
    double entfernung(Ort*) const;

    double 	 getLaegneGrad() const;
    double 	 getBreiteGrad() const;
    unsigned getID() const;
    unsigned getStaticId() const;
    string   getName() const;
    void 	 setLaengeGrad(double);
    void     setBreiteGrad(double);
    void     setName(string);
};

#endif // ORT_H
