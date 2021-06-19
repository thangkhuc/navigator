#ifndef ANZEIGE_DIALOG_H
#define ANZEIGE_DIALOG_H

#include <QDialog>
#include <vertex.h>
#include <adress.h>
#include <poi.h>

namespace Ui {
class Anzeige_Dialog;
}
class Anzeige_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Anzeige_Dialog(vector<Vertex*>, QWidget *parent = nullptr);
    explicit Anzeige_Dialog(Vertex* source, RouteInfor*, QWidget *parent = nullptr); //Routingstabelle anzeigen

    ~Anzeige_Dialog();

private:
    Ui::Anzeige_Dialog *ui;
};

#endif // ANZEIGE_DIALOG_H
