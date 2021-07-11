#include "navigation.h"
#include "test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Navigation w;
    w.karte_importieren();
    Test t(w);
    QTest::qExec(&t);

    w.show();

    return a.exec();

}
