#include "navigation.h"
#include "test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Navigation w;
    w.show();

    Test t(w);
    QTest::qExec(&t);
    return a.exec();
}
