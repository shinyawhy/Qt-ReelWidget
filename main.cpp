#include <QApplication>
#include "reelwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    ReelWidget w;
    testReelWidget tw;
    tw.show();
//    w.show();
    return a.exec();
}
