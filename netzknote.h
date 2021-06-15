#ifndef	NETZKNOTE_H
#define NETZKNOTE_H

class Ort;

//Netzknote ist eine Klasse, die eine Knote und die Entfernung zu dieser Knote speichert
//sie wird sowohl für Nachbare eines Ortes als auch Routingstabelle benutzt.
class Netzknote
{
public:
    Netzknote();
    Netzknote(Ort*, double);

    double getEntferung() const;
    Ort*   getKnote() const;
    void   setEntfernung(double);
    void   setVorneKnote(Netzknote*);
    Netzknote* getVorneKnote() const;

private:
    Ort*   knote;
    double entfernung;
    Netzknote* vorneKnote; //zeigt auf die vorne Knote im Routingspfad
};


#endif // NACHBAR_H
