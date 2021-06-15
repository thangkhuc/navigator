#include "test.h"
#include "navigation.h"

Test :: Test(Navigation& navi) : t(&navi) {}

void Test :: Cossbut_Saarbruecken()
{
    QCOMPARE(t->getNavidaten()[21]->entfernung(t->getNavidaten()[13]), 590.55); //Test für Cottbus [21] und Saarbrücken [13]
}

void Test :: Muenchen_Kiel()
{
    QCOMPARE(t->getNavidaten()[15]->entfernung(t->getNavidaten()[6]), 693.14); //Test für München [15] und Kiel [6]
}

void Test :: Mainz_Wiesbaden()
{
    QCOMPARE(t->getNavidaten()[12]->entfernung(t->getNavidaten()[11]), 7.87); //Test für Mainz[12] und Wiesbaden[11]
}

void Test :: Schwerin_Berlin()
{
    QCOMPARE(t->getNavidaten()[4]->entfernung(t->getNavidaten()[0]), 182.35); //Test für München [4] und Kiel [0]
}
