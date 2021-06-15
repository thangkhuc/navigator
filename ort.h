#ifndef ORT_H
#define ORT_H
#include <iostream>
#include <cmath>
#include <netzknote.h>
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
    vector<Netzknote*> nachbare;
    vector<Netzknote*> routingstabelle;
public:
    Ort();
    virtual ~Ort();
    Ort(double, double, string);
    Ort(double, double, string, unsigned);

    virtual Typ checkTyp() const = 0;
    void   addNachbar(Ort*);
    double entfernung(Ort*) const;

    //return true und setzt das unsigned Variable das ID des Ortes, wenn er Nachbar ist
    bool checkNachbar(Ort*,unsigned&) const;
    bool checkNachbar(Ort*) const; //Overload checkNachbar

    //berechnet routingstabelle für alle Knoten im Netz
    void dijkstra();

    //return die Knote w, die nicht in N' ist und D(w) ist minimal
    Netzknote* minDistance(vector<unsigned>);

    //checkt ob die Knote im Mänge N' ist
    bool checkKnowedNodesSet(Netzknote*, vector<unsigned>);

    // get den vector nachbare
    vector<Netzknote*> getNachbar() const;

    // get einen Nachbar mit dem überladenen ID
    // return null wenn es keinen Nachbar mit dem gegebenen ID gibt
    Netzknote* getNachbar(unsigned) const;

    // get eine Knote in der Routingstabelle mit dem gegebenen ID
    // return null wenn es keine Knote mit dem ID gibt
    Netzknote* getNetzknote(unsigned) const;

    double 	 getLaegneGrad() const;
    double 	 getBreiteGrad() const;
    unsigned getID() const;
    unsigned getStaticId() const;
    string   getName() const;
    void     setRoutingstabelle(vector<Ort*>);
    void 	 setLaengeGrad(double);
    void     setBreiteGrad(double);
    void     setName(string);
};

#endif // ORT_H
