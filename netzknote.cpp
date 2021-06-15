#include "netzknote.h"

Netzknote :: Netzknote() {}

Netzknote :: Netzknote(Ort* knote, double entfernung) : knote(knote), entfernung(entfernung) {}

Ort* Netzknote :: getKnote() const {return this->knote;}

double Netzknote :: getEntferung() const {return this->entfernung;}

void Netzknote :: setEntfernung(double entfernung) {this->entfernung = entfernung;}

void Netzknote :: setVorneKnote(Netzknote* vorneKnote) {this->vorneKnote = vorneKnote;}

Netzknote* Netzknote :: getVorneKnote() const {return this->vorneKnote;}
