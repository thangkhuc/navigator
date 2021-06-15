#ifndef TEST_H
#define TEST_H

#include <QtTest/QTest>
#include <navigation.h>

class Test : public QObject
{
    Q_OBJECT
private slots:
    void Cossbut_Saarbruecken();
    void Muenchen_Kiel();
    void Mainz_Wiesbaden();
    void Schwerin_Berlin();

public:
    Test(Navigation&);
private:
    Navigation* t;
};

#endif // TEST_H
