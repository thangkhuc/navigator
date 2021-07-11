#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPainter>
#include <QInputDialog>

#include <string>
#include <fstream>

#include "adress.h"
#include "poi.h"
#include "anzeige_dialog.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Navigation; }
QT_END_NAMESPACE

const QPoint stammPoint;


class Navigation : public QMainWindow
{
    Q_OBJECT
public:
    Navigation(QWidget *parent = nullptr);
    ~Navigation();

    void meldung(std::string);
    int  aktiv_radioButton(vector<int>&); //return ein vector der Indexs der aktiven Button,
    void routingInfo_importieren();
    void routingInfo_exportieren();
    void karte_importieren();
    vector<Ort*> getNavidaten() const; // für Uni Test
    Ort* getOrt(unsigned) const; // get einen Ort von Navidaten mit dem gegebenen ID

    void paint(const QAbstractButton&, const QAbstractButton&);
    void paint(); //overfload paint Funktion, die alle gezeichnete Linie loeschen
    void paintEvent(QPaintEvent*); // overrided Funktion paintEvent, die mainWidget aktualisiert
    void addRadioButton(Ort* ort);


    //zeichne die Route zwischen zwei Knote auf dem mainWidget
    // erstes Parameter ist Quelle, zweites ist Ziel
    void paintRouting(Vertex*, RouteInfor*);

private slots:
    void on_entfernung_clicked();
    void on_information_clicked();
    void on_alleOrte_clicked();
    void on_routing_clicked();
    void on_import_clicked();
    void on_export_clicked();
    void on_add_button_clicked();

private:
    Ui::Navigation *ui;
    QButtonGroup radioButtonGroup;

    Graph* graph;
    QVector<Ort*> navidaten;
    QList<QRadioButton*> radioButtonList;

    QPixmap* buffer; // für Zeichen der Linie
    // buffer ist ein Widget-Zwischenspeicher, der für die Overloaded paintEvent verwendet wird
    // weil das mainWindow-Widget nur im overfloaded painEvent Funktion erneut gezeichnet werden kann.
};
#endif // NAVIGATION_H
