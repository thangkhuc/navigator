#include "navigation.h"
#include "ui_navigation.h"

Navigation::Navigation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Navigation)
{
    ui->setupUi(this);
    connect(ui->entfernung_button, &QPushButton::clicked, this, &Navigation::on_entfernung_clicked);
    connect(ui->alleOrte_button, &QPushButton::clicked, this, &Navigation::on_alleOrte_clicked);
    connect(ui->information_button, &QPushButton::clicked, this, &Navigation::on_information_clicked);
    connect(ui->routing_button, &QPushButton::clicked, this, &Navigation::on_routing_clicked);
    connect(ui->import_button, &QPushButton::clicked, this, &Navigation::on_import_clicked);
    connect(ui->export_button, &QPushButton::clicked, this, &Navigation::on_export_clicked);

    //Initialisierung des Buffers mit Größr und Farbe ist nötig
    buffer = new QPixmap(this->width(),this->height()); // pass die Größe mit der Größe des Widgets an
    buffer->fill(Qt::white); // set the background color für das buffer Widget
}

void Navigation :: paint(const QAbstractButton& button1, const QAbstractButton& button2)
{
    QPoint point1(button1.geometry().bottomLeft().x() + 6, button1.geometry().bottomLeft().y() - 12);
    QPoint point2(button2.geometry().bottomLeft().x() + 6, button2.geometry().bottomLeft().y() - 12);

    QPainter draw(buffer); // zeichnen zuerst in das Buffer

    // set Breite und Farbe der Linie
    QPen linePen;
    linePen.setWidth(3);
    linePen.setColor(Qt::red);
    draw.setPen(linePen);

    draw.drawLine(point1, point2);
}

void Navigation :: paint()
{
   buffer = new QPixmap(this->width(), this->height());
   buffer->fill(Qt::white);
}

void Navigation :: paintEvent(QPaintEvent*)
{
    QPainter newLine(this);
    newLine.drawPixmap(0, 0, *buffer); // aktualisiert das mainWindow anhand des buffer
}

void Navigation :: paintRouting(Vertex* source, RouteInfor* route)
{
        RouteInfor* tmp = route;
        do {
            QAbstractButton* sourceButton;
            if (route->getPredecessor() == nullptr)
                sourceButton = radioButtonGroup.button(source->getPlace()->getID());
            else
                sourceButton = radioButtonGroup.button(route->getPredecessor()->getDestination()->getPlace()->getID());

            QAbstractButton* destinationButton = radioButtonGroup.button(route->getDestination()->getPlace()->getID());
            paint(*sourceButton, *destinationButton);

            route = route->getPredecessor();
        } while (route != nullptr);

        Anzeige_Dialog anzeige(source, tmp);
        anzeige.exec();
}

Navigation::~Navigation()
{
    delete ui;
    for (int i = 0; i < navidaten.size(); i++)
        delete navidaten[i];

    for (unsigned i = 0; i < graph->getVertices().size(); i++)
        graph->getVertices()[i]->deleteVertex();
}

void Navigation::on_entfernung_clicked()
{
    vector<int> aktivButtonArray; //speichert ID der aktiven Radio Button

    if (aktiv_radioButton(aktivButtonArray) != 2)
        meldung("Es muss zwei Orte gewählt werden");
    else {
        double entfernung = this->navidaten[aktivButtonArray[0] - 1]->entfernung(this->navidaten[aktivButtonArray[1] - 1]);

        paint(*radioButtonGroup.button(aktivButtonArray[0]), *radioButtonGroup.button(aktivButtonArray[1]));

        if (entfernung < 1) {
            QString a = "Entfernung: " + QString::number(entfernung * 1000) + "m";
            meldung(a.toStdString());
        }
        else {
            QString a = "Entfernung: " + QString::number(entfernung) + "km";
            meldung(a.toStdString());
        }
        paint();
    }
}

void Navigation::on_import_clicked()
{
    karte_importieren();
    graph = new Graph(vector<Ort*> (navidaten.begin(), navidaten.end()));
    routingInfo_importieren();

    graph->initializeRoutingsTable();
    // calculate routing table for all vertices in graph

    for (int i = 0; i < radioButtonList.size(); i++)
        delete radioButtonList[i];

    for (unsigned i = 0; i < graph->getVertices().size(); i++) {
        graph->dijkstra(graph->getVertices()[i]);
        addRadioButton(graph->getVertices()[i]->getPlace());
    }

    radioButtonList = findChildren<QRadioButton*> ();
    radioButtonGroup.setExclusive(false);
}

void Navigation::on_export_clicked()
{

    if (navidaten.size() == 0 || radioButtonList.size() == 0)
        meldung("There is no Element in system");
    else {
        ofstream file("../navigation/savedFile.txt");
        for (int i = 0; i < navidaten.size(); i++) {
            Typ typ = navidaten[i]->checkTyp();
            file << static_cast<int> (typ) << "\n";
            file << navidaten.at(i)->getID() << ";";
            file << navidaten.at(i)->getBreiteGrad() << ";";
            file << navidaten.at(i)->getLaegneGrad() << ";";
            file << navidaten.at(i)->getName() << ";";

            if (typ == adr) {
                Adress* adresse = dynamic_cast<Adress*>(navidaten[i]);
                file << adresse->getHausnummer() << ";";
                file << adresse->getGemeinde() << ";";
                file << adresse->getPLZ() << ";";
                file << adresse->getStrasse() << "\n";
            }
            else {
                PoI* poI = dynamic_cast<PoI*>(navidaten[i]);
                file << poI->getBemerkung() << ";";
                file << poI->getKategorie() << "\n";
            }
        }
        routingInfo_exportieren();
        file.close();
    }
}

void Navigation::on_routing_clicked()
{
    paint();
    vector<int> aktivButtonArray; //speichert ID der aktiven Radio Button

    if (aktiv_radioButton(aktivButtonArray) != 2)
        meldung("Es muss zwei Orte gewählt werden");
    else {
        Vertex* source = graph->getVertex(aktivButtonArray[0]);
        Vertex* destination = source->getVertex(aktivButtonArray[1]);

        RouteInfor* route = source->getRouteInfor(destination);
        paintRouting(source, route);
    }
    paint();
}

void Navigation::on_add_button_clicked()
{
    if (this->radioButtonList.size() == 0)
        meldung("noch keine Karte ist importiert!");
    else {
        bool ok;
        double breiteGrad, laengeGrad;
        QString titel = QInputDialog::getText(this, "Name des Ortes", "Geben Sie Name des Ortes ein", QLineEdit::Normal, "", &ok);

        if(!ok) return;
            breiteGrad = QInputDialog::getDouble(this, "Breite Grad", "Breite Grad",0,0,10000000, 6, &ok);
        if(!ok) return;
            laengeGrad = QInputDialog::getDouble(this, "Laenge Grad", "Laenge Grad",0,0,10000000, 6, &ok);
        if(ok) {
            if (breiteGrad < 55 && breiteGrad > 46 && laengeGrad > 5 && laengeGrad < 16) {
                Adress* newOrt= new Adress("", "", titel.toStdString(), 0, 0, laengeGrad, breiteGrad);
                navidaten.push_back(newOrt);
                Vertex* newVertex = new Vertex(newOrt);
                graph->addVertex(newVertex);

                unsigned nearestNode = 1;
                double minDistance = newOrt->entfernung(graph->getVertex(1)->getPlace());
                for (unsigned i = 1; i < graph->getVertices().size(); i++) {
                    double distance = newOrt->entfernung(graph->getVertex(i)->getPlace());

                    if (distance < minDistance && newOrt->getID() != graph->getVertices()[i]->getPlace()->getID()) {
                        minDistance = distance;
                        nearestNode = i;
                    }
                }
                newVertex->addNeighbor(graph->getVertex(nearestNode));
                graph->getVertex(nearestNode)->addNeighbor(newVertex);

                graph->initializeRoutingsTable();

                // erstelle wieder neue Routingstabelle für alle Knoten
                for (unsigned i = 0; i < graph->getVertices().size(); i++) {
                    Vertex* a = graph->getVertices()[i];
                    graph->dijkstra(a);
                }
                addRadioButton(newOrt);
            }
        }
    }
}

void Navigation::on_alleOrte_clicked()
{
    for (unsigned i = 0; i < graph->getVertices().size(); i++) {
        QAbstractButton* stammOrt = radioButtonGroup.button(navidaten[i]->getID());

        for (unsigned j = 0; j < graph->getVertices()[i]->getNeighbors().size(); j++)
            paint(*stammOrt, *radioButtonGroup.button(graph->getVertices()[i]->getNeighbors()[j]->getPlace()->getID()));
    }


    Anzeige_Dialog anzeige(graph->getVertices());
    anzeige.exec();

    paint(); //reset QWidget
}

void Navigation::on_information_clicked()
{
    vector<int> aktivButtonArray;
    aktiv_radioButton(aktivButtonArray);

    // add all selected places in vector verticies
    vector<Vertex*> verticies;
    for (unsigned i = 0; i < aktivButtonArray.size(); i++) {
        for (unsigned j = 0; j < graph->getVertices().size(); j++) {
            if (aktivButtonArray[i] == (int) graph->getVertices()[j]->getPlace()->getID())
                verticies.push_back(graph->getVertices()[j]);
        }
    }

    if (verticies.size() == 1) {
        for (unsigned i = 0; i < verticies[0]->getNeighbors().size(); i++) {
            QAbstractButton* stammOrt = radioButtonGroup.button(verticies[0]->getPlace()->getID());
            QAbstractButton* zielOrt = radioButtonGroup.button(verticies[0]->getNeighbors()[i]->getPlace()->getID());

            paint(*stammOrt, *zielOrt);
        }
    }

    Anzeige_Dialog anzeige(verticies);

    anzeige.exec();

    paint();
}

void Navigation::meldung(std::string text)
{
    QMessageBox* meldung = new QMessageBox();
    meldung->setText(QString::fromStdString(text));
    meldung->exec();
}

int Navigation::aktiv_radioButton(vector<int>& aktivButtonArray)
{
    int count = 0;
    for (int i = 0; i < radioButtonList.size(); i++) {
        if (radioButtonList[i]->isChecked()) {
           count++;
           aktivButtonArray.push_back(radioButtonGroup.id(radioButtonList[i]));
        }
    }
    return count;
}

void Navigation :: karte_importieren()
{
    //Lade Knote und ihre Informationen zuerst
    ifstream file("../navigation/savedFile.txt");

    if (!file.is_open()) {
        throw runtime_error("line 213 (navigation.cpp): saved File is not found");
    }
    else {
        unsigned size = this->navidaten.size();
        for (unsigned i = 0; i < size; i++) {
            delete this->navidaten[navidaten.size() - 1];
            this->navidaten.pop_back();
        }

        int typ;

        while (file >> typ) {
            unsigned ID = 0, PLZ = 0, hausnummer = 0;
            double breiteGrad = 0.0, laengGrad = 0.0;
            string gemeinde = "", strasse = "", name = "", bemerkung = "";
            Kategorie kategorie;

            string line;
            bool nachKomma = false;

            getline(file, line);
            getline(file, line);

            unsigned j = 0;
            int counter = 0;
            int negativExponent = -1;

            while (j < line.size()) {
                if (line.at(j) == ';') {
                    nachKomma = false;
                    negativExponent = -1;
                    counter++;
                    j++;
                }
                else if (counter == 0) { // Get ID des Ortes
                    ID = ID * 10 + line.at(j) - '0';
                    j++;
                }
                else if (counter == 1) { // Get Breitegrad
                    if (line.at(j) == '.') {
                        nachKomma = true;
                        j++;
                    }
                    if (nachKomma){
                        breiteGrad = breiteGrad + pow(10, negativExponent) * (line.at(j) - '0');
                        negativExponent--;
                        j++;
                    }
                    else {
                        breiteGrad = breiteGrad * 10 + line.at(j) - '0';
                        j++;
                    }
                }
                else if (counter == 2) { // Get Laengegrad
                    if (line.at(j) == '.') {
                        nachKomma = true;
                        j++;
                    }
                    if (nachKomma) {
                        laengGrad = laengGrad + pow(10, negativExponent) * (line.at(j) - '0');
                        negativExponent--;
                        j++;
                    }
                    else {
                        laengGrad = laengGrad * 10 + line.at(j) - '0';
                        j++;
                    }
                }
                else if (counter == 3) {
                    name += line.at(j);
                    j++;
                }
                else if (counter == 4) { // Get Hausnummer
                    if (static_cast<Typ>(typ) == adr) {
                        hausnummer = hausnummer * 10 + line.at(j) - '0';
                        j++;
                    }
                    else {
                        bemerkung += line.at(j);
                        j++;
                    }
                }
                else if (counter == 5) { // Get Gemeinde
                    if (static_cast<Typ>(typ) == adr) {
                        gemeinde += line.at(j);
                        j++;
                    }
                    else {
                        kategorie = static_cast<Kategorie>(line.at(j) - '0');
                        j++;
                    }
                }
                else if (counter == 6) { // Get PLZ
                    PLZ = PLZ * 10 + line.at(j) - '0';
                    j++;
                }
                else if (counter == 7) { // Get Strasse
                    strasse += line.at(j);
                    j++;
                }
                else if (counter == 8) {
                    name += line.at(j);
                    j++;
                }
            }

            if (static_cast<Typ>(typ) == adr)
                this->navidaten.push_back(new Adress(strasse, gemeinde, name, hausnummer, PLZ, ID, laengGrad, breiteGrad));
            else
                this->navidaten.push_back(new PoI(kategorie, bemerkung, name, laengGrad, breiteGrad, ID));
        }
    }
}

void Navigation::routingInfo_importieren()
{
    //Lade Routing Informationen nämlich Nachbare
    ifstream routingFile("../navigation/RoutingFile.txt");
    int nodeID;
    string line;

    if (!routingFile.is_open())
        throw underflow_error("line 332 (navigation.cpp): routing file is not found");
    else if (graph->getVertices().size() == 0)
        throw underflow_error("line 339 (navigation.cpp): graph has no vertex, graph should be initialized before call this function");
    else {
        while (routingFile >> nodeID) {
            vector<int> neighborID; // speichert ID der Nachbare
            getline(routingFile, line);
            getline(routingFile, line);

            int number = 0;
            for (unsigned i = 0; i <= line.size(); i++) {
                if (i == line.size() || line.at(i) == ';') {
                    neighborID.push_back(number);
                    number = 0;
                }
                else
                    number = number * 10 + line.at(i) - '0';
            }

            for (unsigned i = 0; i < neighborID.size(); i++)
                graph->getVertex(nodeID)->addNeighbor(graph->getVertex(neighborID[i]));
        }
    }
}

void Navigation::routingInfo_exportieren()
{
    ofstream routingFile("../navigation/RoutingFile.txt");

    for (unsigned i = 0; i < graph->getVertices().size(); i++) {
        routingFile << graph->getVertices()[i]->getPlace()->getID() << "\n";

        unsigned neighborSize = graph->getVertices()[i]->getNeighbors().size();
        for (unsigned j = 0; j < neighborSize; j++) {
            routingFile << graph->getVertices()[i]->getNeighbors()[j]->getPlace()->getID();
            if (j == neighborSize - 1)
                routingFile << "\n";
            else
                routingFile << ";";
        }
    }
}

void Navigation::addRadioButton(Ort* ort)
{
    const QPoint* stammPoint = new QPoint(490,290);
    const double  stammCoordinateX = 52.517037;
    const double  stammCoordinateY = 13.38886;
    const double  witdhRatio = 0.558783 / 60;
    const double  lengthRatio = 4.581695 / 290;

    int newY = stammPoint->y() + (stammCoordinateX - ort->getBreiteGrad()) / witdhRatio;
    int newX = stammPoint->x() - (stammCoordinateY - ort->getLaegneGrad()) / lengthRatio;

    QRadioButton* newRadioButton = new QRadioButton(QString::fromStdString(ort->getName()), this);

    newRadioButton->move(newX, newY);
    newRadioButton->show();
    radioButtonGroup.addButton(newRadioButton, ort->getID());

    radioButtonList = findChildren<QRadioButton*> ();
    radioButtonGroup.setExclusive(false);
    delete stammPoint;
}

vector<Ort*> Navigation::getNavidaten() const {return std :: vector<Ort*> (navidaten.begin(), navidaten.end());}

Ort* Navigation :: getOrt(unsigned ortID) const
{
    for (int i = 0; i < navidaten.size(); i++) {
        if (ortID == navidaten[i]->getID())
            return navidaten[i];
    }
    return nullptr;
}
