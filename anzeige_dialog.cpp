#include "anzeige_dialog.h"
#include "ui_anzeige_dialog.h"

Anzeige_Dialog::Anzeige_Dialog(vector<Vertex*> graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Anzeige_Dialog)
{
    ui->setupUi(this);

    //erzeugt routingstabelle
    QTableWidget* information_table= new QTableWidget(graph.size(), 7, this);

    QStringList labels = {"ID", "Typ", "Name", "Laengegrad", "Breitegrad", "Adresse", "Bemerkung"};
    information_table->setHorizontalHeaderLabels(labels);
    information_table->setGeometry(0, ui->label->height(), this->width(), this->height() - ui->label->height());
    information_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // adjust column width to the window
    information_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // adjust column width to the window

    information_table->setFixedHeight(graph.size() * 30 + 30); // set heigt von der Tabelle

    // set font size vom Label
    ui->label->setText("Information");
    QFont font;
    font.setPointSize(20);
    ui->label->setFont(font);

    for (unsigned row = 0; row < graph.size(); row++) {
        information_table->setRowHeight(row, 30); //set row height für jeden Row

        for (int col = 0; col < 7; col++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            if (col == 0) {
                item->setText(QString::number(graph[row]->getPlace()->getID()));
                information_table->setItem(row, col, item);
            }
            if (col == 1) {
                if (graph[row]->getPlace()->checkTyp() == adr)
                    item->setText("Adresse");
                else if (graph[row]->getPlace()->checkTyp() == poI)
                    item->setText("PoI");
                information_table->setItem(row, col, item);
            }
            if (col == 2) {
                item->setText(QString::fromStdString(graph[row]->getPlace()->getName()));
                information_table->setItem(row, col, item);
            }
            if (col == 3) {
                item->setText(QString::number(graph[row]->getPlace()->getLaegneGrad()));
                information_table->setItem(row, col, item);
            }
            if (col == 4) {
                item->setText(QString::number(graph[row]->getPlace()->getBreiteGrad()));
                information_table->setItem(row, col, item);
            }
            if (col == 5) { // set Adresse Columm
                if (graph[row]->getPlace()->checkTyp() == poI)
                    item->setText("x");
                else  {
                   Adress* a = dynamic_cast<Adress*>(graph[row]->getPlace());
                   QString adresse = QString::fromStdString(a->getStrasse() + " ") + QString::number(a->getHausnummer())
                                    + QString::fromStdString(", ")  + QString::number(a->getPLZ())
                                    + QString::fromStdString(" " + a->getGemeinde());
                   item->setText(adresse);
                }
                information_table->setItem(row, col, item);
            }
            if (col == 6) { // set Bemerkung Columm
                if (graph[row]->getPlace()->checkTyp() == poI) {
                    item->setText(QString::fromStdString((dynamic_cast<PoI*> (graph[row]->getPlace()))->getBemerkung()));
                }
                else
                    item->setText("x");
                information_table->setItem(row, col, item);
            }
        }
    }

    //Wenn nur ein Ort angezeigt werden muss, dann wird seine Nachbare auch anzeigt
    if (graph.size() == 1) {
        ui->nachbar_table->setRowCount(graph[0]->getNeighbors().size());
        ui->nachbar_table->setFixedHeight(graph[0]->getNeighbors().size() * 30 + 30); // set heigt von der Tabelle
        ui->nachbar_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // set font size vom Label
        QFont font;
        font.setPointSize(20);
        ui->label_2->setFont(font);

        for (unsigned row = 0; row < graph[0]->getNeighbors().size(); row++) {
            Vertex* neighbor = graph[0]->getNeighbors()[row];

            ui->nachbar_table->setRowHeight(row, 30); //set row height für jeden Row

            for (int col = 0; col < 7; col++) {
                QTableWidgetItem* item = new QTableWidgetItem();

                if (col == 0) {
                    item->setText(QString::number(neighbor->getPlace()->getID()));
                    ui->nachbar_table->setItem(row, col, item);
                }
                if (col == 1) {
                    if (neighbor->getPlace()->checkTyp() == adr)
                        item->setText("Adresse");
                    else if (neighbor->getPlace()->checkTyp() == poI)
                        item->setText("PoI");
                    ui->nachbar_table->setItem(row, col, item);
                }
                if (col == 2) {
                    item->setText(QString::fromStdString(neighbor->getPlace()->getName()));
                    ui->nachbar_table->setItem(row, col, item);
                }
                if (col == 3) {
                    item->setText(QString::number(neighbor->getPlace()->getLaegneGrad()));
                    ui->nachbar_table->setItem(row, col, item);
                }
                if (col == 4) {
                    item->setText(QString::number(neighbor->getPlace()->getBreiteGrad()));
                    ui->nachbar_table->setItem(row, col, item);
                }
                if (col == 5) {
                    item->setText(QString::number(graph[0]->getRouteInfor(neighbor)->getDistance()) + " km");
                    ui->nachbar_table->setItem(row, col, item);
                }
            }
        }
        this->setFixedSize(information_table->width(),
                           ui->label->height() + information_table->height() + ui->label_2->height() + ui->nachbar_table->height());
    }
    else {
        this->setFixedSize(information_table->width(),
                           ui->label->height() + information_table->height());
        delete ui->nachbar_table;
        delete ui->label_2;
    }
}

Anzeige_Dialog::Anzeige_Dialog(Vertex* source, RouteInfor* route, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Anzeige_Dialog)
{
    ui->setupUi(this);

    //erzeugt label
    ui->label->setText("Routingstabelle");

    QFont font;
    font.setPointSize(20);
    ui->label->setFont(font);

    vector<Vertex*> hopArray; // save all nodes in path
    do {
        hopArray.push_back(route->getDestination());

        if (route->getPredecessor() == nullptr)
            hopArray.push_back(source);

        route = route->getPredecessor();
    }
    while (route != nullptr);

    //erzeugt routingstabelle
    QTableWidget* routingstabelle = new QTableWidget(hopArray.size(), 6, this);

    QStringList labels = {"ID", "Typ", "Name", "Laengegrad", "Breitegrad", "next Hop"};
    routingstabelle->setHorizontalHeaderLabels(labels);
    routingstabelle->setGeometry(0, ui->label->height(), this->width(), this->height() - ui->label->height());
    routingstabelle->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // adjust column width to the window
    routingstabelle->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // adjust column width to the window

    for (unsigned row = 0; row < hopArray.size(); row++) {
        for (int col = 0; col < 6; col++) {
            QTableWidgetItem* item = new QTableWidgetItem();
            //set ID Columm
            if (col == 0) {
                item->setText(QString::number(hopArray[hopArray.size() - 1 - row]->getPlace()->getID()));
                routingstabelle->setItem(row, col, item);
            }

            //set Typ Columm
            if (col == 1) {
                if (hopArray[hopArray.size() - 1 - row]->getPlace()->checkTyp() == adr)
                    item->setText("Adresse");
                else if (hopArray[hopArray.size() - 1 - row]->getPlace()->checkTyp() == poI)
                    item->setText("PoI");
                routingstabelle->setItem(row, col, item);
            }

            //set Name Columm
            if (col == 2) {
                item->setText(QString::fromStdString(hopArray[hopArray.size() - 1 - row]->getPlace()->getName()));
                routingstabelle->setItem(row, col, item);
            }

            //set Längegrad Columm
            if (col == 3) {
                item->setText(QString::number(hopArray[hopArray.size() - 1 - row]->getPlace()->getLaegneGrad()));
                routingstabelle->setItem(row, col, item);
            }

            //set Breitegrad Columm
            if (col == 4) {
                item->setText(QString::number(hopArray[hopArray.size() - 1 - row]->getPlace()->getBreiteGrad()));
                routingstabelle->setItem(row, col, item);
            }

            //set next Hop Columm
            if (col == 5) {
                if (hopArray.size() - 1 - row == 0)
                    item->setText("x");
                else
                    item->setText(QString::fromStdString(hopArray[hopArray.size() - 1 - row - 1]->getPlace()->getName()));
                routingstabelle->setItem(row, col, item);
            }
        }
    }

    delete ui->nachbar_table;
    delete ui->label_2;
}

Anzeige_Dialog::~Anzeige_Dialog()
{
    delete ui;
}
