#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPainter>

#include <string>
#include <fstream>

#include "adress.h"
#include "poi.h"
#include "anzeige_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Navigation; }
QT_END_NAMESPACE

class Navigation : public QMainWindow
{
    Q_OBJECT
public:
    Navigation(QWidget *parent = nullptr);
    ~Navigation();

    void meldung(std::string);
    int  aktiv_radioButton(vector<int>&); //return ein vector der Indexs der aktiven Button,
    void karte_importieren();
    void karte_exportieren();
    vector<Ort*> getNavidaten() const; // für Uni Test
    Ort* getOrt(unsigned) const; // get einen Ort von Navidaten mit dem gegebenen ID

    void updateRoutingstabelle(); //aktualisiert Routingstabelle aller Knoten im Netz
    void paintEvent(QPaintEvent*); // overrided Funktion paintEvent, die mainWidget aktualisiert
    void paint(const QAbstractButton&, const QAbstractButton&);
    void paint(); //overfload paint Funktion, die alle gezeichnete Linie loeschen

    //zeichne die Route zwischen zwei Knote auf dem mainWidget
    // erstes Parameter ist Quelle, zweites ist Ziel
    void paintRouting(Netzknote*, Netzknote*);

private slots:
    void entfernung();
    void information();
    void alleOrte();
    void routing();

private:
    Ui::Navigation *ui;
    QButtonGroup radioButtonGroup;

    QVector<Ort*> navidaten;
    QList<QRadioButton*> radioButtonList;

    QPixmap* buffer; // für Zeichen der Linie
    // buffer ist ein Widget-Zwischenspeicher, der für die Overloaded paintEvent verwendet wird
    // weil das mainWindow-Widget nur im overfloaded painEvent Funktion erneut gezeichnet werden kann.

};
#endif // NAVIGATION_H
