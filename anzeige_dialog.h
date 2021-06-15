#ifndef ANZEIGE_DIALOG_H
#define ANZEIGE_DIALOG_H

#include <QDialog>
#include <navigation.h>

namespace Ui {
class Anzeige_Dialog;
}

class Anzeige_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Anzeige_Dialog(QVector<Ort*>, QWidget *parent = nullptr);
    explicit Anzeige_Dialog(Netzknote*, Netzknote*, QWidget *parent = nullptr); //Routingstabelle anzeigen
    ~Anzeige_Dialog();

private:
    Ui::Anzeige_Dialog *ui;
};

#endif // ANZEIGE_DIALOG_H
