#include "ort.h"

unsigned Ort :: staticID = 1;

Ort :: Ort()
{
    this->breiteGrad = 0.0;
    this->laengeGrad = 0.0;
    this->id = staticID;
    staticID++;
}

Ort :: ~Ort()
{
    for (unsigned i = 0; i < nachbare.size(); i++)
        delete nachbare[i];
}

Ort :: Ort(double laengeGrad, double breiteGrad, string name) : laengeGrad(laengeGrad), breiteGrad(breiteGrad), name(name) {this->id = staticID; staticID++;}

Ort :: Ort(double laengeGrad, double breiteGrad, string name, unsigned ID) : laengeGrad(laengeGrad), breiteGrad(breiteGrad), name(name), id(ID)
{
    while (ID >= this-> staticID)
        staticID++;
}

void     Ort :: setRoutingstabelle(vector<Ort *> navidaten)
{
    for (unsigned i = 0; i < navidaten.size(); i++)
        this->routingstabelle.push_back(new Netzknote(navidaten[i], INFINITY));
}

double   Ort :: getLaegneGrad() const {return this->laengeGrad;}

double   Ort :: getBreiteGrad() const {return this->breiteGrad;}

void     Ort :: setBreiteGrad(double breiteGrad) {this->breiteGrad = breiteGrad;}

void     Ort :: setLaengeGrad(double laengeGrad) {this->laengeGrad = laengeGrad;}

void     Ort :: setName(string name) {this->name = name;}

string   Ort :: getName() const {return this->name;}

unsigned Ort :: getID() const {return this->id;}

unsigned Ort :: getStaticId() const {return this->staticID;}

vector<Netzknote*> Ort :: getNachbar() const {return this->nachbare;}

Netzknote* Ort :: getNachbar(unsigned nachbarID) const
{
    for (unsigned i = 0; i < this->nachbare.size(); i++) {
        if (nachbarID == nachbare[i]->getKnote()->getID())
            return nachbare[i];
    }
    return nullptr;
}

Netzknote* Ort :: getNetzknote(unsigned knoteID) const
{
    for (unsigned i = 0; i < routingstabelle.size(); i++) {
        if (knoteID == routingstabelle[i]->getKnote()->getID())
            return routingstabelle[i];
    }
    return nullptr;
}

double Ort :: entfernung(Ort* ort) const {
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

void Ort :: addNachbar(Ort* ort)
{
   double entfernung = this->entfernung(ort);
   Netzknote* newNachbar = new Netzknote(ort, entfernung);
   nachbare.push_back(newNachbar);
}

bool Ort :: checkNachbar(Ort* ort, unsigned& nachbarID) const
{
    for (unsigned i = 0; i < this->nachbare.size(); i++) {
        if (this->nachbare[i]->getKnote()->getID() == ort->getID()) {
            nachbarID = ort->getID();
            return true;
        }
    }
    return false;
}

bool Ort :: checkNachbar(Ort* ort) const
{
    for (unsigned i = 0; i < this->nachbare.size(); i++) {
        if (this->nachbare[i]->getKnote()->getID() == ort->getID()) {
            return true;
        }
    }
    return false;
}

bool Ort :: checkKnowedNodesSet(Netzknote* knote, vector<unsigned> knowedNodesSet)
{
    for (unsigned i = 0; i < knowedNodesSet.size(); i++) {
        if (knote->getKnote()->getID() == knowedNodesSet[i])
            return true;
    }
    return false;
}

Netzknote* Ort :: minDistance(vector<unsigned> knowedNodesSet)
{
    int minValue = INT_MAX;
    Netzknote* knote;

    for (unsigned i = 0; i < routingstabelle.size(); i++) {
        if (!this->checkKnowedNodesSet(routingstabelle[i], knowedNodesSet) && routingstabelle[i]->getEntferung() <= minValue) {
            minValue = routingstabelle[i]->getEntferung();
            knote = routingstabelle[i];
        }
    }
    return knote;
}

void Ort :: dijkstra()
{
    /* Notation:
     * c(a,b) : die Kosten von Knote a zur Knote b (= inf, wenn a kein Nachbar von b ist)
     * D(v) : aktuelle kurzte Kosten from Quelle zum Ziel v
     * N' : die Mänger aller Knote, deren kurzten Pfad schon erkannt ist.
     */

    // um sicher zu stellen, dass es Knoten in Routingstabelle geben muss
    if (routingstabelle.size() == 0)
        throw std::length_error("Es gibt kein Element in der Routingstabelle");

    else {
        // Speichert ID der Knoten, deren kurzten Pfad schon erkannt ist
        // d.h die Knote ist im N' Mänge
        vector<unsigned> knownedNodesSet;

        //Initialisierungsphase
        //inialisert die Entfernung zu allen Knoten in der Routingstabelle
        for (unsigned i = 0; i < routingstabelle.size(); i++) {
            unsigned nachbarID = 0;

            //die Entfernung zur Quelle ist eher 0 und die Quelle ist in N' Mänge: N' = {u}
            if (routingstabelle[i]->getKnote()->getID() == this->id) {
                routingstabelle[i]->setEntfernung(0);
                knownedNodesSet.push_back(this->id);
            }

            // Wenn die Knote in Routingstabelle ein Nachbar der Quelle ist, setzt die Entfernung zu diesem Nachbar auf die Knote der Tabelle
            // aber die kurzte Entferung zur Knote ist noch nicht erkannt
            // Außerdem wird die vorne Knote der Nachbar Knote auf die Quelle gesetzt
            else if (this->checkNachbar(routingstabelle[i]->getKnote(), nachbarID)) {
                double entfernung = this->getNachbar(nachbarID)->getEntferung();
                routingstabelle[i]->setEntfernung(entfernung);
                routingstabelle[i]->setVorneKnote(this->getNetzknote(this->id));
            }

            //Wenn die Knote kein Nachbar der Quelle ist, setzt die Entfernung = infinity
            else {
                routingstabelle[i]->setEntfernung(INFINITY);
            }
        }


        //Loop Phase
        // finde den kurzten Pfad zu allen Knoten
        for (unsigned i = 0; i < routingstabelle.size(); i++) {

            // finde zuerst die Knote w mithilfe minDistance(), die nicht in N' ist und D(w) ist minimum
            Netzknote* w = minDistance(knownedNodesSet);

            //fügt die Knote w in N' hinzu
            knownedNodesSet.push_back(w->getKnote()->getID());

            //aktualisiert Entfernung D(v) von Quelle zu allen Knoten (v), die Nachbar von w und nicht in N' ist
            for (unsigned v = 0; v < routingstabelle.size(); v++) {

                //aktualisiert nur wenn D(w) + c(w,v) < D(v)
                if (!this->checkKnowedNodesSet(routingstabelle[v], knownedNodesSet)
                    && w->getKnote()->checkNachbar(routingstabelle[v]->getKnote())
                    && w->getEntferung() + w->getKnote()->entfernung(routingstabelle[v]->getKnote()) < routingstabelle[v]->getEntferung())
                {
                    double updateEntfernung = w->getEntferung() + w->getKnote()->entfernung(routingstabelle[v]->getKnote());
                    routingstabelle[v]->setEntfernung(updateEntfernung);

                    //und die vorne Knote der V Knote ist Knote w
                    routingstabelle[v]->setVorneKnote(w);
                }
            }
        }
    }
}
