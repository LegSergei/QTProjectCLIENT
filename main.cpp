#include "qtclient.h"
#include <QtWidgets/QApplication>
#include "MyRegi.h"
#include "MyAutho.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtCLIENT w;
    w.show();
    return a.exec();
}
